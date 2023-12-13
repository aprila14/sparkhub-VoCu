/*
 File partially based on an examplary Nimble application:
 https://github.com/apache/mynewt-core/blob/master/apps/bleuart/src/main.c Functions from that file are marked with a
 comment

 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

// Please keep these 2 lines at the beginning of each cpp module - tag and local log level
static const char* LOG_TAG = "BLEUART_ESP";
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG
// NOTE: control Nimble logging level in modlog.h

#include "bleuart_esp.h"

#include "bleuart.h"
#include "commons.h"
#include "defines.h"
#include "sleep.h"

#include "esp_nimble_hci.h"
#include "host/ble_att.h"
#include "host/ble_gap.h"
#include "host/ble_gatt.h"
#include "host/ble_hs.h"
#include "host/ble_hs_adv.h"
#include "host/ble_l2cap.h"
#include "host/ble_sm.h"
#include "host/ble_uuid.h"
#include "nimble/ble.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"

#include "host/ble_uuid.h"
#ifndef BLE_NIMBLE_PATCH_WAS_APPLIED__V1
#error \
    "ERROR! Required patch was not appllied! please consult README.md and clean_repo_and_patch.sh script on how to apply required patches"
#endif

#include <cstdio>
#include <cstring>

#define CONFIG_BLE_DEVICE_NAME "sparkhub-levelSense"

constexpr uint16_t BLEUART_ESP_NUMBER_OF_TRANSMISSION_BUFFERS = (CONFIG_BT_NIMBLE_ACL_BUF_COUNT - 1); // -1 for safety
constexpr uint16_t BLEUART_ESP_TRANSMISSION_BUFFER_SIZE =
    (CONFIG_BT_NIMBLE_ACL_BUF_SIZE - 5); // theoretically -3 is enough
constexpr uint16_t BLEUART_RX_CIRCULAR_BUFFER_SIZE = (MAX_BLE_PACKET_SIZE * 1.3);
constexpr uint16_t BLEUART_TX_CIRCULAR_BUFFER_SIZE = (MAX_BLE_PACKET_SIZE * 1.2);

// Nimble global parameters - TODO - consider if it is worth refactoring to get rid of global parameters completely
// But is it worth the effort at all?
// (but it would require refactorization of bleuart.c - nimble code)

#ifdef __cplusplus
extern "C"
{
#endif
    void ble_store_config_init(void);
#ifdef __cplusplus
}
#endif

extern uint16_t g_console_conn_handle;
extern uint16_t g_bleuart_attr_read_handle;

uint8_t* bleuartRxData = nullptr;
uint8_t* bleuartTxData = nullptr;

BleuartDriver* g_pBleuartDriver = nullptr;

BleuartDriver::BleuartDriver() : m_txLoopTaskHandle(nullptr), m_isSynced(false), m_isClientConnected(false)
{
    bleuartRxData = new uint8_t[BLEUART_RX_CIRCULAR_BUFFER_SIZE];
    if (bleuartRxData == nullptr)
    {
        LOG_ERROR("Failed to allocate memory for bleuartRxCircularBuffer");
    }

    bleuartTxData = new uint8_t[BLEUART_TX_CIRCULAR_BUFFER_SIZE];
    if (bleuartTxData == nullptr)
    {
        LOG_ERROR("Failed to allocate memory for bleuartTxData");
    }

    m_rxBuffer = new CircularBuffer(bleuartRxData, BLEUART_RX_CIRCULAR_BUFFER_SIZE);
    m_txBuffer = new CircularBuffer(bleuartTxData, BLEUART_TX_CIRCULAR_BUFFER_SIZE);

    m_txDataAvailable = xSemaphoreCreateBinary();

    m_rxBufferMutex = xSemaphoreCreateBinary();
    xSemaphoreGive(m_rxBufferMutex);

    m_rxSomeDataAvailableSemaphore = semaphoreCreateCounting();

    m_txBufferMutex = xSemaphoreCreateBinary();
    xSemaphoreGive(m_txBufferMutex);

    m_clientConnectedSemaphore = xSemaphoreCreateBinary();
}

BleuartDriver::~BleuartDriver()
{
    delete bleuartRxData;
    delete bleuartTxData;

    delete m_rxBuffer;
    delete m_txBuffer;
}

bool BleuartDriver::writeData(const uint8_t* pData, uint32_t dataLength)
{
    if (!m_isClientConnected)
    {
        LOG_ERROR("BLE client is not connected, cannot write data!");
        return false;
    }

    xSemaphoreTake(m_txBufferMutex, portMAX_DELAY);
    bool result = m_txBuffer->writeData(pData, dataLength);
    xSemaphoreGive(m_txBufferMutex);

    if (result)
    {
        LOG_DEBUG("Data queued for transmission - %u bytes", dataLength);
        xSemaphoreGive(m_txDataAvailable);
    }
    else
    {
        LOG_WARNING("Failed to queue data for TX transmission - TX buffer is full!");
        return false;
    }

    return true;
}

int BleuartDriver::readByte(bool blockUntilDataAvailable)
{
    if (blockUntilDataAvailable)
    {
        LOG_DEBUG("Waiting for rxDataAvailableSemaphore...");
        seamphoreTake(m_rxSomeDataAvailableSemaphore);
        LOG_DEBUG("Waiting for rxDataAvailableSemaphore finished");
    }

    int byte = CIRCULAR_BUFFER__NO_DATA;

    {
        LOCK_GUARD(m_rxBufferMutex, rxBuferMutexGuard);
        int byte = m_rxBuffer->readByte();

        if (m_rxBuffer->isDataAvailable())
        {
            if (!isSemaphoreGiven(m_rxSomeDataAvailableSemaphore))
                semaphoreGive(m_rxSomeDataAvailableSemaphore);
        }

        return byte;
    }

    return byte;
}

int BleuartDriver::getFreeTxBufferSize()
{
    xSemaphoreTake(m_rxBufferMutex, portMAX_DELAY);
    uint32_t freeSize = m_txBuffer->getFreeSize();
    xSemaphoreGive(m_rxBufferMutex);

    return (int)freeSize;
}

/**
 * @brief global function, required to be called from other module that we don't really control
 */
void bleuart_handleIncomingData(const uint8_t* pData, uint32_t dataSize)
{
    g_pBleuartDriver->handleIncomingData(pData, dataSize);
}

void BleuartDriver::handleIncomingData(const uint8_t* pData, uint32_t dataSize)
{
#ifdef IS_DEBUG_BUILD
    std::string txt = commons::dataArrayToHexStr(128, pData, (uint32_t)dataSize);
    LOG_INFO("handleIncomingData, size %u: %s", dataSize, txt.c_str());
#else
    LOG_DEBUG("handleIncomingData, size %u", dataSize);
#endif

    {
        LOCK_GUARD(m_rxBufferMutex, rxBuferMutexGuard);
        if (!m_rxBuffer->writeData(pData, dataSize))
        {
            LOG_WARNING("RX Circular buffer overflow - some packets may be lost!");
        }
        else
        {
            LOG_DEBUG("Giving rxSomeDataAvailableSemaphore...");
            if (!isSemaphoreGiven(m_rxSomeDataAvailableSemaphore))
                semaphoreGive(m_rxSomeDataAvailableSemaphore);
        }
    }
}

void BleuartDriver::waitUntilClientConnected()
{
    LOG_INFO("bleuart_waitUntilClientConnected started...");

    if (m_isClientConnected)
    {
        LOG_INFO("Client already conected, not waiting");
        return;
    }

    seamphoreTake(m_clientConnectedSemaphore);

    LOG_INFO("BLE client finally connected!");
    SLEEP_MS(300); // TODO - determine what time is requried to be slept before data sending works after establishing
                   // connection (or if there is some flag for that...)
}

bool BleuartDriver::isClientConnected() const
{
    return m_isClientConnected;
}

void BleuartDriver::printBleMacAddress()
{
    uint8_t macAddress[6] = {0};
    bool    result        = ble_hs_id_copy_addr(BLE_ADDR_PUBLIC, macAddress, NULL);
    if (result != 0)
    {
        LOG_ERROR("Failed to print the EPS BLE address!");
    }

    printf(
        "BLE MAC = %02X:%02X:%02X:%02X:%02X:%02X \n",
        macAddress[5],
        macAddress[4],
        macAddress[3],
        macAddress[2],
        macAddress[1],
        macAddress[0]);
}

int BleuartDriver::handleGapEvent(struct ble_gap_event* event, void* arg)
{
    BleuartDriver* pDriver = reinterpret_cast<BleuartDriver*>(arg); // NOLINT we need reinterpret cast
    return pDriver->_handleGapEvent(event);
}

int BleuartDriver::_handleGapEvent(struct ble_gap_event* event)
{
    struct ble_gap_conn_desc desc = {};
    int                      rc   = 0;
#ifndef IS_DEBUG_BUILD
    UNUSED(rc);
#endif

    LOG_DEBUG("BLE_GAP_EVENT %d", event->type);

    switch (event->type)
    {
        case BLE_GAP_EVENT_NOTIFY_TX:
            LOG_DEBUG("BLE_GAP_EVENT_NOTIFY_TX");
            break;

        case BLE_GAP_EVENT_NOTIFY_RX:
            LOG_DEBUG("BLE_GAP_EVENT_NOTIFY_RX");
            break;

        case BLE_GAP_EVENT_CONNECT:
            LOG_INFO("BLE_GAP_EVENT_CONNECT");
            /* A new connection was established or a connection attempt failed. */
            if (event->connect.status == 0)
            {
                rc = ble_gap_conn_find(event->connect.conn_handle, &desc);
                DEBUG_ASSERT(rc == 0);
                bleuart_set_conn_handle(event->connect.conn_handle);
            }

            if (event->connect.status != 0)
            {
                /* Connection failed; resume advertising. */
                advertise();
            }
            m_isClientConnected = true;
            semaphoreGive(m_clientConnectedSemaphore);
            return 0;

        case BLE_GAP_EVENT_DISCONNECT:
            xSemaphoreTake(m_clientConnectedSemaphore, 0);
            m_isClientConnected = false;
            LOG_INFO("BLE_GAP_EVENT_DISCONNECT");
            /* Connection terminated; resume advertising. */
            advertise();
            return 0;

        case BLE_GAP_EVENT_ADV_COMPLETE:
            LOG_INFO("BLE_GAP_EVENT_ADV_COMPLETE");
            /* Advertising terminated; resume advertising. */
            advertise();
            return 0;

        case BLE_GAP_EVENT_REPEAT_PAIRING:
            LOG_INFO("BLE_GAP_EVENT_REPEAT_PAIRING");
            /* We already have a bond with the peer, but it is attempting to
             * establish a new secure link.  This app sacrifices security for
             * convenience: just throw away the old bond and accept the new link.
             */

            /* Delete the old bond. */
            rc = ble_gap_conn_find(event->repeat_pairing.conn_handle, &desc);
            DEBUG_ASSERT(rc == 0);
            ble_store_util_delete_peer(&desc.peer_id_addr);

            /* Return BLE_GAP_REPEAT_PAIRING_RETRY to indicate that the host should
             * continue with the pairing operation.
             */
            return BLE_GAP_REPEAT_PAIRING_RETRY;
    }

    return 0;
}

void BleuartDriver::advertise() // NOLINT not static
{
    struct ble_gap_adv_params adv_params = {};
    struct ble_hs_adv_fields  fields     = {};
    int                       rc         = 0;

    /*
     *  Set the advertisement data included in our advertisements:
     *     o Flags (indicates advertisement type and other general info).
     *     o Advertising tx power.
     *     o 128 bit UUID
     */

    memset(&fields, 0, sizeof fields);

    /* Advertise two flags:
     *     o Discoverability in forthcoming advertisement (general)
     *     o BLE-only (BR/EDR unsupported).
     */
    fields.flags = BLE_HS_ADV_F_DISC_GEN | // NOLINT - we don't control these flags
                   BLE_HS_ADV_F_BREDR_UNSUP;

    /* Indicate that the TX power level field should be included; have the
     * stack fill this value automatically.  This is done by assiging the
     * special value BLE_HS_ADV_TX_PWR_LVL_AUTO.
     */
    fields.tx_pwr_lvl_is_present = 1;
    fields.tx_pwr_lvl            = BLE_HS_ADV_TX_PWR_LVL_AUTO;

    fields.uuids128             = BLE_UUID128(&gatt_svr_svc_uart_uuid.u); // NOLINT - we don't control these macros
    fields.num_uuids128         = 1;
    fields.uuids128_is_complete = 1;

    rc = ble_gap_adv_set_fields(&fields);
    if (rc != 0)
    {
        return;
    }

    memset(&fields, 0, sizeof fields);
    fields.name = (uint8_t*)ble_svc_gap_device_name(); // NOLINT rabbit hole with const cast, don't touch this is not
                                                       // our code anyway and works
    fields.name_len = strlen(
        (char*)fields.name); //  NOLINT rabbit hole with const cast, don't touch this is not our code anyway and works
    fields.name_is_complete = 1;

    rc = ble_gap_adv_rsp_set_fields(&fields);
    if (rc != 0)
    {
        return;
    }

    /* Begin advertising. */
    memset(&adv_params, 0, sizeof adv_params);
    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;
    rc                   = ble_gap_adv_start(
        BLE_OWN_ADDR_PUBLIC, NULL, BLE_HS_FOREVER, &adv_params, BleuartDriver::handleGapEvent, g_pBleuartDriver);
    if (rc != 0)
    {
        return;
    }
}

void BleuartDriver::synchronizationCallback()
{
    g_pBleuartDriver->_synchronizationCallback();
}

void BleuartDriver::_synchronizationCallback()
{
    LOG_INFO("bleuartSynchronizationCallback");
    m_isSynced = true;

    /* Begin advertising. */
    advertise();
}

/**
 * @brief Run function for the BLE task
 * @param param - user data
 */
static void _runBlePrphHostTask(void* param)
{
    UNUSED(param);

    LOG_INFO("BLE Host Task Started");

    /* This function will return only when nimble_port_stop() is executed */
    nimble_port_run();

    nimble_port_freertos_deinit();
}

/**
 * @brief transmitSingleBuffer Transmit a single buffer
 * @param pData Array to transmit
 * @param dataLength Length of the array to transmit
 * @return True on success, false otherwise
 */
static bool _transmitSingleBuffer(const uint8_t* pData, uint32_t dataLength)
{
    DEBUG_ASSERT(dataLength <= BLEUART_ESP_TRANSMISSION_BUFFER_SIZE);

    struct os_mbuf* om = ble_hs_mbuf_from_flat(pData, (uint16_t)dataLength);
    if (!om)
    {
        LOG_WARNING("Excessive TX transfer - no free buffer from ble_hs_mbuf_from_flat()");
        return false;
    }

#ifdef IS_DEBUG_BUILD
    std::string txt = commons::dataArrayToHexStr(128, pData, (uint32_t)dataLength);
    LOG_INFO("About to send a buffer with %u bytes: %s", dataLength, txt.c_str());
#endif

    int rc = ble_gattc_notify_custom(g_console_conn_handle, g_bleuart_attr_read_handle, om);
    if (rc != 0)
    {
        LOG_WARNING("Excessive TX transfer - failed to execute ble_gattc_notify_custom!");
        return false;
    }

    return true;
}

/**
 * @brief transmitSingleBuffer Transmit a single buffer
 * @param pData Array to transmit
 * @param dataLength Length of the array to transmit
 * @param maxRetries Max number of transmission retrying before giving up
 * @return True on success, false otherwise
 */
static bool _transmitSingleBufferWithRetries(const uint8_t* pData, uint32_t dataLength, int maxRetries)
{
    int sendingIteration = 0;

    for (int i = 0; i < maxRetries; i++)
    {
        sendingIteration++;
        bool dataSent = _transmitSingleBuffer(pData, dataLength);
        if (dataSent)
        {
            LOG_DEBUG("Buffer with %d bytes passed sucesfully for transmission!", dataLength);
            return true;
        }

        if (i == (maxRetries / 10))
        {
            LOG_WARNING("Buffer still not transmitted after %d retires!", i);
        }
        else
        {
            LOG_INFO("Failed to sent a single buffer with %d bytes. Retrying in 10 ms", dataLength);
        }

        SLEEP_MS(30); // chosen arbitraly, after some testing
    }

    return false;
}

void BleuartDriver::performTxLoop(void* pBleuartDriverVoid)
{
    BleuartDriver* pDriver = reinterpret_cast<BleuartDriver*>(pBleuartDriverVoid); // NOLINT we need reinterpret cast
    return pDriver->_performTxLoop();
}

void BleuartDriver::_performTxLoop()
{
    LOG_INFO("performTxLoop started...");
    uint8_t transmissionBuffer[BLEUART_ESP_TRANSMISSION_BUFFER_SIZE] = {
        0}; ///< Used as a working buffer to prepare a BLE transmission data

    const int MAX_NUMBER_OF_BUFFER_TRANSMISSION_BEFORE_SLEEP = 10;
    int buffersSentInRow = 0; ///< Count buffers sent without any sleep. We don't want to send too much buffers at once
                              ///< - it would consume the whole CPU

    xSemaphoreTake(m_txDataAvailable, portMAX_DELAY);
    while (1)
    {
        uint32_t bytesToSend = 0;

        xSemaphoreTake(m_txBufferMutex, portMAX_DELAY);
        LOG_DEBUG("Preparing data buffer for transmission...");
        for (bytesToSend = 0; bytesToSend < BLEUART_ESP_TRANSMISSION_BUFFER_SIZE; bytesToSend++)
        {
            int byte = m_txBuffer->readByte();
            if (byte == CIRCULAR_BUFFER__NO_DATA)
            {
                LOG_DEBUG("TX loop - No more data in TX buffer (already %d bytes in buffer)!", bytesToSend);
                break;
            }
            else
            {
                transmissionBuffer[bytesToSend] = (uint8_t)byte;
            }
        }
        xSemaphoreGive(m_txBufferMutex);

        if (bytesToSend == 0)
        {
            LOG_INFO("TX loop - No more data to send, going back to sleep!");
        }
        else
        {
            // Some data need to be sent

            if (!_transmitSingleBufferWithRetries(transmissionBuffer, bytesToSend, 100))
                LOG_ERROR("Failed to transit a single buffer with %d bytes. Data will be lost!", bytesToSend);

            if (bytesToSend < BLEUART_ESP_TRANSMISSION_BUFFER_SIZE)
            {
                LOG_DEBUG("TX loop - All bytes in TX buffer sent, go back to sleep...");
            }
            else
            {
                buffersSentInRow++;
                if (buffersSentInRow == MAX_NUMBER_OF_BUFFER_TRANSMISSION_BEFORE_SLEEP)
                {
                    LOG_DEBUG("TX loop - Excessive TX transmission - halting transmission for 10 ms");
                    SLEEP_MS(10);
                    buffersSentInRow = 0;
                }

                LOG_DEBUG("TX loop - Still more data waiting for transmission");
                continue;
            }
        }

        xSemaphoreTake(m_txDataAvailable, portMAX_DELAY);
    }
}

bool BleuartDriver::runTask()
{
    LOG_DEBUG("bleuart_initAndStartThread started...");

    esp_err_t espError = esp_nimble_hci_and_controller_init();
    if (espError != ESP_OK)
    {
        LOG_ERROR("esp_nimble_hci_and_controller_init failed with code %d", espError);
        return false;
    }

    nimble_port_init();

    /* Initialize the BLE host. */
    ble_hs_cfg.sync_cb         = BleuartDriver::synchronizationCallback;
    ble_hs_cfg.store_status_cb = ble_store_util_status_rr;

    int rc = bleuart_gatt_svr_init();
    if (rc != 0)
    {
        LOG_ERROR("bleuart_gatt_svr_init failed with code %d", rc);
        return false;
    }

    /* Set the default device name. */
    rc = ble_svc_gap_device_name_set(CONFIG_BLE_DEVICE_NAME);
    if (rc != 0)
    {
        LOG_ERROR("ble_svc_gap_device_name_set failed with code %d", rc);
        return false;
    }

    ble_store_config_init();

    LOG_DEBUG("About to call nimble_port_freertos_init()");
    nimble_port_freertos_init(_runBlePrphHostTask);

    LOG_INFO("Waiting for bleUartControl.isSynced...");
    while (!m_isSynced)
    {
        SLEEP_MS(20);
    }
    SLEEP_MS(500); // TODO - determine if additional sleep required

    BleuartDriver::printBleMacAddress();

    BaseType_t xReturned = xTaskCreate(
        BleuartDriver::performTxLoop,
        "bleuart_tx_loop",
        DEFAULT_STACK_SIZE * 2, // one default stack size is not enough here
        this,
        DEFAULT_TASK_PRIORITY,
        &m_txLoopTaskHandle);
    if (xReturned != pdPASS)
    {
        LOG_ERROR("Failed to create task 'bleuart_tx'!");
        return false;
    }

    LOG_INFO("bleuart_initAndStartThread finished successfully");
    return true;
}
