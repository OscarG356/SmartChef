#include "batido_control.h"
#include "hardware/pwm.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"
#include <stdio.h>

#define PWM_MOTOR_GPIO 20  // Cambia este GPIO según tu conexión


void iniciar_pwm_motor(uint16_t duty_cycle) {
    gpio_set_function(PWM_MOTOR_GPIO, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(PWM_MOTOR_GPIO);
    pwm_set_wrap(slice, 65535);
    pwm_set_chan_level(slice, PWM_CHAN_A, duty_cycle);
    pwm_set_enabled(slice, true);
}

void detener_pwm_motor() {
    uint slice = pwm_gpio_to_slice_num(PWM_MOTOR_GPIO);
    uint channel = pwm_gpio_to_channel(PWM_MOTOR_GPIO);

    pwm_set_chan_level(slice, channel, 0);

    pwm_set_enabled(slice, false);

    gpio_set_function(PWM_MOTOR_GPIO, GPIO_FUNC_SIO);
    gpio_put(PWM_MOTOR_GPIO, 0);
}