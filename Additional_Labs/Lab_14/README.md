# **Lab 14 ADC with LCD**

## **Objective:**
* The objective of this lab is to utilize the `ADC peripheral` alongside the `LCD driver` to create a `digital multimeter (DMM)`. The LCD driver was provided by the main author of this documentation, [`Jesus Minjares`](https://github.com/jminjares4), the driver can be found here: [`LCD Driver`](https://github.com/jminjares4/ESP32-LCD-1602-Driver).
## **Bonus**
- ***Undergrad Bonus:***
  * Use the `digital multimeter` to read a simple voltage-divider circuit.
- ***Grad Bonus:***
  * Using the ADC readings control the `sweeper function` from previous labs. Use up to 6 LEDs to demonstrate it. When the ADC reads the lowest value the one LED must be on and when the ADC reads the highest value the all LEDs must turn on.



## **ESP32 Pinout**
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

## **LCD Example Code**
This is an example code that demostrates how to utilize the LCD driver.
~~~c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/esp_lcd.h"
/* LCD task */
void lcd_task(void *pvParameters){
    /* Create LCD object */
    lcd_t lcd;
    /* Set LCD to default pins */
    lcdDefault(&lcd);
    /* Initialize LCD object */
    lcdInit(&lcd);
    /* Clear previous data on LCD */
    lcdClear(&lcd);

    while(1){
        /* Display Text */
        lcdSetText(&lcd, "Hello World!", 0, 0);
        /* 1 second delay */
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
void app_main(void){
    /* Create LCD Task */
    xTaskCreate(lcd_task, "LCD Task", 2048, NULL, 4, NULL);
}

~~~

# **Lab Template**
~~~c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/esp_lcd.h"
/* Create Function to covert ADC Value to Voltage */

/* Create Function for ADC initialization */
void ADC_setup(void){

}
/* Function for sweeper */
void sweeper_function(uint32_t val){

}
/* LCD task */
void lcd_task(void *pvParameters){
    /* Setup LCD */
    
    while(1){
        /* Get ADC Raw Readings */

        /* Get Conversion */

        /* Display Voltage onto LCD */

        /* Change PWM output to match ADC readings */

    }
}
void app_main(void){
    /* Create LCD Task */
    
}

~~~

## **Warning**
Before anything you must add the library for the LCD driver onto the `CMakeLists.txt` which is found in the lab's main folder.
This would be how to add it:
~~~c
idf_component_register(SRCS "main.c"
                            "driver/esp_lcd.c"
                    INCLUDE_DIRS ".")
~~~

## **Author:**
* [**Jorge Minjares** :zap:](https://github.com/JorgeMinjares)<br>
  * Bachelor of Science in Electrical Engineering<br>
[![Outlook](https://img.shields.io/badge/Microsoft_Outlook-0078D4?style=for-the-badge&logo=microsoft-outlook&logoColor=white&style=flat)](mailto:jminjares5@miners.utep.edu) 
[![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white&style=flat)](https://www.linkedin.com/in/jorge-minjares/) [![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white&style=flat)](https://github.com/JorgeMinjares)