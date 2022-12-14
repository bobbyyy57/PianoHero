#include <FastLED.h>

#define NUM_LEDS 64
#define DATA_PIN 5
#define CLOCK_PIN 13

CRGB leds[NUM_LEDS];

typedef struct task
{
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
} task;

 void fillCol(CRGB color, int c) {
  for (int i = 0; i < 7; ++i) {
    leds[(8*i)+c] = color;
  }
 }

 void initial_buttons(){
    leds[(8*7)+1] = CRGB::White;
    leds[(8*7)+2] = CRGB::Green;
    leds[(8*7)+3] = CRGB::Red;
    leds[(8*7)+4] = CRGB::Yellow;
    leds[(8*7)+5] = CRGB::Blue;
    leds[(8*7)+6] = CRGB::Purple;
    leds[(8*7)+7] = CRGB::White;
 }

  void clear_buttons(){
    leds[(8*6)+2] = CRGB::Black;
    leds[(8*6)+3] = CRGB::Black;
    leds[(8*6)+4] = CRGB::Black;
    leds[(8*6)+5] = CRGB::Black;
    leds[(8*6)+6] = CRGB::Black;
 }

 void green() {
    fillCol(CRGB::Green, 2);
    FastLED.show();
 }
 void red() {
    fillCol(CRGB::Red, 3);
    FastLED.show();
 }
 void yellow() {
    fillCol(CRGB::Yellow, 4);
    FastLED.show();
 }
 void blue() {
    fillCol(CRGB::Blue, 5);
    FastLED.show();
 }

 void pink() {
    fillCol(CRGB::Purple, 6);
    FastLED.show();
 }

  void black(int i) {
    fillCol(CRGB::Black, i);
    FastLED.show();
 }

/*

              leds[(8*r)+2] = CRGB::Green;
              leds[(8*r)+3] = CRGB::Red;
              leds[(8*r)+4] = CRGB::Yellow;
              leds[(8*r)+5] = CRGB::Blue;
              leds[(8*r)+6] = CRGB::Purple;
        
              // Show the leds (only one of which is set to white, from above)
              FastLED.show();
        
              // Turn our current led back to black for the next loop around
              leds[(8*r)+3] = CRGB::Black;
              leds[(8*r)+2] = CRGB::Black;
              leds[(8*r)+4] = CRGB::Black;
              leds[(8*r)+5] = CRGB::Black;
              leds[(8*r)+6] = CRGB::Black;

*/

 // int arr = [3, 0, 3, 3, ]

  unsigned char t = 0; 
  enum States_R{Init, Song1};
  int LED_Matrix(int state) {
    switch(state) {
      case Init: 
          state = Song1;
          // fillCol(CRGB::Gray, 0);
          fillCol(CRGB::Gray, 1);
          fillCol(CRGB::Gray, 7);
          break;
      case Song1: // My Heart Will Go On - Celine Dion
          initial_buttons();

          /// 2:28 VVV
          if (t < 3) {
               yellow();
          }
          else if (t < 4) {
               black(4);
          }
          else if (t < 5) {
               yellow();
          }
          else if (t < 7) {
               black(4);
               red();
          }
          else if (t < 9) {
               black(3);
               yellow();
          }
          else if (t < 11) {
               black(4);
               green();
          }
          else if (t < 13) {
               black(2);
               red();
          }
          else if (t < 15) {
               black(3);
          }
          else if (t < 17) {
               red();
          }
          else if (t < 19) {
               black(3);
               green();
          }
          else if (t < 23) {
               black(2);
               red();
          }
          else if (t < 25) {
               black(3);
               blue();
          }
          else if (t < 29) {
               black(5);
               pink();
          }
          else if (t < 32) {
               black(6);
               blue();
          }
          //// 2:37 vvvv
          else if (t < 34) { // 2
               black(5);
               yellow();
          }
          else if (t < 35) { // 1
               black(4);
          }
          else if (t < 36) { // 1
               red(); 
          }
          else if (t < 38) { // 2
               black(3);
               yellow();
          }
          else if (t < 40) { // 2
               black(4);
               red();
          }
          else if (t < 42) { // 2
               black(3);
               green();
          }
          else if (t < 44) { // 2
               black(2);
               red();
          }
          //// 2:41 + vvvv
          else if (t < 46) { // 2 
               black(3);
          }
          else if (t < 48) {// 2
               red();
          }
          else if (t < 50) {// 2
               black(3);
               green();
          }
          else if (t < 52) {// 4
               black(2);
               red();
          }
          else if (t < 55) {// 2
               black(3);
               blue();
          }
          else if (t < 59) {// 4
               black(5);
               pink();
          }
          else if (t < 62) {// 3
               black(6);
               blue();
          }
          ///// 2:47 vvv
          else if (t < 64) { // 2
               black(5);
               yellow();
          }
          else if (t < 65) { // 1
               black(4);
          }
          else if (t < 66) { // 1
               red();
          }
          else if (t < 68) { // 2
               black(3);
               yellow();
          }
          else if (t < 70) { // 2
               black(4);
               red();
          }
          else if (t < 72) { // 2
               black(3);
               green();
          }
          else if (t < 74 ) { // 2
               black(2);
               red();
          }
          else if (t < 76) { // 2 
               black(3);
          }
          else if (t < 78) { // 2
               red();
          }
          //// 2:51 VVVV
          else if (t < 85) { // 8
               black(3);
               green();
          } 
          else if (t < 89) { // 
               black(2);
               red();
          } 
          else if (t < 93) { // 
               black(3);
               yellow();
          }   
          else if (t < 101) { // 8
               black(4);
               blue();
          } 
          else if (t < 105) { // 4
               black(5);
               pink();
          } 
          else if (t < 107) { // 2
               black(6);
               green();
          } 
          else if (t < 111) { // 4
               black(2);
               pink();
          }
          else if (t < 113) { // 2
               black(6);
               blue();
          } 
          else if (t < 115) { // 2
               black(5);
               yellow();
          } 
          else if (t < 119) { // 4
               black(4);
               red();
          } 
          else if (t < 121) { // 2
               black(3);
               yellow();
          } 
          else if (t < 123) { // 2
               black(4);
               blue();
          } 
          else if (t < 127) { // 4
               black(5);
               yellow();
          } 
          else if (t < 129) { // 2
               black(4);
               blue();
          } 
          else if (t < 131) { // 2
               black(5);
               yellow();
          } 
          else if (t < 133) { // 2
               black(4);
               red();
          } 
          else if (t < 137) { // 4
               black(3);
               yellow();
          } 
          else if (t < 139) { // 2
               black(4);
               red();
          } 
          else if (t < 145) { // 6
               black(3);
               green();
          } 
          else if (t < 151) { // 6
               black(2);
               red();
          } 
          /// 3:14 vv
          else if (t < 159) { // 8
               black(3);
               blue();
          } 
          else if (t < 166) { // 8
               black(5);
               pink();
          } 
          else if (t < 168) { // 2
               black(6);
               green();
          } 
          else if (t < 171) { // 4
               black(2);
               pink();
          }
          else if (t < 173) { // 2
               black(6);
               blue();
          } 
          else if (t < 175) { // 2
               black(5);
               yellow();
          } 
          else if (t < 179) { // 4 888888
               black(4);
               red();
          } 
          else if (t < 181) { // 2
               black(3);
               yellow();
          } 
          else if (t < 183) { // 2
               black(4);
               blue();
          } 
          else if (t < 187) { // 4
               black(5);
               yellow();
          } 
          else if (t < 189) { // 2
               black(4);
               blue();
          } 
          else if (t < 191) { // 2
               black(5);
               yellow();
          } 
          else if (t < 193) { // 2
               black(4);
               red();
          } 
          else if (t < 197) { // 4
               black(3);
               yellow();
          } 
          else if (t < 199) { // 2
               black(4);
               red();
          } 
          else if (t < 205) { // 6
               black(3);
               green();
          } 
          else if (t < 211) { // 6
               black(2);
               red();
          } 
          else if (t < 217) { // 6
               black(3);
               yellow();
          } 
          else {
            black(4);
          }
          
          

        Serial.println(t);

              ++t;
          

          state = Song1;
    }
    return state;
  }

const unsigned short tasksNum = 1;
task tasks[tasksNum];

void setup() {
      FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);
      FastLED.setBrightness(10);
      Serial.begin(9600);
      
      unsigned char n = 0;
      tasks[n].state = Init;
      tasks[n].period = 250;
      tasks[n].elapsedTime = 0;
      tasks[n].TickFct = &LED_Matrix;
//      ++n;
//      tasks[n].state = Init_G;
//      tasks[n].period = 500;
//      tasks[n].elapsedTime = 0;
//      tasks[n].TickFct = &Green_Button;
}

void loop() {
    unsigned char i;
    for (i = 0; i < tasksNum; ++i)
    {
      if ((millis() - tasks[i].elapsedTime) >= tasks[i].period)
      {
        tasks[i].state = tasks[i].TickFct(tasks[i].state);
        tasks[i].elapsedTime = millis(); // Last time this task was ran
      }
    }
    delay(100);
}
