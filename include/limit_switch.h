#ifndef LIMIT_SWITCH_H
#define LIMIT_SWITCH_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Inicializa un servo en el pin GPIO especificado.
 * 
 * @param gpio Pin del servo.
 * @return Slice PWM asociado.
 */
uint pwm_init_servo(uint gpio);

/**
 * @brief Mueve un servo al ángulo deseado.
 * 
 * @param gpio Pin del servo.
 * @param slice Slice PWM.
 * @param angulo Ángulo entre 0 y 180 grados.
 */
void mover_servo(uint gpio, uint slice, float angulo);

/**
 * @brief Estado global de la tapa (true = cerrada).
 */
extern bool tapa_cerrada;

/**
 * @brief Inicializa sensores de tapa y servos, guardando los pines internamente.
 * 
 * @param sensor1_gpio GPIO del sensor 1
 * @param sensor2_gpio GPIO del sensor 2
 * @param servo1_gpio GPIO del servo 1
 * @param slice1 PWM slice del servo 1
 * @param servo2_gpio GPIO del servo 2
 * @param slice2 PWM slice del servo 2
 */
void init_tapa_y_servos(uint sensor1_gpio, uint sensor2_gpio, uint servo1_gpio, uint slice1, uint servo2_gpio, uint slice2);

/**
 * @brief Verifica sensores de tapa y mueve los servos si cambia el estado.
 */
void verificar_tapa_y_mover_servos(void);

#endif // LIMIT_SWITCH_H
