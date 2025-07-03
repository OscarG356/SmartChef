#include "encoder.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include <stdio.h>

static volatile uint64_t last_time = 0;
static volatile uint64_t period = 0;
static volatile bool pulso_recibido = false;

static float rpm_buffer[N] = {0};         // Buffer circular para el promedio
static int rpm_index = 0;
static float rpm_filtrada_anterior = 0.0;

// Filtro de media móvil
static float promedio_rpm(float nueva) {
    rpm_buffer[rpm_index] = nueva;
    rpm_index = (rpm_index + 1) % N;

    float suma = 0;
    for (int i = 0; i < N; i++) {
        suma += rpm_buffer[i];
    }
    return suma / N;
}

// ISR del encoder
static void encoder_isr(uint gpio, uint32_t events) {
    if (gpio == ENCODER_PIN && (events & GPIO_IRQ_EDGE_RISE)) {
        uint64_t now = time_us_64();
        period = now - last_time;
        last_time = now;
        pulso_recibido = true;
    }
}

// Inicialización
void encoder_init(void) {
    gpio_init(ENCODER_PIN);
    gpio_set_dir(ENCODER_PIN, GPIO_IN);
    gpio_pull_up(ENCODER_PIN);
    gpio_set_irq_enabled_with_callback(ENCODER_PIN, GPIO_IRQ_EDGE_RISE, true, &encoder_isr);
}

// Retorna si aún no hay pulsos
bool rpm_esperando_pulsos(void) {
    return !pulso_recibido;
}

// Retorna si el motor está detenido por timeout
bool rpm_motor_detenido(void) {
    return time_us_64() - last_time > TIMEOUT_US;
}

// Cálculo de RPM filtrada
float leer_rpm_filtrada(void) {
    if (!pulso_recibido || rpm_motor_detenido()) {
        return 0.0f;
    }

    if (period > 0) {
        float freq = 1e6f / period;
        float rpm = (60.0f * freq) / PPR;

        if (rpm < 0 || rpm > RPM_MAX) {
            return rpm_filtrada_anterior;
        }

        rpm_filtrada_anterior = promedio_rpm(rpm);
        return rpm_filtrada_anterior;
    }

    return 0.0f;
}
