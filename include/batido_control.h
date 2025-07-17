#ifndef BATIDO_CONTROL_H
#define BATIDO_CONTROL_H

#include <stdint.h>
#include <stdbool.h>

// Inicia el batido: activa el PWM con el duty y empieza el conteo de tiempo
void iniciar_pwm_motor(uint16_t duty_cycle);

// Llamar regularmente para procesar si terminó el batido o el reposo
void detener_pwm_motor(void);
void batido_procesar(void);
void batido_iniciar(int tiempo_segundos, uint16_t pwm);
void batido_procesar();

// Indica si el sistema está ocupado (batiendo o en reposo)
bool batido_esta_ocupado(void);


#endif // BATIDO_CONTROL_H