#define WEST_BUTTON_PIN 2
#define SOUTH_BUTTON_PIN 3
#define PEOPLE_BUTTON_PIN 4

#define LED_W_R 5
#define LED_W_Y 6
#define LED_W_G 7

#define LED_S_R 8
#define LED_S_Y 9
#define LED_S_G 10

#define LED_P_G 11
#define LED_P_R 12

struct State {
unsigned long ST_Out; // 6-bit pattern to street output
unsigned long Time; // delay in ms units
unsigned long Next[8];}; // next state for inputs 0,1,2,3
typedef const struct State SType;

int input,input1, input2, input3;
unsigned long S=0; // index to the current state
//WR W
SType FSM[19]={                                 // W S คน
{B10000101,2500,{0,5,0,5,1,5,1,5}},             // goS         0   
{B10001001,500,{2,2,2,2,2,2,2,2}},             // waitS_2W     1
{B00110001,2500,{2,12,3,12,2,12,3,3}},          // goW         2
{B01010001,500,{0,0,0,0,0,0,0,0}},             // waitW_2S     3
{B10010010,2500,{4,4,6,6,13,13,13,13}},         // goP         4
{B10001001,500,{4,4,4,4,4,4,4,4}},             // waitS_2P     5
{B10010010,500,{7,7,7,7,7,7,7,7}},             // waitP_S1 ติด  6
{B10010000,500,{8,8,8,8,8,8,8,8}},             // waitP_S2 ดับ  7
{B10010010,500,{9,9,9,9,9,9,9,9}},             // waitP_S3 ติด  8
{B10010000,500,{10,10,10,10,10,10,10,10}},     // waitP_S4 ดับ  9
{B10010010,500,{11,11,11,11,11,11,11,11}},     // waitP_S5 ติด  10
{B10010000,500,{0,0,0,0,0,0,0,0}},             // waitP_S6 ดับ  11
{B01010001,500,{4,4,4,4,4,4,4,4}},             // waitW_2P     12
{B10010010,500,{14,14,14,14,14,14,14,14}},     // waitP_W1 ติด  13
{B10010000,500,{15,15,15,15,15,15,15,15}},     // waitP_W2 ดับ  14
{B10010010,500,{16,16,16,16,16,16,16,16}},     // waitP_W3 ติด  15
{B10010000,500,{17,17,17,17,17,17,17,17}},     // waitP_W4 ดับ  16
{B10010010,500,{18,18,18,18,18,18,18,18}},     // waitP_W5 ติด  17
{B10010000,500,{2,2,2,2,2,2,2,2}},             // waitP_W6 ดับ  18
};

void setup() {
  Serial.begin(9600);
  pinMode(LED_W_R, OUTPUT);
  pinMode(LED_W_Y, OUTPUT);
  pinMode(LED_W_G, OUTPUT);
  pinMode(WEST_BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_S_R, OUTPUT);
  pinMode(LED_S_Y, OUTPUT);
  pinMode(LED_S_G, OUTPUT);
  pinMode(SOUTH_BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_P_G, OUTPUT);
  pinMode(LED_P_R, OUTPUT);
  pinMode(PEOPLE_BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
digitalWrite(LED_W_R, !(FSM[S].ST_Out & B10000000));
digitalWrite(LED_W_Y, !(FSM[S].ST_Out & B01000000));
digitalWrite(LED_W_G, !(FSM[S].ST_Out & B00100000));
digitalWrite(LED_S_R, !(FSM[S].ST_Out & B00010000));
digitalWrite(LED_S_Y, !(FSM[S].ST_Out & B00001000));
digitalWrite(LED_S_G, !(FSM[S].ST_Out & B00000100));
digitalWrite(LED_P_G, !(FSM[S].ST_Out & B00000010));
digitalWrite(LED_P_R, !(FSM[S].ST_Out & B00000001));
delay(FSM[S].Time);

input1 = !digitalRead(WEST_BUTTON_PIN);   // 100
input2 = !digitalRead(SOUTH_BUTTON_PIN);  // 010
input3 = !digitalRead(PEOPLE_BUTTON_PIN); // 001
input = ((input1*4)+(input2*2)+(input3));
S = FSM[S].Next[input];
Serial.println(input);
}
