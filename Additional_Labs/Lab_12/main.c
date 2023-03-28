#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "freertos/queue.h"
#include <driver/ledc.h>
#include "freertos/queue.h"
#include <stdio.h>
#include <string.h>

#define UART_NUM        UART_NUM_2
#define BUF_SIZE        1024
#define TASK_MEMORY     1024 

QueueHandle_t LEDQueue;

static void uart_init()
{

}

void sledc_setup(void){

}

void LEDC_task(void *pvParameters)
{

}

static void UART_task(void *arg)
{

}

void app_main(void)
{

    uart_init();
    ledc_setup();

    LEDQueue = xQueueCreate(5, sizeof(int));

    xTaskCreate(UART_task, "UART_task", TASK_MEMORY, NULL, 5, NULL);
    xTaskCreate(LEDC_task, "LEDC_task", 2048, NULL, 5, NULL);
}