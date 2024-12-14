/*
IMS - Aplikace ovládaná dvojicí rotačních enkodérů 
Autor: Michálek Kryštof (xmicha94)
! Není součást mého řešení, převážně generováno AI !
*/

#include "oled.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <string.h>

// SPI device handle
spi_device_handle_t spi;
uint8_t display_buffer[128 * (64 / 8)];

// Function to send data via SPI
void spi_write(uint8_t data) {
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));       // Clear the transaction
    t.length = 8;                   // 8 bits
    t.tx_buffer = &data;            // Data to send
    spi_device_transmit(spi, &t);   // Transmit
}

void send_command(uint8_t command) {
    gpio_set_level(PIN_NUM_DC, 0);  // Set DC to command mode
    spi_write(command);
}

void send_data(uint8_t data) {
    gpio_set_level(PIN_NUM_DC, 1);  // Set DC to data mode
    spi_write(data);
}

void set_page_address(uint8_t page) {
    send_command(0xB0 | page);      // Set page address command
}

void set_column_address(uint8_t column) {
    send_command(0x10 | (column >> 4));     // Set higher column address
    send_command(0x00 | (column & 0x0F));   // Set lower column address
}

void oled_init() {
    // Initialize GPIOs for OLED
    gpio_set_direction(PIN_NUM_CS, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_NUM_DC, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_NUM_RST, GPIO_MODE_OUTPUT);

    // Configure SPI bus
    spi_bus_config_t buscfg = {
        .mosi_io_num = 23,
        .miso_io_num = -1,
        .sclk_io_num = 18,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };
    spi_bus_initialize(VSPI_HOST, &buscfg, 1);

    // Attach OLED to SPI bus
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 1000000, 
        .mode = 0,
        .spics_io_num = PIN_NUM_CS,
        .queue_size = 1,
    };
    spi_bus_add_device(VSPI_HOST, &devcfg, &spi);

    // Reset OLED display
    gpio_set_level(PIN_NUM_RST, 0);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    gpio_set_level(PIN_NUM_RST, 1);

    // Send initialization commands
    send_command(0xAE); // Display OFF
    send_command(0x20); // Set Memory Addressing Mode
    send_command(0x00); // Horizontal Addressing Mode
    send_command(0xB0); // Set Page Start Address for Page Addressing Mode
    send_command(0xC8); // COM Output Scan Direction
    send_command(0x00); // Set low column address
    send_command(0x10); // Set high column address
    send_command(0x40); // Set start line address
    send_command(0x81); // Set contrast control
    send_command(0xFF); // Max contrast
    send_command(0xA1); // Set segment re-map 0 to 127
    send_command(0xA6); // Normal display
    send_command(0xA8); // Set multiplex ratio
    send_command(0x3F); // Ratio 1/64
    send_command(0xA4); // Output follows RAM content
    send_command(0xD3); // Display offset
    send_command(0x00); // No offset
    send_command(0xD5); // Display clock divide ratio/oscillator frequency
    send_command(0xF0); // Divide ratio
    send_command(0xD9); // Pre-charge period
    send_command(0x22); 
    send_command(0xDA); // COM pins configuration
    send_command(0x12); 
    send_command(0xDB); // VCOMH deselect level
    send_command(0x20); 
    send_command(0x8D); // Enable charge pump regulator
    send_command(0x14); 
    send_command(0xAF); // Display ON
}

void set_pixel(int x, int y, int color) {
    uint8_t page = y / 8;               // Determine which page
    uint8_t bit_position = y % 8;        // Determine bit within the page

    uint8_t data = display_buffer[x + page * 128];

    if (color) {
        data |= (1 << bit_position);    // Set the specific bit for this pixel
    }
    else {
        data &= ~(1 << bit_position);  
    }

    display_buffer[x + page * 128] = data;

    set_page_address(page);
    set_column_address(x);
    send_data(data);
}

void erase_display(int color) {
    for (int i = 0; i < (128 * 8); i++) {
        uint8_t invert = color ? 0x00 : 0xFF;
        display_buffer[i] = invert;
        send_data(invert ? 0xFF : 0x00);
    }
}
