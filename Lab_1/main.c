/*
    Author:     Jesus Minjares and Erick A. Baca
                Master of Science in Computer Engineering   

    Course:     EE 5190 Laboratory for Microprocessors Systems II    
     
    Lab 1:
        Objective:
        Understand how to use the gpio driver library from ESPRESSIF. The lab will consist of using a 
        `sweeper`, `led chaser` and any additionals led sequence that the student developed which be
        selected via an input. Students must have a total of three buttons. First button must start `sweeper`
        function which student must have complete from the previous lab. Next, the second button must start the 
        `led chaser` function. Lastly, the third button must toggle the state of the onboard led.

    Buttons 
    -------------
    Button 1 -> start `sweeper`, pull-up
    Button 2 -> start `led chaser` pull-down
    Button 3 -> toggle `onboard led` pull-up

    Bonus
    -------------
    - Undergrad Bonus:
        * Create an other LED sequence and add another button, button configuration does not matter. 
    - Grad Bonus:
        * Add button to start lightshow that was done in the previous lab
        * Do a reset button which will turn off all LEDs 
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

/* Logic MACROs */
#define HIGH 1
#define LOW 0

/**
 * @brief setInputs will initialize uint8_t array as inputs
 *
 * @param led uint8_t * array which hold GPIOs pins
 * @param size size of the array
 * @return None void
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
 * @return None void
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

    int speed = 500; // 0.5 second delay
    while (1)
    {
        vTaskDelay(5/portTICK_PERIOD_MS); //debounce button
        int button0 = gpio_get_level(button[0]); //read button 0
        // add additonal buttons 
        if(button0 == 1){
          // sweep();
        }
        }else{
          vTaskDelay(100/portTICK_PERIOD_MS); //100ms to avoid WDT errors
        }
}
