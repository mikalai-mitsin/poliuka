//
// Created by Mikalai Mitsin on 18/07/2023.
//
#include "valve.h"
#include "driver/gpio.h"


void valve_init(valve_dev_t *dev) {
    gpio_reset_pin(dev->pin);
    gpio_set_direction(dev->pin, GPIO_MODE_OUTPUT);
}

void valve_close(valve_dev_t *dev) {
    gpio_set_level(dev->pin, 0);
}

void valve_open(valve_dev_t *dev) {
    gpio_set_level(dev->pin, 1);
}
