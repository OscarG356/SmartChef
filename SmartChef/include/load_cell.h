#ifndef LOAD_CELL_H
#define LOAD_CELL_H

#include "pico/stdlib.h"

// Pines del HX711
#define HX711_DT 2
#define HX711_SCK 3

// Calibración
#define OFFSET_MANUAL 455L
#define FACTOR_ESCALA -199.026f

void hx711_init();
long leer_hx711();
long promedio(int muestras);
void hx711_sleep();
void hx711_wake();

#endif
