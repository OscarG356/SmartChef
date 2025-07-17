#include <stdio.h>
#include "pico/stdlib.h"
#include "smart_blender.h"
#include "ultrasonic.h"
#include "limit_switch.h"
#include "load_cell.h"
#include "encoder.h"
#include "current_sensor.h"
#include "boton_reset.h"

// Pines del sistema
#define TRIG_PIN        19
#define ECHO_PIN        18
#define SENSOR1_GPIO    10
#define SENSOR2_GPIO    11
#define SERVO1_GPIO     14
#define SERVO2_GPIO     13


int main() {
    stdio_init_all();
    sleep_ms(3000); // Esperar conexión USB

    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }

    printf("\n🔌 Sistema SmartBlender iniciado\n");

    // === Inicializaciones de hardware ===
    current_sensor_init();
    ultrasonic_init(TRIG_PIN, ECHO_PIN);

    uint slice1 = pwm_init_servo(SERVO1_GPIO);
    uint slice2 = pwm_init_servo(SERVO2_GPIO);
    init_tapa_y_servos(SENSOR1_GPIO, SENSOR2_GPIO, SERVO1_GPIO, slice1, SERVO2_GPIO, slice2);

    
   hx711_init();
    encoder_init();
    boton_init(15); // Inicializar botón de reset en GPIO 15
    

    // === Bucle principal ===
    while (true) {
        blender_loop(TRIG_PIN, ECHO_PIN);
        sleep_ms(100);
    }

    return 0;
}