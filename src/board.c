/* board.c - Board-specific hooks */

/*
 * SPDX-FileCopyrightText: 2017 Intel Corporation
 * SPDX-FileContributor: 2018-2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include "esp_log.h"
#include "esp_ble_mesh_sensor_model_api.h"

#include "driver/gpio.h"
#include "board.h"
#define TAG "BOARD"

#define BUTTON_IO_NUM 0
#define BUTTON_ACTIVE_LEVEL 0
#define BLINK_GPIO 14

extern void example_ble_mesh_send_sensor_message(uint32_t opcode);

static uint32_t send_opcode[] = {
    [0] = ESP_BLE_MESH_MODEL_OP_SENSOR_DESCRIPTOR_GET,
    [1] = ESP_BLE_MESH_MODEL_OP_SENSOR_CADENCE_GET,
    [2] = ESP_BLE_MESH_MODEL_OP_SENSOR_SETTINGS_GET,
    [3] = ESP_BLE_MESH_MODEL_OP_SENSOR_GET,
    [4] = ESP_BLE_MESH_MODEL_OP_SENSOR_SERIES_GET,
};
static uint8_t press_count;

static void button_tap_cb(void *arg)
{
    example_ble_mesh_send_sensor_message(send_opcode[press_count++]);
    press_count = press_count % ARRAY_SIZE(send_opcode);
}

static void board_button_init(void)
{   
        
    gpio_pad_select_gpio(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

}

void board_init(void)
{
    board_button_init();
}