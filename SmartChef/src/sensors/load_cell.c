#include <stdio.h>

#include "load_cell.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"

// Pines definidos en load_cell.h: HX711_DT y HX711_SCK

void hx711_init() {
    gpio_init(HX711_DT);
    gpio_set_dir(HX711_DT, GPIO_IN);

    gpio_init(HX711_SCK);
    gpio_set_dir(HX711_SCK, GPIO_OUT);
    gpio_put(HX711_SCK, 0);
}

long leer_hx711() {
    long count = 0;

    while (gpio_get(HX711_DT)); // Espera a que el HX711 esté listo

    for (int i = 0; i < 24; i++) {
        gpio_put(HX711_SCK, 1);
        sleep_us(1);
        count = count << 1;
        gpio_put(HX711_SCK, 0);
        sleep_us(1);

        if (gpio_get(HX711_DT)) count++;
    }

    // Pulso 25 para establecer ganancia
    gpio_put(HX711_SCK, 1);
    sleep_us(1);
    gpio_put(HX711_SCK, 0);
    sleep_us(1);

    // Signo extendido para valores negativos
    if (count & 0x800000) count |= 0xFF000000;

    return count;
}

long promedio(int muestras) {
    long suma = 0;
    for (int i = 0; i < muestras; i++) {
        suma += leer_hx711();
    }
    return suma / muestras;
}

float leer_peso_gramos(void) {
    long lectura_cruda = promedio(10);
    float peso = (((lectura_cruda - OFFSET_MANUAL) / FACTOR_ESCALA)-475);

    if (peso < 0.0f) peso = 0.0f;
    hx711_sleep();
    sleep_ms(3000);  // Simula ciclo de bajo consumo
    hx711_wake();

    return peso;
}

void hx711_sleep() {
    gpio_put(HX711_SCK, 1);
    sleep_us(80);
}

void hx711_wake() {
    gpio_put(HX711_SCK, 0);
}
