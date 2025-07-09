#include "ultrasonic.h"

#define TIMEOUT_US 30000  // 30 ms timeout

void ultrasonic_init(uint trig_pin, uint echo_pin) {
    gpio_init(trig_pin);
    gpio_set_dir(trig_pin, GPIO_OUT);
    gpio_put(trig_pin, 0);

    gpio_init(echo_pin);
    gpio_set_dir(echo_pin, GPIO_IN);
    gpio_pull_down(echo_pin);  // Pull down to ensure a clean start
}

float ultrasonic_get_distance_cm(uint trig_pin, uint echo_pin) {
    // Pulso de 10 us en TRIG
    gpio_put(trig_pin, 1);
    sleep_us(10);
    gpio_put(trig_pin, 0);

    // Esperar subida de ECHO (inicio del pulso)
    uint64_t t0 = time_us_64();
    while (gpio_get(echo_pin) == 0) {
        if ((time_us_64() - t0) > TIMEOUT_US) {
            return -1.0f;
        }
    
    }

    uint64_t pulse_start = time_us_64();

    // Esperar bajada de ECHO (fin del pulso)
    while (gpio_get(echo_pin) == 1) {
        if ((time_us_64() - pulse_start) > TIMEOUT_US) {
            return -1.0f;
        }
        
    }

    uint64_t pulse_end = time_us_64();

    // Duración del pulso
    uint32_t duration = (uint32_t)(pulse_end - pulse_start);
    return duration / 58.0f;
    
}