
#include "defines.h"

#include "test_utils.h"
#include "custom_types.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "json_parser.h"

#include "cloud_controller.h"

#include "mock_mqtt_client.h"

using namespace testing;

class MqttClientControllerMock_1
        : public MqttClientController
{
    public:
    explicit MqttClientControllerMock_1(CloudController* cloudController)
    :
    MqttClientController(nullptr)
    {

    }
    MOCK_METHOD(esp_mqtt_client_config_t, getClientConfiguration, (const prot::cloud_set_credentials::TCloudCredentials& credentials), (override));
};

TEST(MqttClientController, initialization)
{
    resetCmock();

    prot::cloud_set_credentials::TCloudCredentials credentials = {};
    credentials.setCloudAddress("192.168.1.23");

    testing::StrictMock<MqttClientControllerMock_1> mqttClientController(nullptr);

    EXPECT_CALL(mqttClientController, getClientConfiguration(credentials)).WillOnce(Return(esp_mqtt_client_config_t()));

    esp_mqtt_client_init_IgnoreAndReturn(esp_mqtt_client_handle_t());
    esp_mqtt_client_register_event_IgnoreAndReturn(ESP_OK);

    ASSERT_TRUE(mqttClientController.init(credentials));
}

TEST(MqttClientController, start)
{
    resetCmock();
    MqttClientController mqttClientController(nullptr);

    esp_mqtt_client_start_IgnoreAndReturn(ESP_OK);

    ASSERT_TRUE(mqttClientController.start());
}

TEST(MqttClientController, addingMessagesToQueue)
{
    resetCmock();

    MqttClientController mqttClientController(nullptr);

    json_parser::TMessage messageStruct1 =
    {
        .message = "Test message 1",
        .requestId = 1
    };

    json_parser::TMessage messageStruct2 =
    {
        .message = "Test message 2",
        .requestId = 2
    };

    json_parser::TMessage messageStruct3 =
    {
        .message = "Test message 3",
        .requestId = 3
    };

    json_parser::TMessage messageStruct4 =
    {
        .message = "Test message 4",
        .requestId = 4
    };

    mqttClientController.m_messageQueue.push(messageStruct1);
    mqttClientController.m_messageQueue.push(messageStruct2);
    mqttClientController.m_messageQueue.push(messageStruct3);
    mqttClientController.m_messageQueue.push(messageStruct4);

    ASSERT_FALSE(mqttClientController.checkIfMessageBufferIsEmpty());


    json_parser::TMessage messageRead1 = mqttClientController.getNextMessage();
    json_parser::TMessage messageRead2 = mqttClientController.getNextMessage();
    json_parser::TMessage messageRead3 = mqttClientController.getNextMessage();
    json_parser::TMessage messageRead4 = mqttClientController.getNextMessage();

    ASSERT_STRCASEEQ(messageRead1.message.c_str(), messageStruct1.message.c_str());
    ASSERT_STRCASEEQ(messageRead2.message.c_str(), messageStruct2.message.c_str());
    ASSERT_STRCASEEQ(messageRead3.message.c_str(), messageStruct3.message.c_str());
    ASSERT_STRCASEEQ(messageRead4.message.c_str(), messageStruct4.message.c_str());

    ASSERT_EQ(messageRead1.requestId, messageStruct1.requestId);
    ASSERT_EQ(messageRead2.requestId, messageStruct2.requestId);
    ASSERT_EQ(messageRead3.requestId, messageStruct3.requestId);
    ASSERT_EQ(messageRead4.requestId, messageStruct4.requestId);

    ASSERT_TRUE(mqttClientController.checkIfMessageBufferIsEmpty());
}

TEST(MqttClientController, subscribeToTopicWhileUnconnected)
{
    MqttClientController mqttClientController(nullptr);
    ASSERT_FALSE(mqttClientController.subscribeToTopic(std::string("example/topic"), 1));
}

TEST(MqttClientController, subscribeToTopicWhileConnected)
{
    MqttClientController mqttClientController(nullptr);

    mqttClientController.m_connectionStatus = true;

    esp_mqtt_client_subscribe_IgnoreAndReturn(ESP_OK);

    ASSERT_TRUE(mqttClientController.subscribeToTopic(std::string("example/topic"), 1));
}

TEST(MqttClientController, isMqttConnected)
{
    MqttClientController mqttClientController(nullptr);
    mqttClientController.m_connectionStatus = true;

    ASSERT_TRUE(mqttClientController.isMqttConnected());
}

TEST(MqttClientController, isMqttNotConnected)
{
    MqttClientController mqttClientController(nullptr);
    mqttClientController.m_connectionStatus = false;

    ASSERT_FALSE(mqttClientController.isMqttConnected());
}

