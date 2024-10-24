#ifndef OLED_H
#define OLED_H

#include "driver/spi_master.h"

// Define SPI GPIOs for the OLED
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  18
#define PIN_NUM_CS   5
#define PIN_NUM_DC   4
#define PIN_NUM_RST  27

// OLED screen dimensions
#define OLED_WIDTH   128
#define OLED_HEIGHT  64

// Commands for SSD1306
#define OLED_CMD_DISPLAY_OFF 0xAE
#define OLED_CMD_DISPLAY_ON  0xAF
#define OLED_CMD_FILL        0xA5

void oled_init();
void oled_erase_screen();
void oled_fill_screen();
void oled_draw_pixel();

#endif // OLED_H
