/*
    Author:     Jesus Minjares and Erick A. Baca
                Master of Science in Computer Engineering   

    Course:     EE 4178/5190 Laboratory for Microprocessors Systems II    
     
    Lab 0:
        Objective:
            Understand how to use the gpio driver library from `Espressif`. The lab will
            consist of creating a `sweeper` and a `led chaser`. The `sweeper` will iterate over
            mutiple LEDs by turning from the lowest to highest bit and then highest
            to lowest bit. While `led chaser` will have a single led iterating from the lowest to highest
            bit. For both the `sweeper` and `led chaser` use up to 6 GPIOs.

        Bonus:
            Undergrad Bonus:
                Do the `led chaser`
            Grad Bonus:
                Create at least 4 different sequences or patterns and do a light show! 
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LOW  0  /* LOGIC LOW*/
#define HIGH 1  /* LOGIC HIGH*/

/**
 * @brief setOutputs will initialize uint8_t array as outputs
 *
 * @param out uint8_t * array which hold GPIOs pins
 * @param size size of the array
 * @return None
 * @note initialize gpio before using them
 */
void setOutputs(uint8_t *out, int size)
{
    /* iterate over the size of the array */
    for (int i = 0; i < size; i++) 
    {
        esp_rom_gpio_pad_select_gpio(out[i]); /* select the GPIO pins */
        gpio_set_direction(out[i], GPIO_MODE_OUTPUT); /* set direction as outputs */
    }
    return;
}

/**
 * @brief sweep function will sweep among the GPIOs 
 * 
 * @param led uint8_t * array which hold GPIOs pins
 * @param size size of the array
 * @return None
 */
void sweep(uint8_t *led, int size)
{
    /* iterate over the size of the array */
    for ( int i = 0; i < size; i++)
    {
        
    }
    /* iterate over the size of the array */
    for (int i = size - 1; i >= 0; i--)
    {
      
    }
}

/**
 * @brief led_chaser led chaser will make a single led to iterate over the array 
 * 
 * @param led uint8_t * array which hold GPIOs pins
 * @param size size of the array
 * @return None
 */
void led_chaser(uint8_t *led, int size)
{
    /* iterate over the size of the array */
    for (int i = 0; i < size; i++)
    {
      
    }
    /* iterate over the size of the array */
    for (int i = size - 1; i >= 0; i--)
    {
      
    }
}

/**
 * @brief lightShow user define light show 
 * 
 * @param led uint8_t * array which hold GPIOs pins
 * @param size size of the array
 * @return None
 */
void lightShow(uint8_t *led, int size)
{
    /*Use all prior functions to create a custom light show */
}

void app_main(void)
{
    /*GPIOs pins*/
    uint8_t led[] = {2}; /* create an array of single led */
    int size = sizeof(led)/sizeof(uint8_t); /* get size of the array, use sizeof() to allow scalabilty */
    setOutputs(led,size); /* intialize GPIOs pins */
    while (1)
    {
      /* Added led patterns */

      /* Toggle LED using index of your array */
      gpio_set_level(led[0], LOW);
      vTaskDelay(1000 / portTICK_PERIOD_MS); /* 1 second delay */
      gpio_set_level(led[0], HIGH);
      vTaskDelay(1000 / portTICK_PERIOD_MS); /* 1 second delay */
    }
}
