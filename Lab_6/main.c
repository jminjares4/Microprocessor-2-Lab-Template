/*
    Author:     Jesus Minjares and Erick Baca
                Master of Science in Computer Engineering
    Course:     EE 5190 Laboratory for Microprocessors Systems II

    Lab 6:      The purpose of Lab 6 is to get familiar with interrupts
                on ESP32 microcontroller. The lab consist of triggering an
                interrupt by an external push buttons that will display an
                message via serial terminal. Every time the interrupt is trigger
                it will toggle the onboard led to provide visual representation of
                the interrupt occuring. 

                Undergrad Bonus
                    * Modify the code so the “print_task” prints what port caused the interrupt. +10
                Grad Bonus
                    Modify the code so the “print_task” prints what port caused the interrupt and turn different `LEDs` for each interrupt. 
                
    Date:       09-30-2021
    Pinout:
                                         +-----------------------+
                                         | O      | USB |      O |
                                         |        -------        |
                                     3V3 | [ ]               [ ] | VIN
                                     GND | [ ]               [ ] | GND
     Touch3 / HSPI_CS0 / ADC2_3 / GPIO15 | [ ]               [ ] | GPIO13 / ADC2_4 / HSPI_ID / Touch4
 CS / Touch2 / HSPI_WP / ADC2_2 /  GPIO2 | [ ]               [ ] | GPIO12 / ADC2_5 / HSPI_Q / Touch5
      Touch0 / HSPI_HD / ADC2_0 /  GPIO4 | [ ]               [ ] | GPIO14 / ADC2_6 / HSPI_CLK / Touch6
                         U2_RXD / GPIO16 | [ ]               [ ] | GPIO27 / ADC2_7 / Touch7
                         U2_TXD / GPIO17 | [ ]               [ ] | GPIO26 / ADC2_9 / DAC2
                      V_SPI_CS0 /  GPIO5 | [ ]  ___________  [ ] | GPIO25 / ADC2_8 / DAC1
                SCK / V_SPI_CLK / GPIO18 | [ ] |           | [ ] | GPIO33 / ADC1_5 / Touch8 / XTAL32
        U0_CTS / MSIO / V_SPI_Q / GPIO19 | [ ] |           | [ ] | GPIO32 / ADC1_4 / Touch9 / XTAL32
                 SDA / V_SPI_HD / GPIO21 | [ ] |           | [ ] | GPIO35 / ADC1_7 
                  CLK2 / U0_RXD /  GPIO3 | [ ] |           | [ ] | GPIO34 / ADC1_6 
                  CLK3 / U0_TXD /  GPIO1 | [ ] |           | [ ] | GPIO39 / ADC1_3 / SensVN 
        SCL / U0_RTS / V_SPI_WP / GPIO22 | [ ] |           | [ ] | GPIO36 / ADC1_0 / SensVP 
                MOSI / V_SPI_WP / GPIO23 | [ ] |___________| [ ] | EN 
                                         |                       |
                                         |  |  |  ____  ____  |  |
                                         |  |  |  |  |  |  |  |  |
                                         |  |__|__|  |__|  |__|  |
                                         | O                   O |
                                         +-----------------------+
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#define ESP_INTR_FLAG_DEFAULT 0 //default flag for interrupt 
static xQueueHandle gpio_queue = NULL; // create a global quue
static void IRAM_ATTR gpio_isr_handler(void *arg)
{
    //store argument in gpio_num
    uint32_t gpio_num = (uint32_t)arg;

    //send queue?

}
static void print_task(void *arg)
{
    uint32_t gpio_num;
    while (1)
    {
        //wait for queue?
        printf("GPIO[%d] caused an interrupt\n", gpio_num);
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
void setUpGPIO()
{
    /* See example for more information */
    gpio_config_t io_conf;
    // INPUT
    io_conf.intr_type = ;
    io_conf.mode = ;
    io_conf.pin_bit_mask = ;
    io_conf.pull_down_en = ;
    io_conf.pull_up_en = ;
    gpio_config(&io_conf);
    // OUTPUT
    io_conf.intr_type = ;
    io_conf.mode = ;
    io_conf.pin_bit_mask = ;
    io_conf.pull_down_en = ;
    io_conf.pull_up_en = ;
    gpio_config(&io_conf);
    // Set ISR
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(xxx, gpio_isr_handler, (void *)xxx);
}
void app_main()
{
    setUpGPIO(); //set the GPIO and interrupts
    gpio_queue = xQueueCreate(10, sizeof(uint32_t)); //create a queue instance
    xTaskCreate(&print_task, "print_task", 2048, NULL, 10, NULL); //create a task
