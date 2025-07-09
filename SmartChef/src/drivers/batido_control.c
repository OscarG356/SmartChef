#include "batido_control.h"
#include "hardware/pwm.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"
#include <stdio.h>

#define PWM_MOTOR_GPIO 15  // Cambia este GPIO según tu conexión

// Estados internos
static enum {IDLE, BATIENDO, REPOSANDO} estado = IDLE;
static absolute_time_t timer_inicio;
static int duracion_timer = 0;

void iniciar_pwm_motor(uint16_t duty_cycle) {
    gpio_set_function(PWM_MOTOR_GPIO, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(PWM_MOTOR_GPIO);
    pwm_set_wrap(slice, 65535);
    pwm_set_chan_level(slice, PWM_CHAN_A, duty_cycle);
    pwm_set_enabled(slice, true);
}

void detener_pwm_motor() {
    uint slice = pwm_gpio_to_slice_num(PWM_MOTOR_GPIO);
    pwm_set_enabled(slice, false);
}

void batido_iniciar(int tiempo_segundos, uint16_t pwm) {
    if (estado == IDLE) {
        iniciar_pwm_motor(pwm);
        duracion_timer = tiempo_segundos;
        timer_inicio = get_absolute_time();
        estado = BATIENDO;
    }
}

void batido_procesar() {
    if (estado == IDLE) return;
    if (absolute_time_diff_us(timer_inicio, get_absolute_time()) < duracion_timer * 1000000) return;

    if (estado == BATIENDO) {
        detener_pwm_motor();
        printf("✅ Fin del batido\n");

        extern int get_tiempo_reposo();  // Esta función viene de recetas.h
        duracion_timer = get_tiempo_reposo();
        timer_inicio = get_absolute_time();
        printf("⏳ Reposo: %ds\n", duracion_timer);
        estado = REPOSANDO;
    } else if (estado == REPOSANDO) {
        printf("🏁 Fin del reposo\n");
        estado = IDLE;
    }
}

bool batido_esta_ocupado() {
    return estado != IDLE;
}
