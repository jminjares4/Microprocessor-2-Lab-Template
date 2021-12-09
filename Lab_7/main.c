/*
    Author:     Jesus Minjares and Erick Baca
                Master of Science in Computer Engineering
    Course:     EE 5190 Laboratory for Microprocessors Systems II 

    Lab 7:      
            Objective:
                The objective Lab is to use the LEDC driver with ADC driver
                to control the duty cycle of the GPIO by sending the ADC value
                through a queue. 
            Undergrad Bonus:
                Add a port interrupt to stop and resume the 
                timer of the pwm signal of the driver.
            Grad Bonus:
                Create another task with two more PWM pins using the LEDC driver and start on at 
                the highest duty cycle and then drop down. When it reaches 0
                start the next pin from 0 to the highest duty cycle. Continue 
                the same pattern to make it look like a wave.

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
#include <driver/ledc.h>
#include <driver/adc.h>
static xQueueHandle duty_queue = NULL;
void ADCtask(void *pvParameter)
{
    while (1)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
void PWMtask(void *pvParameter)
{
    while (1)
    {
        ledc_set_duty();
        ledc_update_duty();
    }
}
void setADC()
{
    adc1_config_width();
    adc1_config_channel_atten(, );
}
void setPWM()
{
    ledc_timer_config_t timerConfig;
    timerConfig.duty_resolution = ;
    timerConfig.timer_num = ;
    timerConfig.freq_hz = ;
    timerConfig.speed_mode = ;
    ledc_timer_config(&timerConfig);
    ledc_channel_config_t tChaConfig;
    tChaConfig.gpio_num = ;
    tChaConfig.speed_mode = ;
    tChaConfig.channel = ;
    tChaConfig.intr_type = ;
    tChaConfig.timer_sel = ;
    tChaConfig.duty = ;
    ledc_channel_config(&tChaConfig);
}
void app_main()
{
    setADC();
    setPWM();
    duty_queue = xQueueCreate(10, sizeof(int));
    xTaskCreate(&ADCtask, "ADCtask", 2048, NULL, 5, NULL);
    xTaskCreate(&PWMtask, "PWMtask", 2048, NULL, 5, NULL);
}