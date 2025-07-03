#include "pico/stdlib.h"
#include <stdio.h>

// Pines del HX711
#define HX711_DT 3
#define HX711_SCK 2

// ⚖️ Calibración manual
#define OFFSET_MANUAL 127500L     // Valor promedio sin peso (ajusta si cambia)
#define FACTOR_ESCALA 1076.44f    // Valor calibrado: (314800 - 127500) / 174 g

// Leer 24 bits desde HX711
long leer_hx711() {
    long count = 0;

    while (gpio_get(HX711_DT)); // Espera a que el módulo esté listo

    for (int i = 0; i < 24; i++) {
        gpio_put(HX711_SCK, 1);
        sleep_us(1);
        count = count << 1;
        gpio_put(HX711_SCK, 0);
        sleep_us(1);

        if (gpio_get(HX711_DT)) count++;
    }

    // Pulso 25 para fijar ganancia a 128
    gpio_put(HX711_SCK, 1);
    sleep_us(1);
    gpio_put(HX711_SCK, 0);
    sleep_us(1);

    // Conversión a número negativo si es necesario
    if (count & 0x800000) count |= 0xFF000000;

    return count;
}

// Promedia N muestras
long promedio(int muestras) {
    long suma = 0;
    for (int i = 0; i < muestras; i++) {
        suma += leer_hx711();
    }
    return suma / muestras;
}

// Apagar y encender HX711 entre lecturas
void hx711_sleep() {
    gpio_put(HX711_SCK, 1);
    sleep_us(80); // más de 60us para dormir
}
void hx711_wake() {
    gpio_put(HX711_SCK, 0);
}

int main() {
    stdio_init_all();

    // Esperar conexión por USB (opcional)
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }

    printf("Balanza lista con offset manual\n");

    gpio_init(HX711_DT);
    gpio_set_dir(HX711_DT, GPIO_IN);

    gpio_init(HX711_SCK);
    gpio_set_dir(HX711_SCK, GPIO_OUT);
    gpio_put(HX711_SCK, 0); // asegúrate de dejar el reloj en bajo

    while (true) {
        long lectura_cruda = promedio(10);                // Crudo
        long lectura_neta = lectura_cruda - OFFSET_MANUAL; // Tara manual
        float peso = lectura_neta / FACTOR_ESCALA;         // Conversión a gramos

        printf("Crudo: %ld\tNeto: %ld\tPeso: %.1f g\n", lectura_cruda, lectura_neta, peso);

        hx711_sleep();
        sleep_ms(3000);  // Espera 3 segundos
        hx711_wake();
    }

    return 0;
}
