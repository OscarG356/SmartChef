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
void mostrar_recetas_disponibles();
void imprimir_receta_completa(const char* nombre);

// Funciones de acceso
const char* obtener_ingrediente(const Receta* receta, int idx, float* cantidad);
int total_ingredientes(const Receta* receta);
int get_tiempo_batido(const Receta* receta);
int get_tiempo_reposo(const Receta* receta);
int get_pwm_batido(const Receta* receta);

// Extras
float escalar_ingrediente(float base, int personas);
float escalar_tiempo(float tiempo, int personas);
void buscar_ingrediente(const char* json, const char* clave);

#endif
