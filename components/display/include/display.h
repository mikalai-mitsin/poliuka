//
// Created by Mikalai Mitsin on 02/08/2023.
//

#ifndef POLIUKA_IDF_DISPLAY_H
#define POLIUKA_IDF_DISPLAY_H

#include "driver/gpio.h"
#include "ssd1306.h"

typedef struct {
    ssd1306_handle_t dev;
} display_dev_t;

void display_init(display_dev_t *dev);

void display_print(display_dev_t *dev, char *data_str);

#endif //POLIUKA_IDF_DISPLAY_H
