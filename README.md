# **Microprocessor-2-Lab-Template :zap:**
## **Objective**
The objective of this labs to to provide a more interactive assignments for students
to be engage in Microprocessor 2. Some of the labs will be new and other will be from previous semester but with a better documentation.
## **Setup**
* The students must have an `ESP32`
* Install either `Visual Studio Code` or `Eclipse` 
## **Installation**
* Window's Linux Subystem, MacOS, or Linux
  * Use the following link to install all the dependencies in the terminal and follow the guide: [ESP32 Installation](https://github.com/jminjares4/ESP32_Installation)
* Window 
  * Link to install ESP-IDF Windows Installer: [Window Installer](https://dl.espressif.com/dl/esp-idf/?idf=4.4)



## **Labs**
- [Lab 0 Lightshow](https://github.com/jminjares4/Microprocessor-2-Lab-Template/tree/main/Lab_0)
- [Lab 1 LED Controller with Inputs](https://github.com/jminjares4/Microprocessor-2-Lab-Template/tree/main/Lab_1)
- [Lab 2 Microwave](https://github.com/jminjares4/Microprocessor-2-Lab-Template/tree/main/Lab_2)
- [Lab 3 Introduction to FreeRTOS](https://github.com/jminjares4/Microprocessor-2-Lab-Template/tree/main/Lab_3) 
- [Lab 4 FreeRTOS: Semaphores](https://github.com/jminjares4/Microprocessor-2-Lab-Template/tree/main/Lab_4)
- [Lab 5 FreeRTOS: Queue](https://github.com/jminjares4/Microprocessor-2-Lab-Template/tree/main/Lab_5)
- [Lab 6 GPIO, Interrupts, and Queues](https://github.com/jminjares4/Microprocessor-2-Lab-Template/tree/main/Lab_6)
- [Lab 7 Peripheral and Queues: ADC and PWM (LEDC)](https://github.com/jminjares4/Microprocessor-2-Lab-Template/tree/main/Lab_7)
- [Lab 8 DAC Peripheral](https://github.com/jminjares4/Microprocessor-2-Lab-Template/tree/main/Lab_8)
- [Additional Labs](https://github.com/jminjares4/Microprocessor-2-Lab-Template/tree/main/Additional_Labs)
  - [Lab 9 WiFi and Internet of Things (IOT)](https://github.com/jminjares4/Microprocessor-2-Lab-Template/tree/main/Additional_Labs/Lab_9)
  - [Lab 10 WiFi and Internet of Things (IOT): Servo Motor Control](https://github.com/jminjares4/Microprocessor-2-Lab-Template/tree/main/Additional_Labs/Lab_10)
  - [Lab 11 WiFi and Internet of Things (IOT): ADC MOnitor](https://github.com/jminjares4/Microprocessor-2-Lab-Template/tree/main/Additional_Labs/Lab_11)

## **Development**
|Microcontroller|Software|IDE|Real-time Operating System (RTOS) |
| :---| :---| :---| :---|
| ![ESP32](https://img.shields.io/static/v1?label=&logo=espressif&message=Espressif+ESP32&&color=000000) | ![](https://img.shields.io/badge/Code-C-informational?style=flat&logo=C&color=003B57)| ![Visual Studio Code](https://img.shields.io/badge/Visual_Studio_Code-0078D4?style=flat&logo=visual%20studio%20code&logoColor=white) | ![FreeRTOS](https://img.shields.io/static/v1?label=OS&message=FreeRTOS&color=green&logo=data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAGMAAAAmCAIAAABfzLIdAAAKgklEQVR42u2aBVRcOxPHee7u7u7%2BWqzu7u7u7u4GNZy6UeVRd1rcXWpI3Ys7VH4f817ellvuYdnPy5ycPXeTySb5Z%2FKfyew1u3235BflXUo%2FnXQ99j4ucVcyzhUW5ZdA5m%2Bk0nKubw5fPH5nyyHbaleUSbvbbo92ycpPL4kUQNJWQruizNjX5WJ68t9IXc%2B6NHFXGxpGejT0iHJOvh6XnnsjOz%2Fj%2FixpOdcSrkVzvIa71wOT6fu6ZOal%2FonU6qBZVI3yaIxlKWOrkLhLQcP%2FqA8y7pEOfDXLyEsZ8UcDvh887qaUCgsLHdbbpKSk8Hzz5s209DSpz8jMsF05jc%2F7BCz3KEeQGbezRX5hrhnI8WWoe92U7CtKIz09vc1M85iYaJ4DgwPaTagu9YmJCe3nmfP5X7KSW7du3fxLeP6n9z2XekoI62zKCbOA5L0CW0FR3j2R8vI90mGWtRYpLG7YsGEd75a%2BffuOGzdu06ZNV69eZQYlBp41a1anTp06llnWr1%2BPdWsXcOLEiWnTptWoUeP9999%2FuVg%2B%2FvjjBg0a2NjYnDt3TsYtTWg9dOhQt27dvv7661dfffXNN9%2F85ZdfhgwZ4uvrqx0L2hrmXhd84i8Hm%2Fkm7uRpwq7WBUX5JZCKio7id496e4KUYH8q4aRCavny5Waly9NPPz1w4EDwUtvFPJ5%2F%2FnkzY6Ry5cpXrlwxnPrly5d79%2B796KOPltblmWeemTBhAvO%2Fp5lcv369fv36DzzwgLYjlS1atKCjoX5uQbZQFSevVKQ6LrDsY9N40NJWvW0a9nKtxgOlr22TrvbWgpS9vb2M8dRTT73xl7z00ksPP%2FywGv7333%2B%2FePGiTBqgLSwsnrtbWJhoPvjgg88%2B%2B2yJ1latWl24cEHN6tSpU9iO%2BnHG%2BvXXX7EsS0vLTz75xHDcOnXqqHGVFBQUVK1aVRQeeeSRH374oVGjRg0bNvzqq6%2F4KmCFhYUxTyOQQtvRbcEStxnaYu82LzcvFx07OzsZlfGCg4OPFUtsbOzevXu7dOny0EMPSSuWlZOTI7yQmpoaHx8fbSAHDx4UNVDmUERFRRm2njx5kr7KHD788ENRBtOhQ4d6eXmhcP78eUA5ffr0%2Fv37sRcQF522bduWMBDOsjS9%2BOKLS5Ys4ffZBmz27Nmz9G3WrBk%2F6%2B%2FvX1RUVHakyiQKqebNmzOeYkdGysjI6Nmzp7S%2B9tprZ86cASZFooZCk6jBF4DICS2hoIbr2rWrMiUOPtDk5eXdMhDGZScGDRokamzVhg0bDKmnSZMm0gSZJicnqybpm5aWFh4eDnZ8NQKp%2FPz8GY6jL1%2B5LPySmJQo9SmpKeMXD4DLDZHieMNHJXBkVHUWjh49KhulFfZTIYVJqllqKVy4CZPBLejQdlZW1k8%2F%2FSS%2FWa1aNSxRwfH9999LvZOTU3Z29j3JHjGap5Tv8%2Fb1ajPZUuv79JG6ceOGHH5k586dcIQpSOFPRY3VRkZGsnk6vn%2Fz5s2KjEJDQ2Xx1CsEZ8%2BenZsLgeiL8UhJlGAkUmgmiouBaL29vVlbuZFiqT%2F%2F%2FLOoEZdcu3ZNf3mcoxdeeEH0nZ2dOR%2BCVPv27aXyyy%2B%2FVITwn0dq4sSJ0vrNN9%2FExcWx2nIjBfuIiwT6jRs36puDcrLys9BWZmam1GPaKj4ghoqIiBBOMBWpTjZW7WdbdZxj3WGuVZ9V1XigUNPN0RraKoGUYdTDShYsWCA%2Bm8%2F58%2BeLFZQbKfyp6Dz22GOBgYHKPHUOYOfOnaVL69at4QFFvoQdij2ffPLJXr16HT9%2BHGTLjxSyadeaNR7O2uK2Y5XMVSH1zjvvNG7cmEkg1atXxwdL%2FRNPPDFy5EimwtaZgpSPj4%2FyejExMbIwfSGGkC716tUz3EVQY5ISSSgu69ChA1avnaSpUYIyfoWUVphK7dq1165dm5CQIDRhClK7d%2B9WMZfS0ZcxY8ZIl5o1a166dMnQ3IhgXFxcPvvsM1FQmzpixAiOOQpGI8XWefkfXr9jeUhEoNScOHXMYdPc7nPqljh90OfnxcJKhAj4JDrn1Ij1mYiUik6534lO2W0KYzdESh1DxoUioHaZsIIVojAunkLW73Lpu7p6u3mVucrsPLRt3tpxvV2r91hUa%2FH66bm5OYKUmg3siLNjGW5ublw4VDiDtZuOVFBQkLo2gX5ZTh%2F3cOmCv9N6GzEuAlfiewcHB9hDgdW%2Ff39uBUYgxQ91n1%2FHZfMiztoad5fOi60Guzb12L%2BJuE4UtIwugTJGBJVwRxfLsrW1Nf30JSUlCbPw6enpqcN6avJYtPzs2LFjCRp0NJke18lKlSopmueqwGaUFSkOc7s55uHhYTwDfJcl1t7%2BRwzH0IkSGIbshzR9%2BumndGdCpiBFPE2rqHFf04de0kFkMkR%2F3bp1YiP6yGKqUJV0WbFiBUMYESW0nWW%2BesNyL7%2FDO%2Fa6t59rsW7TKp4pPgFe7KoOUhIBwSlqYAnQTYk88Q%2Bixj1WbERHQEddOTm5wpX6worIKEgvwhrALStSqLaeZt7FzrKLnQWlu7M1n1LazK6UkHBKHykWPHjwYGklbmCTTUFKcmGKqshb6FAVu0KKTpTbtWtXxlgcNKFXdSs0AinkwsUL%2BDhtSUpOYuyy3JC5zUvAEhAQYEqMjuCScKyiSfwttHhP0%2BjTp4%2Bokd7aunWrIlZ8C1ma0qybgFZmyxk8fPiwEaePeQSHB6oO3Mi1M9NHijmZm5uLwoABA%2BAaU5ACaMxKefTffvsNy8IQlD4POF%2B8sEpgEijgCtQOrVq1ikoSWCQ2cFOGHdlUEoHSsW7duuQtjGP01tMrR0SEy5qbjalEptgopJgBt1NRePvtt8kHUVNupCTiJSmowOKeRG6AaGD48OHkm6ysrDAHFfeSg2f9xAHaNB6t%2BBk64haJNiEHlWtmnh4eHqzduHsfLM5gPDNe25nmpC%2BMQgqhUm42LM%2FV1RXETUEK4a47c%2BZM%2BU2tKBYfPXo08Z3iGuVkatWqpYDWCoyOKyBM1Yk8d5UPKZK5cCdEOH78eFjgnkeGTBAbiG0TNwhlaD06vPPRRx9xLrj66CMllkVWF1sgGcD%2FFw8VC%2FbFlZAjyV8sBPSgr40kJDdN6op73wcffMBlWzqCO8HU5MmTyQtDMnqZvLCzR4r%2FZ2%2BUW5htiFRHG6t%2BC5qPsO84zK5DT9eqfef%2F45kybFEnCFXokx0gbCNiKo0pQRnvw8kn6yreWrsAwAIjOEULZWmcBesxNHdvfAU8zSfpPWaCFQsH6fg4ggymhD5ZM9DB%2BuiIxwBcbUf%2BeQccysmrEWZnU07KlxNXIgydiO2qqfPXjteWuSvG3Ui5odaJ6MzMKJ1y%2FC3KPFk8nwhfjfpPVPrqdww76wky%2FOV3I%2FuyWdHNQt7o4PtSrxE8K6UKyS%2FKnXuwN8jYeg4CWzOqApP3iVmtCJiWkZtCTYVgRA4%2Bo4vfQ6gTezHgz3ddAGxDqI2ANcqjkaPPWLcw203hi%2B%2FP4hZqY%2Bc9St5qGbqtzo6YZXe9aVZ0q2hv%2FFpeDKJZSkUZu6PZ0VPumJFC6i6T8zyxZVXgTDvvkUu8ht%2BfBYPihTLvBA9et9O851kGqZA7hD1hk6I9Qm8AAAAASUVORK5CYII%3D)|

## **License**
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

### Authors
* [***Jesus Minjares***](https://github.com/jminjares4)
  * **Master of Science in Computer Engineering**
  * Embedded Software Engineer 
  * Contact <br>
  [![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white&style=flat)](https://www.linkedin.com/in/jesus-minjares-157a21195/) [![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white&style=flat)](https://github.com/jminjares4)
* [***Erick Baca***](https://github.com/eabaca2419)
  * **Master of Science in Computer Engineering** 
  * 5G Security Engineer

