#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "oled.h"

// Define pinnumbers for the oled display
#define OLED_RST GPIO_NUM_27  // Reset pin
#define OLED_CS GPIO_NUM_5      // Chip select pin
#define OLED_DC GPIO_NUM_4     // Data/Command pin
#define OLED_CLK GPIO_NUM_18    // Clock pin
#define OLED_MOSI GPIO_NUM_23   // Data pin

// Define pin numbers for the rotary encoders
#define POS_X_A GPIO_NUM_25     // CLK pin for encoder X
#define POS_X_B GPIO_NUM_26     // DT pin for encoder X
#define POS_Y_A GPIO_NUM_17     // CLK pin for encoder Y
#define POS_Y_B GPIO_NUM_16     // DT pin for encoder Y

#define DELAY (1 / portTICK_PERIOD_MS)  // Delay in milliseconds for FreeRTOS

// Variables for tracking encoders position
int pos_x = 0, pos_y = 0;
int pos_x_last, pos_y_last;
int pos_x_val, pos_y_val;
int color = 1;

// Function to read the encoder state with debounce
int read_encoder(int pin_a, int pin_b, int *last_state) {
    int current_state = gpio_get_level(pin_a);
    if (current_state != *last_state) {
        vTaskDelay(DELAY);  // Debounce delay
        if (current_state != gpio_get_level(pin_a)) { // Check again after delay
            return 0; // No change
        }
        *last_state = current_state; // Update last state
        // Determine the direction of rotation
        return (gpio_get_level(pin_b) == current_state) ? 1 : -1; // Clockwise or counterclockwise
    }
    return 0; // No change
}

void app_main() {
    gpio_set_direction(OLED_DC, GPIO_MODE_OUTPUT);
    gpio_set_direction(OLED_RST, GPIO_MODE_OUTPUT);
    // Initialize the OLED
    oled_init();

    // Set GPIO direction for the encoders
    gpio_set_direction(POS_X_A, GPIO_MODE_INPUT);
    gpio_set_direction(POS_X_B, GPIO_MODE_INPUT);
    gpio_set_direction(POS_Y_A, GPIO_MODE_INPUT);
    gpio_set_direction(POS_Y_B, GPIO_MODE_INPUT);
    // Read initial state of pin A
    int pos_x_last = gpio_get_level(POS_X_A);
    int pos_y_last = gpio_get_level(POS_Y_A);

    while (1) {
        // Read the encoders
        int x_change = read_encoder(POS_X_A, POS_X_B, &pos_x_last);
        int y_change = read_encoder(POS_Y_A, POS_Y_B, &pos_y_last);

        // Update positions based on the direction of rotation
        if((pos_x > 0 && x_change == -1) || (pos_x < 127 && x_change == 1))
            pos_x += x_change;
        if((pos_y > 0 && y_change == 1) || (pos_y < 63 && y_change == -1))
            pos_y -= y_change;

        // Print positions if changed
        if (x_change != 0 || y_change != 0) {
            printf("Position: [%d,%d]\n", pos_x, pos_y);
            //oled_draw_pixel(pos_x, pos_y);
        }

        if(pos_x == pos_y)
        {
            oled_fill_screen();
        }
        else
        {
            oled_erase_screen();
        }

        // Small delay for loop consistency
        vTaskDelay(DELAY);
    }
}
