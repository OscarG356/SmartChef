#include "load_cell.h"
#include "hardware/gpio.h"
#include "pico/time.h"

// Variables estáticas para guardar los pines
static uint8_t PIN_DT;
static uint8_t PIN_SCK;

void hx711_init(uint8_t dt_pin, uint8_t sck_pin) {
    PIN_DT = dt_pin;
    PIN_SCK = sck_pin;

    gpio_init(PIN_DT);
    gpio_set_dir(PIN_DT, GPIO_IN);

    gpio_init(PIN_SCK);
    gpio_set_dir(PIN_SCK, GPIO_OUT);
    gpio_put(PIN_SCK, 0);
}

long leer_hx711() {
    long count = 0;

    // Espera a que el módulo esté listo
    while (gpio_get(PIN_DT));

    for (int i = 0; i < 24; i++) {
        gpio_put(PIN_SCK, 1);
        sleep_us(1);
        count = count << 1;
        gpio_put(PIN_SCK, 0);
        sleep_us(1);
        if (gpio_get(PIN_DT)) {
            count++;
        }
    }

    // Pulso 25 para fijar ganancia a 128
    gpio_put(PIN_SCK, 1);
    sleep_us(1);
    gpio_put(PIN_SCK, 0);
    sleep_us(1);

    // Conversión a número negativo si es necesario
    if (count & 0x800000) {
        count |= ~0xFFFFFF;
    }

    return count;
}

long promedio(int muestras) {
    long suma = 0;
    for (int i = 0; i < muestras; i++) {
        suma += leer_hx711();
    }
    return suma / muestras;
}

void hx711_sleep() {
    gpio_put(PIN_SCK, 1);
    sleep_us(80); // más de 60us para dormir
}

void hx711_wake() {
    gpio_put(PIN_SCK, 0);
}