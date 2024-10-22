#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"

void app_main(void)
{

    // Print "Hello, World!" once to the console
    printf("Hello, World!\n");

    while (true) {
        // Commented to prevent repeated printing
        printf("a\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);  // Delay for 1 second
    }
}
