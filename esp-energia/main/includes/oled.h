#ifndef OLED_H
#define OLED_H

#include "ssd1306.h"

void oled_init();
void oled_display_text(const char *text);
void oled_task();

#endif // OLED_H