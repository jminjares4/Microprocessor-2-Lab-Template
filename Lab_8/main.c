/*
    Author:     Jesus Minjares and Erick Baca
                Master of Science in Computer Engineering
    Course:     EE 5190 Laboratory for Microprocessors Systems II
    Lab 8:      
        Objective:
                The purpose of this lab is to use DAC driver of espressif. The main task
                consist of using GPIO 25 generate a sine wave. On GPIO 26, generate a triangle wave.
                The ESP32 DAC has a 8 bit resolution therefore 0-255 bits or 0.0-3.3V output.
        Undergrad Bonus:
                In GPIO 26, generate sawtooth wave.
        Grad Bonus:
                In GPIO 25, generate another sine wave but this time make it run in 10 Hz                

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
#include <math.h>
#include "sdkconfig.h"
#include <driver/dac.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
void createTriangleWave(void *pvParameter)
{
    dac_output_enable(DAC_CHANNEL_1);
    static int i = 0;
    while (1)
    {
        dac_output_voltage(DAC_CHANNEL_1, i);
        // compute trialgular waveform value
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
void createSineWave(void *pvParameter)
{
    dac_output_enable(DAC_CHANNEL_2);
    static int i = 0;
    float val;
    int n;
    while (1)
    {
        // compute sine waveform value
        dac_output_voltage(DAC_CHANNEL_2, n);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
void app_main()
{
    xTaskCreate(&createTriangleWave, "createTriangleWave", 4096, NULL, 5, NULL);
    xTaskCreate(&createSineWave, "createSineWave", 4096, NULL, 5, NULL);
}