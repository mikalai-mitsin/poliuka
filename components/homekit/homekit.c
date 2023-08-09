#include <sys/cdefs.h>
//
// Created by Mikalai Mitsin on 05/08/2023.
//

#include "homekit.h"
#include "unit.h"
#include <string.h>
#include <esp_err.h>
#include <esp_netif.h>
#include <esp_log.h>

#include <hap_apple_servs.h>
#include <hap_apple_chars.h>


#define SETUP_CODE    CONFIG_HOMEKIT_SETUP_CODE
#define SETUP_ID      CONFIG_HOMEKIT_SETUP_ID

static const char *TAG = "homekit";

static hap_char_t *active;
static hap_char_t *in_use;
static hap_char_t *prog_mod;

static unit_dev_t *unit_dev;

static void app_homekit_show_qr(void) {
    ESP_LOGW(TAG, "Cannot show QR code for HomeKit pairing as the raw setup code is not available.");
}

static void app_homekit_event_handler(
        void *arg,
        esp_event_base_t event_base,
        int32_t event_id,
        void *event_data
) {
    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        if (hap_get_paired_controller_count() == 0) {
            app_homekit_show_qr();
        } else {
            ESP_LOGI(TAG, "Accessory is already paired with a controller.");
        }
    } else if (event_base == HAP_EVENT && event_id == HAP_EVENT_CTRL_UNPAIRED) {
        if (hap_get_paired_controller_count() == 0) {
            app_homekit_show_qr();
        }
    }
}

static int switch_identify(hap_acc_t *ha) {
    ESP_LOGI(TAG, "Accessory identified");
    return HAP_SUCCESS;
}

static int irrigation_read(hap_char_t *hc, hap_status_t *status_code, void *serv_priv, void *read_priv) {
    if (hap_req_get_ctrl_id(read_priv)) {
        ESP_LOGI(TAG, "Received read from %s", hap_req_get_ctrl_id(read_priv));
    }
    const hap_val_t *cur_val = hap_char_get_val(hc);
    if (!strcmp(hap_char_get_type_uuid(hc), HAP_CHAR_UUID_IN_USE)) {
        hap_val_t new_val = {
                .b= unit_dev->in_use,
        };
        if (cur_val->b == new_val.b) {
            hap_char_update_val(hc, &new_val);
        }
        *status_code = HAP_STATUS_SUCCESS;
    }
    if (!strcmp(hap_char_get_type_uuid(hc), HAP_CHAR_UUID_ACTIVE)) {
        hap_val_t new_val = {
                .b= unit_dev->in_use,
        };
        if (cur_val->b == new_val.b) {
            hap_char_update_val(hc, &new_val);
        }
        *status_code = HAP_STATUS_SUCCESS;
    }
    return HAP_SUCCESS;
}

static int irrigation_write(
        hap_write_data_t write_data[],
        int count,
        void *serv_priv,
        void *write_priv
) {
    int i = HAP_SUCCESS;
    hap_write_data_t *write;
    for (i = 0; i < count; i++) {
        write = &write_data[i];
        if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_ACTIVE)) {
            ESP_LOGI(TAG, "Received Write. Active %s", write->val.b ? "On" : "Off");
            /* Set the switch state */
            if (write->val.b) {
                unit_open(unit_dev);
            } else {
                unit_close(unit_dev);
            }
            /* Update the HomeKit characteristic */
            hap_char_update_val(write->hc, &(write->val));
            homekit_update_in_use(write->val.b);
            /* Report to RainMaker */
            *(write->status) = HAP_STATUS_SUCCESS;
        } else if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_PROGRAM_MODE)) {
            ESP_LOGI(TAG, "Received Write. Program mode %s", write->val.b ? "On" : "Off");
            hap_char_update_val(write->hc, &(write->val));
            *(write->status) = HAP_STATUS_SUCCESS;
        } else if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_IN_USE)) {
            ESP_LOGI(TAG, "Received Write. In use %s", write->val.b ? "On" : "Off");
            hap_char_update_val(write->hc, &(write->val));
            *(write->status) = HAP_STATUS_SUCCESS;
        } else {
            *(write->status) = HAP_STATUS_RES_ABSENT;
        }
    }
    return HAP_SUCCESS;
}

esp_err_t homekit_update_active(bool state) {
    hap_val_t new_value = {
            .b = state,
    };
    hap_char_update_val(active, &new_value);
    return ESP_OK;
}

esp_err_t homekit_update_prog_mod(bool state) {
    hap_val_t new_value = {
            .b = state,
    };
    hap_char_update_val(prog_mod, &new_value);
    return ESP_OK;
}

esp_err_t homekit_update_in_use(bool state) {
    hap_val_t new_value = {
            .b = state,
    };
    hap_char_update_val(in_use, &new_value);
    return ESP_OK;
}

_Noreturn void vTaskUpdateState(void *pvParameters) {
    unit_dev_t *unit = (unit_dev_t *) (pvParameters);
    while (true) {
        homekit_update_in_use(unit->in_use);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}


esp_err_t homekit_start(unit_dev_t *unit) {
    unit_dev = unit;
    hap_acc_t *accessory;
    hap_serv_t *service;
    hap_init(HAP_TRANSPORT_WIFI);

    hap_acc_cfg_t cfg = {
            .name = "Poiuka-IDF",
            .manufacturer = "Mikalai Mitsin",
            .model = "2",
            .serial_num = "001122334455",
            .fw_rev = "1.0",
            .hw_rev = NULL,
            .pv = "0.1.0",
            .identify_routine = switch_identify,
            .cid = HAP_CID_OTHER,
    };
    accessory = hap_acc_create(&cfg);
    service = hap_serv_irrigation_system_create(1, 0, 0);

    hap_serv_add_char(service, hap_char_name_create("Pump"));

    hap_serv_set_write_cb(service, irrigation_write);
    hap_serv_set_read_cb(service, irrigation_read);


    active = hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_ACTIVE);
    in_use = hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_IN_USE);
    prog_mod = hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_PROGRAM_MODE);


    hap_acc_add_serv(accessory, service);
    hap_add_accessory(accessory);

    hap_set_setup_code(SETUP_CODE);
    hap_set_setup_id(SETUP_ID);

    if (hap_get_paired_controller_count() == 0) {
        app_homekit_show_qr();
    }

    hap_enable_mfi_auth(HAP_MFI_AUTH_NONE);
    esp_event_handler_register(HAP_EVENT, HAP_EVENT_CTRL_UNPAIRED, &app_homekit_event_handler, NULL);

    if (hap_start() == 0) {
        ESP_LOGI(TAG, "HomeKit started successfully");
        xTaskCreate(vTaskUpdateState, "update homekit state", 2048, unit, tskIDLE_PRIORITY, NULL);
        return ESP_OK;
    }

    ESP_LOGE(TAG, "Failed to start HomeKit");
    return ESP_FAIL;
}