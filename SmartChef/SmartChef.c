#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "recetas.h"
#include "batido_control.h"

#define BUFFER_LEN 64

int main() {
    stdio_init_all();
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }

    printf("\n🧪 Test Librería Batido Iniciado\n");

    char buffer[BUFFER_LEN];
    int paso = 0;

    while (true) {
        batido_procesar();

        if (batido_esta_ocupado()) {
            sleep_ms(100);
            continue;
        }

        printf("\n📥 Ingresa el nombre de la receta o 'r' para avanzar paso a paso:\n");

        // Leer línea bloqueante por getchar
        int idx = 0;
        char c;
        while (1) {
            c = getchar();
            if (c == '\n' || c == '\r') break;
            if (idx < BUFFER_LEN - 1) {
                buffer[idx++] = c;
            }
        }
        buffer[idx] = '\0';

        if (strcmp(buffer, "r") == 0) {
            if (paso < total_ingredientes()) {
                const char* ing;
                float cant;
                ing = obtener_ingrediente(paso, &cant);
                printf("🧂 %s: %.2f g\n", ing, cant);
                paso++;
            } else if (!batido_esta_ocupado() && paso == total_ingredientes()) {
                int tiempo_batido = get_tiempo_batido();
                int pwm_batido = get_pwm_batido();
                printf("🌀 Iniciando batido: %ds @ PWM %d\n", tiempo_batido, pwm_batido);
                batido_iniciar(tiempo_batido, pwm_batido);
                paso++; // Para evitar múltiples arranques
            } else {
                printf("✅ Ingredientes y batido completados\n");
            }
        } else if (strlen(buffer) > 0) {
            if (cargar_receta(buffer)) {
                printf("✅ Receta '%s' cargada\n", buffer);
                paso = 0;
            } else {
                printf("❌ Receta no encontrada\n");
            }
        }
    }

    return 0;
}
