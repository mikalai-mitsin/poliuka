//
// Created by Mikalai Mitsin on 30/07/2023.
//

#ifndef POLIUKA_IDF_PUMP_H
#define POLIUKA_IDF_PUMP_H

#include "driver/gpio.h"


typedef struct {
    gpio_num_t pin;
} pump_dev_t;

void pump_init(pump_dev_t *dev);

void pump_watering(pump_dev_t *dev, int delay);

#endif //POLIUKA_IDF_PUMP_H
