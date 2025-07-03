#ifndef LOAD_CELL_H
#define LOAD_CELL_H

#include "pico/stdlib.h"

/**
 * @brief Configura los pines GPIO para el HX711.
 * 
 * @param dt_pin Pin de Datos (DT).
 * @param sck_pin Pin de Reloj (SCK).
 */
void hx711_init(uint8_t dt_pin, uint8_t sck_pin);

/**
 * @brief Lee el valor crudo de 24 bits del HX711.
 * 
 * @return El valor crudo leído.
 */
long leer_hx711(void);

/**
 * @brief Obtiene el promedio de varias lecturas crudas.
 * 
 * @param muestras El número de muestras a promediar.
 * @return El valor crudo promedio.
 */
long promedio(int muestras);

/**
 * @brief Pone el HX711 en modo de bajo consumo.
 */
void hx711_sleep(void);

/**
 * @brief Saca al HX711 del modo de bajo consumo.
 */
void hx711_wake(void);

#endif // LOAD_CELL_H