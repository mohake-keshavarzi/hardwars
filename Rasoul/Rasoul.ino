/*********************************************************************
  This is an example for our Monochrome OLEDs based on SH110X drivers

  This example is for a 128x64 size display using I2C to communicate
  3 pins are required to interface (2 I2C and one reset)

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada  for Adafruit Industries.
  BSD license, check license.txt for more information
  All text above, and the splash screen must be included in any redistribution

  i2c SH1106 modified by Rupert Hirst  12/09/21
*********************************************************************/



#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
                         // e.g. the one with GM12864-77 written on it
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define VRY 26
#define VRX 25
#define PUSH 27


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

bool GAME_ON = true;
uint32_t SCORE = 0;

uint8_t FRAME_ID = 0;
uint8_t Y_OFFSET = 0;
bool JUMP = false;
bool JUMP_MAX = false;

uint8_t CLOUD_1_X_OFFSET = 0;
uint8_t CLOUD_2_X_OFFSET = 0;
uint8_t OBSTACLE_X_OFFSET = 0;

void setup()   {
  pinMode(PUSH, INPUT_PULLUP);
  Serial.begin(115200);

  delay(250); // wait for the OLED to power up
  display.begin(i2c_Address, true); 
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();

  // draw a single pixel
  display.drawPixel(100, 10, SH110X_WHITE);
  // Show the display buffer on the hardware.
  // NOTE: You _must_ call display after making any drawing commands
  // to make them visible on the display hardware!
  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();
  display.setTextSize(3);                 // Size can be 1, 2, 3, ...
  display.setTextColor(SH110X_WHITE);

}


void loop() {

  if (GAME_ON) {
    if(!digitalRead(PUSH) && Y_OFFSET == 0){
      JUMP=true;
    }

    if (JUMP) {
      Y_OFFSET += 5;
      FRAME_ID = 0;

      if (Y_OFFSET > 20) {
        JUMP = false;
        JUMP_MAX = true;
      }
    } else {
      if (FRAME_ID == 1) {
        FRAME_ID = 2;
      } else if (FRAME_ID == 2) {
        FRAME_ID = 1;
      } else {
        FRAME_ID = 1;
      }
    }

    if (JUMP_MAX) {
      Y_OFFSET -= 5;

      if (Y_OFFSET == 0) {
        JUMP_MAX = false;
      }
    }

    CLOUD_1_X_OFFSET += 1;
    if (CLOUD_1_X_OFFSET > 170) {
      CLOUD_1_X_OFFSET = 0;
    }

    CLOUD_2_X_OFFSET += 1;
    if (CLOUD_2_X_OFFSET > 210) {
      CLOUD_2_X_OFFSET = 0;
    }

    OBSTACLE_X_OFFSET += 8;
    if (OBSTACLE_X_OFFSET > 160) {
      OBSTACLE_X_OFFSET = 0;
    }

    if (145 - OBSTACLE_X_OFFSET <= 38 && 145 - OBSTACLE_X_OFFSET >= 30) {
      if (Y_OFFSET <= 10) {
        GAME_ON = false;
      }
    }

    SCORE++;

  } else {
    if(!digitalRead(PUSH)){
      GAME_ON = true;
      SCORE = 0;

      FRAME_ID = 0;
      Y_OFFSET = 0;
      JUMP = false;
      JUMP_MAX = false;

      CLOUD_1_X_OFFSET = 0;
      CLOUD_2_X_OFFSET = 0;
      OBSTACLE_X_OFFSET = 0;
    }
  }

    display.clearDisplay();
    display.setCursor(10, 10);
    //int y=analogRead(VRY);
    //int x=analogRead(VRX);

    display.fillRect(0, 50, 128, 22, 1);
    display.setCursor(3, 55);
    display.setTextSize(1);
    display.setTextColor(0);
    
    if (GAME_ON) {
      display.printf("Score: %d\n", SCORE);
    } else {
      display.printf("YOU LOST! Score: %d\n", SCORE);
    }

    drawDinosaur(Y_OFFSET, FRAME_ID);

    drawCloud1(CLOUD_1_X_OFFSET, 0);
    drawCloud2(CLOUD_2_X_OFFSET, 0);

    drawObstacle(OBSTACLE_X_OFFSET);


  
  
  display.display();

  delay(100);

}

void drawDinosaur(const int8_t y_offset, const uint8_t frame_id) {

  // frame_id:
  //     0: idle
  //     1: left leg up
  //     2: right leg up

  uint8_t id = frame_id % 3;

  // feet
  if (id == 0) {
    display.fillRect(35, 47-y_offset, 4, 3, 1);
    display.fillRect(29, 47-y_offset, 4, 3, 1);
    display.fillRect(35, 41-y_offset, 2, 6, 1);
    display.fillRect(29, 41-y_offset, 2, 6, 1);
  } else if (id == 1) {
    display.fillRect(35, 47-y_offset, 4, 3, 1);
    display.fillRect(29, 47-y_offset-3, 4, 3, 1);
    display.fillRect(35, 41-y_offset, 2, 6, 1);
    display.fillRect(29, 41-y_offset-3, 2, 6, 1);
  } else {
    display.fillRect(35, 47-y_offset-3, 4, 3, 1);
    display.fillRect(29, 47-y_offset, 4, 3, 1);
    display.fillRect(35, 41-y_offset-3, 2, 6, 1);
    display.fillRect(29, 41-y_offset, 2, 6, 1);
  }

  // body
  display.fillRect(29, 29-y_offset, 8, 12, 1);
  
  // head
  display.fillRect(32, 26-y_offset, 5, 3, 1);
  display.fillCircle(34, 32-y_offset, 2, 0);

  // tail
  display.fillRect(21, 35-y_offset, 8, 6, 1);
  display.fillRect(21, 33-y_offset, 2, 2, 1);

  // beak
  display.fillTriangle(37, 30-y_offset, 37, 34-y_offset, 40, 32-y_offset, 1);
  
}

void drawCloud1(uint8_t offset_x, uint8_t offset_y) {
  display.fillCircle(158-offset_x, 10+offset_y, 4, 1);
  display.fillCircle(162-offset_x, 7+offset_y, 4, 1);
  display.fillCircle(166-offset_x, 10+offset_y, 5, 1);
  display.fillCircle(168-offset_x, 7+offset_y, 5, 1);
  display.fillCircle(168-offset_x, 11+offset_y, 3, 1);
}

void drawCloud2(uint8_t offset_x, uint8_t offset_y) {
  display.fillCircle(198-offset_x, 10+offset_y, 4, 1);
  display.fillCircle(202-offset_x, 7+offset_y, 4, 1);
  display.fillCircle(206-offset_x, 10+offset_y, 5, 1);
  display.fillCircle(208-offset_x, 7+offset_y, 5, 1);
  display.fillCircle(208-offset_x, 11+offset_y, 3, 1);
}

void drawObstacle(uint8_t offset_x) {
  display.fillRect(145-offset_x, 34, 6, 16, 1);
  display.drawLine(142-offset_x, 40, 145-offset_x, 40, 1);
  display.drawLine(140-offset_x, 44, 145-offset_x, 44, 1);
  display.drawLine(151-offset_x, 37, 154-offset_x, 37, 1);
  display.drawLine(151-offset_x, 40, 152-offset_x, 40, 1);
  display.drawLine(146-offset_x, 36, 146-offset_x, 48, 0);
  display.drawLine(149-offset_x, 36, 149-offset_x, 48, 0);
}
