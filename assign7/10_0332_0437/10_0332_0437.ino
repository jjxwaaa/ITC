#include <EEPROM.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TimerOne.h>
#include <DHT.h>
#define OLED_RESET -1
Adafruit_SSD1306 OLED(OLED_RESET);
//DHT
float t;
#define DHTPIN  6              
#define DHTTYPE DHT22           
DHT dht(DHTPIN, DHTTYPE);
//WARNINGS
#define buzzer 11
#define DO 261.6
#define ME 270
//SWITCH
#define modeSw 2
#define saveSw 3
#define upSw 4
#define downSw 5          
int32_t bounce;
//STATE
#define None 0
#define Setup 1
#define Alarm 2
#define Stopwatch 3
#define TempMode 4
#define SetupMin 5
#define AlarmMin 6
int state = 0;
//BOOL CLICK
bool save = false;
bool mode = false;
bool up = false;
bool down = false;
bool setting = false;
bool warning = false;
bool isSetAlarm = false;

float temp = dht.readTemperature();
float hum = dht.readHumidity();


struct Time
{
    int hour;
    int minute;
    int sec;
};
Time currentTime = {0, 0, 0};
Time loadTime = {0, 0, 0};
Time isAlarm = {0, 0, 0};
Time isStw = {0, 0, 0};
void Speaker()
{
    tone(buzzer, DO);
    delay(200);
    tone(buzzer,ME);
    delay(200);
}
void sendSensor()
{
    float t = dht.readTemperature();
    String temp = String(t) + " °C";
}
void plusOneSec(Time &time)
{
    time.sec++;
    time.minute += time.sec / 60;
    time.sec %= 60;
    time.hour += time.minute / 60;
    time.minute %= 60;
    time.hour %= 24;
}
void TimeRuning()
{
    plusOneSec(currentTime);
    if (state == Stopwatch && setting)
    {
        plusOneSec(isStw);
    }
}
void showTime()
{
    // HOUR
    OLED.setCursor(9.5, 15);
    OLED.setTextSize(2.9999);
    if (currentTime.hour < 10)
    {
        OLED.print("0");
    }
    OLED.print(currentTime.hour);
    // MINUTE
    OLED.setTextSize(3.00);
    OLED.print(":");
    OLED.setTextSize(2.9999);
    if (currentTime.minute < 10)
    {
        OLED.print("0");
    }
    OLED.print(currentTime.minute);
    // SECOND
    OLED.setCursor(100, 2);
    OLED.setTextSize(1.00);
    if (currentTime.sec < 10)
    {
        OLED.print("0");
    }
    OLED.print(currentTime.sec);
    OLED.print(" s");
    
    //TEMPERATURE
    OLED.setCursor(22.5, 1);
    OLED.setTextSize(0.01);
    OLED.print(dht.readTemperature());
    OLED.print(" C");
}
void showTimeSetup()
{
    // HOUR
    OLED.setCursor(2, 5);
    OLED.setTextSize(3);
    if (currentTime.hour < 10)
    {
        OLED.print("0");
    }
    OLED.print(currentTime.hour);
    // MINUTE
    OLED.setTextSize(3.00);
    OLED.print(":");
    OLED.setTextSize(3);
    if (currentTime.minute < 10)
    {
        OLED.print("0");
    }
    OLED.print(currentTime.minute);
    // SECOND
    OLED.setCursor(100, 2);
    OLED.setTextSize(1.00);
    if (currentTime.sec < 10)
    {
        OLED.print("0");
    }
    OLED.print(currentTime.sec);
    OLED.print(" s");
}
void AlarmWarning()
{
    if (isAlarm.hour == currentTime.hour && isAlarm.minute == currentTime.minute)
    {
        if (state != Alarm || state !=AlarmMin )
        {
           Speaker();
        }
    }
}
void showMode(String str)
{
    // MODE
    OLED.setCursor(95, 13);
    OLED.setTextSize(0.40);
    OLED.print(str);
    OLED.setCursor(99, 24);
    OLED.setTextSize(0.40);
    OLED.print("MODE");
}
void showAlarm()
{
    // HOUR
    OLED.setCursor(2, 5);
    OLED.setTextSize(3.00);
    if (isAlarm.hour < 10)
    {
        OLED.print("0");
    }
    OLED.print(isAlarm.hour);
    // MINUTE
    OLED.setTextSize(2.5);
    OLED.print(":");
    OLED.setTextSize(3.00);
    if (isAlarm.minute < 10)
    {
        OLED.print("0");
    }
    OLED.print(isAlarm.minute);
    // AM
    OLED.setCursor(105, 0.5);
    OLED.setTextSize(1.00);
    if (isAlarm.hour >= 0 && isAlarm.hour <= 12)
    {
        OLED.print("AM");
    }
    // PM
    if (isAlarm.hour >= 13 && isAlarm.hour <= 23)
    {
        OLED.print("PM");
    }
}
void showSTW()
{
    // MINUTE
    OLED.setCursor(2, 5);
    OLED.setTextSize(3.00);
    if (isStw.minute < 10)
    {
        OLED.print("0");
    }
    OLED.print(isStw.minute);
    // SECOND
    OLED.setTextSize(2.5);
    OLED.print(":");
    OLED.setTextSize(3.00);
    if (isStw.sec < 10)
    {
        OLED.print("0");
    }
    OLED.print(isStw.sec);
    // HOUR
    OLED.setCursor(100, 0.5);
    OLED.setTextSize(1.00);
    if (isStw.hour < 10)
    {
        OLED.print("0");
    }
    OLED.print(isStw.hour);
    OLED.print(" h");
    // MODE
    OLED.setCursor(103, 13);
    OLED.setTextSize(0.40);
    OLED.print("STW");
    OLED.setCursor(99, 24);
    OLED.setTextSize(0.40);
    OLED.print("MODE");
}
void dayMode()
{
     // AM
    OLED.setCursor(105, 20);
    OLED.setTextSize(1.00);
    if (currentTime.hour >= 0 && currentTime.hour <= 12)
    {
        OLED.print("AM");
    }
    // PM
    if (currentTime.hour >= 13 && currentTime.hour <= 23)
    {
        OLED.print("PM");
    }
}
void showTemp()
{
    //TEMP
    OLED.setCursor(22, 2);
    OLED.setTextSize(2);
    //OLED.print("Temp :");
    OLED.print(dht.readTemperature());
    OLED.print(" C");
    
    //HUMIDITY
    OLED.setCursor(24, 25);
    OLED.setTextSize(1);
    OLED.print("Hum : ");
    OLED.print(dht.readHumidity());
    OLED.print(" %");
}
void timeTemp()
{
     //TEMPERATURE
    OLED.setCursor(22.5, 1);
    OLED.setTextSize(0.01);
    OLED.print(dht.readTemperature());
    OLED.print(" C");
}
void settingSTU(int x)
{
    OLED.setCursor(x, 11);
    OLED.setTextSize(3.00);
    OLED.print("__");
}
void pressButton(int pin, void (*callback)())
{
    if (!digitalRead(pin))
    {
        if (millis() - bounce > 300)
        {
            callback();
        }
        bounce = millis();
    }
}
void setup()
{
    Serial.begin(9600);
    dht.begin();
    OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    OLED.setTextColor(WHITE);

    pinMode(modeSw, INPUT_PULLUP);
    pinMode(saveSw, INPUT_PULLUP);
    pinMode(upSw, INPUT_PULLUP);
    pinMode(downSw, INPUT_PULLUP);

    Timer1.initialize(1000000); // 1s 
    Timer1.attachInterrupt(TimeRuning);

    EEPROM.get(0, loadTime);
    currentTime.sec = loadTime.sec;
    currentTime.minute = loadTime.minute;
    currentTime.hour = loadTime.hour;
    Serial.println(currentTime.hour);
    Serial.println(currentTime.minute);
    Serial.println(currentTime.sec);
    if (currentTime.hour < 0)
    {
        currentTime = {0, 0, 0};
    }
}

void modeSwPressed()
{
    if (state == Stopwatch)
    {
        setting = false;
        isStw = {0, 0, 0};
    }

    if (setting)
    {
        if (state == Setup)
        {
            state = SetupMin;
        }
        else if (state == SetupMin)
        {
            state = Setup;
        }
        else if (state == Alarm)
        {
            state = AlarmMin;
        }
        else if (state == AlarmMin)
        {
            state = Alarm;
        }
    }
    else
    {
        state++;
        state %= 5;
        //สลับ ชม.นาที.
    }
}
void saveSwPressed()
{
    if (state == None)
    {
        EEPROM.put(0, currentTime);
        Serial.println(currentTime.hour);
        Serial.println(currentTime.minute);
        Serial.println(currentTime.sec);
        if (isSetAlarm)
        {
            noTone(buzzer);
            isSetAlarm = false;
        }   
    }
    else if (state == Setup || state == SetupMin)
    {
        if (isSetAlarm)
        {
            noTone(buzzer);
            isSetAlarm = false;
        }   
        if (setting)
        {
            //กลับไป state 0
            state = None;
            setting = false;
        }
        //เข้าสู่ setup
        else
        {
            setting = true;
        }
    }
    else if (state == Alarm || state == AlarmMin)
    {
        if (setting)
        {
            // saveการตั้งค่า
            EEPROM.put(0, isAlarm);
            isSetAlarm = true;
            setting = false;
        }
        //เข้าสู่ alarm
        else
        {
            setting = true;
        }
    }
    else if (state == Stopwatch)
    {
        if (isSetAlarm)
        {
            noTone(buzzer);
            isSetAlarm = false;
        }   
        //เข้าสู่ STW
        setting = !setting;
        // stop play
    }
}
void upSwPressed()
{
    if (setting)
    {
        if (state == Setup)
        {
            currentTime.hour++;
            currentTime.hour %= 24;
        }
        else if (state == SetupMin)
        {
            currentTime.minute++;
            currentTime.minute %= 60;
        }
        else if (state == Alarm)
        {
            isAlarm.hour++;
            isAlarm.hour %= 24;
        }
        else if (state == AlarmMin)
        {
            isAlarm.minute++;
            isAlarm.minute %= 60;
        }
    }
}
void downSwPressed()
{
    if (setting)
    {
        if (state == Setup)
        {
            currentTime.hour--;
            if (currentTime.hour < 0)
            {
                currentTime.hour = 23;
            }
        }
        else if (state == SetupMin)
        {
            currentTime.minute--;
            if (currentTime.minute < 0)
            {
                currentTime.minute = 59;
            }
        }
        else if (state == Alarm)
        {
            isAlarm.hour--;
            if (isAlarm.hour < 0)
            {
                isAlarm.hour = 23;
            }
        }
        else if (state == AlarmMin)
        {
            isAlarm.minute--;
            if (isAlarm.minute < 0)
            {
                isAlarm.minute = 59;
            }
        }
    }
}

void loop()
{
    Serial.println(state);
    OLED.clearDisplay();
    //กดปุ่ม
    pressButton(modeSw, modeSwPressed);
    pressButton(saveSw, saveSwPressed);
    pressButton(upSw, upSwPressed);
    pressButton(downSw, downSwPressed);
    // saveEEPROM
    if (currentTime.sec == 0)
    {
        EEPROM.put(0, currentTime);
    }
    // TimeRun
    if (state == None)
    {
        showTime();
        dayMode();
        timeTemp();
    }
    // STU
    else if (state == Setup || state == SetupMin)
    {
        showTimeSetup();
        showMode("SETUP");
        if (setting)
        {
            settingSTU(2 + (48 * (state == SetupMin)));  
        }
    }
    // ALARM
    else if (state == Alarm || state == AlarmMin)
    {
        showAlarm();
        showMode("ALARM");
        if (setting)
        {
            settingSTU(2 + (48 * (state == AlarmMin)));
        }
    }
    // STW
    else if (state == Stopwatch)
    {
        showSTW();
    }
    else if (state == TempMode)
    {
        showTemp();
    }
   
    if (isSetAlarm)
    {
    AlarmWarning();
    }
    
    OLED.display();
}
