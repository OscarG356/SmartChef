#include <stdio.h>
#include "smart_blender.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/sync.h"
#include "hardware/structs/scb.h"
#include "flash_i2c.h"
#include "current_sensor.h"
#include "load_cell.h"


// Cambiar las macros a variables globales modificables
uint32_t blend_cycle_ms = 10000; // Tiempo de batido en milisegundos
uint32_t motor_pwm_level = 50000; // Nivel de PWM para el motor

// Estructura interna para manejar el estado y los tiempos
static struct {
    SmartBlender *instance;
    uint slice_num;
    uint chan_num;
    absolute_time_t cycle_start_time;
} internal_state;

// --- Prototipos de funciones internas ---
static void start_motor();
static void stop_motor();
static void button_irq_handler(uint gpio, uint32_t events);

// --- Implementación de funciones públicas ---

void smart_blender_init(SmartBlender *blender, uint motor_pin, uint button_pin) {
    internal_state.instance = blender; // Guardamos la instancia para el IRQ
    blender->motor_pwm_pin = motor_pin;
    blender->start_button_pin = button_pin;
    blender->state = BLENDER_IDLE;
    blender->on_cycle_finished = NULL;
    blender->on_error = NULL;

    // Configurar PWM para el motor
    gpio_set_function(blender->motor_pwm_pin, GPIO_FUNC_PWM);
    internal_state.slice_num = pwm_gpio_to_slice_num(blender->motor_pwm_pin);
    internal_state.chan_num = pwm_gpio_to_channel(blender->motor_pwm_pin);
    pwm_set_enabled(internal_state.slice_num, false); // Empezar apagado

    // Configurar el botón de inicio con interrupción
    gpio_init(blender->start_button_pin);
    gpio_set_dir(blender->start_button_pin, GPIO_IN);
    gpio_pull_up(blender->start_button_pin); // Asumimos un botón a GND

    // Configurar y habilitar la interrupción para el botón
    gpio_set_irq_enabled_with_callback(blender->start_button_pin, GPIO_IRQ_EDGE_FALL, true, &button_irq_handler);
}

void smart_blender_task(SmartBlender *blender) {
    switch (blender->state) {
        case BLENDER_IDLE:
            // Entrar en modo de bajo consumo (DORMANT)
            printf("Entrando en modo DORMANT desde BLENDER_IDLE...\n");

            // Configurar el pin de interrupción para despertar
            gpio_init(blender->start_button_pin);
            gpio_set_dir(blender->start_button_pin, GPIO_IN);
            gpio_pull_up(blender->start_button_pin);
            gpio_set_irq_enabled_with_callback(blender->start_button_pin, GPIO_IRQ_EDGE_FALL, true, &button_irq_handler);

            // Detener el procesador hasta que ocurra una interrupción
            __wfi();

            printf("Salí del modo DORMANT.\n");
            break;

        case BLENDER_RUNNING:
            // Comprobar si el tiempo del ciclo ha terminado
            if (absolute_time_diff_us(internal_state.cycle_start_time, get_absolute_time()) / 1000 > blend_cycle_ms) {
                stop_motor();
                blender->state = BLENDER_STOPPED;
                // Notificar al programa principal que el ciclo ha terminado
                if (blender->on_cycle_finished) {
                    blender->on_cycle_finished(blender->state);
                }
            }
            break;

        case BLENDER_STOPPED:
        case BLENDER_ERROR:
            // El sistema está detenido. Esperar a que el botón inicie un nuevo ciclo.
            break;

        case BLENDER_RECEIVING_RECIPE: {
            char buffer[128];
            uint8_t receta_id;

            // Leer identificador de receta por serial
            if (scanf("%hhu", &receta_id) > 0) {
                printf("Identificador de receta recibido: %d\n", receta_id);

                // Leer receta desde la memoria flash
                Receta receta;
                leer_receta(receta_id, &receta);

                printf("Receta: %s\n", receta.nombre);
                printf("Tiempo de batido: %d ms\n", receta.tiempo_batido);
                printf("Velocidad del motor: %d\n", receta.velocidad_motor);
                printf("Cantidad esperada: %d g\n", receta.cantidad);

                // Configurar parámetros dinámicos
                blend_cycle_ms = receta.tiempo_batido;
                motor_pwm_level = receta.velocidad_motor;

                // Verificar cantidades con sensores
                float current = current_sensor_get_current();
                long weight = leer_hx711();

                if (weight < receta.cantidad) {
                    printf("Cantidad insuficiente: %ld g\n", weight);
                    blender->state = BLENDER_ERROR;
                } else {
                    printf("Cantidad correcta: %ld g\n", weight);
                    blender->state = BLENDER_RUNNING;
                    internal_state.cycle_start_time = get_absolute_time();
                    start_motor();
                }
            }
            break;
        }
    }
}

void smart_blender_start(SmartBlender *blender) {
    if (blender->state == BLENDER_IDLE || blender->state == BLENDER_STOPPED) {
        blender->state = BLENDER_RUNNING;
        internal_state.cycle_start_time = get_absolute_time();
        start_motor();
    }
}

void smart_blender_register_callback(SmartBlender *blender, BlenderCallbackType type, blender_callback_t callback) {
    if (type == CALLBACK_CYCLE_FINISHED) {
        blender->on_cycle_finished = callback;
    } else if (type == CALLBACK_ERROR) {
        blender->on_error = callback;
    }
}

// --- Implementación de funciones internas ---

static void start_motor() {
    pwm_set_chan_level(internal_state.slice_num, internal_state.chan_num, motor_pwm_level);
    pwm_set_enabled(internal_state.slice_num, true);
}

static void stop_motor() {
    pwm_set_enabled(internal_state.slice_num, false);
}

/**
 * @brief Manejador de la interrupción del botón.
 * Se ejecuta cuando se presiona el botón de inicio.
 */
static void button_irq_handler(uint gpio, uint32_t events) {
    // Iniciar el ciclo de la licuadora si no está ya en marcha
    if (internal_state.instance) {
        smart_blender_start(internal_state.instance);
    }
}
