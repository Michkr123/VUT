#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ssd1306.h"
#include "driver/gpio.h"

// OLED display configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SSD1306_I2C_ADDRESS 0x3C  // Change this if necessary

// Encoder pin definitions
#define ENCODER1_CLK_PIN  12  // Encoder 1 CLK
#define ENCODER1_DT_PIN   14  // Encoder 1 DT
#define ENCODER1_SW_PIN   13  // Encoder 1 button
#define ENCODER2_CLK_PIN  25  // Encoder 2 CLK
#define ENCODER2_DT_PIN   27  // Encoder 2 DT
#define ENCODER2_SW_PIN   26  // Encoder 2 button

// Initial coordinates
int x = 0;  // X position (0-127)
int y = 0;  // Y position (0-63)

// Function to read encoder
int readEncoder(int clkPin, int dtPin) {
    static int lastCLK = HIGH;
    int currentCLK = gpio_get_level(clkPin);
    
    int value = 0;

    // Check for clock rising edge
    if (currentCLK == LOW && lastCLK == HIGH) {
        int dtState = gpio_get_level(dtPin);
        value = (dtState == LOW) ? 1 : -1; // Determine direction
    }

    lastCLK = currentCLK; // Store the current clock state
    return value;
}

void app_main(void) {
    // Initialize the OLED display
    ssd1306_init(SSD1306_I2C_ADDRESS, SCREEN_WIDTH, SCREEN_HEIGHT);
    ssd1306_clear_screen(SSD1306_COLOR_BLACK);

    // Configure GPIO for encoders
    gpio_set_direction(ENCODER1_CLK_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(ENCODER1_DT_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(ENCODER1_SW_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(ENCODER2_CLK_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(ENCODER2_DT_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(ENCODER2_SW_PIN, GPIO_MODE_INPUT);

    while (1) {
        // Read encoders
        int encoder1Value = readEncoder(ENCODER1_CLK_PIN, ENCODER1_DT_PIN);
        int encoder2Value = readEncoder(ENCODER2_CLK_PIN, ENCODER2_DT_PIN);

        // Update X and Y values based on encoder readings
        x = (x + encoder1Value + 128) % 128; // Wrap around
        y = (y + encoder2Value + 64) % 64;   // Wrap around

        // Clear display and draw the selected pixel
        ssd1306_clear_screen(SSD1306_COLOR_BLACK);
        ssd1306_draw_pixel(x, y, SSD1306_COLOR_WHITE);
        ssd1306_refresh();  // Refresh the display

        // Check button presses to reset the display
        if (gpio_get_level(ENCODER1_SW_PIN) == 0 || gpio_get_level(ENCODER2_SW_PIN) == 0) {
            ssd1306_clear_screen(SSD1306_COLOR_BLACK);  // Reset display to all black
            ssd1306_refresh();
            vTaskDelay(500 / portTICK_PERIOD_MS);  // Debounce delay
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);  // Adjust this value for a smoother experience
    }
}
