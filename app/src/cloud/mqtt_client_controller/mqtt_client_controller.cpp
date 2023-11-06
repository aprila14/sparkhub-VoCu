// Please keep these 2 lines at the beginning of each cpp module
static const char* LOG_TAG = "MqttClient";
#define LOG_LOCAL_LEVEL ESP_LOG_INFO


#include "mqtt_client_controller.h"

#include "app_controller.h"
#include "commons.h"
#include "config_nvs.h"
#include "json_parser.h"
#include "sleep.h"

namespace
{

const std::string TOPIC_RPC_REQUESTS_FROM_CLOUD = std::string("v1/devices/me/rpc/request/+");
const std::string TOPIC_RPC_REQUESTS_FROM_CLOUD_AWS = std::string("v1/devices/me/rpc/request/100");
const std::string TOPIC_STATUS_REPORT_RESPONSES = std::string("v1/devices/me/rpc/response/2");
const std::string TOPIC_HEARTBEAT_RESPONSES = std::string("v1/devices/me/rpc/response/5");

constexpr uint8_t MAX_QUEUE_SIZE = 10;
constexpr int MQTT_CLIENT_ERROR_PUBLISH = -1;
constexpr int MQTT_CLIENT_SUBSCRIPTION_ERROR = -1;
constexpr int MQTT_CLIENT_UNSUBSCRIBE_ERROR = -1;

void _mqttEventHandler(void* handlerArgs, esp_event_base_t base, int32_t eventId, void* eventData)
{
    MqttClientController* pMqttClientController = reinterpret_cast<MqttClientController*>(handlerArgs); // NOLINT - we need reinterpret cast
    pMqttClientController->eventHandler(pMqttClientController, base, eventId, eventData);
}

} // unnamed namespace

MqttClientController::MqttClientController(CloudController* cloudController)
:
m_connectionStatus(false),
m_messageQueue(),
m_mqttClient(nullptr),
m_taskHandle(nullptr)
{
    m_pCloudController = cloudController;
    m_messageQueueMutex = mutexCreate();
}

void MqttClientController::runTask()
{
    LOG_INFO("runTask() started...");

    BaseType_t xReturned = xTaskCreate(
                run,
                LOG_TAG,
                DEFAULT_HUGE_STACK_SIZE,
                this,
                DEFAULT_TASK_PRIORITY,
                &m_taskHandle);
    if (xReturned != pdPASS)
        LOG_ERROR("Failed to create a task: %s", LOG_TAG);
}

void MqttClientController::run(void* pObject)
{
    MqttClientController* pMqttClientController = reinterpret_cast<MqttClientController*>(pObject);  // NOLINT
    pMqttClientController->_run();
}

void MqttClientController::_run()
{
    while(true)
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

bool MqttClientController::interpretMessage(
        const json_parser::TMessage& messageStruct,
        json_parser::EMsgMethod msgMethod,
        json_parser::TFrame* frame)
{
    switch (msgMethod)
    {
        case json_parser::EMsgMethod::MSG_METHOD_RPC_COMMAND:
        {
            json_parser::printFrame(*frame);
            return handleFrame(*frame);
        }

        case json_parser::EMsgMethod::MSG_METHOD_GET_LIGHT_INTENSITY:
        {
            m_pCloudController->handleWidgetGetLightIntensityLevel(messageStruct.requestId);
            LOG_INFO("Request Id: %d", messageStruct.requestId);
            break;
        }

        case json_parser::EMsgMethod::MSG_METHOD_SET_LIGHT_INTENSITY:
        {
            bool result = m_pCloudController->handleWidgetSetLightIntensityLevel(frame->frameData.setLightLevelStruct, messageStruct.requestId);

            if (!result)
            {
                LOG_INFO("Could not set light intensity to the desired level");
                return false;
            }
            break;
        }

        case json_parser::EMsgMethod::MSG_METHOD_FAILED:
        {
            LOG_INFO("It was not possible to parse the incoming message, MSG_METHOD_FAILED");
            return false;
        }

        case json_parser::EMsgMethod::MSG_METHOD_UNKNOWN:
        {
            LOG_INFO("It was not possible to intrpret the incoming message, MSG_METHOD_FAILED");
            return false;
        }

        default:
        {
            LOG_INFO("Unlisted message method, could not be parsed");
            return false;
        }
    }

    return true;
}

void MqttClientController::handleMessages()
{
    while (!(checkIfMessageBufferIsEmpty()))
    {
        json_parser::TMessage newMessageStruct = getNextMessage();
        std::string newMessage = newMessageStruct.message;
        json_parser::TFrame newFrame = {};
        json_parser::EMsgMethod msgMethod = json_parser::extractMethodAndFillFrame(newMessage, &newFrame);


        LOG_INFO("msgMethod: %s", json_parser::getMsgMethodString(msgMethod).c_str());

        bool success = interpretMessage(newMessageStruct, msgMethod, &newFrame);

        if (!success)
        {
            LOG_INFO("It was not possible to interpret the incoming message");
            continue;
        }
        else
        {
            LOG_INFO("Message interpreted correctly");
        }
    }
}

bool MqttClientController::handleFrame(const json_parser::TFrame& frame) // NOLINT - we don't want to make it const
{
    bool result = false;
    switch(frame.msgCode)
    {
        case json_parser::EMsgCode::MSG_SET_LIGHT_INTENSITY_LEVEL:
            result = m_pCloudController->handleSetLightIntensityLevel(frame.frameData.setLightLevelStruct);
            if (!result)
            {
                LOG_INFO("Could not set light intensity level");
                return false;
            }
            break;
        case json_parser::EMsgCode::MSG_SET_LIGHT_INTENSITY_LEVEL_RESPONSE:
            LOG_INFO("Unexpected event occured, this message should be sent from ESP to cloud, not the other way round");
            return false;
        case json_parser::EMsgCode::MSG_STATUS_REPORT:
            LOG_INFO("Unexpected event occured, this message should be sent from ESP to cloud, not the other way round");
            return false;
        case json_parser::EMsgCode::MSG_STATUS_REPORT_RESPONSE:
            m_pCloudController->handleStatusReportResponse(frame.frameData.responseStruct.ACK);
            break;
        case json_parser::EMsgCode::MSG_HEARTBEAT:
            LOG_INFO("Unexpected event occured, this message should be sent from ESP to cloud, not the other way round");
            return false;
        case json_parser::EMsgCode::MSG_HEARTBEAT_RESPONSE:
            m_pCloudController->handleHeartbeatResponse();
            break;
        case json_parser::EMsgCode::MSG_GET_LIGHT_INTENSITY_LEVEL:
            break;
        case json_parser::EMsgCode::MSG_OTA_UPDATE_LINK:
        {
            LOG_INFO("Handling frame MSG_OTA_UPDATE_LINK");
            m_pCloudController->handleOtaUpdateLink(frame.frameData.otaUpdateLinkStruct);
            break;
        }

        case json_parser::EMsgCode::MSG_OTA_UPDATE_LINK_RESPONSE:
        {
            LOG_INFO("Unexpected event occured, this message should be sent from ESP to cloud, not the other way round");
            return false;
        }

        case json_parser::EMsgCode::MSG_TIME_SLOTS_LIST:
        {
            LOG_INFO("Handling frame MSG_TIME_SLOTS_LIST");
            //TODO call appropiate CloudController function
            break;
        }

        case json_parser::EMsgCode::MSG_TIME_SLOTS_LIST_RESPONSE:
        {
            LOG_INFO("Unexpected event occured, this message should be sent from ESP to cloud, not the other way round");
            return false;
        }
        default:
        {
            LOG_INFO("Cannot handle the frame, unknown msgCode");
            return false;
        }
    }

    return true;
}

void MqttClientController::subscribeToRequiredTopics()  // NOLINT - we don't want to make it const
{
#if BUILD_WITH_THINGSBOARD
    subscribeToTopic(TOPIC_RPC_REQUESTS_FROM_CLOUD, 2);
    subscribeToTopic(TOPIC_HEARTBEAT_RESPONSES, 1);
    subscribeToTopic(TOPIC_STATUS_REPORT_RESPONSES, 1);
#else
    std::string rpcRequestTopicAws = m_pCloudController->getClientUuid() + std::string("/requests");
    std::string heartbeatResponseTopicAws = m_pCloudController->getClientUuid() + std::string("/heartbeat/response");
    std::string deviceStatusResponseTopicAws = m_pCloudController->getClientUuid() + std::string("/deviceStatus/response");

    subscribeToTopic(rpcRequestTopicAws, 1);
    subscribeToTopic(heartbeatResponseTopicAws, 1);
    subscribeToTopic(deviceStatusResponseTopicAws, 1);
#endif // BUILD_WITH_THINGSBOARD
}

bool MqttClientController::init(const prot::cloud_set_credentials::TCloudCredentials& credentials)
{
    esp_mqtt_client_config_t mqttConfig = getClientConfiguration(credentials);
    m_mqttClient = esp_mqtt_client_init(&mqttConfig);

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

esp_mqtt_client_config_t MqttClientController::getClientConfiguration(const prot::cloud_set_credentials::TCloudCredentials& credentials) // NOLINT - we don't want to make it const
{
    const prot::cloud_set_credentials::TCloudCertificatePack& cloudCertificatePack = pConfig->getCloudCertificates();

    esp_mqtt_client_config_t mqttConfig = {};
    mqttConfig.uri = credentials.cloudAddress;

    mqttConfig.cert_pem = cloudCertificatePack.serverPublicCertificate;
    mqttConfig.client_cert_pem = cloudCertificatePack.clientPublicCertificate;
    mqttConfig.client_key_pem = cloudCertificatePack.clientPrivateKey;

    return mqttConfig;
}

void MqttClientController::eventHandler(void* handlerArgs, esp_event_base_t base, int32_t eventId, void* eventData)
{
    esp_mqtt_event_handle_t event = static_cast<esp_mqtt_event_handle_t>(eventData);
    esp_mqtt_client_handle_t client = event->client;
    json_parser::TMessage messageStruct = {};
    int32_t requestId = -1;

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

            LOG_DEBUG("Message payload: \n%.*s", event->data_len, event->data);
            LOG_DEBUG("Message length: %d", event->data_len);
            LOG_DEBUG("Message topic: %.*s", event->topic_len, event->topic);
            LOG_DEBUG("Topic length: %d", event->topic_len);

            if (event->topic_len == 0)
            {
                LOG_INFO("Received message was too long and it was divided into two chunks, ESP32 not able to parse it, it will be rejected");
                break;
            }
            if (m_messageQueue.size() > MAX_QUEUE_SIZE)
            {
                LOG_INFO("Not possible to add message to queue, queue size exceeded ");
                break;
            }
            std::string newMessage = std::string(event->data, 0, event->data_len);
            requestId = json_parser::extractRequestIdFromTopic(std::string(event->topic,
                                                                           0,
                                                                           event->topic_len));
            {
                LOCK_GUARD(m_messageQueueMutex, messageQueueMutex);
                messageStruct.message = newMessage;
                messageStruct.requestId = requestId;
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

void MqttClientController::stop()   // NOLINT - we don't want to make it const
{
    esp_mqtt_client_stop(m_mqttClient);
}

bool MqttClientController::sendMessage(const std::string &topic, const std::string& message) // NOLINT - we don't want to make it const
{
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

bool MqttClientController::sendWidgetLightIntensity(const json_parser::TSetLightLevel& setLightLevelStructure, int32_t requestId)
{
    std::string message = std::to_string(setLightLevelStructure.lightIntensityLevel);
    std::string topic = std::string("v1/devices/me/rpc/response/") + std::to_string(requestId);

    LOG_INFO("Topic: %s, message: %s", topic.c_str(), message.c_str());

    bool result = sendMessage(topic, message);

    return result;
}

json_parser::TMessage MqttClientController::getNextMessage()
{
    json_parser::TMessage messageStruct =
    {
        .message = "",
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

bool MqttClientController::waitUntilMqttConnected(uint32_t timeoutMs) const
{
    constexpr uint32_t SLEEP_TIME_BETWEEN_CONNECTION_CHECKS = 100;
    uint32_t counter = 0;
    while(!isMqttConnected())
    {
        counter+=SLEEP_TIME_BETWEEN_CONNECTION_CHECKS;
        if (counter > timeoutMs && timeoutMs > 0)
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
