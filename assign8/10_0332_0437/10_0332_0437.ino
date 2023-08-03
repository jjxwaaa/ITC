#include <Arduino_FreeRTOS.h>
#include <queue.h>

// Define LED
#define LED_RED 8
#define LED_YELLOW 10
#define LED_GREEN 9

// Define Button
#define BUTTON_RED 2
#define BUTTON_YELLOW 4
#define BUTTON_GREEN 3

// Defin Delay
#define Delay(x) vTaskDelay(pdMS_TO_TICKS(x))

QueueHandle_t RED_QUEUE;
QueueHandle_t YELLOW_QUEUE;
QueueHandle_t GREEN_QUEUE;

void setup()
{
    RED_QUEUE = xQueueCreate(10, sizeof(bool));
    YELLOW_QUEUE = xQueueCreate(10, sizeof(bool));
    GREEN_QUEUE = xQueueCreate(10, sizeof(bool));

    xTaskCreate(redTaskReceiver, "RED TASK RECIEVER", 128, (void *)LED_RED, 1, NULL);
    xTaskCreate(redTaskSender, "RED TASK SENDER", 128, (void *)BUTTON_RED, 1, NULL);
    xTaskCreate(yellowTaskReceiver, "YELLOW TASK RECIEVER", 128, (void *)LED_YELLOW, 1, NULL);
    xTaskCreate(yellowTaskSender, "YELLOW TASK SENDER", 128, (void *)BUTTON_YELLOW, 1, NULL);
    xTaskCreate(greenTaskReceiver, "GREEN TASK RECEIVER", 128, (void *)LED_GREEN, 1, NULL);
    xTaskCreate(greenTaskSender, "GREEN TASK SENDER", 128, (void *)BUTTON_GREEN, 1, NULL);
}

void redTaskSender(void *pvParameters)
{
    unsigned short button = (unsigned short)pvParameters;
    int is_button_pressed = false;

    pinMode(button, INPUT_PULLUP);

    while (true)
    {
        if (isButtonPressed(button) && !is_button_pressed)
        {
            is_button_pressed = true;

            xQueueSend(RED_QUEUE, &button, 0);
        }

        if (!isButtonPressed(button))
            is_button_pressed = false;

        Delay(50);
    }
}

void redTaskReceiver(void *pvParameters)
{

    unsigned short led = (unsigned short)pvParameters;

    pinMode(led, OUTPUT);

    unsigned short red_count = 0;

    while (true)
    {

        // REMOVE VALUE
        if (red_count > 0 && led == LED_RED)
            red_count--;

        bool is_received = false;

        // RECEIVE
        if (led == LED_RED)
            is_received = xQueueReceive(RED_QUEUE, NULL, 0);

        // IF RED LED RECEIVE
        if (is_received && led == LED_RED)
            red_count += 3;

        // RED LED CONDITION
        if (red_count > 0 && led == LED_RED)
            digitalWrite(LED_RED, HIGH);
        else if (!red_count && led == LED_RED)
            digitalWrite(LED_RED, LOW);

        Delay(1000);
    }
}

void yellowTaskSender(void *pvParameters)
{
    unsigned short button = (unsigned short)pvParameters;
    int is_button_pressed = false;

    pinMode(button, INPUT_PULLUP);

    while (true)
    {
        if (isButtonPressed(button) && !is_button_pressed)
        {
            is_button_pressed = true;
            xQueueSend(YELLOW_QUEUE, &button, 0);
        }
        if (!isButtonPressed(button))
            is_button_pressed = false;

        Delay(50);
    }
}

void yellowTaskReceiver(void *pvParameters)
{

    unsigned short led = (unsigned short)pvParameters;
    pinMode(led, OUTPUT);

    bool is_yellow_blink = true;
    bool yellow_blink = true;

    while (true)
    {

        bool is_received = false;
        if (led == LED_YELLOW)
            is_received = xQueueReceive(YELLOW_QUEUE, NULL, 0);

        // RECEIVE
        if (is_received && led == LED_YELLOW)
            is_yellow_blink = !is_yellow_blink;

        // YELLOW LED CONDITION
        if (is_yellow_blink && led == LED_YELLOW)
        {
            digitalWrite(LED_YELLOW, !yellow_blink);
            yellow_blink = !yellow_blink;
        }
        else if (!is_yellow_blink && led == LED_YELLOW)
        {
            digitalWrite(LED_YELLOW, LOW);
        }
        Delay(500);
    }
}

void greenTaskSender(void *pvParameters)
{
    unsigned short button = (unsigned short)pvParameters;
    int is_button_pressed = false;

    pinMode(button, INPUT_PULLUP);

    while (true)
    {
        if (isButtonPressed(button) && !is_button_pressed)
        {
            is_button_pressed = true;
            xQueueSend(GREEN_QUEUE, &button, 0);
        }

        if (!isButtonPressed(button))
        {
            is_button_pressed = false;
        }
        Delay(50);
    }
}

void greenTaskReceiver(void *pvParameters)
{
    unsigned short led = (unsigned short)pvParameters;

    pinMode(led, OUTPUT);

    unsigned short green_count = 0;

    while (true)
    {

        bool is_received = false;
        if (led == LED_GREEN)
            is_received = xQueueReceive(GREEN_QUEUE, NULL, 0);

        // RECEIVE
        if (is_received && led == LED_GREEN && green_count == 0)
            green_count = 6;

        if (green_count > 0 && green_count % 2 != 0 && led == LED_GREEN)
        {
            digitalWrite(led, LOW);
            green_count -= 1;
        }
        else if (green_count > 0 && green_count % 2 == 0 && led == LED_GREEN)
        {
            digitalWrite(led, HIGH);
            green_count -= 1;
        }

        Delay(500);
    }
}

bool isButtonPressed(int pin)
{
    Delay(10);
    if (digitalRead(pin) == LOW)
        return true;
    return false;
}

void loop() {}
