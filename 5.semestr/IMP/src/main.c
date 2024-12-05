#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
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

SemaphoreHandle_t xSemaphore; // Semaphore for accessing OLED display
QueueHandle_t xQueueEncoderA;
QueueHandle_t xQueueEncoderB;

// Variables for tracking encoder position and button states
volatile int pos_x_player_a = 125, pos_y_player_a = 32;
volatile int pos_x_player_b = 0, pos_y_player_b = 32;
volatile int real_pos_y_player_a = 32, real_pos_y_player_b = 32;
volatile int ball_pos_x = 64, ball_pos_y = 32, ball_x_dir = 2, ball_y_dir = 0; // Set y_dir to 2
volatile int prev_ball_pos_x = 64, prev_ball_pos_y = 32;

volatile bool x_button_pressed = false; // Flag for X button press
volatile bool y_button_pressed = false; // Flag for Y button press
volatile bool update_player_a_flag = false;
volatile bool update_player_b_flag = false;
volatile int color = 1;
volatile int game = 1;
volatile int speed = 10, time = 0;
volatile int points_a = 0, points_b = 0;

void update_ball() {
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
        // Update the changed pixels for the ball
        if (prev_ball_pos_x != ball_pos_x || prev_ball_pos_y != ball_pos_y) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (!(prev_ball_pos_x + i == 63)) {
                        set_pixel(prev_ball_pos_x + i, prev_ball_pos_y + j, 0); // Clear previous position
                    }
                    set_pixel(ball_pos_x + i, ball_pos_y + j, color); // Draw new position
                }
            }
            prev_ball_pos_x = ball_pos_x;
            prev_ball_pos_y = ball_pos_y;
        }
        xSemaphoreGive(xSemaphore);
    }
}

void update_player_a() {
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
        // Draw new position
        for (int i = 125; i < 128; i++) {
            for (int j = 0; j < 63; j++) {
                if (j >= pos_y_player_a && j <= pos_y_player_a + 10) {
                    set_pixel(i, j, color); // Draw new position
                } 
                else {
                    set_pixel(i, j, !color); // Clear pixels outside the player's range
                }
            }
        }
        real_pos_y_player_a = pos_y_player_a;
        xSemaphoreGive(xSemaphore);
    }
}

void update_player_b() {
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
        // Draw new position
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 63; j++) {
                if (j >= pos_y_player_b && j <= pos_y_player_b + 10) {
                    set_pixel(i, j, color); // Draw new position
                } else {
                    set_pixel(i, j, !color); // Clear pixels outside the player's range
                }
            }
        }
        real_pos_y_player_b = pos_y_player_b;
        xSemaphoreGive(xSemaphore);
    }
}

// Task to handle ball movement
void ball_task(void *pvParameters) {
    while (1) {
        update_ball();

        if (time % speed == 0) {
            ball_pos_x += ball_x_dir;
            ball_pos_y += ball_y_dir;
        }

        if (ball_pos_x < 3) {
            ball_x_dir = -ball_x_dir;
            ball_pos_x = 3;
            if (!(ball_pos_y >= pos_y_player_b - 2 && ball_pos_y <= pos_y_player_b + 10)) {
                game = 0;
                points_b++;
            }
        } else if (ball_pos_x > 122) {
            ball_x_dir = -ball_x_dir;
            ball_pos_x = 122;
            if (!(ball_pos_y >= pos_y_player_a - 2 && ball_pos_y <= pos_y_player_a + 10)) {
                game = 0;
                points_b++;
            }
        }

        if (ball_pos_y < 0) {
            ball_y_dir = -ball_y_dir;
            ball_pos_y = 0;
        } else if (ball_pos_y > 61) {
            ball_y_dir = -ball_y_dir;
            ball_pos_y = 61;
        }

        delay(DELAY);
    }
}

// Task to handle player A movement
void playerA_task(void *pvParameters) {
    while (1) {
        if (update_player_a_flag) {
            update_player_a();
            update_player_a_flag = false;
        }
        delay(DELAY);
    }
}

// Task to handle player B movement
void playerB_task(void *pvParameters) {
    while (1) {
        if (update_player_b_flag) {
            update_player_b();
            update_player_b_flag = false;
        }
        delay(DELAY);
    }
}

void reset_players() {
    pos_x_player_a = 125;
    pos_y_player_a = 32;
    pos_x_player_b = 0;
    pos_y_player_b = 32;

    update_player_a_flag = true;
    update_player_b_flag = true;
}

void set_game() {
    time = 0;
    speed = 10;

    for (int i = 0; i < 64; i++) {
        set_pixel(63, i, color);
    }
}

void reset_ball() {
    ball_pos_x = 64;
    ball_pos_y = 32;
    ball_x_dir = 2;
    ball_y_dir = 0;
}

// Interrupt handler for player_a encoder
void IRAM_ATTR pos_x_isr_handler(void* arg) {
    static int last_a = 0;
    int a = gpio_get_level(POS_X_A);
    int b = gpio_get_level(POS_X_B);

    if (a != last_a && a == 1) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        int event = (b == a) ? 1 : -1; // 1 for clockwise, -1 for counter-clockwise
        xQueueSendFromISR(xQueueEncoderA, &event, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    last_a = a;
}

// Interrupt handler for player_b encoder
void IRAM_ATTR pos_y_isr_handler(void* arg) {
    static int last_a = 0;
    int a = gpio_get_level(POS_Y_A);
    int b = gpio_get_level(POS_Y_B);

    if (a != last_a && a == 1) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        int event = (b == a) ? -1 : 1; // -1 for clockwise, 1 for counter-clockwise
        xQueueSendFromISR(xQueueEncoderB, &event, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    last_a = a;
}

// Interrupt handler for X button
void IRAM_ATTR x_button_isr_handler(void* arg) {
    if (!game)
        x_button_pressed = true; // Set flag on button press
}

// Interrupt handler for Y button
void IRAM_ATTR y_button_isr_handler(void* arg) {
    if (!game)
        y_button_pressed = true; // Set flag on button press
}

// Task to handle player_a encoder events
void playerA_encoder_task(void *pvParameters) {
    int event;
    while (1) {
        if (xQueueReceive(xQueueEncoderA, &event, portMAX_DELAY)) {
            pos_y_player_a += event * 2;
            if (pos_y_player_a < 0) pos_y_player_a = 0;
            if (pos_y_player_a > 52) pos_y_player_a = 52;
            update_player_a_flag = true;
        }
    }
}

// Task to handle player_b encoder events
void playerB_encoder_task(void *pvParameters) {
    int event;
    while (1) {
        if (xQueueReceive(xQueueEncoderB, &event, portMAX_DELAY)) {
            pos_y_player_b += event * 2;
            if (pos_y_player_b < 0) pos_y_player_b = 0;
            if (pos_y_player_b > 52) pos_y_player_b = 52;
            update_player_b_flag = true;
        }
    }
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
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);
    gpio_isr_handler_add(POS_Y_A, pos_y_isr_handler, NULL);

    // Configure X button
    io_conf.intr_type = GPIO_INTR_NEGEDGE; // Trigger on falling edge
    io_conf.pin_bit_mask = (1ULL << POS_X_BUTTON);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);
    gpio_isr_handler_add(POS_X_BUTTON, x_button_isr_handler, NULL);

    // Configure Y button
    io_conf.intr_type = GPIO_INTR_NEGEDGE; // Trigger on falling edge
    io_conf.pin_bit_mask = (1ULL << POS_Y_BUTTON);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);
    gpio_isr_handler_add(POS_Y_BUTTON, y_button_isr_handler, NULL);
}

void app_main() {
    // Initialize the OLED
    oled_init();
    init_gpio();
    erase_display(color);

    // Create a semaphore for accessing the display
    xSemaphore = xSemaphoreCreateMutex();
    if (xSemaphore == NULL) {
        printf("Failed to create semaphore\n");
        return;
    }

    // Initialize queues for encoder events
    xQueueEncoderA = xQueueCreate(10, sizeof(int));
    xQueueEncoderB = xQueueCreate(10, sizeof(int));
    if (xQueueEncoderA == NULL || xQueueEncoderB == NULL) {
        printf("Failed to create queues\n");
        return;
    }

    set_game();
    update_player_a();
    update_player_b();

    // Create tasks for player movements and ball movement
    xTaskCreate(playerA_task, "PlayerA_Task", 2048, NULL, 1, NULL);
    xTaskCreate(playerB_task, "PlayerB_Task", 2048, NULL, 1, NULL);
    xTaskCreate(ball_task, "Ball_Task", 2048, NULL, 1, NULL);

    // Create tasks for encoder events
    xTaskCreate(playerA_encoder_task, "PlayerA_Encoder_Task", 2048, NULL, 1, NULL);
    xTaskCreate(playerB_encoder_task, "PlayerB_Encoder_Task", 2048, NULL, 1, NULL);

    while (1) {
        while (game) {
            // Update positions based on the direction of rotation
            time++;
            if (time % ((1000 / speed) * 4) == 0) {
                if (speed > 1) {
                    speed--;
                }
            }

            // Small delay for loop consistency and yielding to the scheduler
            delay(DELAY);
        }

        erase_display(!color);
        volatile int player_a_ready = 0, player_b_ready = 0;
        x_button_pressed = false;
        y_button_pressed = false;
        while (!game) {
            // Handle button presses outside the ISR
            if (x_button_pressed) {
                player_a_ready = 1;
                x_button_pressed = false; // Reset flag
            }
            if (y_button_pressed) {
                player_b_ready = 1;
                y_button_pressed = false; // Reset flag
            }
            if (player_a_ready && player_b_ready) {
                erase_display(color);
                game = 1;
                set_game();
                reset_ball();
                reset_players();
            }
        }
    }
}

