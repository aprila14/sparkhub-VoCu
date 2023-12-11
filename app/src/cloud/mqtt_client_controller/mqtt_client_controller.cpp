// Please keep these 2 lines at the beginning of each cpp module
static const char* LOG_TAG = "MqttClient";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO

#include "mqtt_client_controller.h"

#include "app_controller.h"
#include "cloud_config.h"
#include "commons.h"
#include "config_nvs.h"
#include "json_parser.h"
#include "sleep.h"

namespace
{
constexpr uint8_t MAX_QUEUE_SIZE                 = 10;
constexpr int     MQTT_CLIENT_ERROR_PUBLISH      = -1;
constexpr int     MQTT_CLIENT_SUBSCRIPTION_ERROR = -1;
constexpr int     MQTT_CLIENT_UNSUBSCRIBE_ERROR  = -1;

void _mqttEventHandler(void* handlerArgs, esp_event_base_t base, int32_t eventId, void* eventData)
{
    MqttClientController* pMqttClientController =
        reinterpret_cast<MqttClientController*>(handlerArgs); // NOLINT - we need reinterpret cast
    pMqttClientController->eventHandler(pMqttClientController, base, eventId, eventData);
}

} // unnamed namespace

MqttClientController::MqttClientController(CloudController* cloudController) :
    m_connectionStatus(false),
    m_messageQueue(),
    m_mqttClient(nullptr),
    m_taskHandle(nullptr)
{
    m_pCloudController  = cloudController;
    m_messageQueueMutex = mutexCreate();
}

void MqttClientController::runTask()
{
    LOG_INFO("runTask() started...");

    BaseType_t xReturned =
        xTaskCreate(run, LOG_TAG, DEFAULT_HUGE_STACK_SIZE, this, DEFAULT_TASK_PRIORITY, &m_taskHandle);
    if (xReturned != pdPASS)
        LOG_ERROR("Failed to create a task: %s", LOG_TAG);
}

void MqttClientController::run(void* pObject)
{
    MqttClientController* pMqttClientController = reinterpret_cast<MqttClientController*>(pObject); // NOLINT
    pMqttClientController->_run();
}

void MqttClientController::_run()
{
    while (true)
    {
        perform();
    }
}

void MqttClientController::perform()
{
    constexpr int SLEEP_TIME_BETWEEN_HANDLING_MESSAGES = 100;

    handleMessages();
    SLEEP_MS(SLEEP_TIME_BETWEEN_HANDLING_MESSAGES);
}

void MqttClientController::handleMessages()
{
    while (!(checkIfMessageBufferIsEmpty()))
    {
        LOG_ERROR("msg is received but not handled!!!");
    }
}

void MqttClientController::subscribeToRequiredTopics() // NOLINT - we don't want to make it const
{
    // TODO add later subscribe topics
}

bool MqttClientController::init(const prot::cloud_set_credentials::TCloudCredentials& credentials)
{
    esp_mqtt_client_config_t mqttConfig = getClientConfiguration(credentials);
    m_mqttClient                        = esp_mqtt_client_init(&mqttConfig);

    int errorCode = esp_mqtt_client_register_event(m_mqttClient, MQTT_EVENT_ANY, _mqttEventHandler, this);

    if (errorCode != ESP_OK)
    {
        LOG_INFO("Could not register event while executing MqttClientControler::init()");
        return false;
    }
    else
    {
        return true;
    }
}

esp_mqtt_client_config_t MqttClientController::getClientConfiguration(
    const prot::cloud_set_credentials::TCloudCredentials& credentials) // NOLINT - we don't want to make it const
{
    const prot::send_certificates::TCertificatePack& cloudCertificatePack = pConfig->getCertificatePack();

    esp_mqtt_client_config_t mqttConfig = {};

    mqttConfig.port                        = 8883;
    mqttConfig.keepalive                   = 60; // seconds
    mqttConfig.skip_cert_common_name_check = true;

    if (credentials.isSetCloudAddress())
    {
        mqttConfig.uri = credentials.cloudAddress;
    }
    else
    {
        LOG_ERROR("No cloud address provided");
    }

    if (credentials.isSetCloudDeviceId())
    {
        mqttConfig.client_id = credentials.cloudDeviceId;
    }
    else
    {
        LOG_ERROR("No cloud device id provided");
    }

    if (credentials.isSetCloudMqttUsername())
    {
        mqttConfig.username = credentials.cloudMqttUsername;
    }
    else
    {
        LOG_ERROR("No cloud mqtt username provided");
    }

    // TODO: turn on certificate verification after adding proper CA certificate in the Azure Device Provisioning
    // Service mqttConfig.cert_pem = DEFAULT_SERVER_PUBLIC_CERT;

    if (cloudCertificatePack.isSetFullChainCertificate())
    {
        mqttConfig.client_cert_pem = cloudCertificatePack.fullChainCertificate;
    }
    else
    {
        LOG_ERROR("No device public cert provided");
    }

    if (cloudCertificatePack.isSetPrivateKey())
    {
        mqttConfig.client_key_pem = cloudCertificatePack.privateKey;
    }
    else
    {
        LOG_ERROR("No device private key provided");
    }

    return mqttConfig;
}

void MqttClientController::eventHandler(void* handlerArgs, esp_event_base_t base, int32_t eventId, void* eventData)
{
    esp_mqtt_event_handle_t  event         = static_cast<esp_mqtt_event_handle_t>(eventData);
    esp_mqtt_client_handle_t client        = event->client;
    json_parser::TMessage    messageStruct = {};
    int32_t                  requestId     = -1;

    switch (eventId)
    {
        case MQTT_EVENT_ERROR:
            LOG_ERROR("MQTT Error occured");
            handleErrorCodes(event->error_handle);
            break;
        case MQTT_EVENT_CONNECTED:
            LOG_INFO("MQTT Connected");
            setConnectionStatus(true);
            m_pCloudController->setConnectedStatus();
            subscribeToRequiredTopics();
            break;
        case MQTT_EVENT_DISCONNECTED:
            LOG_INFO("MQTT Disconnected");
            setConnectionStatus(false);
            m_pCloudController->setNotConnectedStatus();
            break;
        case MQTT_EVENT_SUBSCRIBED:
            LOG_INFO("Subscribed to the topic");
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            LOG_INFO("Unsubscribed from the topic");
            break;
        case MQTT_EVENT_PUBLISHED:
            LOG_INFO("Published to the topic");
            break;
        case MQTT_EVENT_DATA:
        {
            LOG_INFO("New data has arrived");

            LOG_INFO("Message payload: \n%.*s", event->data_len, event->data);
            LOG_INFO("Message length: %d", event->data_len);
            LOG_INFO("Message topic: %.*s", event->topic_len, event->topic);
            LOG_INFO("Topic length: %d", event->topic_len);

            if (event->topic_len == 0)
            {
                LOG_INFO("Received message was too long and it was divided into two chunks, ESP32 not able to parse "
                         "it, it will be rejected");
                break;
            }
            if (m_messageQueue.size() > MAX_QUEUE_SIZE)
            {
                LOG_INFO("Not possible to add message to queue, queue size exceeded ");
                break;
            }
            std::string newMessage = std::string(event->data, 0, event->data_len);

            {
                LOCK_GUARD(m_messageQueueMutex, messageQueueMutex);
                messageStruct.message   = newMessage;
                messageStruct.requestId = -1;
                m_messageQueue.push(messageStruct);
            }

            break;
        }
        case MQTT_EVENT_BEFORE_CONNECT:
            LOG_INFO("Event 'BEFORE_CONNECT' has occured");
            break;
        case MQTT_EVENT_DELETED:
            LOG_INFO("Timeout occured and message has been deleted from internal outbox");
            break;
        default:
            LOG_INFO("Another event %d", eventId);
            break;
    }
}

bool MqttClientController::start() // NOLINT - we don't want to make it const
{
    esp_err_t errorCode = esp_mqtt_client_start(m_mqttClient);
    if (errorCode != ESP_OK)
    {
        commons::printErrorCode(errorCode, "Error while starting Mqtt Client");
        return false;
    }

    return true;
}

void MqttClientController::stop() // NOLINT - we don't want to make it const
{
    esp_mqtt_client_stop(m_mqttClient);
}

bool MqttClientController::sendMessage(
    const std::string& topic,
    const std::string& message) // NOLINT - we don't want to make it const
{
    LOG_INFO("sendMessage, topic: %s, payload: %s", topic.c_str(), message.c_str());
    int msgId = esp_mqtt_client_publish(m_mqttClient, topic.c_str(), message.c_str(), 0, 1, 0);
    if (msgId == MQTT_CLIENT_ERROR_PUBLISH)
    {
        LOG_ERROR("Error while sending message");
        return false;
    }
    else
    {
        return true;
    }
}

bool MqttClientController::checkIfMessageBufferIsEmpty() const
{
    return m_messageQueue.empty();
}

json_parser::TMessage MqttClientController::getNextMessage()
{
    json_parser::TMessage messageStruct = {
        .message   = "",
        .requestId = -1,
    };

    if (checkIfMessageBufferIsEmpty())
    {
        LOG_INFO("Message buffer empty, could not read anything");
        return messageStruct;
    }
    else
    {
        LOCK_GUARD(m_messageQueueMutex, messageQueueMutex);
        messageStruct = m_messageQueue.front();
        m_messageQueue.pop();
        return messageStruct;
    }
}

bool MqttClientController::isMqttConnected() const
{
    return m_connectionStatus;
}

bool MqttClientController::waitUntilMqttConnected(int32_t timeoutMs) const
{
    constexpr int32_t SLEEP_TIME_BETWEEN_CONNECTION_CHECKS = 100;
    int32_t           counter                              = 0;
    while (!isMqttConnected())
    {
        counter += SLEEP_TIME_BETWEEN_CONNECTION_CHECKS;
        if (timeoutMs > 0 && counter > timeoutMs)
        {
            LOG_INFO("Could not connect to cloud, timeout occured");
            return false;
        }
        SLEEP_MS(SLEEP_TIME_BETWEEN_CONNECTION_CHECKS);
    }

    return true;
}

const esp_mqtt_client_handle_t& MqttClientController::getMqttClient() // NOLINT we don't want to make it const
{
    return m_mqttClient;
}

bool MqttClientController::subscribeToTopic(const std::string& topic, int qos) const
{
    if (isMqttConnected())
    {
        int errorCode = esp_mqtt_client_subscribe(m_mqttClient, topic.c_str(), qos);
        if (errorCode == MQTT_CLIENT_SUBSCRIPTION_ERROR)
        {
            LOG_ERROR("Error occured, could not subscribe to the topic");
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        LOG_INFO("MQTT Client not connected, could not subscribe to the topic");
        return false;
    }
}

void MqttClientController::setConnectionStatus(bool status)
{
    m_connectionStatus = status;
}

void MqttClientController::handleErrorCodes(const esp_mqtt_error_codes_t* errorHandle)
{
    commons::printErrorCode(errorHandle->esp_tls_last_esp_err, "Error reported from esp-tls occured");
    commons::printErrorCode(errorHandle->esp_tls_stack_err, "Error reported from tls stack occured");
    commons::printErrorCode(errorHandle->esp_transport_sock_errno, "Captured as transport's socket errno");
}
