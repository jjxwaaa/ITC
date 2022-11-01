#define b1 12
#define b2 11
#define b3 10
#define l1 4
#define l2 3
#define l3 2
#define delayTime 80
#define toggleLED(x, i)                         \
    {                                           \
        if (millis() - cooldown[i] > delayTime) \
        {                                       \
            ledStartTime[i] = millis();         \
            ledState[i] = !ledState[i];         \
            digitalWrite(x, ledState[i]);       \
        }                                       \
        cooldown[i] = millis();                 \
    }
bool ledState[3] = {LOW, LOW, LOW};
long int ledStartTime[3] = {0, -2000, 0};
int button[3] = {b1, b2, b3};
bool buttonState[3] = {LOW, HIGH, HIGH};
bool reading[3];
bool lastButtonState[3] = {LOW, HIGH, HIGH};
unsigned long long int lastDebounceTime[3];
unsigned long long int cooldown[3];
bool yellow = LOW;
void debounce(int i)
{
    reading[i] = digitalRead(button[i]);
    if (reading[i] != lastButtonState[i])
        lastDebounceTime[i] = millis();

    if (millis() - lastDebounceTime[i] > delayTime)
    {
        buttonState[i] = reading[i];
    }
    lastButtonState[i] = reading[i];
}
void setup()
{
    Serial.begin(9600);
    pinMode(l1, OUTPUT);
    pinMode(l2, OUTPUT);
    pinMode(l3, OUTPUT);
    pinMode(b1, INPUT);        //PULLDOWN
    pinMode(b2, INPUT);        //PULLUP
    pinMode(b3, INPUT_PULLUP); //PULLUPBUILTIN
}
void loop()
{
    debounce(0); // b1 RED
    if (!ledState[0])
    {
        debounce(2); // b3 GREEN
        if (!ledState[2])
            debounce(1); // b2 YELLOW
    }
    if (millis() - ledStartTime[0] > 3000) // RED
    {
        ledState[0] = LOW;
        digitalWrite(l1, ledState[0]);
    }
    if (millis() - ledStartTime[2] > 3000) //GREEN
    {
        ledState[2] = LOW;
        digitalWrite(l3, ledState[2]);
    }
    if (millis() - ledStartTime[1] > 2000) // YELLOW
    {
        ledState[1] = LOW;
        digitalWrite(l2, ledState[1]);
    }
    else
    {
        if (millis() - ledStartTime[1] < 500)
        {
            yellow = HIGH;
            digitalWrite(l2, yellow);
        }
        else if (millis() - ledStartTime[1] < 1000)
        {
            yellow = LOW;
            digitalWrite(l2, yellow);
        }
        else if (millis() - ledStartTime[1] < 1500)
        {
            yellow = HIGH;
            digitalWrite(l2, yellow);
        }
    }
    //Setting LED
    if (buttonState[0] == HIGH)
        toggleLED(l1, 0) 
    if (buttonState[1] == LOW)
        toggleLED(l2, 1) 
    if (buttonState[2] == LOW)
        toggleLED(l3, 2)
}.
..
