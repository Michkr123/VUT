/*
IMS - Aplikace ovládaná dvojicí rotačních enkodérů 
Autor: Michálek Kryštof (xmicha94)
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "oled.h"

// Piny pro OLED display
#define OLED_RST GPIO_NUM_27
#define OLED_CS GPIO_NUM_5 
#define OLED_DC GPIO_NUM_4
#define OLED_CLK GPIO_NUM_18
#define OLED_MOSI GPIO_NUM_23

// Piny pro enkodery
#define PLAYER_A_CLK GPIO_NUM_26
#define PLAYER_A_DT  GPIO_NUM_25

#define PLAYER_B_CLK GPIO_NUM_17
#define PLAYER_B_DT  GPIO_NUM_16

// Piny pro tlačítka
#define PLAYER_A_BUTTON GPIO_NUM_13
#define PLAYER_B_BUTTON GPIO_NUM_14

// Pomocné definice pro práci s delay
#define delay vTaskDelay
#define DELAY (20 / portTICK_PERIOD_MS)
#define DEBOUNCE_DELAY 10

// Proměnné pro debounce
volatile unsigned long last_interrupt_time_a = 0, last_interrupt_time_b = 0;

// Semafor pro ovladani displaye
SemaphoreHandle_t xBallSemaphore;

// Fronty pro přerrušení enkodery
QueueHandle_t xQueueEncoderA;
QueueHandle_t xQueueEncoderB;

// Proměnné pro hráče a míč
volatile int pos_x_player_a = 125, pos_y_player_a = 32;
volatile int pos_x_player_b = 0, pos_y_player_b = 32;
volatile int ball_pos_x = 64, ball_pos_y = 32, ball_x_dir = 2, ball_y_dir = 2;
volatile int prev_ball_pos_x = 64, prev_ball_pos_y = 32;

// Vlajky pro detekci stisknutí tlačítek
volatile bool x_button_pressed = false;
volatile bool y_button_pressed = false; 

// Vlajky pro aktualizaci hráčských pozic
volatile bool update_player_a_flag = false;
volatile bool update_player_b_flag = false;

// Proměnné pro průběh hry
volatile int game = 1;
volatile int speed = 10, time = 0;

// Pomocné pole pro vykreslení obrazovky při prohře
int game_over_screen[64][128] = { 
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},  {0}, {0}, //10
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //20
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0}, //30
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, //40
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, //50
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, //60
    {0}, {0}, {0}, {0} // 64
    };

// Vykreslení pozice míče
void update_ball() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if(prev_ball_pos_x + i != 63){
                set_pixel(prev_ball_pos_x + i, prev_ball_pos_y + j, 0);
                set_pixel(ball_pos_x + i, ball_pos_y + j, 1);
            }
        }
    }
    prev_ball_pos_x = ball_pos_x;
    prev_ball_pos_y = ball_pos_y;
}

// Vykreslení pozice hráče A
void update_player_a() {
    if (xSemaphoreTake(xBallSemaphore, portMAX_DELAY) == pdTRUE) {
        for (int i = pos_x_player_a; i < 128; i++) {
            for (int j = 0; j < 63; j++) {
                if (j >= pos_y_player_a && j <= pos_y_player_a + 10) {
                    set_pixel(i, j, 1);
                } else {
                    set_pixel(i, j, 0);
                } 
            }
        }
        xSemaphoreGive(xBallSemaphore);
    }
}

// Vykreslení pozice hráče B
void update_player_b() {
    if (xSemaphoreTake(xBallSemaphore, portMAX_DELAY) == pdTRUE) {
        for (int i = pos_x_player_b; i < 3; i++) {
            for (int j = 0; j < 63; j++) {
                if (j >= pos_y_player_b && j <= pos_y_player_b + 10) {
                    set_pixel(i, j, 1);
                } else {
                    set_pixel(i, j, 0);
                }
            }
        }
        xSemaphoreGive(xBallSemaphore);
    }
}

// Funkce pro proces míč
void ball_task(void *pvParameters) {
    while (1) {
        // Rychlost míče
        if (time % speed == 0) {
            ball_pos_x += ball_x_dir;
            ball_pos_y += ball_y_dir;
        }

        // Kontrola hranic displaye osy X
        if (ball_pos_x < 3) {
            ball_x_dir = -ball_x_dir;
            ball_pos_x = 3;
            if (!(ball_pos_y >= pos_y_player_b - 2 && ball_pos_y <= pos_y_player_b + 10)) {
                game = 0;
            }
        } else if (ball_pos_x > 122) {
            ball_x_dir = -ball_x_dir;
            ball_pos_x = 122;
            if (!(ball_pos_y >= pos_y_player_a - 2 && ball_pos_y <= pos_y_player_a + 10)) {
                game = 0;
            }
        }

        // Kontrola hranic displaye osy Y
        if (ball_pos_y < 0) {
            ball_y_dir = -ball_y_dir;
            ball_pos_y = 0;
        } else if (ball_pos_y > 61) {
            ball_y_dir = -ball_y_dir;
            ball_pos_y = 61;
        }

        // Vykreslení míče
        if (xSemaphoreTake(xBallSemaphore, portMAX_DELAY) == pdTRUE) {
            update_ball();
            xSemaphoreGive(xBallSemaphore);
        }
        delay(DELAY);
    }
}

// Funcke pro proces hráč A
void playerA_task(void *pvParameters) {
    while (1) {
        if (update_player_a_flag) {
            update_player_a();
            update_player_a_flag = false;
        }
        delay(pdMS_TO_TICKS(10));
    }
}

// Funcke pro proces hráč B
void playerB_task(void *pvParameters) {
    while (1) {
        if (update_player_b_flag) {
            update_player_b();
            update_player_b_flag = false;
        }
        delay(pdMS_TO_TICKS(10));
    }
}

void reset_players() {
    pos_y_player_a = 32;
    pos_y_player_b = 32;
    update_player_a_flag = true;
    update_player_b_flag = true;
}

void set_game() {
    time = 0;
    speed = 10;
    for (int i = 0; i < 64; i++) {
        set_pixel(63, i, 1);
    }
}

void reset_ball() {
    ball_pos_x = 64;
    ball_pos_y = 32;
    ball_x_dir = 2;
    ball_y_dir = 2;
}

// Funkce pro přerušení enkoderu hráče A
void IRAM_ATTR player_a_isr_handler(void* arg) {
    static int last_a = 0;
    int a = gpio_get_level(PLAYER_A_CLK);
    int b = gpio_get_level(PLAYER_A_DT);

    unsigned long interrupt_time = esp_timer_get_time() / 1000;

    if (a != last_a && a == 1 && (interrupt_time - last_interrupt_time_a > DEBOUNCE_DELAY)) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        int event = (b == a) ? 1 : -1; 
        xQueueSendFromISR(xQueueEncoderA, &event, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        last_interrupt_time_a = interrupt_time; 
    }
    last_a = a;
}

// Funkce pro přerušení enkoderu hráče B
void IRAM_ATTR player_b_isr_handler(void* arg) {
    static int last_a = 0;
    int a = gpio_get_level(PLAYER_B_CLK);
    int b = gpio_get_level(PLAYER_B_DT);

    unsigned long interrupt_time = esp_timer_get_time() / 1000;

    if (a != last_a && a == 1 && (interrupt_time - last_interrupt_time_b > DEBOUNCE_DELAY)) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        int event = (b == a) ? -1 : 1;
        xQueueSendFromISR(xQueueEncoderB, &event, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        last_interrupt_time_b = interrupt_time; 
    }
    last_a = a;
}

// přerušení pro stisk tlačítka hráče A
void IRAM_ATTR x_button_isr_handler(void* arg) {
    if (!game) {
        x_button_pressed = true;
    }
}

// přerušení pro stisk tlačítka hráče B
void IRAM_ATTR y_button_isr_handler(void* arg) {
    if (!game) {
        y_button_pressed = true;
    }
}

// Pomocná funkce pro frontu hráče A
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

// Pomocná funkce pro frontu hráče B
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

// Inicializace pinů
void init_gpio() {
    gpio_config_t io_conf;
    gpio_install_isr_service(0);

    // Enkoder hráče A
    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    io_conf.pin_bit_mask = (1ULL << PLAYER_A_CLK) | (1ULL << PLAYER_A_DT);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);
    gpio_isr_handler_add(PLAYER_A_CLK, player_a_isr_handler, NULL);

    // Enkoder hráče B
    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    io_conf.pin_bit_mask = (1ULL << PLAYER_B_CLK) | (1ULL << PLAYER_B_DT);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);
    gpio_isr_handler_add(PLAYER_B_CLK, player_b_isr_handler, NULL);

    // Tlačítko hráče A 
    io_conf.intr_type = GPIO_INTR_NEGEDGE;
    io_conf.pin_bit_mask = (1ULL << PLAYER_A_BUTTON);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);
    gpio_isr_handler_add(PLAYER_A_BUTTON, x_button_isr_handler, NULL);

    // Tlačítko hráče B
    io_conf.intr_type = GPIO_INTR_NEGEDGE;
    io_conf.pin_bit_mask = (1ULL << PLAYER_B_BUTTON);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);
    gpio_isr_handler_add(PLAYER_B_BUTTON, y_button_isr_handler, NULL);
}

// Vykreslení obrazovky při prohře
void game_over() {
    for(int x = 0; x < 128; x++)
    {
        for(int y = 0; y < 64; y++)
        {
            set_pixel(127 - x,63 - y, game_over_screen[y][x]);
        }
    }
}

void app_main() {
    // Inicializace OLED displaye
    oled_init();
    init_gpio();
    erase_display(1);

    // Inicializace semaforu
    xBallSemaphore = xSemaphoreCreateBinary();
    if (xBallSemaphore == NULL) {
        printf("Failed to create semaphore\n");
        return;
    }
    xSemaphoreGive(xBallSemaphore);

    // Inicializace front pro enkodery
    xQueueEncoderA = xQueueCreate(10, sizeof(int));
    xQueueEncoderB = xQueueCreate(10, sizeof(int));
    if (xQueueEncoderA == NULL || xQueueEncoderB == NULL) {
        printf("Failed to create queues\n");
        return;
    }

    set_game();
    update_player_a();
    update_player_b();

    TaskHandle_t xHandle_player_a = NULL;
    TaskHandle_t xHandle_player_b = NULL;
    TaskHandle_t xHandle_ball = NULL;
    TaskHandle_t xHandle_encoder_a = NULL;
    TaskHandle_t xHandle_encoder_b = NULL;

    while (1) {
        // Vytvoření procesů pro hráče a míč
        xTaskCreate(playerA_task, "PlayerA_Task", 4096, NULL, 2, &xHandle_player_a);
        xTaskCreate(playerB_task, "PlayerB_Task", 4096, NULL, 2, &xHandle_player_b);
        xTaskCreate(ball_task, "Ball_Task", 4096, NULL, 3, &xHandle_ball);
        xTaskCreate(playerA_encoder_task, "PlayerA_Encoder_Task", 4096, NULL, 1, &xHandle_encoder_a);
        xTaskCreate(playerB_encoder_task, "PlayerB_Encoder_Task", 4096, NULL, 1, &xHandle_encoder_b);

        while (game) {
            // Logika zrychlování míče
            time++;
            if (time % ((1000 / speed) * 4) == 0) {
                if (speed > 1) {
                    speed--;
                }
            }
            delay(DELAY);
        }

        // Smazání procesů pro hráče a míč
        vTaskDelete(xHandle_player_a);
        vTaskDelete(xHandle_player_b);
        vTaskDelete(xHandle_ball);
        vTaskDelete(xHandle_encoder_a);
        vTaskDelete(xHandle_encoder_b);

        // Prohra -> čekání na novou hru
        game_over();
        while (!game) {
            delay(100);
            // Nastavení nové hry
            if (x_button_pressed && y_button_pressed) {
                erase_display(1);
                game = 1;
                reset_ball();
                reset_players();
                x_button_pressed = false;
                y_button_pressed = false;
                set_game();
            }
        }
    }
}

