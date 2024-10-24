#include "oled.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <string.h>

// SPI device handle
static spi_device_handle_t oled_handle;

static void spi_pre_transfer_callback(spi_transaction_t *t) {
    int dc = (int)t->user;
    gpio_set_level(PIN_NUM_DC, dc);
}

void oled_init() {
    esp_err_t ret;

    // SPI bus configuration
    spi_bus_config_t buscfg = {
        .miso_io_num = -1,  // We don't need MISO for OLED
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };

    // Initialize the SPI bus
    ret = spi_bus_initialize(VSPI_HOST, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);

    // OLED device configuration
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 10 * 1000 * 1000,  // 10 MHz
        .mode = 0,                           // SPI mode 0
        .spics_io_num = PIN_NUM_CS,
        .queue_size = 1,
        .pre_cb = spi_pre_transfer_callback, // Callback to set DC pin
    };

    // Add the OLED to the SPI bus
    ret = spi_bus_add_device(VSPI_HOST, &devcfg, &oled_handle);
    ESP_ERROR_CHECK(ret);

    // Reset OLED
    gpio_set_level(PIN_NUM_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(PIN_NUM_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(100));

    // Send initialization commands
    uint8_t init_cmds[] = {
        OLED_CMD_DISPLAY_OFF,   // Turn off display
        0x20, 0x00,             // Set memory addressing mode to horizontal
        0xA8, 0x3F,             // Set multiplex ratio to 1/64
        0xD3, 0x00,             // Set display offset to 0
        0x40,                   // Set display start line to 0
        0xA1,                   // Set segment re-map to normal
        0xC8,                   // Set COM output scan direction to remap
        0xDA, 0x12,             // Set COM pins hardware config
        0x81, 0x7F,             // Set contrast
        0xA4,                   // Set display to output RAM contents
        0xD5, 0x80,             // Set display clock divide ratio
        0x8D, 0x14,             // Enable charge pump regulator
        OLED_CMD_DISPLAY_ON,     // Turn on display
    };

    for (int i = 0; i < sizeof(init_cmds); i++) {
        spi_transaction_t t;
        memset(&t, 0, sizeof(t));
        t.length = 8;
        t.tx_buffer = &init_cmds[i];
        t.user = (void*)0;  // Command mode (DC low)
        spi_device_transmit(oled_handle, &t);
    }
}

void oled_erase_screen() {
    uint8_t clear_data = 0x00;  // 128 bytes of 0x00 for black pixels

    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 8;
    t.tx_buffer = clear_data;
    t.user = (void*)0;  // Command mode
    spi_device_transmit(oled_handle, &t);
}


void oled_fill_screen() {
    uint8_t fill_cmd = OLED_CMD_FILL;

    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 8;
    t.tx_buffer = &fill_cmd;
    t.user = (void*)0;  // Command mode
    spi_device_transmit(oled_handle, &t);
}

void oled_draw_pixel(int posX, int posY) {
    // Ensure the coordinates are within bounds
    if (posX < 0 || posX >= 128 || posY < 0 || posY >= 64) return;

    // Calculate which page the pixel is on and the bit position within that page
    int page = posY / 8;  // Each page corresponds to 8 rows
    int bit = posY % 8;

    // Prepare commands to set the page and column addresses
    uint8_t column_cmds[] = {
        0xB0 + page,          // Set page address
        (uint8_t)(posX & 0x0F), // Set lower column address
        (uint8_t)(0x10 | ((posX >> 4) & 0x0F)) // Set higher column address
    };

    // Send the commands to set the page and column addresses
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = sizeof(column_cmds) * 8; // Each command is 8 bits
    t.tx_buffer = column_cmds;
    t.user = (void*)0;  // Command mode
    spi_device_transmit(oled_handle, &t);

    // Read the current pixel data for that column
    uint8_t pixel_data;
    memset(&t, 0, sizeof(t));
    t.length = 8;  // Read 8 bits (one byte)
    t.tx_buffer = NULL;  // No data to send for read
    t.rx_buffer = &pixel_data;  // Read into this buffer
    t.user = (void*)1;  // Data mode
    spi_device_transmit(oled_handle, &t);

    // Set the bit for the pixel without erasing others
    pixel_data |= (1 << bit); // Set the bit for the specified pixel

    // Write the modified pixel data back to the display
    memset(&t, 0, sizeof(t));
    t.length = 8;  // 8 bits for the pixel data
    t.tx_buffer = &pixel_data;
    t.user = (void*)1;  // Data mode
    spi_device_transmit(oled_handle, &t);
}