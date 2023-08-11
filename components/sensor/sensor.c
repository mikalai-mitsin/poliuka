//
// Created by Mikalai Mitsin on 30/07/2023.
//

#include "include/sensor.h"
#include "esp_adc/adc_oneshot.h"

void sensor_init(sensor_dev_t *dev) {
    adc_oneshot_unit_init_cfg_t init_config = {
            .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &dev->unitHandle));
    adc_oneshot_chan_cfg_t config = {
            .atten = ADC_ATTEN_DB_0,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(dev->unitHandle, ADC_CHANNEL_0, &config));
}

int map(int x, int in_min, int in_max, int out_min, int out_max) {
    const long run = in_max - in_min;
    if (run == 0) {
        return -1; // AVR returns -1, SAM returns 0
    }
    const long rise = out_max - out_min;
    const long delta = x - in_min;
    return (delta * rise) / run + out_min;
}

int sensor_moisture(sensor_dev_t *dev) {
    int result = 0;
    ESP_ERROR_CHECK(adc_oneshot_read(dev->unitHandle, ADC_CHANNEL_0, &result));
    return result;
}


int sensor_percentage(sensor_dev_t *dev) {
    int moisture = sensor_moisture(dev);
    return map(moisture, dev->airLevel, dev->waterLevel, 0, 100);
}

