#include "recetas.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

extern Receta receta_actual;  // Declaración externa para acceder a receta_actual
// Base de datos embebida
static Receta recetas[] = {
    {
        .nombre = "Panquecas Esponjosas",
        .porcion = "1 persona",
        .ingredientes = { "Harina de trigo", "Leche", "Huevo", "Azucar", "Polvo de hornear", "Mantequilla derretida", "Sal", "Vainilla" },
        .cantidades = { 60, 90, 25, 10, 3, 10, 0.5, 1 },
        .total_ingredientes = 8,
        .tiempo_batido = 10,
        .tiempo_reposo = 5,
        .pwm_batido = 650000
    },
    {
        .nombre = "Arepa de Maiz",
        .porcion = "1 persona",
        .ingredientes = { "Mezcla P.A.N.", "Agua", "Leche", "Aceite vegetal" },
        .cantidades = { 60, 120, 30, 2.5 },
        .total_ingredientes = 4,
        .tiempo_batido = 5,
        .tiempo_reposo = 3,
        .pwm_batido = 28000
    }
};

#define TOTAL_RECETAS (sizeof(recetas)/sizeof(recetas[0]))

// === FUNCIONES PRINCIPALES ===

int cargar_receta(const char* nombre, Receta* destino) {
    for (int i = 0; i < TOTAL_RECETAS; i++) {
        if (strcmp(recetas[i].nombre, nombre) == 0) {
            *destino = recetas[i];  // Copia todos los datos de la receta
            return 1;
        }
    }
    return 0;
}

void mostrar_recetas_disponibles() {
    printf("📋 Recetas disponibles:\n");
    for (int i = 0; i < TOTAL_RECETAS; i++) {
        printf(" %d. %s\n", i + 1, recetas[i].nombre);
    }
}

// === FUNCIONES DE ACCESO DIRECTO ===

const char* obtener_ingrediente(const Receta* receta, int idx, float* cantidad) {
    if (!receta || idx >= receta->total_ingredientes) return NULL;
    *cantidad = receta->cantidades[idx];
    return receta->ingredientes[idx];
}

int total_ingredientes(const Receta* receta) {
    return receta ? receta->total_ingredientes : 0;
}

int get_tiempo_batido(const Receta* receta) {
    return receta ? receta->tiempo_batido : 0;
}

int get_tiempo_reposo(const Receta* receta) {
    printf("📦 get_tiempo_reposo() = %d\n", receta_actual.tiempo_reposo);
    return receta ? receta->tiempo_reposo : 0;
}

int get_pwm_batido(const Receta* receta) {
    return receta ? receta->pwm_batido : 0;
}

// === FUNCIONES EXTRA (opcional) ===

void buscar_ingrediente(const char* json, const char* clave) {
    const char* ptr = strstr(json, clave);
    if (ptr) {
        char nombre[64];
        float cantidad = 0;
        sscanf(ptr, " \"%63[^\"]\": %f", nombre, &cantidad);
        printf("🔍 %s: %.2f g\n", nombre, cantidad);
    } else {
        printf("❌ No se encontró \"%s\"\n", clave);
    }
}

float escalar_ingrediente(float base, int personas) {
    return base * personas;
}

float escalar_tiempo(float tiempo_base, int personas) {
    return tiempo_base * sqrtf((float)personas);
}

void imprimir_receta_completa(const char* nombre) {
    Receta receta_tmp;
    if (!cargar_receta(nombre, &receta_tmp)) {
        printf("❌ Receta \"%s\" no encontrada\n", nombre);
        return;
    }

    printf("📄 Receta: %s\nPorcion: %s\n", receta_tmp.nombre, receta_tmp.porcion);
    for (int i = 0; i < receta_tmp.total_ingredientes; i++) {
        printf(" - %s: %.2f g\n", receta_tmp.ingredientes[i], receta_tmp.cantidades[i]);
    }
    printf("🌀 Tiempo batido: %ds, PWM: %d\n⏳ Reposo: %ds\n",
        receta_tmp.tiempo_batido,
        receta_tmp.pwm_batido,
        receta_tmp.tiempo_reposo);
}
