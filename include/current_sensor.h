#ifndef CURRENT_SENSOR_H
#define CURRENT_SENSOR_H

#include "pico/stdlib.h"
#include "hardware/adc.h"

/**
 * @brief Inicializa el ADC para el sensor de corriente en el pin por defecto.
 */
void current_sensor_init(void);

/**
 * @brief Lee la corriente del sensor.
 * @return El valor de la corriente en Amperios.
 */
float current_sensor_get_current(void);

#endif // CURRENT_SENSOR_H
