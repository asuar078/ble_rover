#ifndef BLE_SERVER_H_
#define BLE_SERVER_H_

#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"

#define GATTS_SERVICE_UUID_MOTOR   0x00FF
#define GATTS_CHAR_UUID_MOTOR      0x2A58
#define GATTS_DESCR_UUID_MOTOR     0x3333
#define GATTS_NUM_HANDLE_MOTOR     4

#define GATTS_SERVICE_UUID_BATTERY   0x180F
#define GATTS_CHAR_UUID_BATTERY      0x2A19
#define GATTS_DESCR_UUID_BATTERY     0x2902
#define GATTS_NUM_HANDLE_BATTERY     4

#define DEVICE_NAME            "BLE ROVER"
#define MANUFACTURER_DATA_LEN  17

#define ROVER_CHAR_VAL_LEN_MAX 0x40

#define PREPARE_BUF_MAX_SIZE 1024

#define PROFILE_NUM 2
#define MOTOR_APP_ID 0
#define BATTERY_APP_ID 1

///Declare the static function
void motor_gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
void battery_gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);

void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);

#endif
