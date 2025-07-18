#include <stdio.h>
#include "smart_blender.h"
#include "recetas.h"
#include "load_cell.h"
#include "limit_switch.h"
#include "batido_control.h"
#include "current_sensor.h"
#include "ultrasonic.h"
#include "encoder.h"
#include "pico/stdlib.h"
#include "hardware/sync.h"
#include "boton_reset.h"
// #include "pico/timeout_helper.h" // opcional según entorno

typedef enum {
    ESTADO_CARGAR_RECETA,
    ESTADO_PESAR_INGREDIENTES,
    ESTADO_VERIFICAR_TAPA,
    ESTADO_BATIDO,
    ESTADO_REPOSO,
    ESTADO_FINALIZADO
} Estado;

static Estado estado_actual = ESTADO_CARGAR_RECETA;
Receta receta_actual;
int receta_id = 0;
float peso_final = 0.0f;

// --- Funciones de cada estado ---
static void estado_cargar_receta() {
    char buffer[64];
            printf("📝 Ingresa el nombre de la receta:\n");

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

                printf("🔤 Recibido: '%c' (0x%02x)\n", c, c);

                if (c == '\r' || c == '\n') break;
                if (idx < sizeof(buffer) - 1) {
                    buffer[idx++] = c;
                }
            }

            buffer[idx] = '\0';

            if (!cargar_receta(buffer, &receta_actual)) {
                printf("❌ Receta '%s' no encontrada\n", buffer);
            }

            printf("✅ Receta '%s' cargada\n", buffer);
            estado_actual = ESTADO_PESAR_INGREDIENTES;
}

static void estado_pesar_ingredientes() {
    printf("⚖️ Iniciando pesaje de ingredientes...\n");
    printf("total_ingredientes: %d\n", receta_actual.total_ingredientes);

    for (int i = 0; i < receta_actual.total_ingredientes; i++) {
        const char* nombre;
        float cantidad;
        nombre = obtener_ingrediente(&receta_actual, i, &cantidad);

        printf("➡️ Poner ingrediente: %s (%.1f g)\n", nombre, cantidad);

        while (leer_peso_gramos() < cantidad) {
            float peso_actual = leer_peso_gramos();
            printf("📦 Peso actual: %.1f g / %.1f g\n", peso_actual, cantidad);

            if (cantidad - (peso_final - peso_actual) < 5.0f) {
                printf("✅ %s agregado\n", nombre);
                peso_final += peso_actual;
                break;
            }
            sleep_ms(500);
        }
    }
    printf("✅ Todos los ingredientes pesados. Verificando tapa...\n");
    estado_actual = ESTADO_BATIDO;
}

static void estado_batido(uint trig_pin, uint echo_pin) {
    printf("🌀 Iniciando batido...\n");
    absolute_time_t inicio_batido = get_absolute_time();
    iniciar_pwm_motor(receta_actual.pwm_batido);

    while (true) {
        verificar_tapa_y_mover_servos();
        if (tapa_cerrada) {
            printf("✅ Tapa cerrada. Iniciando batido...\n");
            iniciar_pwm_motor(receta_actual.pwm_batido);
            int64_t tiempo_transcurrido = absolute_time_diff_us(inicio_batido, get_absolute_time());
            printf("⏱️ Batido en curso: %d ms\n", (int)(tiempo_transcurrido));
            printf("Duración total del batido: %d segundos\n", receta_actual.tiempo_batido * 1000000);

            if (tiempo_transcurrido < receta_actual.tiempo_batido * 10000000) {
                float corriente = current_sensor_get_current();
                float nivel = ultrasonic_get_distance_cm(trig_pin, echo_pin);
                float rpm = leer_rpm_filtrada();
                printf("🔁 Corriente: %.2f A, Nivel: %.1f cm, RPM: %.1f\n", corriente, nivel, rpm);
            }
            estado_actual = ESTADO_REPOSO;
         
        } else {
            printf("⚠️ Tapa abierta. Abortando batido...\n");
            detener_pwm_motor();
            sleep_ms(1000);
            return;
        }
    }
}

static void estado_reposo(uint trig_pin, uint echo_pin) {
    printf("😴 Reposando...\n");
    absolute_time_t inicio_reposo = get_absolute_time();
    absolute_time_t fin_reposo = delayed_by_ms(inicio_reposo, receta_actual.tiempo_reposo * 1000);

    while (absolute_time_diff_us(get_absolute_time(), fin_reposo) > 0) {
        verificar_tapa_y_mover_servos();
        if (!tapa_cerrada) {
            printf("⚠️ Tapa abierta durante el reposo. Abortando...\n");
            estado_actual = ESTADO_FINALIZADO;
            break;
        }
    }
    printf("🏁 Fin del reposo.");
    if (!tapa_cerrada) {
        float nivel_final = ultrasonic_get_distance_cm(trig_pin, echo_pin);
        printf("Nivel final: %.1f cm\n", nivel_final);
        estado_actual = ESTADO_FINALIZADO;
    }
}

static void estado_finalizado() {

    printf("🔓 Sistema desbloqueado. Proceso completado exitosamente.\n");
    printf("Esperando a que el usuario presione el botón de reset...\n");

    boton_reset_estado();
    sleep_ms(500);
    __wfi();

    if (boton_fue_presionado()) {
        printf("🔄 Botón de reset presionado. Reiniciando sistema...\n");
        boton_reset_estado();
        estado_actual = ESTADO_CARGAR_RECETA;
    } else {
        printf("❌ No se presionó el botón de reset. Saliendo...\n");
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
