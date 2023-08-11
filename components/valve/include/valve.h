//
// Created by Mikalai Mitsin on 18/07/2023.
//

#ifndef POLIUKA_VALVE_H
#define POLIUKA_VALVE_H

#include "driver/gpio.h"


typedef struct {
    gpio_num_t pin;
} valve_dev_t;


void valve_init(valve_dev_t *dev);

void valve_open(valve_dev_t *dev);

void valve_close(valve_dev_t *dev);

#endif //POLIUKA_VALVE_H
