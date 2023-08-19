//
// Created by Mikalai Mitsin on 30/07/2023.
//

#ifndef POLIUKA_IDF_UNIT_H
#define POLIUKA_IDF_UNIT_H

#include "sensor.h"
#include "valve.h"
#include "pump.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

typedef enum {
    OnTaskType = 0,
    OffTaskType = 1,
} unit_task_type;

typedef struct {
    unit_task_type type;
    int16_t delay;
} unit_task_t;

typedef struct {
    int limit;
    sensor_dev_t *sensor;
    valve_dev_t *valve;
    pump_dev_t *pump;
    bool in_use;
    QueueHandle_t queue;
} unit_dev_t;

void unit_start(unit_dev_t *dev);

void unit_watering(unit_dev_t *dev);

void unit_open(unit_dev_t *dev);

void unit_close(unit_dev_t *dev);

int unit_moisture(unit_dev_t *dev);

int unit_percentage(unit_dev_t *dev);

#endif //POLIUKA_IDF_UNIT_H
