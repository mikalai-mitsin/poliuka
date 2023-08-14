#include <sys/cdefs.h>
//
// Created by Mikalai Mitsin on 30/07/2023.
//

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include "unit.h"
#include "pump.h"

static const char *TAG = "unit";


QueueHandle_t xQueue;

_Noreturn void unit_task_queue(void *param) {
    unit_dev_t *unit = (unit_dev_t *) param;
    unit_task_t task;
    while (1) {
        if (xQueueReceive(xQueue, &(task), (TickType_t) 10)) {
            ESP_LOGI(TAG, "handled task type %d delay %d", task.type, task.delay);
            switch (task.type) {
                case OnTaskType:
                    unit_open(unit);
                    break;
                case OffTaskType:
                    unit_close(unit);
                    break;
                default:
                    break;
            }
            vTaskDelay(task.delay / portTICK_PERIOD_MS);
            continue;
        }
        unit_close(unit);
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

_Noreturn void unit_task_watering(void *param) {
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
    xQueue = xQueueCreate(2, sizeof(unit_task_t));
    xTaskCreate(
            unit_task_queue,
            "queue_handler",
            4096,
            dev,
            tskIDLE_PRIORITY,
            NULL
    );
    xTaskCreate(
            unit_task_watering,
            "watering",
            4096,
            dev,
            tskIDLE_PRIORITY,
            NULL
    );
}

void unit_watering(unit_dev_t *dev) {
    unit_task_t on = {
            .type = OnTaskType,
            .delay = 2000,
    };
    xQueueSend(xQueue, (void *) &on, 2000 / portTICK_PERIOD_MS);
    unit_task_t delay = {
            .type = OffTaskType,
            .delay = 2000,
    };
    xQueueSend(xQueue, (void *) &delay, 2000 / portTICK_PERIOD_MS);
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