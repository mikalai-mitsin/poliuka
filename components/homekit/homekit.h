//
// Created by Mikalai Mitsin on 05/08/2023.
//

#ifndef POLIUKA_IDF_HOMEKIT_H
#define POLIUKA_IDF_HOMEKIT_H

#include <stdbool.h>
#include <esp_err.h>
#include <hap.h>
#include "unit.h"

esp_err_t homekit_start(unit_dev_t *unit);

esp_err_t homekit_update_active(bool state);

esp_err_t homekit_update_prog_mod(bool state);

esp_err_t homekit_update_in_use(bool state);

#endif //POLIUKA_IDF_HOMEKIT_H
