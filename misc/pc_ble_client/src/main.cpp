#include "defines.h"
#include "misc.h"
#include "bleuart.h"
#include "protocol_control.h"
#include "ble_controller.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <pthread.h>


void* runBleThread(void* context)
{
    BleuartDriver* pBleuartDriver = (BleuartDriver*) context;
    if (!pBleuartDriver->setupBlz())
    {
        LOG_CRIT("Cannot start BLE module");
        pBleuartDriver->deinit();
        return NULL;
    }

    while (1)
    {
        LOG_INFO("About to connect to BLE peripheral...");

        if (pBleuartDriver->connect())
        {
            pBleuartDriver->runLoop();
        }

        if (pBleuartDriver->shouldStop)
        {
            pBleuartDriver->deinit();
            LOG_WARN("Stop signal received, exiting thread");
            return NULL;
        }

        pBleuartDriver->cleanupConnection();

        LOG_WARN("Connection with BLE peripheral failed or closed. Retrying in 1 s...");
        sleep(1);
    }

    return NULL;
}


int main(int argc, char **argv)
{
    blz_set_log_handler(blzlibLogHandler);
    LOG_INF("Application started");

    const char* adapterName = "hci0";
    const char* targetMac = "7C:9E:BD:CF:05:AE";  // ESP32 MAC is printed at startup - copy it from there


    if (argc > 1)
        targetMac = argv[1];

    if (argc > 2)
        adapterName = argv[2];

    BleuartDriver bleuartDriver(adapterName, targetMac);

    pthread_t bleThread;
    pthread_create(&bleThread, NULL, runBleThread, &bleuartDriver);


    LOG_INFO("Waiting untill BLE is connected...");
    while (!bleuartDriver.isConnected)
    {
        usleep(10*1000);
    }
    LOG_INFO("Waiting untill BLE is connected - finished");


    BleController bleController(&bleuartDriver);
    bleController.runTask();

    LOG_INFO("Waiting for BLE thread to finish");
    bleuartDriver.shouldStop = true;
    pthread_join(bleThread, NULL);

    LOG_INFO("Apllication closed gracefully!");
    return 0;
}
