# **Lab 2 Interrupts :zap:**

### **Objective**
***
* Given the program template shown in listing 1, edit it so that the code so that you replicate the functions of a microwave.
* The code is expected to have a total of 6 buttons which will simulate the functions of the microwave. 
  - The first and second button should control a counter that goes up or down. 
  - The next two buttons should act as a start or reset counter.
  - The last 2 buttons are your inteerupts which simulates if the door open or closes. Since the lab is a microwave the timer should stop in the event that the door open button is been pressed the timer should stop and cannnot start again until the close door button has been pressed
* Print the counter time as you add or subtract the total amount of time the microwave will run. Then when the timer start button is pressed the counter should display in the screen as it goes down every 500 ms.

### **Bonus**
***
* Modify the code by using the gpio_config_t to create an output LED which will display when the door is open or close.+10
* Create anothe interrupt button which do the pause option in the microwave.+10
### **ESP32 Pinout**
~~~
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
~~~
### **Example**
Here is an example of a single GPIO set as ouptut and an input button. The following code will turn the onboard LED as long as the button is press.
~~~c
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h" 
#include "freertos/task.h" 
#include "driver/gpio.h"

#define ESP_INTR_FLAG_DEFAULT 0 
#define ONBOARDLED 2
#define GPIO_OUTPUT_PIN_SEL (1ULL << ONBOARDLED)

#define TOOGLELED 22
#define GPIO_INPUT_PIN_SEL (1ULL << TOOGLELED)

static void IRAM_ATTR gpio_isr_handler(void* arg) {
  GPIO.out ^= BIT2;
}
void setUpInterrupts() {
    gpio_config_t io_conf;  //Access the GPIO strcuture
    //INPUT     
    io_conf.intr_type = GPIO_INTR_POSEDGE;  //Set up as Positive Edge
    io_conf.mode = GPIO_MODE_INPUT;     //Set pins as input
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;  //Set all the interrupt buttons  
    io_conf.pull_down_en = 1;   //Enable as a pulldown button
    io_conf.pull_up_en = 0; 
    gpio_config(&io_conf);

    //OUTPUT 
    io_conf.intr_type = GPIO_INTR_DISABLE;  //Its an output so the interrupt is disable
    io_conf.mode = GPIO_MODE_OUTPUT;    //Set pin as output
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL; //Set up all the LED that we are going to use
    io_conf.pull_down_en = 0; 
    io_conf.pull_up_en = 0; 
    gpio_config(&io_conf);

    //Set ISR 
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);    //Ste the flag for th einterrupt

    /*Let the buttons selected to interract with the interrupt*/
    gpio_isr_handler_add(TOOGLELED, gpio_isr_handler, (void*) TOOGLELED);        
}
void app_main() {
    /*Setup the interrupt buttons and LEDs*/
    setUpInterrupts();
    while(1){
         vTaskDelay(100/portTICK_PERIOD_MS);
    }
}
~~~
### **Lab Template**
~~~c
/*
    Author:     Jesus Minjares and Erick A. Baca
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
~~~


---
## **C helpful functions**

For this lab, there are couple additional functions from ESPRESSIF that are important for using inputs. In the previous lab we use the function `gpio_set_direction(gpio_num_t gpio_num, gpio_mode_t mode)` in order to set our inputs and outputs. In this lab, we will use the `gpio_config_t` which will help us setup the interrupt function.
~~~c 
typedef struct {
    uint64_t pin_bit_mask;          /*!< GPIO pin: set with bit mask, each bit maps to a GPIO */
    gpio_mode_t mode;               /*!< GPIO mode: set input/output mode                     */
    gpio_pullup_t pull_up_en;       /*!< GPIO pull-up                                         */
    gpio_pulldown_t pull_down_en;   /*!< GPIO pull-down                                       */
    gpio_int_type_t intr_type;      /*!< GPIO interrupt type                                  */
} gpio_config_t;
~~~
In order to fill in the structure `gpio_config_t`, each variable is refference to another structure. Inside the `uint64_t pin_bin_mask` it need a to bit shift the pin number that converts from a int to an unsigned long long by using `1ULL << BUTTON`. After tknowing the the next variables have different function structures. For the `gpio_mode_t mode`, use the listing below in order to set what your button will do ass well if you want to set it up as an output. If you are using the output mode just remember that in ordet o call an output you nned to use `GPIO.out = BIT#`.
~~~c
typedef enum {
    GPIO_MODE_DISABLE = GPIO_MODE_DEF_DISABLE,                                                         /*!< GPIO mode : disable input and output             */
    GPIO_MODE_INPUT = GPIO_MODE_DEF_INPUT,                                                             /*!< GPIO mode : input only                           */
    GPIO_MODE_OUTPUT = GPIO_MODE_DEF_OUTPUT,                                                           /*!< GPIO mode : output only mode                     */
    GPIO_MODE_OUTPUT_OD = ((GPIO_MODE_DEF_OUTPUT) | (GPIO_MODE_DEF_OD)),                               /*!< GPIO mode : output only with open-drain mode     */
    GPIO_MODE_INPUT_OUTPUT_OD = ((GPIO_MODE_DEF_INPUT) | (GPIO_MODE_DEF_OUTPUT) | (GPIO_MODE_DEF_OD)), /*!< GPIO mode : output and input with open-drain mode*/
    GPIO_MODE_INPUT_OUTPUT = ((GPIO_MODE_DEF_INPUT) | (GPIO_MODE_DEF_OUTPUT)),                         /*!< GPIO mode : output and input mode                */
} gpio_mode_t;
~~~
Then in the next two points of the `gpio_config_t` which are `gpio_pulldown_t` or `gpio_pullup_t` is where the button is enable as a pulldown or as a pullup. Finally the last configuration `gpio_int_type_t`, it decides what type of edge will the button read and the cofiguration is shown below. 
~~~c
typedef enum {
    GPIO_INTR_DISABLE = 0,     /*!< Disable GPIO interrupt                             */
    GPIO_INTR_POSEDGE = 1,     /*!< GPIO interrupt type : rising edge                  */
    GPIO_INTR_NEGEDGE = 2,     /*!< GPIO interrupt type : falling edge                 */
    GPIO_INTR_ANYEDGE = 3,     /*!< GPIO interrupt type : both rising and falling edge */
    GPIO_INTR_LOW_LEVEL = 4,   /*!< GPIO interrupt type : input low level trigger      */
    GPIO_INTR_HIGH_LEVEL = 5,  /*!< GPIO interrupt type : input high level trigger     */
    GPIO_INTR_MAX,
} gpio_int_type_t;
~~~
Next, you will need to set the interrupt flag into 0 which uses the function : `gpio_install_isr_service(int intr_alloc_flags)`
~~~c
esp_err_t gpio_install_isr_service(int intr_alloc_flags)
~~~
Lastly, there is a function that allows the interrupt button to enter the `static void IRAM_ATTR gpio_isr_handler(void* arg)`. In order to enter this function you need to declare using the function `gpio_isr_handler_add(gpio_num_t gpio_num, gpio_isr_t isr_handler, void *args)`.
~~~c
esp_err_t gpio_isr_handler_add(gpio_num_t gpio_num, gpio_isr_t isr_handler, void *args)
~~~

### **Pull-up and Pull-down Configuration**

<img width="413" alt="button configuation" src="https://user-images.githubusercontent.com/60948298/144836131-96f04e0f-c7f7-443f-b35c-814fb9db4e29.png">

### **Additional Links**
* [Espressif GPIO Driver API](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html#)
---
### **Authors**
* [***Jesus Minjares***](https://github.com/jminjares4)
  * **Master of Science in Computer Engineering** <br>
    [![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white&style=flat)](https://www.linkedin.com/in/jesusminjares/) [![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white&style=flat)](https://github.com/jminjares4)
* [***Erick Baca***](https://github.com/eabaca2419)
  * **Master of Science in Computer Engineering** <br>
    [![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white&style=flat)](https://www.linkedin.com/in/erick-baca/) [![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white&style=flat)](https://github.com/eabaca2419)


