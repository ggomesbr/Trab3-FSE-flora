#include "water_sensor.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "esp_sleep.h"
#include "light_sleep.h"
#include "json_treatment.h"

#define TAG "WATER_SENSOR"
#define LED_GPIO GPIO_NUM_2


static esp_adc_cal_characteristics_t adc_chars;

float water_level;

bool led_state = false;

void led_toggle() {
    led_state = !led_state;
    gpio_set_level(LED_GPIO, led_state);
}

void led_init() {
    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
}

void water_sensor_init() {
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(WATER_SENSOR_CHANNEL, ADC_ATTEN_DB_11);
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 0, &adc_chars);
}

int get_average_water_adc() {
    int sum = 0;
    for (int i = 0; i < NUM_SAMPLES; i++) {
        sum += adc1_get_raw(WATER_SENSOR_CHANNEL);
    }
    return sum / NUM_SAMPLES;
}


float water_get_calibrated_level() {
    int raw_adc = get_average_water_adc();
    raw_adc = (raw_adc < ADC_MIN) ? ADC_MIN : (raw_adc > ADC_MAX) ? ADC_MAX : raw_adc;

    if (ADC_MAX == ADC_MIN) return 0.0;

    float water_level = ((float)(raw_adc - ADC_MIN) / (ADC_MAX - ADC_MIN)) * 100.0;
    return (water_level < 0.0) ? 0.0 : (water_level > 100.0) ? 100.0 : water_level;
}

void water_sensor_task(void *pvParameters) {
    water_sensor_init();
    led_init();
    while (1) {
        water_level = water_get_calibrated_level();
        ESP_LOGI(TAG, "Nível de Água: %.2f%%", water_level);

        // Aqui você pode adicionar código para enviar os dados via MQTT, se necessário

        if(water_level < 10.0) {
                ESP_LOGE(TAG, "Nível de água muito baixo! %.2f%%", water_level);
                led_toggle();

            } else {
                gpio_set_level(LED_GPIO, false);
                ESP_LOGI(TAG, "Nível de água: %.2f%%", water_level);
                send_water_telemetry(&water_level);
                ESP_LOGI(TAG, "Entrando em light sleep");
                light_sleep_task();
            }

            ESP_LOGI(TAG, "Acordando do light sleep");


        // Aguardar até o próximo ciclo
        vTaskDelay(pdMS_TO_TICKS(500));  // Pequeno delay para evitar sobrecarga da CPU
    }

}
