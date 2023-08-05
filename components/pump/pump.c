//
// Created by Mikalai Mitsin on 30/07/2023.
//

#include "pump.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"


void pump_init(pump_dev_t *dev) {
    gpio_reset_pin(dev->pin);
    gpio_set_direction(dev->pin, GPIO_MODE_OUTPUT);
}

void pump_watering(pump_dev_t *dev, int delay) {
    gpio_set_level(dev->pin, 1);
    vTaskDelay(delay / portTICK_PERIOD_MS);
    gpio_set_level(dev->pin, 0);
}