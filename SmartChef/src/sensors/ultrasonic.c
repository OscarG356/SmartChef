#include "ultrasonic.h"
#include "hardware/gpio.h"
#include "pico/time.h"

// Función interna para enviar el pulso de disparo
static void send_trigger_pulse(uint8_t trig_pin) {
    gpio_put(trig_pin, 1);
    sleep_us(10);
    gpio_put(trig_pin, 0);
}

// Función interna para medir la duración del pulso de eco
static uint32_t measure_pulse_duration(uint8_t echo_pin) {
    // Espera a que el pin de eco se ponga en alto
    while (gpio_get(echo_pin) == 0) {
        tight_loop_contents();
    }
    uint64_t start = time_us_64();

    // Espera a que el pin de eco se ponga en bajo
    while (gpio_get(echo_pin) == 1) {
        tight_loop_contents();
    }
    uint64_t end = time_us_64();

    return (uint32_t)(end - start);
}

void ultrasonic_init(uint8_t trig_pin, uint8_t echo_pin) {
    gpio_init(trig_pin);
    gpio_set_dir(trig_pin, GPIO_OUT);
    gpio_put(trig_pin, 0);

    gpio_init(echo_pin);
    gpio_set_dir(echo_pin, GPIO_IN);
}

float ultrasonic_get_distance_cm(uint8_t trig_pin, uint8_t echo_pin) {
    send_trigger_pulse(trig_pin);
    uint32_t pulse_width = measure_pulse_duration(echo_pin);
    
    // La distancia en cm es la duración del pulso en µs / 58
    return (float)pulse_width / 58.0f;
}