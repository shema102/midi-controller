#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

void vApplicationMallocFailedHook(void *pvReturnAddress) {
    (void)pvReturnAddress;
    taskDISABLE_INTERRUPTS();
    for(;;) { }
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    (void)xTask; (void)pcTaskName;
    taskDISABLE_INTERRUPTS();
    for(;;) { }
}