#ifndef LDR_H
#define LDR_H

#include "driver/adc.h"

#define LDR_ADC_CHANNEL ADC1_CHANNEL_6


typedef struct {
    adc_unit_t adc_device;
    adc2_channel_t adc_channel;
    float k1;
    float k2;
} ldr_handle_t;

void ldr_init();
float ldr_get_calibrated_illuminance(ldr_handle_t *ldr);
void ldr_task(void *params);
float get_ldr_illuminance();

#endif // LDR_H
