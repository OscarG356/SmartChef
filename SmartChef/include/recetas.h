#ifndef RECETAS_H
#define RECETAS_H

#define MAX_INGREDIENTES 10

typedef struct {
    char nombre[64];
    char porcion[32];
    char ingredientes[MAX_INGREDIENTES][64];
    float cantidades[MAX_INGREDIENTES];
    int total_ingredientes;
    int tiempo_batido;    // segundos
    int tiempo_reposo;    // segundos
    int pwm_batido;       // valor PWM
} Receta;

// Funciones principales
int cargar_receta(const char* nombre, Receta* destino);

// Funciones de acceso
const char* obtener_ingrediente(const Receta* receta, int idx, float* cantidad);

#endif
