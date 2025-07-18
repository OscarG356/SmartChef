#ifndef BATIDO_CONTROL_H
#define BATIDO_CONTROL_H

#include <stdint.h>
#include <stdbool.h>

// Inicia el batido: activa el PWM con el duty y empieza el conteo de tiempo
void iniciar_pwm_motor(uint16_t duty_cycle);
void detener_pwm_motor(void);


#endif // BATIDO_CONTROL_H