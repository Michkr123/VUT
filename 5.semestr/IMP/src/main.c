#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "oled.h"

// Define pin numbers for the OLED display
#define OLED_RST GPIO_NUM_27  // Reset pin
#define OLED_CS GPIO_NUM_5    // Chip select pin
#define OLED_DC GPIO_NUM_4    // Data/Command pin
#define OLED_CLK GPIO_NUM_18  // Clock pin
#define OLED_MOSI GPIO_NUM_23 // Data pin

// Define pin numbers for the rotary encoders
#define POS_X_A GPIO_NUM_26    // CLK pin for encoder X
#define POS_X_B GPIO_NUM_25    // DT pin for encoder X
#define POS_Y_A GPIO_NUM_17    // CLK pin for encoder Y
#define POS_Y_B GPIO_NUM_16    // DT pin for encoder Y

// Define pin numbers for the encoder buttons
#define POS_X_BUTTON GPIO_NUM_13 // Button for encoder X
#define POS_Y_BUTTON GPIO_NUM_14 // Button for encoder Y

#define delay vTaskDelay
#define DELAY (10 / portTICK_PERIOD_MS) // Delay in milliseconds for FreeRTOS

// Variables for tracking encoder position and button states
volatile int pos_x_player_a = 125, pos_y_player_a = 32;
volatile int pos_x_player_b = 0, pos_y_player_b = 32;
volatile int real_pos_y_player_a = 32, real_pos_y_player_b = 32;

volatile int ball_pos_x = 64, ball_pos_y = 32, ball_x_dir = 2, ball_y_dir = 2;
volatile int prev_ball_pos_x = 64, prev_ball_pos_y = 32;

volatile bool x_button_pressed = false; // Flag for X button press
volatile bool y_button_pressed = false; // Flag for Y button press
volatile bool update_player_a_flag = false;
volatile bool update_player_b_flag = false;
volatile int color = 1;

void update_ball() {
    // Update the changed pixels for the ball
    if (prev_ball_pos_x != ball_pos_x || prev_ball_pos_y != ball_pos_y) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                set_pixel(prev_ball_pos_x + i, prev_ball_pos_y + j, 0); // Clear previous position
                set_pixel(ball_pos_x + i, ball_pos_y + j, color); // Draw new position
            }
        }
        prev_ball_pos_x = ball_pos_x;
        prev_ball_pos_y = ball_pos_y;
    }
}

void update_player_a() {
    // Clear entire area for player A
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 63; j++) {
            set_pixel(125 + i, j, 0); // Clear the area
        }
    }
    // Draw new position
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 10; j++) {
            set_pixel(pos_x_player_a + i, pos_y_player_a + j, color); // Draw new position
        }
    }
    real_pos_y_player_a = pos_y_player_a;
}

void update_player_b() {
    // Clear entire area for player B
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 63; j++) {
            set_pixel(0 + i, j, 0); // Clear the area
        }
    }
    // Draw new position
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 10; j++) {
            set_pixel(pos_x_player_b + i, pos_y_player_b + j, color); // Draw new position
        }
    }
    real_pos_y_player_b = pos_y_player_b;
}

// Interrupt handler for player_a encoder
void IRAM_ATTR pos_x_isr_handler(void* arg) {
    static int last_a = 0;
    int a = gpio_get_level(POS_X_A);
    int b = gpio_get_level(POS_X_B);

    if (a != last_a && a == 1) {
        if (b == a) {
            pos_y_player_a += 2;
        } else {
            pos_y_player_a -= 2;
        }
        if (pos_y_player_a < 0) pos_y_player_a = 0;
        if (pos_y_player_a > 52) pos_y_player_a = 52;
    }
    last_a = a;
    update_player_a_flag = true;
}

// Interrupt handler for Y encoder
void IRAM_ATTR pos_y_isr_handler(void* arg) {
    static int last_a = 0;
    int a = gpio_get_level(POS_Y_A);
    int b = gpio_get_level(POS_Y_B);

    if (a != last_a && a == 1) {
        if (b == a) {
            pos_y_player_b -= 2;
        } else {
            pos_y_player_b += 2;
        }
        if (pos_y_player_b < 0) pos_y_player_b = 0;
        if (pos_y_player_b > 52) pos_y_player_b = 52;
    }
    last_a = a;
    update_player_b_flag = true;
}

// Interrupt handler for X button
void IRAM_ATTR x_button_isr_handler(void* arg) {
    x_button_pressed = true; // Set flag on button press
}

// Interrupt handler for Y button
void IRAM_ATTR y_button_isr_handler(void* arg) {
    y_button_pressed = true; // Set flag on button press
}

// Initialize GPIOs and interrupts for the encoders and buttons
void init_gpio() {
    gpio_config_t io_conf;

    // Configure X encoder
    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    io_conf.pin_bit_mask = (1ULL << POS_X_A) | (1ULL << POS_X_B);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(POS_X_A, pos_x_isr_handler, NULL);

    // Configure Y encoder
    io_conf.pin_bit_mask = (1ULL << POS_Y_A) | (1ULL << POS_Y_B);
    gpio_config(&io_conf);
    gpio_isr_handler_add(POS_Y_A, pos_y_isr_handler, NULL);

    // Configure X button
    io_conf.intr_type = GPIO_INTR_NEGEDGE; // Trigger on falling edge
    io_conf.pin_bit_mask = (1ULL << POS_X_BUTTON);
    gpio_config(&io_conf);
    gpio_isr_handler_add(POS_X_BUTTON, x_button_isr_handler, NULL);

    // Configure Y button
    io_conf.pin_bit_mask = (1ULL << POS_Y_BUTTON);
    gpio_config(&io_conf);
    gpio_isr_handler_add(POS_Y_BUTTON, y_button_isr_handler, NULL);
}

void app_main() {
    // Initialize the OLED
    oled_init();
    init_gpio();
    erase_display(color);

    update_player_a();
    update_player_b();

    while (1) {
        // Update positions based on the direction of rotation
        update_ball();
        
        if (update_player_a_flag) {
            update_player_a();
            update_player_a_flag = false;
        }

        if (update_player_b_flag) {
            update_player_b();
            update_player_b_flag = false;
        }

        ball_pos_x += ball_x_dir;
        ball_pos_y += ball_y_dir;

        if (ball_pos_x < 3) {
            ball_x_dir = -ball_x_dir;
            ball_pos_x = 3;
        } else if (ball_pos_x > 122) {
            ball_x_dir = -ball_x_dir;
            ball_pos_x = 122;
        }

        if (ball_pos_y < 0) {
            ball_y_dir = -ball_y_dir;
            ball_pos_y = 0;
        } else if (ball_pos_y > 61) {
            ball_y_dir = -ball_y_dir;
            ball_pos_y = 61;
        }

        printf("x: %i, y: %i\n", ball_pos_x, ball_pos_y);

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

        // Small delay for loop consistency and yielding to the scheduler
        delay(DELAY);
    }
}
