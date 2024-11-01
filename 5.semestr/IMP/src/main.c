#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "oled.h"

// Define pin numbers for the OLED display
#define OLED_RST GPIO_NUM_27  // Reset pin
#define OLED_CS GPIO_NUM_5     // Chip select pin
#define OLED_DC GPIO_NUM_4     // Data/Command pin
#define OLED_CLK GPIO_NUM_18   // Clock pin
#define OLED_MOSI GPIO_NUM_23  // Data pin

// Define pin numbers for the rotary encoders
#define POS_X_A GPIO_NUM_26    // CLK pin for encoder X
#define POS_X_B GPIO_NUM_25    // DT pin for encoder X
#define POS_Y_A GPIO_NUM_17    // CLK pin for encoder Y
#define POS_Y_B GPIO_NUM_16    // DT pin for encoder Y

// Define pin numbers for the encoder buttons
#define POS_X_BUTTON GPIO_NUM_13 // Button for encoder X
#define POS_Y_BUTTON GPIO_NUM_14 // Button for encoder Y

#define DELAY (1 / portTICK_PERIOD_MS) // Delay in milliseconds for FreeRTOS

// Variables for tracking encoder position and button states
int pos_x = 0, pos_y = 0;
int pos_x_last, pos_y_last;
volatile bool x_button_pressed = false; // Flag for X button press
volatile bool y_button_pressed = false; // Flag for Y button press
int color = 1;

// Function to read the encoder state with debounce
int read_encoder(int pin_a, int pin_b, int *last_state) {
    int current_state = gpio_get_level(pin_a);
    if (current_state != *last_state) {
        vTaskDelay(DELAY); // Debounce delay
        if (current_state != gpio_get_level(pin_a)) { // Check again after delay
            return 0; // No change
        }
        *last_state = current_state; // Update last state
        // Determine the direction of rotation
        return (gpio_get_level(pin_b) == current_state) ? 1 : -1; // Clockwise or counterclockwise
    }
    return 0; // No change
}

// Function to handle button press for X button
void IRAM_ATTR x_button_isr_handler(void* arg) {
    x_button_pressed = true; // Set flag on button press
}

// Function to handle button press for Y button
void IRAM_ATTR y_button_isr_handler(void* arg) {
    y_button_pressed = true; // Set flag on button press
}

// Initialize GPIOs and interrupts for the buttons
void init_buttons() {
    gpio_config_t io_conf;
    
    // Configure X button
    io_conf.intr_type = GPIO_INTR_NEGEDGE;  // Trigger on falling edge (button press)
    io_conf.pin_bit_mask = (1ULL << POS_X_BUTTON);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE; // Enable pull-up
    gpio_config(&io_conf);

    // Configure Y button
    io_conf.pin_bit_mask = (1ULL << POS_Y_BUTTON);
    gpio_config(&io_conf);

    // Install ISR service and add handlers
    gpio_install_isr_service(0);
    gpio_isr_handler_add(POS_X_BUTTON, x_button_isr_handler, NULL);
    gpio_isr_handler_add(POS_Y_BUTTON, y_button_isr_handler, NULL);
}

void app_main() {
    // Initialize the OLED
    oled_init();
    init_buttons();
    erase_display(color);

    // Set GPIO direction for the encoders
    gpio_set_direction(POS_X_A, GPIO_MODE_INPUT);
    gpio_set_direction(POS_X_B, GPIO_MODE_INPUT);
    gpio_set_direction(POS_Y_A, GPIO_MODE_INPUT);
    gpio_set_direction(POS_Y_B, GPIO_MODE_INPUT);

    // Read initial state of pin A
    pos_x_last = gpio_get_level(POS_X_A);
    pos_y_last = gpio_get_level(POS_Y_A);

    while (1) {
        // Read the encoders
        int x_change = read_encoder(POS_X_A, POS_X_B, &pos_x_last);
        int y_change = read_encoder(POS_Y_A, POS_Y_B, &pos_y_last);

        // Update positions based on the direction of rotation
        if ((pos_x > 0 && x_change == -1) || (pos_x < 127 && x_change == 1))
            pos_x += x_change;
        if ((pos_y > 0 && y_change == 1) || (pos_y < 63 && y_change == -1))
            pos_y -= y_change;

        // Print positions if changed
        if (x_change != 0 || y_change != 0) {
            printf("Position: [%d,%d]\n", pos_x, pos_y);
            set_pixel(pos_x, pos_y, color);
        }

        // Handle button presses outside the ISR
        if (x_button_pressed) {
            color = 1;
            erase_display(color);
            x_button_pressed = false; // Reset flag
        }

        if (y_button_pressed) {
            color = 0;
            erase_display(color);
            y_button_pressed = false; // Reset flag
        }

        // Small delay for loop consistency
        vTaskDelay(DELAY);
    }
}
