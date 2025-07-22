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
        .ingredientes = {"Harina de trigo      ","Leche                ","Huevo                ","Azucar               ","Polvo de hornear     ","Mantequilla derretida","Sal                  ","Vainilla             "},
        .cantidades = { 60, 90, 25, 10, 3, 10, 0.5, 1 },
        .total_ingredientes = 8,
        .tiempo_batido = 1,
        .tiempo_reposo = 1,
        .pwm_batido = 65535*0.9f
    },
    {
        .nombre = "Arepa de Maiz",
        .porcion = "1 persona",
        .ingredientes = { "Mezcla P.A.N.", "Agua", "Leche", "Aceite vegetal" },
        .cantidades = { 60, 120, 30, 2.5 },
        .total_ingredientes = 4,
        .tiempo_batido = 2,
        .tiempo_reposo = 1,
        .pwm_batido = 65535*0.8f
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

// === FUNCIONES DE ACCESO DIRECTO ===

const char* obtener_ingrediente(const Receta* receta, int idx, float* cantidad) {
    if (!receta || idx >= receta->total_ingredientes) return NULL;
    *cantidad = receta->cantidades[idx];
    return receta->ingredientes[idx];
}
