//
// Created by Mikalai Mitsin on 30/07/2023.
//

#include <esp_log.h>
#include "unit.h"

static const char *TAG = "unit";

void unit_tick(unit_dev_t *dev) {
    ESP_LOGD(TAG, "tick");

    if (sensor_percentage(dev->sensor) < dev->limit) {
        valve_open(dev->valve);
        pump_watering(dev->pump, 2000);
        valve_close(dev->valve);
    }
};

int unit_moisture(unit_dev_t *dev) {
    return sensor_moisture(dev->sensor);
};

int unit_percentage(unit_dev_t *dev) {
    return sensor_percentage(dev->sensor);
};