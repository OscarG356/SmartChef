#ifndef SMART_BLENDER_H
#define SMART_BLENDER_H

#include "pico/stdlib.h"

// Estados de funcionamiento de la licuadora
typedef enum {
    BLENDER_IDLE,      // Esperando para iniciar, bajo consumo.
    BLENDER_RUNNING,   // Ciclo de licuado en progreso.
    BLENDER_STOPPED,   // Ciclo finalizado o detenido por el usuario.
    BLENDER_ERROR,     // Error detectado (sobrecorriente, etc.).
    BLENDER_RECEIVING_RECIPE // Recibiendo receta y verificando cantidades
} SmartBlenderState;

// Tipos de callback para notificar al programa principal
typedef enum {
    CALLBACK_CYCLE_FINISHED,
    CALLBACK_ERROR
} BlenderCallbackType;

// Firma del callback
typedef void (*blender_callback_t)(SmartBlenderState new_state);

// Estructura principal que contiene la configuración y el estado de la licuadora
typedef struct {
    // --- PINES ---
    uint motor_pwm_pin;
    uint start_button_pin; // Pin para iniciar ciclo y despertar de WFI

    // --- ESTADO ---
    SmartBlenderState state;

    // --- CALLBACKS ---
    blender_callback_t on_cycle_finished;
    blender_callback_t on_error;

} SmartBlender;

/**
 * @brief Inicializa la licuadora inteligente.
 * 
 * Configura los pines, interrupciones y el estado inicial.
 * @param blender Puntero a la estructura SmartBlender.
 * @param motor_pin Pin PWM para el motor.
 * @param button_pin Pin para el botón de inicio/despertar.
 */
void smart_blender_init(SmartBlender *blender, uint motor_pin, uint button_pin);

/**
 * @brief Bucle principal de la lógica de la licuadora.
 * 
 * Esta función debe ser llamada repetidamente en el bucle principal del programa.
 * Gestiona la máquina de estados.
 * @param blender Puntero a la estructura SmartBlender.
 */
void smart_blender_task(SmartBlender *blender);

/**
 * @brief Inicia un ciclo de licuado.
 * 
 * Cambia el estado a RUNNING si está en IDLE o STOPPED.
 * @param blender Puntero a la estructura SmartBlender.
 */
void smart_blender_start(SmartBlender *blender);

/**
 * @brief Registra una función de callback para eventos específicos.
 *
 * @param blender Puntero a la estructura SmartBlender.
 * @param type Tipo de evento para el que se registra el callback.
 * @param callback Puntero a la función de callback.
 */
void smart_blender_register_callback(SmartBlender *blender, BlenderCallbackType type, blender_callback_t callback);

/**
 * @brief Recibe recetas y verifica cantidades de ingredientes.
 * 
 * Cambia el estado a RECEIVING_RECIPE y procesa los datos recibidos.
 * @param blender Puntero a la estructura SmartBlender.
 */
void smart_blender_receive_recipe(SmartBlender *blender);

#endif // SMART_BLENDER_H
