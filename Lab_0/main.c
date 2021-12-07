/*
    Author:     Jesus Minjares and Erick A. Baca
                Master of Science in Computer Engineering   

    Course:     EE 5190 Laboratory for Microprocessors Systems II    
     
    Lab 0:
        Objective:
                Understand how to use the driver library to create 
                outputs. The user must create a sweeper that turns
                ON each LED in sequential order and then go back by
                turning each LED OFF.
        Undergrad Bonus:
                The first bonus deals with having an LED chaser. The 
                LED should turn ON in sequential order; however, only
                one LED should be ON at a time.
        Grad Bonus:
                For the Graduate Bonus, it is expected to be creative 
                and original. The bonus consist in create a light show
                with more that 4 distinct sequences or patterns that
                the student likes or can do.
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
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define HIGH    1
#define LOW     0

/**
 * @brief setOutputs will initialize uint8_t array as outputs
 *
 * @param out uint8_t * array which hold GPIOs pins
 * @param size size of the array
 * @return None void
 * @note initialize gpio before using them
 */
void setOutputs(uint8_t *out, int size)
{
    for (int i = 0; i < size; i++) // itierate over the size of the array
    {
        gpio_pad_select_gpio(out[i]);                 // select the GPIO pins
        gpio_set_direction(out[i], GPIO_MODE_OUTPUT); // set direction as outputs
    }
    return;
}

/**
 * @brief sweep function will sweep among the GPIOs 
 * 
 * @param led uint8_t * array which hold GPIOs pins
 * @param size size of the array
 */
void sweep(uint8_t *led, int size)
{
    for ( int i = 0; i < size; i++) //iterate over the size of the array
    {
        
    }
    for (int i = size - 1; i >= 0; i--) //iterate over the size of hte array 
    {
      
    }
}

/**
 * @brief led_chaser led chaser will make a single led to iterate over the array 
 * 
 * @param led uint8_t * array which hold GPIOs pins
 * @param size size of the array
 */
void led_chaser(uint8_t *led, int size)
{
    for (int i = 0; i < size; i++) //iterate over the size of the array
    {
      
    }
    for (int i = size - 1; i >= 0; i--) //iterate over the size of hte array 
    {
      
    }
}

/**
 * @brief lightShow user define light show 
 * 
 * @param led uint8_t * array which hold GPIOs pins
 * @param size size of the array
 */
void lightShow(uint8_t *led, int size)
{
    /*Use all prior functions to create a custom light show */
}

void app_main(void)
{
    /*GPIOs pins*/
    uint8_t led[] = {2}; //create an array of single led 
    int size = sizeof(led)/sizeof(uint8_t); //get size of the array, use sizeof() to allow scalabilty 
    setOutputs(led,size); //intialize GPIOs pins
    while (1)
    {
      // sweep(led, size); //sweep function
      //led_chaser(led,size);
      //lightShow(led,size); //bonus 

      // Toggle LED using index of your array
      gpio_set_level(led[0]);
      vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second delay
      gpio_set_evel(led[0];)
      vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second delay
}