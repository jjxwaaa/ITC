#define button1 12
#define button2 10
#define button3 11
#define led1 4
#define led2 2
#define led3 3    
              
int b[4] = {button1, button1, button2, button3};
int l[4] = {led1, led1, led2, led3};
int reading[4];
int buttonState[4] = {LOW, LOW, HIGH, HIGH};
int lastButtonState[4] = {LOW, LOW, HIGH, HIGH};
int ledState[4] = {LOW, LOW, LOW, LOW};
//int lastLedState[4] = {LOW, LOW, LOW, LOW};
long long lastDebounceTime[4] = {0, 0, 0, 0};
long long ledStartTime[4] = {0, 0, 0, -2000};
int Delay = 50;
long long cooltime[4] = {0, 0, 0, 0};
int coolDown[4] = {0,0,0,2000};

void setup(){
    Serial.begin(9600);
    pinMode(button1, INPUT);
    pinMode(button3, INPUT);
    pinMode(button2, INPUT_PULLUP);
  
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
}

void debounce(int it){
  
    reading[it] = digitalRead(b[it]);
    
    if(lastButtonState[it] != reading[it]){
        lastDebounceTime[it] = millis();
        Serial.print("pressed button ");
        Serial.println(it);
    }
  
    if(millis() - lastDebounceTime[it] > Delay){
        buttonState[it] = reading[it];
    }
  
    lastButtonState[it] = reading[it];
    
}

void toggle(int it){
    if (millis() - cooltime[it] > Delay) 
    {       
      int chk = 0;
      for(int i = 1; i < it;i++){
        if(ledState[i] == HIGH) chk = 1;
      }
      
      if(chk == 1 && ledState[it] == HIGH){
          ledStartTime[it] = millis();         
          ledState[it] = LOW;         
          digitalWrite(l[it], ledState[it]);
      }
      else if(chk == 0 && millis() - ledStartTime[it] > coolDown[it]){
          ledStartTime[it] = millis();         
          ledState[it] = !ledState[it];         
          digitalWrite(l[it], ledState[it]);
      }
                
    }                                      
     cooltime[it] = millis();
}

void loop(){
  
    debounce(1);
    debounce(2);
    debounce(3);
    
  
    if(millis() - ledStartTime[1] > 3000){
        ledState[1] = LOW;
        digitalWrite(led1, LOW);  
    }
    
    if(millis() - ledStartTime[2] > 3000){
        ledState[2] = LOW;
        digitalWrite(led2, LOW);  
    }
  
    if(millis() - ledStartTime[3] > 2000){
        ledState[3] = LOW;
        digitalWrite(led3, LOW);  
    }
  else{
      if(millis() - ledStartTime[3] < 500){
          ledState[3] = HIGH; 
      }
      else if(millis() - ledStartTime[3] < 1000){
          ledState[3] = LOW;  
      }
      else if(millis() - ledStartTime[3] < 1500){
          ledState[3] = HIGH; 
      }
      
      digitalWrite(led3, ledState[3]);  
    }
 
    if(buttonState[1] == HIGH){
        //Serial.println("toggle 1");
        toggle(1);
    }
    if(buttonState[2] == LOW){
        //Serial.println("toggle 2");
        toggle(2);
    }
    if(buttonState[3] == LOW){
        //Serial.println("toggle 3");
        toggle(3);
    }
  
}
