//
// Created by Mikalai Mitsin on 02/08/2023.
//

#include "display.h"
#include "esp_log.h"
#include "ssd1306.h"
#include "unit.h"

#define I2C_MASTER_SCL_IO 22        /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 21        /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_1    /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 100000   /*!< I2C master clock frequency */

static const char *TAG = "display";

static ssd1306_handle_t ssd1306_dev = NULL;

void display_init(display_dev_t *dev) {
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = (gpio_num_t) I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = (gpio_num_t) I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    conf.clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL;

    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
    ssd1306_dev = ssd1306_create(I2C_MASTER_NUM, SSD1306_I2C_ADDRESS);
    ssd1306_refresh_gram(ssd1306_dev);
    ssd1306_clear_screen(ssd1306_dev, 0x00);
    dev->dev = ssd1306_dev;
}

void display_print(display_dev_t *dev, char *data_str) {
    ssd1306_draw_string(dev->dev, 0, 0, (const uint8_t *) data_str, 16, 1);
    ssd1306_refresh_gram(dev->dev);
}

_Noreturn void display_update_task(void *param) {
    unit_dev_t *unit = (unit_dev_t *) param;
    while (true) {
        ESP_LOGD(TAG, "tick");
        char percentage[16];
        sprintf(percentage, "Percentage: %d", unit_percentage(unit));
        ssd1306_draw_string(ssd1306_dev, 0, 0, (const uint8_t *) percentage, 14, 1);
        char in_use[16];
        sprintf(in_use, "In use %d", unit->in_use);
        ssd1306_draw_string(ssd1306_dev, 0, 20, (const uint8_t *) in_use, 16, 1);
        ssd1306_refresh_gram(ssd1306_dev);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void display_start(unit_dev_t *unit) {
    xTaskCreate(
            display_update_task,
            "display",
            4096,
            unit,
            tskIDLE_PRIORITY,
            NULL
    );
}
