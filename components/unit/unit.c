//
// Created by Mikalai Mitsin on 30/07/2023.
//

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "unit.h"

static const char *TAG = "unit";

_Noreturn void vTaskWatering(void *param) {
    unit_dev_t *unit = (unit_dev_t *) param;
    while (true) {
        ESP_LOGD(TAG, "tick");
        if (sensor_percentage(unit->sensor) < unit->limit) {
            unit_watering(unit);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void unit_start(unit_dev_t *dev) {
    xTaskCreate(
            vTaskWatering,
            "watering",
            4096,
            dev,
            tskIDLE_PRIORITY,
            NULL
    );
}

void unit_watering(unit_dev_t *dev) {
    unit_open(dev);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    unit_close(dev);
}

void unit_close(unit_dev_t *dev) {
    ESP_LOGD(TAG, "unit close");
    pump_off(dev->pump);
    valve_close(dev->valve);
    ESP_LOGD(TAG, "unit closed");
    dev->in_use = false;
}

void unit_open(unit_dev_t *dev) {
    ESP_LOGD(TAG, "unit open");
    valve_open(dev->valve);
    pump_on(dev->pump);
    ESP_LOGD(TAG, "unit opened");
    dev->in_use = true;
}

int unit_moisture(unit_dev_t *dev) {
    return sensor_moisture(dev->sensor);
}

int unit_percentage(unit_dev_t *dev) {
    return sensor_percentage(dev->sensor);
}