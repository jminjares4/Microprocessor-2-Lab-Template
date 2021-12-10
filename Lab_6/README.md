# **`LAB 6 GPIO, Interrupts, and Queues` :zap:**

## **Objective:**
***
* Understand how to use interrupt and send data through a queue with ['FreeRTOS Queues`]((https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html#queue-api). In this lab, the main focus will be sending data from a interrupt to a task.

* Create a program which uses port interrupts to trigger a task.
* The port interrupts must be connected to external push buttons which will trigger the “print_task”.
* On the “print_task” your program should print that the interrupt was trigger.

 - ***Undergrad Bonus***
    * Modify the code so the “print_task” prints what port caused the interrupt. +10
 - ***Grad Bonus***
   - * Modify the code so the “print_task” prints what port caused the interrupt and turn different `LEDs` for each interrupt. 

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
The following example is demostration of how to use interrupts in the ESP32. The interrupt routine will toggle the state of the `onboard led` every time its pressed. There is also another task that will print a message every second. 
~~~c
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define ESP_INTR_FLAG_DEFAULT 0
#define ONBOARD_LED     2
#define GPIO_BUTTON_0   23

/*  ULL is an 8 bytes data type and the GPIO pins need to 
    be set to ULL because gpio_config_t pin_bit_mask is an uint64_t. 
    The pin_bit_mask is 8 bytes long therefore the GPIO has to be set to the
    corresponding data type by using left bit shift 
*/
#define GPIO_OUTPUT_SEL 1ULL << ONBOARD_LED
#define GPIO_INPUT_SEL  1ULL << GPIO_BUTTON_0

void IRAM_ATTR gpio_isr_handler(void *arg)
{
    GPIO.out ^= BIT2; //toggle onboard LED
}
void print_task(void *arg)
{
    while (1)
    {
        printf("Printing data from print_task\n"); //print message
        vTaskDelay(1000/portTICK_RATE_MS); // 1 second delay
    }
}
void setUpGPIO()
{
    gpio_config_t io_conf;

    /* INPUT */ 
    io_conf.intr_type = GPIO_INTR_NEGEDGE; // set as POSEDGE for interrupt 
    io_conf.mode = GPIO_MODE_INPUT; // set input 
    io_conf.pin_bit_mask = GPIO_INPUT_SEL; // set gpio that will be used for input 
    io_conf.pull_down_en = 0;  // disable pull down 
    io_conf.pull_up_en = 1;  // enable pull up 

    gpio_config(&io_conf); // set configuration 

    /* OUTPUT */
    io_conf.intr_type = GPIO_INTR_DISABLE; // disable interrupt 
    io_conf.mode = GPIO_MODE_OUTPUT; // set as output 
    io_conf.pin_bit_mask = GPIO_OUTPUT_SEL; // set gpio that will be used for output 
    io_conf.pull_down_en = 0 ; // disable pull down 
    io_conf.pull_up_en = 0; // disable pull up 

    gpio_config(&io_conf); // set configuration

    /*  Set ISR to non-share memory 
        If this value is 0, it will default to allocating
        a non-shared interrupt of level 1, 2 or 3.
    */
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

    /* Attached GPIO interrupt to interrupt routine and pass an argument */
    gpio_isr_handler_add(GPIO_BUTTON_0, gpio_isr_handler, (void *)GPIO_BUTTON_0);
}
void app_main()
{
    setUpGPIO(); //set up configuration of the GPIOs 
    xTaskCreate(&print_task, "print_task", 2048, NULL, 10, NULL);
}
~~~

### **Template Code**
~~~c
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
}
~~~

---
## **C helpful functions**

For this lab, we use prior function from Lab 2 that are important for using inputs. In the previous labs we use the function `gpio_set_direction(gpio_num_t gpio_num, gpio_mode_t mode)` in order to set our inputs and outputs. In this lab, we will use the `gpio_config_t` which will help us setup the interrupt function.
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
Moreoover, there is a function that allows the interrupt button to enter the `static void IRAM_ATTR gpio_isr_handler(void* arg)`. In order to enter this function you need to declare using the function `gpio_isr_handler_add(gpio_num_t gpio_num, gpio_isr_t isr_handler, void *args)`.
~~~c
esp_err_t gpio_isr_handler_add(gpio_num_t gpio_num, gpio_isr_t isr_handler, void *args)
~~~
Lastly, there is an very important function was is need to fulfill this lab. We have use queues in the previous labs to send data from one task to another. However, it this lab we need to send data from an interrupt to a task, therefore we will use a special function to send the queue: **`xQueueSendFromISR()`**. Similar to the previous function that we have use to send data with a queue, `xQueueSendFromISR` is not different with a slight parameter. `BaseType_t *pxHigherPriorityTaskWoken` parameter set to *`NULL`*.
~~~c
 BaseType_t xQueueSendFromISR
           (
               QueueHandle_t xQueue,
               const void *pvItemToQueue,
               BaseType_t *pxHigherPriorityTaskWoken
           );
~~~

### **Pull-up and Pull-down Configuration**

<img width="413" alt="button configuation" src="https://user-images.githubusercontent.com/60948298/144836131-96f04e0f-c7f7-443f-b35c-814fb9db4e29.png">

### **Additional Links**
* [Espressif GPIO Driver API](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html#)
* [FreeRTOS Queue](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html#queue-api)
* [FreeRTOS Espressif Documenation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html)
* [FreeRTOS Documenation](https://www.freertos.org/a00125.html)
---
### **Author**
* [`Jesus Minjares`](https://github.com/jminjares4)
  * Master of Science in Computer Engineering
* [`Erick Baca`](https://github.com/eabaca2419)
  * Master of Science in Computer Engineering
