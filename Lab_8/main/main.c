/*
    Author:     Jesus Minjares and Erick Baca
                Master of Science in Computer Engineering

    Course:     EE 4178/5190 Laboratory for Microprocessors Systems II

    Lab 8: 
        Objective:
            The objective for this is to lab understand how to use Espressif `DAC` driver. Digital to Analog Converter (DAC) is
            a peripheral that converts digital signals to analog signals. The ESP32 has DAC of 8-bit resolution ranging
            from 0-255 bit value or 0.0-3.3V. ESP32 has only two DAC channels: channel 1 (GPIO 25) and channel 2 (GPIO 26). 
            Students must generate a triangle wave with `channel 1` and sine wave with `channel 2`.

            | Tasks         |   Description                       |
            | :---          |   :---                              |
            | Triangle Wave | create triangle wave with `GPIO 25` |
            | Sine Wave     | create sine wave with `GPIO 26`     |

            | GPIO Pin | DAC Channel   |
            | :---     | :---          |
            | `GPIO 25`  | Channel 1   |
            | `GPIO 26`  | Channel 2   | 

            | Digital   | Analog    |
            | :---      | :---      |
            | 0         | 0.0 v     |
            | ...       | ...       |
            | 255       | 3.3 v     |
        
        Bonus:
            Undegrad Bonus:
                Replace `triangle` wave with a `saw-tooth` wave

            Grad Bonus:
                Modify `sine` wave to operate @ **10** hertz
*/

#include <stdio.h>
#include <math.h>
#include <driver/dac.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* Triangle wave task */
void createTriangleWave(void *pvParameters)
{
    /* Enable DAC channel 1 */
    dac_output_enable(DAC_CHANNEL_1);
    /* create variable to store value */
    static int i = 0;

    while (1)
    {
        /* Generate triangle wave */

        /* output voltage through channel 1 */
        dac_output_voltage(DAC_CHANNEL_1, i);

        vTaskDelay(10 / portTICK_PERIOD_MS); /* 10 ms*/
    }
}

/* Sine wave task */
void createSineWave(void *pvParameters)
{
    /* Enable DAC channel 2 */
    dac_output_enable(DAC_CHANNEL_2);
    /* Variables to compute sine wave */
    static int i = 0;
    float val;
    int n = 0;

    while (1)
    {
        /* Compute sine wave */
        

        /* output voltage through channel 2 */
        dac_output_voltage(DAC_CHANNEL_2, n);

        vTaskDelay(10 / portTICK_PERIOD_MS);  /* 10 ms*/
    }
}

void app_main()
{
    /* Create tasks */
    xTaskCreate(&createTriangleWave, "createTriangleWave", 4096, NULL, 5, NULL);
    xTaskCreate(&createSineWave, "createSineWave", 4096, NULL, 5, NULL);
}