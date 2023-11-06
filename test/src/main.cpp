#include "gtest/gtest.h"
#include "unity.h"

#include "app_controller_mock.h"
#include "config_nvs.h"
#include "config_nvs_mock.h"


int GlobalExpectCount = 0;
int GlobalVerifyOrder = 0;

void tearDown()
{

}

void setUp()
{

}


void createGlobalSingletons()
{
    // TOOD - consider creating them separatelly for each test case

//    static ConfigNvsMock configNvsMock;
    static testing::NiceMock<ConfigNvsMock> configNvsMock;
    pConfig = &configNvsMock;

    // just copy it to every interested unit test
    /*
    static testing::StrictMock<app::AppControllerMock> appController;
    app::pAppController = &appController;
    */
}

int main(int argc, char **argv) 
{   
    createGlobalSingletons();

    UnityBegin("Tests demo");
    UNITY_END();

    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    return result;
}
