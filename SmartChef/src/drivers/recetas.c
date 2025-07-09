// === recetas.c ===
#include "recetas.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

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
        .pwm_batido = 32768
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

static Receta* receta_actual = NULL;
static int idx_ingrediente = 0;

int cargar_receta(const char* nombre) {
    for (int i = 0; i < TOTAL_RECETAS; i++) {
        if (strcmp(recetas[i].nombre, nombre) == 0) {
            receta_actual = &recetas[i];
            idx_ingrediente = 0;
            return 1;
        }
    }
    receta_actual = NULL;
    return 0;
}

const char* obtener_siguiente_ingrediente() {
    if (!receta_actual || idx_ingrediente >= receta_actual->total_ingredientes) return NULL;
    return receta_actual->ingredientes[idx_ingrediente++];
}

const char* obtener_ingrediente(int idx, float* cantidad) {
    if (!receta_actual || idx >= receta_actual->total_ingredientes) return NULL;
    *cantidad = receta_actual->cantidades[idx];
    return receta_actual->ingredientes[idx];
}

int total_ingredientes() {
    return receta_actual ? receta_actual->total_ingredientes : 0;
}

int get_tiempo_batido() {
    return receta_actual ? receta_actual->tiempo_batido : 0;
}

int get_tiempo_reposo() {
    return receta_actual ? receta_actual->tiempo_reposo : 0;
}

int get_pwm_batido() {
    return receta_actual ? receta_actual->pwm_batido : 0;
}

// Utilidades extra (opcionalmente usadas)
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
    if (!cargar_receta(nombre)) {
        printf("❌ Receta \"%s\" no encontrada\n", nombre);
        return;
    }

    printf("📄 Receta: %s\nPorcion: %s\n", receta_actual->nombre, receta_actual->porcion);
    for (int i = 0; i < receta_actual->total_ingredientes; i++) {
        printf(" - %s: %.2f g\n", receta_actual->ingredientes[i], receta_actual->cantidades[i]);
    }
    printf("🌀 Tiempo batido: %ds, PWM: %d\n⏳ Reposo: %ds\n",
        receta_actual->tiempo_batido,
        receta_actual->pwm_batido,
        receta_actual->tiempo_reposo);
}
