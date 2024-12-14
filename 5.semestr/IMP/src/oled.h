/*
IMS - Aplikace ovládaná dvojicí rotačních enkodérů 
Autor: Michálek Kryštof (xmicha94)
! Není součást mého řešení, převážně generováno AI !
*/

#ifndef OLED_H
#define OLED_H

#include "driver/spi_master.h"
#include "driver/gpio.h"

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
void spi_write(uint8_t data);
void send_command(uint8_t command);
void send_data(uint8_t data);
void set_page_address(uint8_t page);
void set_column_address(uint8_t column);
void set_pixel(int x, int y, int color);
void erase_display();
void game_over();

#endif // OLED_H
