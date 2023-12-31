#ifndef MQTTCLIENTCONTROLLER_H
#define MQTTCLIENTCONTROLLER_H

#include "json_parser.h"
#include "protocol_types.h"

#include "esp_event_base.h"
#include "mqtt_client.h"
#include "mutex.h"
#include "stdint.h"
#include <queue>
#include <string>

class CloudController;

class MqttClientController
{

public:
    MqttClientController(CloudController* cloudController);

    /**
     * @brief Create a task for MQTT Client Controller
     */
    void runTask();

    /**
     * @brief Initialize MqttClientController with the provided credentials and register event handler for all MQTT
     * events
     */
    bool init(const prot::cloud_set_credentials::TCloudCredentials& credentials);

    /**
     * @brief Callback function executed when any of the MQTT events occur
     */
    void eventHandler(void* handlerArgs, esp_event_base_t base, int32_t eventId, void* eventData);

    /**
     * @brief Sends a message to the specified topic
     *
     */

    bool sendMessage(const std::string& topic, const std::string& message);

    /**
     * @brief Subscribes to a specific topic
     */
    bool subscribeToTopic(const std::string& topic, int qos) const;

    /**
     * @brief Checks the message queue for new messages and returns true if any of them are found
     */
    bool checkIfMessageBufferIsEmpty() const;

    /**
     * @brief Returns next message from the queue if it's not empty
     * @return
     */
    json_parser::TMessage getNextMessage();

    /**
     * @brief start Starts the Mqtt module. This function shall be called after WiFi connection is established
     */

    bool start();

    /**
     * @brief stop Stops the Mqtt module. This function shall be called whenever connection with cloud should be closed
     */

    void stop();

    /**
     * @brief Checks whether MQTT_CONNECT event has already occured. Underlying field is changed to 'false' when
     * MQTT_DISCONNECT event occurs
     * @return
     */
    bool isMqttConnected() const;

    /**
     * @brief Function waits until connection with MQTT broker is established with a given timeout
     */
    bool waitUntilMqttConnected(int32_t timeoutMs) const;

    /**
     * Function returning m_mqttClient - needed for the functions for OTA with AWS
     */
    const esp_mqtt_client_handle_t& getMqttClient();

    bool addTopicForSubscription(std::string topic, int qos);
    struct TMqttClientSubscription
    {
        std::string topic;
        int         qos;
    };

#if !TESTING
private:
#endif

    virtual esp_mqtt_client_config_t
    getClientConfiguration(const prot::cloud_set_credentials::TCloudCredentials& credentials);

    void setConnectionStatus(bool status);

    static void handleErrorCodes(const esp_mqtt_error_codes_t* errorHandle);
    void        perform();
    void        handleMessages();

    /**
     * @brief Function subscribing to all topics that are required for communication. It shall be called whenever
     * connection with MQTT broker is established
     */
    void subscribeToRequiredTopics();

    static void run(void* pObject);
    void        _run();

    static constexpr uint8_t MAX_TOPIC_SUBSCRIPTIONS = 5;

    bool m_connectionStatus; // boolean value representing if client is connected (true) or disconnected (false) from
                             // the broker

    mutex_t m_messageQueueMutex = nullptr; ///< Protects access to message queue

    std::queue<json_parser::TMessage> m_messageQueue; // in case any new message shows up it's added to this queue

    esp_mqtt_client_handle_t m_mqttClient; // handle to esp mqtt client - needed to call all lower-level functions

    TaskHandle_t m_taskHandle; // handle to runTask

    CloudController* m_pCloudController; // Pointer to CloudController - needed for calling it's functions TODO: change
                                         // to observer pattern

    uint32_t m_subscriptionCounter;

    TMqttClientSubscription m_topicsForSubscription[MAX_TOPIC_SUBSCRIPTIONS];
};

#endif // MQTTCLIENTCONTROLLER_H
