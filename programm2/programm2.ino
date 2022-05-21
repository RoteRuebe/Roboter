#include <PS4Controller.h>

void drive ( int left, int right );
void pair();

//Arduino Pins 4, 12, 8, 7 sind Richtung Pins (Logic  Hi oder Lo)
//Arduino Pins 3, 11, 5, 6 sind Speed Pins (PWM 0 ..255)
#define ldp 17  //Left Direction Pin
#define lsp 25
#define rdp 14
#define rsp 27  //Right Speed Pin

//m3 und m4:
//12, 16, 14, 27

//19, 23

int ges;
float turn = 0;

int prevges;
float prevturn = 0;

int doRumble = 1;
void setup() {  
  PS4.begin("30:9C:23:75:1A:0B");
  pinMode(ldp, OUTPUT);
  pinMode(lsp, OUTPUT);
  pinMode(rdp, OUTPUT);
  pinMode(rsp, OUTPUT);
}

int crntMillis, prevMillis = 0 ;
int lm_ist, rm_ist, lm_soll, rm_soll = 0;
//Linker/Rechter Motor

void loop() {
  if( !PS4.isConnected() ) {
    drive(0, 0);
    pair();
  }

  if (PS4.Down()) 
    doRumble = !doRumble;
    
  ges = PS4.R2Value();
  if (!ges)
    ges = -PS4.L2Value();

  crntMillis = millis();
  if (crntMillis - prevMillis > 100) {
    prevMillis = crntMillis;
    if ( abs(ges > 10 && doRumble))
      PS4.setRumble(abs(ges)/3, 0);
    else
      PS4.setRumble(0, 0);
      
    //PS4.setLed( abs(ges), abs(ges), 255);
    PS4.sendToController();
  }

  turn = PS4.LStickX();
  if (turn > 0) {
    turn  = (64 - turn)/64.0;
    lm_soll = ges;
    rm_soll = ges*turn;
  }
  else {
    turn = (turn + 64 )/64.0;
    lm_soll = ges*turn;
    rm_soll = ges;
  }

  if (lm_ist != lm_soll) {
    if (lm_ist > lm_soll)
      lm_ist -= 1;
      
    else if (lm_ist < lm_soll)
      lm_ist += 1;
  }

  if (rm_ist != rm_soll) {
    if (rm_ist > rm_soll)
      rm_ist -= 1;
      
    else if (rm_ist < rm_soll)
      rm_ist += 1;
  }
  
  drive(lm_ist, rm_ist);
}


void drive( int left, int right) {
  digitalWrite( ldp, left > 0 );
  digitalWrite( rdp, right > 0 );

  analogWrite( lsp, abs(left) );
  analogWrite( rsp, abs(right) );
}

void pair () {
  while (1) {
    if( PS4.isConnected() ) {
      return;
    }
       
    delay(50);
  }
}
