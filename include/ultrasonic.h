#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "pico/stdlib.h"  // Para tipo uint

void ultrasonic_init(uint trig_pin, uint echo_pin);
float ultrasonic_get_distance_cm(uint trig_pin, uint echo_pin);

#endif // ULTRASONIC_H
