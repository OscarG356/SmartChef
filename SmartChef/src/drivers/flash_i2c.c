#include "flash_i2c.h"
#include "hardware/i2c.h"
#include <string.h>

#define FLASH_I2C_ADDR 0x50 // Dirección I2C de la memoria 24LC16B

void escribir_receta(uint8_t index, Receta *receta) {
    uint8_t buffer[64];
    memcpy(buffer, receta, sizeof(Receta));
    uint16_t direccion = index * sizeof(Receta);

    i2c_write_blocking(i2c_default, FLASH_I2C_ADDR, (uint8_t *)&direccion, 2, true);
    i2c_write_blocking(i2c_default, FLASH_I2C_ADDR, buffer, sizeof(Receta), false);
}

void leer_receta(uint8_t index, Receta *receta) {
    uint16_t direccion = index * sizeof(Receta);
    uint8_t buffer[64];

    i2c_write_blocking(i2c_default, FLASH_I2C_ADDR, (uint8_t *)&direccion, 2, true);
    i2c_read_blocking(i2c_default, FLASH_I2C_ADDR, buffer, sizeof(Receta), false);

    memcpy(receta, buffer, sizeof(Receta));
}
