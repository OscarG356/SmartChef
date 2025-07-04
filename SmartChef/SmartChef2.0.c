#include <stdio.h>
#include "pico/stdlib.h"
#include "smart_blender.h"

#define MOTOR_PIN 2 // Pin GPIO para el motor
#define BUTTON_PIN 3 // Pin GPIO para el botón

int main() {
    stdio_init_all();

    // Crear una instancia de SmartBlender
    SmartBlender blender;

    // Inicializar la licuadora inteligente
    smart_blender_init(&blender, MOTOR_PIN, BUTTON_PIN);

    while (true) {
        // Ejecutar la tarea de la licuadora
        smart_blender_task(&blender);
    }

    return 0;
}