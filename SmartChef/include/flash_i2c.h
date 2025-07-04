#ifndef FLASH_I2C_H
#define FLASH_I2C_H

#include <stdint.h>

typedef struct {
    char nombre[32];       // Nombre de la receta
    uint16_t tiempo_batido; // Tiempo de batido en milisegundos
    uint16_t velocidad_motor; // Nivel de PWM para el motor (0 a 65535)
    uint16_t cantidad;     // Cantidad esperada (por ejemplo, en gramos)
} Receta;

void escribir_receta(uint8_t index, Receta *receta);
void leer_receta(uint8_t index, Receta *receta);

#endif // FLASH_I2C_H
