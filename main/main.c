#include <nvs_flash.h>
#include "esp_log.h"
#include "pump.h"
#include "sensor.h"
#include "valve.h"
#include "unit.h"
#include "display.h"
#include "homekit.h"
#include "wifi.h"

static const char *TAG = "main";

pump_dev_t pump = {
        .pin = CONFIG_PUMP_PIN,
};

sensor_dev_t sensor = {
        .pin= CONFIG_UNIT_1_SOIL_MOISTURE_PIN,
        .waterLevel = CONFIG_UNIT_1_SOIL_MOISTURE_WATER_VALUE,
        .airLevel = CONFIG_UNIT_1_SOIL_MOISTURE_AIR_VALUE,
};

valve_dev_t valve = {
        .pin = CONFIG_UNIT_1_VALVE_PIN,
};

unit_dev_t unit = {
        .limit = CONFIG_UNIT_1_DRY_LEVEL,
        .sensor = &sensor,
        .valve = &valve,
        .pump = &pump,
        .in_use = false,
};

display_dev_t display = {};

void app_main() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    wifi_init_sta();
    ESP_LOGI(TAG, "init");
    pump_init(&pump);
    sensor_init(&sensor);
    display_init(&display);
    ESP_LOGI(TAG, "run tasks");
    unit_start(&unit);
    homekit_start(&unit);
}