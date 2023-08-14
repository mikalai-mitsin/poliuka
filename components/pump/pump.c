//
// Created by Mikalai Mitsin on 30/07/2023.
//

#include "pump.h"
#include "driver/gpio.h"


void pump_init(pump_dev_t *dev) {
    gpio_reset_pin(dev->pin);
    gpio_set_direction(dev->pin, GPIO_MODE_OUTPUT);
}

void pump_on(pump_dev_t *dev) {
    gpio_set_level(dev->pin, 1);
}

void pump_off(pump_dev_t *dev) {
    gpio_set_level(dev->pin, 0);
}
