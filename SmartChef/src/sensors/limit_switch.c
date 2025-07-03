#include "limit_switch.h"
#include <stdio.h>

// Inicializa los pines como entradas con pull-up
void limit_switch_init(void) {
    gpio_init(SWITCH1_PIN);
    gpio_set_dir(SWITCH1_PIN, GPIO_IN);
    gpio_pull_up(SWITCH1_PIN);

    gpio_init(SWITCH2_PIN);
    gpio_set_dir(SWITCH2_PIN, GPIO_IN);
    gpio_pull_up(SWITCH2_PIN);
}

// Verifica si ambos sensores están presionados (estado LOW)
bool tapa_cerrada(void) {
    return gpio_get(SWITCH1_PIN) == 0 && gpio_get(SWITCH2_PIN) == 0;
}

// Imprime por serial si la tapa no está cerrada correctamente
void verificar_tapa_por_serial(void) {
    if (!tapa_cerrada()) {
        printf(" Tapa mal cerrada. Verifica los sensores.\n");
    }
}
