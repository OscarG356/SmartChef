#include <stdio.h>
#include "smart_blender.h"
#include "recetas.h"
#include "load_cell.h"
#include "limit_switch.h"
#include "batido_control.h"
#include "current_sensor.h"
#include "ultrasonic.h"
#include "pico/stdlib.h"
#include "hardware/sync.h"
#include "boton_reset.h"

typedef enum {
    ESTADO_CARGAR_RECETA,
    ESTADO_PESAR_INGREDIENTES,
    ESTADO_VERIFICAR_TAPA,
    ESTADO_BATIDO,
    ESTADO_REPOSO,
    ESTADO_FINALIZADO
} Estado;

static Estado estado_actual = ESTADO_FINALIZADO;
Receta receta_actual;
int receta_id = 0;
float peso_final = 0.0f;

//******************************************
#define LED_PIN 25
static void inicializar_led() {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, true); // Enciende el LED
}

//*******************************************

// --- Funciones de cada estado ---
static void estado_cargar_receta() {
    char buffer[64];

            int idx = 0;
            char c;

            // Esperamos hasta 10 segundos por la entrada completa
            absolute_time_t inicio = get_absolute_time();

            while (absolute_time_diff_us(inicio, get_absolute_time()) < 10000000) {
                int ch = getchar_timeout_us(0);
                if (ch < 0) {
                    sleep_ms(10); // Evita uso innecesario de CPU
                    continue;
                }
                c = (char)ch;

                if (c == '\r' || c == '\n') break;
                if (idx < sizeof(buffer) - 1) {
                    buffer[idx++] = c;
                }
            }

            buffer[idx] = '\0';

            if (!cargar_receta(buffer, &receta_actual)) {
                return; // Si no se carga la receta, salimos
            }
            estado_actual = ESTADO_PESAR_INGREDIENTES;
            inicializar_led();
}

static void estado_pesar_ingredientes() {

    for (int i = 0; i < receta_actual.total_ingredientes; i++) {
        const char* nombre;
        float cantidad;
        nombre = obtener_ingrediente(&receta_actual, i, &cantidad);

        while (leer_peso_gramos() < cantidad) {
            float peso_actual = leer_peso_gramos();
            float peso_diferencia = cantidad - (peso_final - peso_actual);

            if (peso_diferencia < 5.0f) {
                peso_final += peso_actual;
                break;
            }
            sleep_ms(500);
            printf("1:%s:%.2fg:%.2f", nombre, cantidad, peso_diferencia);
        }
    }
    estado_actual = ESTADO_BATIDO;
}

static void estado_batido(uint trig_pin, uint echo_pin) {
    absolute_time_t inicio_batido = get_absolute_time();
    iniciar_pwm_motor(receta_actual.pwm_batido);
    printf(receta_actual.tiempo_batido);

    while (true) {
        verificar_tapa_y_mover_servos();
        if (tapa_cerrada) {
            iniciar_pwm_motor(receta_actual.pwm_batido);
            int64_t tiempo_transcurrido = absolute_time_diff_us(inicio_batido, get_absolute_time());

            if (tiempo_transcurrido < receta_actual.tiempo_batido * 1000000) {
                float corriente = current_sensor_get_current();
                float nivel = ultrasonic_get_distance_cm(trig_pin, echo_pin);
            }
            estado_actual = ESTADO_REPOSO;
         
        } else {
            detener_pwm_motor();
            sleep_ms(1000);
            return;
        }
    }
}

static void estado_reposo(uint trig_pin, uint echo_pin) {
    absolute_time_t inicio_reposo = get_absolute_time();
    absolute_time_t fin_reposo = delayed_by_ms(inicio_reposo, receta_actual.tiempo_reposo * 1000);

    printf(receta_actual.tiempo_reposo);

    while (absolute_time_diff_us(get_absolute_time(), fin_reposo) > 0) {
        verificar_tapa_y_mover_servos();
        if (!tapa_cerrada) {
            estado_actual = ESTADO_FINALIZADO;
            break;
        }
    }
    if (!tapa_cerrada) {
        float nivel_final = ultrasonic_get_distance_cm(trig_pin, echo_pin);
        estado_actual = ESTADO_FINALIZADO;
    }
}

static void estado_finalizado() {
    boton_reset_estado();
    sleep_ms(500);
    __wfi();
    if (boton_fue_presionado()) {
        boton_reset_estado();
        estado_actual = ESTADO_CARGAR_RECETA;
        printf("Init\n");
    } else {
        estado_actual = ESTADO_FINALIZADO;
    }
}

// --- Loop principal ---
void blender_loop(uint trig_pin, uint echo_pin) {
    switch (estado_actual) {
        case ESTADO_CARGAR_RECETA:
            estado_cargar_receta();
            break;
        case ESTADO_PESAR_INGREDIENTES:
            estado_pesar_ingredientes();
            break;
        case ESTADO_BATIDO:
            estado_batido(trig_pin, echo_pin);
            break;
        case ESTADO_REPOSO:
            estado_reposo(trig_pin, echo_pin);
            break;
        case ESTADO_FINALIZADO:
            estado_finalizado();
            break;
        default:
            break;
    }
}

