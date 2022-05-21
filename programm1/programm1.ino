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
  PS4.begin("4c:eb:bd:a8:93:16");
  //80:ea:23:3c:00:9f
  pinMode(ldp, OUTPUT);
  pinMode(lsp, OUTPUT);
  pinMode(rdp, OUTPUT);
  pinMode(rsp, OUTPUT);
}

int crntMillis, prevMillis = 0 ;
void loop() {
  if( !PS4.isConnected() ) {
    drive(0, 0);
    pair();
  }

  if ( PS4.Triangle() )
    disco_mode();
    
  ges = PS4.R2Value();
  if (!ges)
    ges = -PS4.L2Value();

  if (PS4.Options()) 
    doRumble = !doRumble;
  
  crntMillis = millis();
  if (crntMillis - prevMillis > 20) {
    prevMillis = crntMillis;
    if ( abs(ges > 10 && doRumble))
      PS4.setRumble(abs(ges)/3, 0);
    else
      PS4.setRumble(0, 0);
      
    PS4.setLed( abs(ges), abs(ges), 255);
    PS4.sendToController();
  }

  turn = PS4.LStickX();
  if (turn > 0) {
    turn  = (64 - turn)/64.0;
    if ( abs(prevges - ges) > 128 || abs(prevturn - turn) > 0.5) {
      drive(0, 0);
      delay(100);
    }
    drive( ges, ges*turn );
  }
  
  else {
    turn = (turn + 64 )/64.0;
    if ( abs(prevges - ges) > 128 || abs(prevturn - turn) > 0.5) {
      drive(0, 0);
      delay(100);
    }
    drive( ges*turn, ges );
  }

  if ( abs(prevges - ges) > 128 || abs(prevturn - turn) > 64) {
    drive(0, 0);
    delay(100);
  }


  prevturn = turn;
  prevges = ges;
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

//gebrauch auf eigene Gefahr!
void disco_mode() {
  int red = 0;
  int green = 0;
  int blue = 255;
  PS4.setRumble(255, 255);
  for (int i = 0; i <= 255*3; i++) {
    PS4.setLed(red, green, blue);
  
    if (red > 0 && blue == 0) {
      red--;
      green++;
    }
    if (green > 0 && red == 0) {
      green--;
      blue++;
    }
    if (blue > 0 && green == 0) {
      red++;
      blue--;
    }
    PS4.sendToController();
    drive( 255, -255 );
    delay(3);
  }
  PS4.setLed(0, 0, 255);
  PS4.setRumble(0, 0);
  PS4.sendToController();
  drive(0, 0);
  return;
}
