#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "pico/stdlib.h"

/**
 * @brief Inicializa los pines para el sensor ultrasónico.
 * 
 * @param trig_pin Pin de Trigger.
 * @param echo_pin Pin de Echo.
 */
void ultrasonic_init(uint8_t trig_pin, uint8_t echo_pin);

/**
 * @brief Mide la distancia en centímetros.
 * 
 * @param trig_pin Pin de Trigger.
 * @param echo_pin Pin de Echo.
 * @return La distancia medida en cm.
 */
float ultrasonic_get_distance_cm(uint8_t trig_pin, uint8_t echo_pin);

#endif // ULTRASONIC_H