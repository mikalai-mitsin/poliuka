#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_log.h"
#include "pump.h"
#include "sensor.h"
#include "valve.h"
#include "unit.h"
#include "display.h"

#define STACK_SIZE 4096


static const char *TAG = "main";

pump_dev_t pump = {
        .pin = CONFIG_PUMP_PIN,
};

sensor_dev_t sensor1 = {
        .pin= CONFIG_UNIT_1_SOIL_MOISTURE_PIN,
        .waterLevel = CONFIG_UNIT_1_SOIL_MOISTURE_WATER_VALUE,
        .airLevel = CONFIG_UNIT_1_SOIL_MOISTURE_AIR_VALUE,
};

valve_dev_t valve1 = {
        .pin = CONFIG_UNIT_1_VALVE_PIN,
};

unit_dev_t unit1 = {
        .limit = CONFIG_UNIT_1_DRY_LEVEL,
        .sensor = &sensor1,
        .valve = &valve1,
        .pump = &pump,
};

display_dev_t display = {};

_Noreturn void vTaskWatering(void *pvParameters) {
    while (true) {
        unit_tick(&unit1);
        ESP_LOGI("watering task", "percentage: %d", unit_percentage(&unit1));
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

_Noreturn void vTaskInterface(void *pvParameters) {
    while (true) {
        int percentage = unit_percentage(&unit1);
        char str[80];
        sprintf(str, "per: %i", percentage);
        display_print(&display, str);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void app_main() {
    ESP_LOGI(TAG, "init");
    pump_init(&pump);
    sensor_init(&sensor1);
    display_init(&display);
    ESP_LOGI(TAG, "run tasks");
    xTaskCreate(vTaskWatering, "watering", STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vTaskInterface, "interface", STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
}