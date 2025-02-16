#include "led_control.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "ldr.h"

#define TAG "LED_CONTROL"

void led_control_init(void) {
    // Configuração do Timer PWM
    ledc_timer_config_t timer_config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 1000,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer_config);

    // Configuração do Canal PWM
    ledc_channel_config_t channel_config = {
        .gpio_num = LED_1,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&channel_config);

    // Instalação da função de fade
    ledc_fade_func_install(0);

    // Configuração do botão BOOT como entrada
    gpio_set_direction(BOOT_BUTTON, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BOOT_BUTTON, GPIO_PULLUP_ONLY);

    ESP_LOGI(TAG, "LED e Botão BOOT inicializados.");
}

void led_task(void *pvParameter) {
    while (true) {
        int button_state = gpio_get_level(BOOT_BUTTON);

        float luminancia = get_ldr_illuminance();

        ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, (luminancia/100)*255, 1000, LEDC_FADE_WAIT_DONE); //tenho que publicar o illuminance. ao inves de 50 ;e o illuminance

        vTaskDelay(pdMS_TO_TICKS(100)); // Pequeno delay para evitar leituras erradas
    }
}