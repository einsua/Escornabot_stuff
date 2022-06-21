// Wireless Keyboard for Escornabot using analog Joystick with switch button and HM-10 BLE module

const byte pinX = A2;
const byte pinY = A3;
const byte pinSW = 2;
const unsigned long debouncing=30, longPressed=900, loopInterval=5;

// The 5 buttons commands are FORWARD, BACKWARD, TURN LEFT, TURN RIGHT, GO
const char button_command[5][2]={{'n','N'},{'s','S'},{'w','W'},{'e','E'},{'g','G'}};

unsigned long previousTime=0, currentTime=0;

byte button_state[5][2]={{0,0},{0,0},{0,0},{0,0},{0,0}};
unsigned long pressed_time[5][2]={{0,0},{0,0},{0,0},{0,0},{0,0}};

// Reading values from joystick
byte joystick(int i){

  int valueRX=analogRead(pinX);
  int valueRY=analogRead(pinY);
  int valueSW=digitalRead(pinSW);

  if ((i==0 && valueRY<120) || (i==1 && valueRY>900) ||(i==2 && valueRX<120) ||(i==3 && valueRX>900) ||(i==4 && valueSW==0)){
    return 1;
  }
  return 0;
}

void setup() {
  
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(pinSW, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("Joystick ready");
}

void loop() {
  
  currentTime=millis();

  if((currentTime-previousTime)>loopInterval){

    for (int i=0; i<5; i++){
      
      // FDB - Final Debugging
      if (button_state[i][1]==0 && (currentTime-pressed_time[i][1])>30){
        button_state[i][1]=joystick(i);
      }
      
      // IDB - Initial Debugging
      if (button_state[i][1]==1 && (currentTime-pressed_time[i][0])>30){
        button_state[i][1]=joystick(i);
      }
  
      if (button_state[i][0]==0 && button_state[i][1]==1){
        pressed_time[i][0]=currentTime;
        button_state[i][0]=button_state[i][1];
        Serial.println("PRESSED event");
      }
      if (button_state[i][0]==1 && button_state[i][1]==1){
        pressed_time[i][1]=currentTime;
        if (pressed_time[i][1]-pressed_time[i][0]>longPressed){
          button_state[i][0]=2;
          Serial.println("LONGPRESSED event");
          Serial.println(button_command[i][1]);
        }
      }
      if (button_state[i][0]==1 && button_state[i][1]==0){
        pressed_time[i][1]=currentTime;
        button_state[i][0]=button_state[i][1];
        Serial.println("RELEASED event");
        Serial.println(button_command[i][0]);
      }
      if (button_state[i][0]==2 && button_state[i][1]==0){
        pressed_time[i][1]=currentTime;
        button_state[i][0]=button_state[i][1];
        Serial.println("LONGRELEASED event");
      }
    }
    previousTime=currentTime;
  }
}
