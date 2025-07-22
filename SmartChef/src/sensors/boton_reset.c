#include "boton_reset.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

static volatile bool boton_presionado = false;
static uint boton_gpio = 15;

void boton_irq_callback(uint gpio, uint32_t events) {
    if (gpio == boton_gpio && (events & GPIO_IRQ_EDGE_RISE)) {
        boton_presionado = true;
    }
}

void boton_init(uint gpio_pin) {
    boton_gpio = gpio_pin;
    gpio_init(boton_gpio);
    gpio_set_dir(boton_gpio, GPIO_IN);
    gpio_pull_up(boton_gpio);

    gpio_set_irq_enabled_with_callback(boton_gpio, GPIO_IRQ_EDGE_RISE, true, &boton_irq_callback);
}

bool boton_fue_presionado(void) {
    return boton_presionado;
}

void boton_reset_estado(void) {
    boton_presionado = false;
}
