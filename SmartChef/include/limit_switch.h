#ifndef LIMIT_SWITCH_H
#define LIMIT_SWITCH_H

#include "pico/stdlib.h"

#define SWITCH1_PIN 10
#define SWITCH2_PIN 11


void limit_switch_init(void);
bool tapa_cerrada(void);
void verificar_tapa_por_serial(void);


#endif
