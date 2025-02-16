#include "ldr.h"

void ldr_init() {
    // Inicializa o ADC2 corretamente antes de rodar as tasks
    adc2_config_channel_atten(ADC2_CHANNEL_0, ADC_ATTEN_DB_11);
}

int get_average_adc(ldr_handle_t *ldr) {
    int sum = 0;
    for (int i = 0; i < NUM_SAMPLES; i++) {
        int raw_value;
        adc2_get_raw(ldr->adc_channel, ADC_WIDTH_BIT_12, &raw_value); // Captura ADC2 corretamente
        sum += raw_value;
        vTaskDelay(pdMS_TO_TICKS(10)); // Pequeno delay para reduzir ruído
    }
    return sum / NUM_SAMPLES;
}

// 🔹 Função para calcular a iluminância calibrada corretamente
float ldr_get_calibrated_illuminance(ldr_handle_t *ldr) {
    int raw_adc = get_average_adc(ldr); // Captura o valor do ADC

    if (raw_adc < ADC_MIN) raw_adc = ADC_MIN;
    if (raw_adc > ADC_MAX) raw_adc = ADC_MAX;

    if (ADC_MAX == ADC_MIN) return 0.0;

    float illuminance = (1.0 - ((float)(raw_adc - ADC_MIN) / (ADC_MAX - ADC_MIN))) * 100.0;

    if (illuminance < 0.0) illuminance = 0.0;
    if (illuminance > 100.0) illuminance = 100.0;

    return illuminance;
}

void ldr_task (void *params) {
    ldr_handle_t ldr = {
        .adc_device = ADC_2,  
        .adc_channel = ADC2_CHANNEL_0, 
        .k1 = 49.95095,
        .k2 = 10.34034
    };


    while (true) {
        float illuminance = ldr_get_calibrated_illuminance(&ldr);

        printf("Iluminância Ajustada: %.1f%%\n", illuminance);
        printf("--------------------\n");

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}
