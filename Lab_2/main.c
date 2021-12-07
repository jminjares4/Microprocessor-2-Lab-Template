/*
    Author:     Erick A. Baca
                Master of Science in Computer Engineering   
    Course:     EE 5190 Laboratory for Microprocessors Systems II    
     
    Lab 2:
        Objective:
            The main objective for this lab is to learn how to properly use interrupt buttons in the
            ESP32. The lab is main objective is to create a microwave which inteerupts the timer if 
            the door is open. 
        Bonus Undergrad:
            Use the same gpio_config_t to  an create output to display when the door is open or close.
        Bonus Grad:
            Use another button to create a stop action.
        

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
#include "driver/gpio.h"


#define TIMERUP_BUTTON  3
#define TIMERDOWN_BUTTON 21
#define STARTTIMER_BUTTON 19
#define RESETTIMER_BUTTON 18

#define ESP_INTR_FLAG_DEFAULT 0 
#define LED1 2
#define LED2 15
#define GPIO_OUTPUT_PIN_SEL (1ULL << LED2 | 1ULL << LED1)

#define DOORCLOSE 23
#define DOOROPEN 22
#define STOPBUTTON 5
#define GPIO_INPUT_PIN_SEL (1ULL << DOORCLOSE | 1ULL << DOOROPEN | 1ULL << STOPBUTTON)

int doorFlag = 0;   //Flag for the door if its open or close
int stopFlag = 0;   //Flag for stop button
int count = 0;      //Variable that stores the total counter time

#define _BONUS_ TRUE
//#define _BONUS_GRAD_ TRUE

/**
 * @brief Interrupt function 
 *
 * @param arg uint32_t * int which holds GPIO number the interrupt came  to
 * @return None void
 * @note initialize interrupt before using them
 */
static void IRAM_ATTR gpio_isr_handler(void* arg) {  
    uint32_t gpio_num = (uint32_t) arg; //Save the argument into a variable to compare the flag outputted

}

void microwave() {
    /*Save the Value of the buttons in a designated variable*/
    //read button 0
    //read button 1
    //read button 2
    //read button 3

    /*If the first button is pressed add 10 seconds to the total time*/

    /*If the second button is pressed subtract 10 seconds to the total time*/
        /*If the time is lower that 0 keep the time as 0*/
        /*Else subtract the total time by 10*/
    /*If the thrid button is pressed, start the count down*/
        //Iterrate the counter
            //If by any chance the interrupt stop flag is activate stop the count down
            //Clear the stop flag
            //If by any chance the interrupt door open flag is activate stop the count down
            //Print the remaining time 
    /*If the fourth button is pressed reset the counter time*/
}

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
    }
    return;
}
/**
 * @brief setUpInterrupts will initialize the interrupt buttons stored 
 *        in GPIO_INPUT_PIN_SEL and GPIO_OUTPUT_PIN_SEL
 * @note initialize gpio before using them
 */
void setUpInterrupts() {
    gpio_config_t io_conf;  //Access the GPIO strcuture
    //INPUT     
    io_conf.intr_type =     
    io_conf.mode = //Set pins as input
    io_conf.pin_bit_mask =  //Set all the interrupt buttons  
    io_conf.pull_down_en = ;   //Enable as a pulldown button
    io_conf.pull_up_en = ; 
    gpio_config(&io_conf);

    gpio_isr_handler_add(, gpio_isr_handler, (void*));     //Setup the interruptbutton that access the interrupt function
}

void app_main() {
    /*Set up Array for normal GPIOS*/
    uint8_t button[] = {3};
    int button_size = sizeof(button) / sizeof(uint8_t);
    /* Initialize Inputs */
    setInputs(button, button_size);
    /*Setup the interrupt buttons and LEDs*/
    setUpInterrupts();
    printf("Time: %d\n", count);
    while(1){
        microwave();
         vTaskDelay(100/portTICK_PERIOD_MS);
    }
}