#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include <queue.h>
#include "pico/stdlib.h"

#include "MathClass.h"

MathClass *pMath;

static QueueHandle_t xQueue = NULL;

void led_task(void*)
{   
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    TickType_t delay;
    uint uiQueueValue = 0;
    bool fast = false, last = false;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    pMath = new MathClass(0);

    // first 5 flashes slow, second 5 fast, repeat

    while (true)
    {

        fast = pMath->value() > 4;

        if (fast != last)
        {
            const char* msg = fast ? "Fast mode" : "Slow mode";
            printf("%s\n", msg);
            last = fast;
        }

        delay = fast ? 20 : 50;
        gpio_put(LED_PIN, 1);
        uiQueueValue++;
        xQueueSend(xQueue, &uiQueueValue, 0U);

        vTaskDelay(delay);
        gpio_put(LED_PIN, 0);
        uiQueueValue++;
        xQueueSend(xQueue, &uiQueueValue, 0U);
        
        vTaskDelay(delay);

        pMath->inc();
    
        if (pMath->value() == 10)
        {
            pMath->setValue(0);
            vTaskDelay(100);
            printf("Waiting...\n");
        } 
    }
}

void usb_task(void* param)
{
    uint uiQueueValue;

    while (true)
    {
        xQueueReceive(xQueue, &uiQueueValue, portMAX_DELAY);

        if (uiQueueValue % 2 == 0)
            printf("LED On: %d\n", uiQueueValue);
        else
            printf("LED Off: %d\n", uiQueueValue);
    }


}

int main()
{
    stdio_init_all();

    xQueue = xQueueCreate(1, sizeof(uint));

    xTaskCreate(led_task, "LED_Task", 256, NULL, 1, NULL);
    xTaskCreate(usb_task, "USB_Task", 256, NULL, 1, NULL);
    vTaskStartScheduler();

    while(1){};
}
