/*
    Author:     Jesus Minjares and Erick A. Baca
                Master of Science in Computer Engineering   

    Course:     EE 4178/5190 Laboratory for Microprocessors Systems II       
     
    Lab 1:
        Objective:
            Understand how to use the gpio driver library from `Espressif`. The lab will consist of
            using a `sweeper`, `led chaser` and any additionals led sequence that the student developed
            and be selected via an input. Students must have a total of three buttons. First button must 
            start `sweeper` function which student must have complete from the previous lab. Next, the 
            second button must start the `led chaser` function. Lastly, the third button must toggle the
            state of the onboard led.

            | Button |  Function    |   Mode    | 
            | :---   | :---         | :---      |  
            | B0     | `Sweeper`    | Pull-down |
            | B1     | `Led chaser` | Pull-down |
            | B2     | `Toggle`     | Pull-down |

        Bonus:
            Undergrad Bonus:
                Create an other LED sequence and add another button, as pull-up. 
            Grad Bonus:
                Add button to start lightshow from previous lab
                Do a reset button which will turn off all LEDs 

*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LOW  0  /* LOGIC LOW*/
#define HIGH 1  /* LOGIC HIGH*/

/**
 * @brief setInputs will initialize uint8_t array as inputs
 *
 * @param led uint8_t * array which hold GPIOs pins
 * @param size size of the array
 * @return None
 * @note initialize gpio before using them
 */
void setInputs(uint8_t *in, int size)
{
    for (int i = 0; i < size; i++) // itierate over the size of the array
    {
                 // select the GPIO pins
                // set direction as inputs
                // set as pull-down
    }
    return;
}
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
    for (int i = 0; i < size; i++) // itierate over the size of the array
    {
       
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
   
}

/*Replace with yours*/
void light_show(){

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

}

void app_main(void)
{
    /* Select GPIOs output and store in led array */
    uint8_t led[] = {13}; //replace with your GPIO pins 
    /* Select GPIOs inputs and store in button array */
    uint8_t button[] = {22, 23};

    /* Use sizeof() to get the size of the arrays */
    int led_size = sizeof(led) / sizeof(uint8_t);
    int button_size = sizeof(button) / sizeof(uint8_t);

    /* Initialize Inputs */
    setInputs(button, button_size);
    /* Initialize Outputs */
    setOutputs(led, led_size);

    while (1)
    {
        vTaskDelay(5/portTICK_PERIOD_MS); //debounce button
        int button0 = gpio_get_level(button[0]); //read button 0
        // add additonal buttons 
        if(button0 == 1){
          // sweep();
        }
        else{
          vTaskDelay(100/portTICK_PERIOD_MS); //100ms to avoid WDT errors
        }
    }
}