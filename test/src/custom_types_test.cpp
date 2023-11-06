#include "custom_types.h"
#include "defines.h"
#include "test_utils.h"

#include "gtest/gtest.h"



TEST(CustomTypes, TWiFiCredentials_normalUsage)
{
    resetCmock();
    TWiFiCredentials wifiCredentials;
    ASSERT_EQ(wifiCredentials.ssid[0], 0);
    ASSERT_EQ(wifiCredentials.password[0], 0);

    ASSERT_FALSE(wifiCredentials.setPassword("extraaaaaaaaaaaaaaaaaaa looooooooooooooonggggggggggggggggggg "
                                             "paaaaaaaaaaaaaawwwwwwwwwoooorddd"));
    ASSERT_FALSE(wifiCredentials.setSsid("extraaaaa long ssid 1212121212121212121212121212121212121"));

    ASSERT_TRUE(wifiCredentials.setSsid("correctSSID"));
    ASSERT_TRUE(wifiCredentials.setPassword("correctPassword"));

    ASSERT_EQ(std::string(wifiCredentials.ssid), "correctSSID");
    ASSERT_EQ(std::string(wifiCredentials.password), "correctPassword");


    TWiFiCredentials wifiCredentials2;
    ASSERT_FALSE(wifiCredentials2 == wifiCredentials);

    wifiCredentials2.setPassword("correctPassword");
    ASSERT_FALSE(wifiCredentials2 == wifiCredentials);


    ASSERT_TRUE(wifiCredentials2.setSsid("correctSSID"));
    ASSERT_TRUE(wifiCredentials2 == wifiCredentials);
}
