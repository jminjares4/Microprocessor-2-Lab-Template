/*
    Author:     Jesus Minjares and Erick Baca
                Master of Science in Computer Engineering
    
    Course:     EE 4178/5190 Laboratory for Microprocessors Systems II

    Lab 5:
        Objective:
            Understand how to use the queues with FreeRTOS. In this lab, 
            the main focus will be using queues to send data among tasks. 
            Students must a create queue to send and receive data between two tasks.
            Task 1 must send an integer i.e. (`10`) and then modified to use a string i.e. ("Fall 2022").
            Task 2 should receive the data and print it.

            | Task          |  Description                     |
            | :---          | :---                             |
            | **Task 1**    | Send data to **queue**           |
            | **Task 2**    | Receive **queue** data and print |

        Bonus:
            Undergrad Bonus:
                Modify the code to use the structure provided below on a queue.   

                typedef struct {
                    char str[10];
                    int32_t val;
                }Undergaduate_t;
   
            Grad Bonus:
                Modify the code to use the structure provided below on a queue.
                Also, add an LED to change states based on `isLedOn`.
                
                typedef struct {
                    char str[10];
                    int32_t val;
                    char studentName[15];
                    uint32_t studentID;
                    bool isLedOn;
                }Graduate_t;

*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

/* Undergraduate Bonus */
/**
 * @brief Undergraduate bonus
 * 
 */
typedef struct {
    char str[10]; /*!< string */
    int32_t val;  /*!< value */
}Undergaduate_t;

/* Graduate Bonus */
/**
 * @brief Graduate bonus
 * 
 */
typedef struct {
    char str[10];           /*!< string */
    int32_t val;            /*!< value */
    char studentName[15];   /*!< student name */
    uint32_t studentID;     /*!< student id */
    bool isLedOn;           /*!< led state */
}Graduate_t;

/* Global Queue handle */
QueueHandle_t xQueue;

/* Task 1 */
void task1(void *pvParameters){
    int32_t data; /* store data */
    while(1){
        /* Send data from queue */
    }
}

/* Task 2 */
void task2(void *pvParameters){
    int32_t dataReceive; /* store queue data */

    while(1){
        /* Wait to receive from queue */
    }
}

void app_main(void){
    /*
        Create data to be sent: global or static variables
    */

    /* Create Queue of type int32_t */
    xQueue = xQueueCreate(5, sizeof(int32_t));
   
    /* Check if the queue was create successfully */
    if(xQueue != NULL){
        /* Create sender tasks and send int32_t as arguments */

        /* Create reciever task */

    }
}
