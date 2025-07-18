#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "limit_switch.h"

// Estado global
bool tapa_cerrada = false;

// Pines internos
static uint sensor1_pin, sensor2_pin;
static uint servo1_pin, servo2_pin;
static uint slice_servo1, slice_servo2;

// === Implementación de funciones de servos ===
uint pwm_init_servo(uint gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(gpio);

    pwm_set_wrap(slice, 24999);       // 20ms = 50Hz
    pwm_set_clkdiv(slice, 64.0f);     // 125MHz / 64 = 1.953 MHz
    pwm_set_enabled(slice, true);

    return slice;
}

void mover_servo(uint gpio, uint slice, float angulo) {
    float pulso_ms = 0.5f + (angulo / 180.0f) * 2.0f;  // entre 0.5 ms y 2.5 ms
    uint ciclos = (uint)(pulso_ms * 1953);             // convertir a ciclos
    pwm_set_chan_level(slice, pwm_gpio_to_channel(gpio), ciclos);
}

// === Inicialización de sensores y servos ===
void init_tapa_y_servos(uint sensor1_gpio, uint sensor2_gpio, uint servo1_gpio, uint slice1, uint servo2_gpio, uint slice2) {
    // Guardar pines
    sensor1_pin = sensor1_gpio;
    sensor2_pin = sensor2_gpio;
    servo1_pin = servo1_gpio;
    servo2_pin = servo2_gpio;
    slice_servo1 = slice1;
    slice_servo2 = slice2;

    // Inicializar sensores con pull-up (activos en LOW)
    gpio_init(sensor1_pin);
    gpio_set_dir(sensor1_pin, GPIO_IN);
    gpio_pull_up(sensor1_pin);

    gpio_init(sensor2_pin);
    gpio_set_dir(sensor2_pin, GPIO_IN);
    gpio_pull_up(sensor2_pin);
}

// === Lógica de verificación de tapa ===
void verificar_tapa_y_mover_servos(void) {
    bool s1 = !gpio_get(sensor1_pin);
    bool s2 = !gpio_get(sensor2_pin);

    if (s1 && s2 && !tapa_cerrada) {
        mover_servo(servo1_pin, slice_servo1, 0.0f);
        mover_servo(servo2_pin, slice_servo2, 180.0f);
        tapa_cerrada = true;
    } else if ((!s1 || !s2) && tapa_cerrada) {
        mover_servo(servo1_pin, slice_servo1, 180.0f);
        mover_servo(servo2_pin, slice_servo2, 0.0f);
        tapa_cerrada = false;
    }
}
