//display 128x64 (128/8 == 16 (Width))
//snake size 4x4 (64/8 == 8  (Height))

#include "SSD1306_minimal.h"
#include <avr/pgmspace.h>

SSD1306_Mini oled;

#define WIDTH 32
#define HEIGHT 16

int wPin = 2;
int aPin = 8;
int sPin = 1;
int dPin = 7;

int lastTime, currentTime;

// there are different wall types
unsigned char wall[5][4]= { 
  0x0, 0x0, 0x0, 0x0,
  0xf, 0xf, 0xf, 0xf,
  0xf, 0x9, 0x9, 0xf,
  0x9, 0x9, 0x9, 0x9,
  0x9, 0x6, 0x6, 0x9,
};

// the ball shape
unsigned char ball[4]= { 
  0x6, 0x9, 0x9, 0x6 
};

unsigned char posX = 7; 
unsigned char posY = 10; 
char velX = 0;
char velY = 0;

// this is the room shape
const uint8_t room[] PROGMEM ={
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};

unsigned char getBlock (unsigned char x, unsigned char y) { //from the game's perspective
  return pgm_read_byte(&(room[(x)*WIDTH + y]));
}

void displayRoom(){
  oled.startScreen();

  uint8_t upperY;
  uint8_t lowerY;
  uint8_t data[4];
  
  for (char y=0;y<HEIGHT; y+=2 ){
    for (char x=0;x<WIDTH; x++){
      // room
      upperY = pgm_read_byte(&room[ y   *WIDTH + x]);
      lowerY = pgm_read_byte(&room[(y+1)*WIDTH + x]);

      for (int i = 0; i<4; i++) {
        data[i] = 0x0;
        
        if (lowerY) {data[i]|= wall[lowerY][i] << 4;}
        if (upperY) {data[i]|= wall[upperY][i] << 0;}
        
        if ((y   == posX) && (x == posY)) {data[i]|= ball[i] << 0;}
        if ((y+1 == posX) && (x == posY)) {data[i]|= ball[i] << 4;}
      }
  
      // send a bunch of data in one xmission
      Wire.beginTransmission(SlaveAddress);
      Wire.send(GOFi2cOLED_Data_Mode);            // data mode
      for (int i = 0; i<4; i++) {Wire.send(data[i]);}
      Wire.endTransmission();
    }
  }  
}

void setup() {
  pinMode(4, OUTPUT);
  pinMode(wPin, INPUT_PULLUP);
  pinMode(aPin, INPUT_PULLUP);
  pinMode(sPin, INPUT_PULLUP);
  pinMode(dPin, INPUT_PULLUP);
  oled.init(0x3c);
}
 
void loop(){
  bool w = !digitalRead(wPin); //Button is pressed, when digitalRead returns 0
  bool a = !digitalRead(aPin);
  bool s = !digitalRead(sPin);
  bool d = !digitalRead(dPin);
  

  if (a) {velX =  1;}
  if (d) {velX = -1;}
  if (!a && !d) {velX = 0;}

  if (w) {velY = -1;}
  if (s) {velY =  1;}
  if (!w && !s) {velY = 0;}
  
  if (getBlock(posX,posY+velY)==0) {posY += velY;}
  if (getBlock(posX+velX,posY)==0) {posX += velX;}

  displayRoom();
  delay(5); 
}
