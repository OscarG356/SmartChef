
#ifndef RECETAS_H
#define RECETAS_H

#include <stddef.h>

#define MAX_ING 10
#define MAX_NOMBRE 64
#define MAX_LINEA 512

// Estructura para representar una receta
typedef struct {
    const char* nombre;
    const char* porcion;
    const char* ingredientes[MAX_ING];
    float cantidades[MAX_ING];
    int total_ingredientes;
    int tiempo_batido;   // segundos
    int tiempo_reposo;   // segundos
    int pwm_batido;      // nivel de PWM (0-65535)
} Receta;

// JSON embebido (si se usa)
extern const char* recetas_json;

// Cargar receta
int cargar_receta(const char* nombre);

// Ingredientes paso a paso
const char* obtener_siguiente_ingrediente();
const char* obtener_ingrediente(int idx, float* cantidad);
int total_ingredientes();

// Parámetros
int get_tiempo_batido();
int get_tiempo_reposo();
int get_pwm_batido();

// Utilidades
void buscar_ingrediente(const char* json, const char* clave);
float escalar_ingrediente(float base, int personas);
float escalar_tiempo(float tiempo_base, int personas);
void imprimir_receta_completa(const char* nombre);

#endif
