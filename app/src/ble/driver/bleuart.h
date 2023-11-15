/*
 *  File copied from Nimble code, as it is not included in the ESP-IDF build, and adjusted to work with ESP32 instead of mynewt
 *  Copied from: esp-idf/components/bt/host/nimble/nimble/nimble/host/services/bleuart/include/bleuart/bleuart.h"

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

#ifndef _BLEUART_H_
#define _BLEUART_H_


#include "host/ble_uuid.h"


void bleuart_init(void);

int bleuart_svc_register(void);

int bleuart_gatt_svr_init(void);

void bleuart_set_conn_handle(uint16_t conn_handle);

extern const ble_uuid128_t gatt_svr_svc_uart_uuid;


#endif /* _BLEUART_H */
