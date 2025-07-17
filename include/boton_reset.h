#ifndef BOTON_RESET_H
#define BOTON_RESET_H

#include "pico/stdlib.h"

void boton_init(uint gpio_pin);
bool boton_fue_presionado(void);
void boton_reset_estado(void);

#endif
