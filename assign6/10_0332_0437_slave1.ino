//SLAVE1
#include<Wire.h>
#define MASTER 1
#define SLAVE1 2
#define SLAVE2 3

char receiveMessage[100] = "Token#0#";
String inputMessage = "";
char finalMessage[100] = "Token#0#";
String target;

long long lastTime = 0;
int cooldown = 500;
int step = 0;
int ready = 0;

int debug = 0;


void setup()
{
  Wire.begin(SLAVE1);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
}

void send(){
  if(step == 2){
    //Serial.println(step);
   Wire.beginTransmission(SLAVE2);
   Wire.write(finalMessage);
   Wire.endTransmission();
    if(debug){
     Serial.print("From Slave1 : "); 
    Serial.println(finalMessage);
    }
    step = 0;
  }
  else {
    Wire.beginTransmission(SLAVE2);
   Wire.write(receiveMessage);
   Wire.endTransmission();
    if(debug){
     Serial.print("From Slave1 : "); 
    Serial.println(receiveMessage);
    }
  }
  ready = 0;
  lastTime = millis();
}
void hoy(){
  finalMessage[6] = '0';
  finalMessage[8] = '\0';
}

void receiveEvent(int nBytes)
{
  //receiveMessage = "";
  int i;
  for(i = 0; i < nBytes; i++)
  {
    receiveMessage[i] = Wire.read(); //อ่านค่าไว้ทีละตัว
  }
  receiveMessage[i] = '\0';
  if(1){
   Serial.print("Receive : ");
   Serial.println(receiveMessage);
  }
  
  ready = 1;
  if(receiveMessage[6] == '1' && receiveMessage[8] == '1'){
    for(int j = 10; j < i; j++){
      Serial.print(receiveMessage[j]);
    }
    Serial.println();
    
    receiveMessage[6] = '0';
    receiveMessage[8] = '\0';
  }
  else if(step != 2){
    int j = 0;
    for(j = 0; j < i; j++)
    {
      finalMessage[j] = receiveMessage[j];
    }
    finalMessage[j] = '\0';
  }
  
}

void genMessage(int len){
  finalMessage[6] = '1';
  finalMessage[7] = '#';
  finalMessage[8] = target[0];
  finalMessage[9] = '#';
  
  int i;
  for(i = 0; i < len; i++){
    finalMessage[10 + i] = inputMessage[i];
    //Serial.println(inputMessage[i]);
  }
  finalMessage[10 + i] = '\0';
  if(debug){
   Serial.print("Message : ");
   Serial.println(finalMessage);
  }
  ready = 1;
  step = 2;
}

void loop()
{
  
  while(Serial.available()){
    inputMessage = Serial.readString();

     if(step == 0){
       target = inputMessage;
       if(target[0] - '0' < 1 || target[0] - '0' > 2 || inputMessage.length() > 1){
         Serial.println("I love you");
       }
       else
        step = 1;
     }
     else if(step == 1){
       genMessage(inputMessage.length());
     }
  }
  
  if(ready && millis() - lastTime > cooldown){
    send();
  }
}
