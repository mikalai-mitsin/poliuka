//
// Created by Mikalai Mitsin on 30/07/2023.
//

#ifndef POLIUKA_IDF_SENSOR_H
#define POLIUKA_IDF_SENSOR_H


#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"

typedef struct {
    gpio_num_t pin;
    int waterLevel;
    int airLevel;
    adc_oneshot_unit_handle_t unitHandle;
} sensor_dev_t;


void sensor_init(sensor_dev_t *dev);

int sensor_moisture(sensor_dev_t *dev);

int sensor_percentage(sensor_dev_t *dev);

#endif //POLIUKA_IDF_SENSOR_H
