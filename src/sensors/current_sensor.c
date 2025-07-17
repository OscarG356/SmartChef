#include "current_sensor.h"

#define VREF              3.3f
#define ADC_RESOLUTION    4096.0f
#define OFFSET_VOLT       1.712f
#define SENSITIVITY       0.073f
#define ADC_PIN           26
#define ADC_INPUT         0

void current_sensor_init(void) {
    adc_init();
    adc_gpio_init(ADC_PIN);
    adc_select_input(ADC_INPUT);
}

float current_sensor_get_current(void) {
    uint16_t raw = adc_read();
    float voltage = raw * VREF / ADC_RESOLUTION;
    float current = (voltage - OFFSET_VOLT) / SENSITIVITY;
    
    if (current < 0.0f) {
        current = 0.0f;
    }
    
    return current;
}