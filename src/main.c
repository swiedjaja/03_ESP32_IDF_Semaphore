/*
Binary Semaphore Demo: how to send notification from one thread to other
*/
#include <stdio.h>
#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <driver/gpio.h>
#include "device.h"
#include "gpio_arduino.h"

SemaphoreHandle_t xBinarySemaphore;
volatile int nCount = 0;

void taskTrigger(void* arg)
{
    while (1)
    {
        gpio_set_level(LED_RED, LED_ON);
        nCount++;
        printf("Trigger: %d\n", nCount);
        fflush(stdout);
        vTaskDelay(10/portTICK_RATE_MS);
        gpio_set_level(LED_RED, LED_OFF);
        xSemaphoreGive(xBinarySemaphore);
        vTaskDelay(3000/portTICK_RATE_MS);
    }
}

void taskListener(void* arg)
{
    while (1)
    {
        xSemaphoreTake(xBinarySemaphore, portMAX_DELAY); 
        gpio_set_level(LED_GREEN, LED_ON); 
        vTaskDelay(100/portTICK_RATE_MS);
        printf("Listener : %d\n", nCount);
        fflush(stdout);
        gpio_set_level(LED_GREEN, LED_OFF);
    }
}

void app_main() {
    gpio_pinMode(LED_BUILTIN, OUTPUT);
    gpio_pinMode(LED_GREEN, OUTPUT);
    gpio_pinMode(LED_RED, OUTPUT);

    xBinarySemaphore = xSemaphoreCreateBinary();

    // xTaskCreatePinnedToCore(taskTrigger, "taskProducer", configMINIMAL_STACK_SIZE+1024, NULL, 2, NULL, 0);
    // xTaskCreatePinnedToCore(taskListener, "taskListener", configMINIMAL_STACK_SIZE+1024, NULL, 1, NULL, 1);

    xTaskCreate(taskTrigger, "taskTrigger", configMINIMAL_STACK_SIZE+2048, NULL, 4, NULL);
    xTaskCreate(taskListener, "taskListener", configMINIMAL_STACK_SIZE+2048, NULL, 5, NULL);
    while (1)
    {
        gpio_set_level(LED_BUILTIN, LED_BUILTIN_ON);
        vTaskDelay(100/portTICK_RATE_MS);
        gpio_set_level(LED_BUILTIN, LED_BUILTIN_OFF);
        vTaskDelay(1000/portTICK_RATE_MS);
    }
}