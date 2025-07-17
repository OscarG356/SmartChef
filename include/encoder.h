#ifndef ENCODER_RPM_H
#define ENCODER_RPM_H

#include "pico/stdlib.h"

// === CONFIGURACIÓN ===
#define ENCODER_PIN 27
#define PPR 20                      // Pulsos por revolución
#define TIMEOUT_US 300000          // Tiempo sin pulsos para considerar motor detenido
#define N 5                         // Tamaño del filtro de media móvil
#define RPM_MAX 10000              // Límite superior de RPM válida

#ifdef __cplusplus
extern "C" {
#endif

void encoder_init(void);
float leer_rpm_filtrada(void);
bool rpm_motor_detenido(void);
bool rpm_esperando_pulsos(void);

#ifdef __cplusplus
}
#endif

#endif
