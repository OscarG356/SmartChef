#include "pico/stdlib.h"
#include <stdio.h>

#define TRIG_PIN 16
#define ECHO_PIN 17

void send_trigger_pulse() {
    gpio_put(TRIG_PIN, 1);
    sleep_us(10); // Pulso de 10 µs
    gpio_put(TRIG_PIN, 0);
}

uint32_t measure_pulse() {
    while (gpio_get(ECHO_PIN) == 0); // Espera a que comience el pulso
    uint64_t start = time_us_64();

    while (gpio_get(ECHO_PIN) == 1); // Espera a que termine el pulso
    uint64_t end = time_us_64();

    return (uint32_t)(end - start);
}

int main() {
    stdio_init_all();

    gpio_init(TRIG_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    gpio_put(TRIG_PIN, 0);

    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);

    while (true) {
        send_trigger_pulse();
        uint32_t pulse_width = measure_pulse();
        float distance_cm = pulse_width / 58.0f;

        printf("Distancia: %.2f cm\n", distance_cm);
        sleep_ms(500);
    }

    return 0;
}
