#include "oled.h"
#include "ssd1306.h"
#include "string.h"
//#include "water_sensor.h"
#include <time.h>
#include "esp_log.h"
#include "dht11.h"

#define I2C_MASTER_SDA_IO 21   
#define I2C_MASTER_SCL_IO 22   
#define I2C_MASTER_RESET -1    

#define TAG "OLED"

char water_level_str[20];
SSD1306_t dev;

SSD1306_t oled_dev; 

void oled_init() {
    
    i2c_master_init(&oled_dev, I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO, I2C_MASTER_RESET);
    
    ssd1306_init(&oled_dev, 128, 64);
    ssd1306_clear_screen(&oled_dev, false);
}

void oled_display_text(const char *text) {
    ssd1306_clear_screen(&oled_dev, false);
    ssd1306_display_text(&oled_dev, 0, text, 12, false);
}

void oled_task() {
    
    while (true)
    {
        ssd1306_display_text(&dev, 0, "--- FLORA ---", 16, false);
        char lineChar[20];
        sprintf(&lineChar[0], "%.2f", temperature);
        oled_display_text(lineChar);
        srand(time(NULL));
        
        ssd1306_display_text(&dev, 2, lineChar, strlen(lineChar), false);
        
        ESP_LOGI(TAG, "WATER LEVEL: %.f%% funcao: %.f%%", temperature, humidity);

        vTaskDelay(500 / portTICK_PERIOD_MS);

    }

}

