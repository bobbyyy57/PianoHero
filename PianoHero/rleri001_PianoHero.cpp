#include <FastLED.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>
#include "pitches.h"

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

 void clearLED() {
    black(2);
    black(3);
    black(4);
    black(5);
    black(6);
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

void initial() {
    clearLED();
    fillCol(CRGB::Gray, 1);
    fillCol(CRGB::Gray, 7);
    initial_buttons();
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

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


int GREEN = A0;
int RED = A1;
int YELLOW = A2;
int BLUE = 6;
int PINK = 7;



int VRx = A5;
int VRy = A4;
int SW = 3;

int RESET = 4;

int xPosition = 0;
int yPosition = 0;
int SW_state = 0;
int mapX = 0;
int mapY = 0;

int up = 0;
int down = 0; 
int butt = 0; 
int reset = 0;

int score = 0;
int end = 0;

int song_choice = 0;
int countdown_finish = 0;

int period = 250; 

int data = 0; 
int high_score1 = 0; 
int high_score2 = 0; 
int new_high = 0;

unsigned char j = 0;
unsigned char c = 0;

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

 

enum States_EEPROM{Init_EEPROM};
  int HighScore(int state) {
    switch(state) {
      case Init_EEPROM: 

      Serial.println(song_choice);

        if (song_choice == 1) {
            if (score > high_score1 ) {

                if (reset == 0) {
                    new_high = 1;
        
                    if (score > 510) {
                        EEPROM.write(2, score - 510);
                    }
                    else if (score > 255) {
                        EEPROM.write(1, score - 255);
                    }
                    else {
                        EEPROM.write(0, score);
                    }

                    if (end == 1) {
                        high_score1 = EEPROM.read(0) + EEPROM.read(1) + EEPROM.read(2);
                    }
                }
            
            }
        }
        else if (song_choice == 2) {
            if (score > high_score2) {

                if (reset == 0) {
                    new_high = 1;
        
                    if (score > 510) {
                        EEPROM.write(5, score - 510);
                    }
                    else if (score > 255) {
                        EEPROM.write(4, score - 255);
                    }
                    else {
                        EEPROM.write(3, score);
                    }

                    if (end == 1) {
                        Serial.println("REACHED");
                        high_score2 = EEPROM.read(3) + EEPROM.read(4) + EEPROM.read(5);
                    }
                }
            
            }

        }
          
          state = Init_EEPROM;
    }
    return state; 
  }



enum States_Joy{Start_Joy};
int Joystick(int state) {
    switch(state) {
      case Start_Joy:
          
            xPosition = analogRead(VRx);
            yPosition = analogRead(VRy);
            SW_state = digitalRead(SW);
            mapX = map(xPosition, 0, 1023, -512, 512);
            mapY = map(yPosition, 0, 1023, -512, 512);


            if (mapX <= -400 && mapY < 60) {
                up = 1;
            }
            else if (mapX >= 400 && mapY < 60) {
                down = 1;
            }
            else {
                up = 0;
                down = 0;
            }

            if (SW_state == 0) {
                butt = 1;
            }
            else {
              butt = 0;
            }

            if (digitalRead(RESET) == 1) {
              reset = 1;
            }

            else if (digitalRead(RESET) == 0) {
              reset = 0;
            }

            state = Start_Joy;

    }

    return state; 
  
}

int skip = 0; 
enum States_LCD{Start_LCD, Menu_1, Menu_2, Menu_3, Countdown, Score, End};
int LCD(int state) {
    switch(state) {
       case Start_LCD: 
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("   PIANO HERO");
              lcd.setCursor(0, 1);
              lcd.print(" click to start");

              new_high = 0;
              score = 0;

              if (butt == 1) {
                state = Menu_1;
              }
              else {
                state = Start_LCD;
              }

              break;
        case Menu_1: 
                            
              lcd.clear();
              
              lcd.setCursor(0,0);
              lcd.print(">");        
              
              lcd.setCursor(1, 0);
              lcd.print("Titanic Theme");

              lcd.setCursor(1, 1);
              lcd.print("7 Nation Army");

              if (reset == 1) {
                state = Start_LCD;
              }
              else if (down == 1) {
                state = Menu_2;
              }
              else if (butt == 1) {
                song_choice = 1;
                period = 250;
                state = Countdown;
              }
              else {
                state = Menu_1;
              }

              break;
              
         case Menu_2: 
                            
              lcd.clear();
              
              lcd.setCursor(0,1);
              lcd.print(">");        
              
              lcd.setCursor(1, 0);
              lcd.print("Titanic Theme");

              lcd.setCursor(1, 1);
              lcd.print("7 Nation Army");

              if (reset == 1) {
                state = Start_LCD;
              }
              else if (up == 1) {
                state = Menu_1;
              }
              else if (down == 1) {
                state = Menu_3;
              }
              else if (butt == 1) {
                song_choice = 2;
                period = 50;
                state = Countdown;
              }
              else {
                state = Menu_2;
              }

              break;
         case Menu_3: 
                            
              lcd.clear();
              
              lcd.setCursor(0,0);
              lcd.print(">");        
              
              lcd.setCursor(1, 0);
              lcd.print("Supalonely");

              if (reset == 1) {
                state = Start_LCD;
              }
              else if (up == 1) {
                state = Menu_2;
              }
              else if (butt == 1) {
                song_choice = 3;
                state = Countdown;
              }
              else {
                state = Menu_3;
              }
              break;
         case Countdown: 
               lcd.clear();

               if (reset == 1) {
                  state = Start_LCD;
               }

               lcd.setCursor(0, 1);
               lcd.print("HIGH SCORE: ");

               if (song_choice == 1) lcd.print(high_score1);
               if (song_choice == 2) lcd.print(high_score2);

               if (c < 4) {
                 lcd.setCursor(4, 0);
                 lcd.print("3");   
                 ++c; 
                 
               }
               else if (c < 8) {
                 lcd.setCursor(8, 0);
                 lcd.print("2");
                 ++c;
               }
               else if (c < 12) {
                 lcd.setCursor(12, 0);
                 lcd.print("1");
                 ++c;
               }
              
               if (c < 12) {
                  ++c; 
                  state = Countdown;
               }
               else {
                  c = 0;
                  countdown_finish = 1;
                  state = Score;
               }
              
               break;
         case Score: 

              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("  SCORE: ");
              lcd.print(score);

              if (reset == 1) {
                state = Start_LCD;
              }
              else if (end == 1) {
                state = End;
              }
              else {
                state = Score;
              }

              break;
         case End: 
                lcd.clear();
              if (reset == 1) {
                state = Start_LCD;
              }
              else if (j < 20 && butt == 0) {
                  lcd.setCursor(0,0);
                  lcd.print(" YOU DID GREAT! ");
                  lcd.setCursor(0,1);
                  lcd.print("   SCORE: "); 
                  lcd.print(score);
                  
                  ++j;
                  state = End;
              }
              else if (j < 40 && new_high == 1 && butt == 0) {
                  lcd.setCursor(0,0);
                  lcd.print(" NEW HIGH SCORE");
                  lcd.setCursor(0,1);
                  lcd.print("   SCORE: "); 
                  lcd.print(score);
                  
                  ++j;
                  state = End;
              }
              else {
                  score = 0;
                  j = 0;
                  state = Menu_1;
              }
              break;
              
    }

    return state; 
  
  
}

  unsigned char t = 0;        /// GREEN - G ; RED - R ; YELLOW - Y ; BLUE = B ; PINK = P
  enum States_LED{Init, Song1, Song2};
  int LED_Matrix(int state) {
    switch(state) {
      case Init: 

          initial();
          period = 250;
          end = 0;
          t = 0;
          
            if (countdown_finish == 1 && song_choice == 1) {
              countdown_finish = 0;
              state = Song1;
            }
            else if (countdown_finish == 1 && song_choice == 2) {
              countdown_finish = 0;
              state = Song2;
            }
            else {
              state = Init;
            }
          break;
      case Song1: // My Heart Will Go On - Celine Dion
          
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
          else if (t < 212) { // 6
               black(2);
               red();
          } 
          else if (t < 220) { // 6
               black(3);
               yellow();
          } 
          else {
            black(4);
            end = 1;
          }
          ++t;

          Serial.println(end);

          if (reset == 1) {
            state = Init;
          }
          else if (end == 0) {
             state = Song1;
          }
          else if (end == 1) {
            state = Init;
          }
          break;
      case Song2: // 7 Nation Army
        if (t < 3) { // 3
            blue();
          }
        else if (t < 4) { // 1
            black(5);
            pink();
        }
        else if (t < 5) { // 1
            black(6);
            blue();
        }
        else if (t < 6) { // 1
            black(5);
            yellow();
        }
        else if (t < 10) { // 4
            black(4);
            red();
        }
        else if (t < 14) { // 4
            black(3);
            green();
        } //-------------------------------------- 1
        else if (t < 17) { // 3
            black(2);
            blue();
          }
        else if (t < 18) { // 1
            black(5);
            pink();
        }
        else if (t < 19) { // 1
            black(6);
            blue();
        }
        else if (t < 20) { // 1
            black(5);
            yellow();
        }
        else if (t < 24) { // 4
            black(4);
            red();
        }
        else if (t < 28) { // 4
            black(3);
            green();
        } //-------------------------------------- 2
        else if (t < 31) { // 3
            black(2);
            blue();
          }
        else if (t < 32) { // 1
            black(5);
            pink();
        }
        else if (t < 33) { // 1
            black(6);
            blue();
        }
        else if (t < 34) { // 1
            black(5);
            yellow();
        }
        else if (t < 38) { // 4
            black(4);
            red();
        }
        else if (t < 42) { // 4
            black(3);
            green();
        } //-------------------------------------- 3
        else if (t < 45) { // 3
            black(2);
            blue();
          }
        else if (t < 46) { // 1
            black(5);
            pink();
        }
        else if (t < 47) { // 1
            black(6);
            blue();
        }
        else if (t < 48) { // 1
            black(5);
            yellow();
        }
        else if (t < 52) { // 4
            black(4);
            red();
        } 
        else if (t < 56) { // 4
            black(3);
            green();
        } //-------------------------------------- 4
        else if (t < 59) { // 3
            black(2);
            blue();
          }
        else if (t < 60) { // 1
            black(5);
            pink();
        }
        else if (t < 61) { // 1
            black(6);
            blue();
        }
        else if (t < 62) { // 1
            black(5);
            yellow();
        }
        else if (t < 66) { // 4
            black(4);
            red();
        }
        else if (t < 70) { // 4
            black(3);
            green();
        } //-------------------------------------- 5
        else if (t < 73) { // 3
            black(2);
            blue();
          }
        else if (t < 74) { // 1
            black(5);
            pink();
        }
        else if (t < 75) { // 1
            black(6);
            blue();
        }
        else if (t < 76) { // 1
            black(5);
            yellow();
        }
        else if (t < 80) { // 4
            black(4);
            red();
        }
        else if (t < 84) { // 4
            black(3);
            green();
        } 
        else {
          black(2);
          end = 1;
        }
        

        ++t;

          Serial.println(t);

          if (reset == 1) {
            state = Init;
          }
          else if (end == 0) {
             state = Song2;
          }
          else if (end == 1) {
            state = Init;
          }
        break;
    
    }
    return state;
  }

enum States_B{Init_B, Song1_B, Song2_B} state;
int Buttons(int state) {
    switch(state) {
      case Init_B: 

            if (countdown_finish == 1 && song_choice == 1) {
              state = Song1_B;
            }
            else if (countdown_finish == 1 && song_choice == 2) {
              state = Song2_B;
            }
            else {
              state = Init;
            }

          break;
          
      case Song1_B:

            if (reset == 1) {
                state = Init_B;
            }

          if (t < 3+1) {
               if (digitalRead(YELLOW) == 1) {
                   tone(2, NOTE_E3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 5+1) {
               if (digitalRead(YELLOW) == 1) {
                   tone(2, NOTE_E3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 7+1) {
               if (digitalRead(RED) == 1) {
                   tone(2, NOTE_E3);
                   ++score;
               }
               else {
                   noTone(2);
               }

          }
          else if (t < 9+1) {
               if (digitalRead(YELLOW) == 1) {
                   tone(2, NOTE_E3);
                   ++score;
               }
               else {
                   noTone(2);
               }
            
          }
          else if (t < 11+1) {
               if (digitalRead(GREEN) == 1) {
                   tone(2, NOTE_DS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 13+1) {
               if (digitalRead(RED) == 1) {
                   tone(2, NOTE_E3);
                   ++score;
               }
               else {
                   noTone(2);
               }

          }
          else if (t < 15+1) {
          }
          else if (t < 17+1) {
               if (digitalRead(RED) == 1) {
                   tone(2, NOTE_E3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 19+1) {
               if (digitalRead(GREEN) == 1) {
                   tone(2, NOTE_DS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 23+1) {
               if (digitalRead(RED) == 1) {
                   tone(2, NOTE_E3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 25+1) {
               if (digitalRead(BLUE) == 1) {
                   tone(2, NOTE_FS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 29+1) {
               if (digitalRead(PINK) == 1) {
                   tone(2, NOTE_GS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 32+1) {
               if (digitalRead(BLUE) == 1) {
                   tone(2, NOTE_FS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 34+1) { // 2
               if (digitalRead(YELLOW) == 1) {
                   tone(2, NOTE_E3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 36+1) { // 1 
               if (digitalRead(RED) == 1) {
                   tone(2, NOTE_E3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 38+1) { // 2
               if (digitalRead(YELLOW) == 1) {
                   tone(2, NOTE_E3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 40+1) { // 2
               if (digitalRead(RED) == 1) {
                   tone(2, NOTE_E3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 42+1) { // 2
               if (digitalRead(GREEN) == 1) {
                   tone(2, NOTE_DS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 44+1) { // 2
               if (digitalRead(RED) == 1) {
                   tone(2, NOTE_E3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 46+1) { // 2 ----------- DONE2
          }
          else if (t < 48) {// 2
               if (digitalRead(RED) == 1) {
                   tone(2, NOTE_E3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 50+1) {// 2
               if (digitalRead(GREEN) == 1) {
                   tone(2, NOTE_DS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 52+1) {// 4
               if (digitalRead(RED) == 1) {
                   tone(2, NOTE_E3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 55+1) {// 2
               if (digitalRead(BLUE) == 1) {
                   tone(2, NOTE_FS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 59+1) {// 4
               if (digitalRead(PINK) == 1) {
                   tone(2, NOTE_GS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 62+1) {// 3
               if (digitalRead(BLUE) == 1) {
                   tone(2, NOTE_FS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 64+1) { // 2
                if (digitalRead(YELLOW) == 1) {
                   tone(2, NOTE_E3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 65+1) { // 1 ---------------- DONE3
          }
          else if (t < 66+1) { // 1
               if (digitalRead(RED) == 1) {
                   tone(2, NOTE_E3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 68+1) { // 2
               if (digitalRead(YELLOW) == 1) {
                   tone(2, NOTE_E3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 70+1) { // 2 --------
               if (digitalRead(RED) == 1) {
                   tone(2, NOTE_E3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 72+1) { // 2
               if (digitalRead(GREEN) == 1) {
                   tone(2, NOTE_DS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 74+1 ) { // 2
               if (digitalRead(RED) == 1) {
                   tone(2, NOTE_E3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 76+1) { // 2 ---------------------------DONE4
          }
          else if (t < 78) { // 2
               if (digitalRead(RED) == 1) {
                   tone(2, NOTE_E3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 85+1) { // 8
               if (digitalRead(GREEN) == 1) {
                   tone(2, NOTE_B2);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 89+1) { // 
               if (digitalRead(RED) == 1) {
                   tone(2, NOTE_CS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 93+1) { // 
               if (digitalRead(YELLOW) == 1) {
                   tone(2, NOTE_DS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }   
          else if (t < 101+1) { // 8
               if (digitalRead(BLUE) == 1) {
                   tone(2, NOTE_E3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 105+1) { // 4
               if (digitalRead(PINK) == 1) {
                   tone(2, NOTE_FS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 107+1) { // 2
               if (digitalRead(GREEN) == 1) {
                   tone(2, NOTE_B2);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 111+1) { // 4
               if (digitalRead(PINK) == 1) {
                   tone(2, NOTE_B3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 113+1) { // 2
               if (digitalRead(BLUE) == 1) {
                   tone(2, NOTE_A3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 115+1) { // 2
               if (digitalRead(YELLOW) == 1) {
                   tone(2, NOTE_GS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 119+1) { // 4
               if (digitalRead(RED) == 1) {
                   tone(2, NOTE_FS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 121+1) { // 2
               if (digitalRead(YELLOW) == 1) {
                   tone(2, NOTE_GS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 123+1) { // 2
               if (digitalRead(BLUE) == 1) {
                   tone(2, NOTE_A3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 127+1) { // 4
               if (digitalRead(YELLOW) == 1) {
                   tone(2, NOTE_GS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 129+1) { // 2
               if (digitalRead(BLUE) == 1) {
                   tone(2, NOTE_FS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } //------------------------------------------ DONE5
          else if (t < 131+1) { // 2
               if (digitalRead(YELLOW) == 1) {
                   tone(2, NOTE_E3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 133+1) { // 2
               if (digitalRead(RED) == 1) {
                   tone(2, NOTE_DS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 137+1) { // 4
               if (digitalRead(YELLOW) == 1) {
                   tone(2, NOTE_E3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 139+1) { // 2
               if (digitalRead(RED) == 1) {
                   tone(2, NOTE_DS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 145+1) { // 6
               if (digitalRead(GREEN) == 1) {
                   tone(2, NOTE_CS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 151+1) { // 6
               if (digitalRead(RED) == 1) {
                   tone(2, NOTE_B2);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 159+1) { // 8
               if (digitalRead(BLUE) == 1) {
                   tone(2, NOTE_E3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 166+1) { // 8
               if (digitalRead(PINK) == 1) {
                   tone(2, NOTE_FS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 168+1) { // 2
               if (digitalRead(GREEN) == 1) {
                   tone(2, NOTE_B2);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 171+1) { // 4
               if (digitalRead(PINK) == 1) {
                   tone(2, NOTE_B3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          }
          else if (t < 173+1) { // 2
               if (digitalRead(BLUE) == 1) {
                   tone(2, NOTE_A3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 175+1) { // 2
               if (digitalRead(YELLOW) == 1) {
                   tone(2, NOTE_GS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 179+1) { // 4 
               if (digitalRead(RED) == 1) {
                   tone(2, NOTE_FS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 181+1) { // 2 -------------------DONE6
               if (digitalRead(YELLOW) == 1) {
                   tone(2, NOTE_GS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 183+1) { // 2
               if (digitalRead(BLUE) == 1) {
                   tone(2, NOTE_A3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 187+1) { // 4
               if (digitalRead(YELLOW) == 1) {
                   tone(2, NOTE_GS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 189+1) { // 2
               if (digitalRead(BLUE) == 1) {
                   tone(2, NOTE_FS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 191+1) { // 2
               if (digitalRead(YELLOW) == 1) {
                   tone(2, NOTE_E3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 193+1) { // 2
               if (digitalRead(RED) == 1) {
                   tone(2, NOTE_DS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 197+1) { // 4
               if (digitalRead(YELLOW) == 1) {
                   tone(2, NOTE_E3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 199+1) { // 2
               if (digitalRead(RED) == 1) {
                   tone(2, NOTE_DS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 205+1) { // 6
               if (digitalRead(GREEN) == 1) {
                   tone(2, NOTE_CS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 212+1) { // 6
               if (digitalRead(RED) == 1) {
                   tone(2, NOTE_DS3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else if (t < 217+1) { // 6
               if (digitalRead(YELLOW) == 1) {
                   tone(2, NOTE_E3);
                   ++score;
               }
               else {
                   noTone(2);
               }
          } 
          else {
            noTone(2);
          }
          
          if (reset == 1) {
            state = Init_B;
          }
          else if (end == 0) {
             state = Song1_B;
          }
          else if (end == 1) {
            state = Init_B;
          }
          break;
      case Song2_B: 
        if (t < 3+1) { // 3
            if (digitalRead(BLUE) == 1) {
                tone(2, NOTE_E1);
                ++score;
            }
            else {
                noTone(2);
            }
        }
        else if (t < 4+1) { // 1
            if (digitalRead(PINK) == 1) {
                tone(2, NOTE_G1);
                ++score;
            }
            else {
                noTone(2);
            }
        }
        else if (t < 5+1) { // 1
            if (digitalRead(BLUE) == 1) {
                tone(2, NOTE_E1);
                ++score;
            }
            else {
                noTone(2);
            }
        }
        else if (t < 6+1) { // 1
            if (digitalRead(YELLOW) == 1) {
                tone(2, NOTE_D1);
                ++score;
            }
            else {
                noTone(2);
            }
        }
        else if (t < 10+1) { // 4
            if (digitalRead(RED) == 1) {
                tone(2, NOTE_C1);
                ++score;
            }
            else {
                noTone(2);
            }
        }
        else if (t < 14+1) { // 4
            if (digitalRead(GREEN) == 1) {
                tone(2, NOTE_B0);
                ++score;
            }
            else {
                noTone(2);
            }
        } //-------------------------------------- 1
        else if (t < 17+1) { // 3
            if (digitalRead(BLUE) == 1) {
                tone(2, NOTE_E1);
                ++score;
            }
            else {
                noTone(2);
            }
          }
        else if (t < 18+1) { // 1
            if (digitalRead(PINK) == 1) {
                tone(2, NOTE_G1);
                ++score;
            }
            else {
                noTone(2);
            }
        }
        else if (t < 19+1) { // 1
            if (digitalRead(BLUE) == 1) {
                tone(2, NOTE_E1);
                ++score;
            }
            else {
                noTone(2);
            }
        }
        else if (t < 20+1) { // 1
            if (digitalRead(YELLOW) == 1) {
                tone(2, NOTE_D1);
                ++score;
            }
            else {
                noTone(2);
            }
        }
        else if (t < 24+1) { // 4
            if (digitalRead(RED) == 1) {
                tone(2, NOTE_C1);
                ++score;
            }
            else {
                noTone(2);
            }
        }
        else if (t < 28+1) { // 4
            if (digitalRead(GREEN) == 1) {
                tone(2, NOTE_B0);
                ++score;
            }
            else {
                noTone(2);
            }
        } //-------------------------------------- 2
        else if (t < 31+1) { // 3
            if (digitalRead(BLUE) == 1) {
                tone(2, NOTE_E1);
                ++score;
            }
            else {
                noTone(2);
            }
          }
        else if (t < 32+1) { // 1
            if (digitalRead(PINK) == 1) {
                tone(2, NOTE_G1);
                ++score;
            }
            else {
                noTone(2);
            }
        }
        else if (t < 33+1) { // 1
            if (digitalRead(BLUE) == 1) {
                tone(2, NOTE_E1);
                ++score;
            }
            else {
                noTone(2);
            }
        }
        else if (t < 34+1) { // 1
            if (digitalRead(YELLOW) == 1) {
                tone(2, NOTE_D1);
                ++score;
            }
            else {
                noTone(2);
            }
        }
        else if (t < 38+1) { // 4
            if (digitalRead(RED) == 1) {
                tone(2, NOTE_C1);
                ++score;
            }
            else {
                noTone(2);
            }
        }
        else if (t < 42+1) { // 4
            if (digitalRead(GREEN) == 1) {
                tone(2, NOTE_B0);
                ++score;
            }
            else {
                noTone(2);
            }
        } //-------------------------------------- 3
        else if (t < 45+1) { // 3
            if (digitalRead(BLUE) == 1) {
                tone(2, NOTE_E1);
                ++score;
            }
            else {
                noTone(2);
            }
          }
        else if (t < 46+1) { // 1
            if (digitalRead(PINK) == 1) {
                tone(2, NOTE_G1);
                ++score;
            }
            else {
                noTone(2);
            }
        }
        else if (t < 47+1) { // 1
           if (digitalRead(BLUE) == 1) {
                tone(2, NOTE_E1);
                ++score;
            }
            else {
                noTone(2);
            }
        }
        else if (t < 48+1) { // 1
            if (digitalRead(YELLOW) == 1) {
                tone(2, NOTE_D1);
                ++score;
            }
            else {
                noTone(2);
            }
        }
        else if (t < 52+1) { // 4
            if (digitalRead(RED) == 1) {
                tone(2, NOTE_C1);
                ++score;
            }
            else {
                noTone(2);
            }
        } 
        else if (t < 56+1) { // 4
            if (digitalRead(GREEN) == 1) {
                tone(2, NOTE_B0);
                ++score;
            }
            else {
                noTone(2);
            }
        } //-------------------------------------- 4
        else if (t < 59+1) { // 3
            if (digitalRead(BLUE) == 1) {
                tone(2, NOTE_E1);
                ++score;
            }
            else {
                noTone(2);
            }
          }
        else if (t < 60+1) { // 1
            if (digitalRead(PINK) == 1) {
                tone(2, NOTE_G1);
                ++score;
            }
            else {
                noTone(2);
            }
        }
        else if (t < 61+1) { // 1
            if (digitalRead(BLUE) == 1) {
                tone(2, NOTE_E1);
                ++score;
            }
            else {
                noTone(2);
            }
        }
        else if (t < 62+1) { // 1
            if (digitalRead(YELLOW) == 1) {
                tone(2, NOTE_D1);
                ++score;
            }
            else {
                noTone(2);
            }
        }
        else if (t < 66+1) { // 4
            if (digitalRead(RED) == 1) {
                tone(2, NOTE_C1);
                ++score;
            }
            else {
                noTone(2);
            }
        }
        else if (t < 70+1) { // 4
            if (digitalRead(GREEN) == 1) {
                tone(2, NOTE_B0);
                ++score;
            }
            else {
                noTone(2);
            }
        } //-------------------------------------- 5
        else if (t < 73+1) { // 3
            if (digitalRead(BLUE) == 1) {
                tone(2, NOTE_E1);
                ++score;
            }
            else {
                noTone(2);
            }
          }
        else if (t < 74+1) { // 1
            if (digitalRead(PINK) == 1) {
                tone(2, NOTE_G1);
                ++score;
            }
            else {
                noTone(2);
            }
        }
        else if (t < 75+1) { // 1
            if (digitalRead(BLUE) == 1) {
                tone(2, NOTE_E1);
                ++score;
            }
            else {
                noTone(2);
            }
        }
        else if (t < 76+1) { // 1
            if (digitalRead(YELLOW) == 1) {
                tone(2, NOTE_D1);
                ++score;
            }
            else {
                noTone(2);
            }
        }
        else if (t < 80+1) { // 4
            if (digitalRead(RED) == 1) {
                tone(2, NOTE_C1);
                ++score;
            }
            else {
                noTone(2);
            }
        }
        else if (t < 84+1) { // 4
            if (digitalRead(GREEN) == 1) {
                tone(2, NOTE_B0);
                ++score;
            }
            else {
                noTone(2);
            }
        } 
        else {
            noTone(2);
        }

        if (reset == 1) {
            state = Init_B;
          }
          else if (end == 0) {
             state = Song2_B;
          }
          else if (end == 1) {
            state = Init_B;
          }
          break;
    
    } 
    return state;
}

const unsigned short tasksNum = 5;
task tasks[tasksNum];

void setup() {
      FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);
      FastLED.setBrightness(10);

      lcd.begin(16, 2);
  
      pinMode(VRx, INPUT);
      pinMode(VRy, INPUT);
      pinMode(RESET, INPUT);
      pinMode(SW, INPUT_PULLUP); 

      high_score1 = EEPROM.read(0) + EEPROM.read(1) + EEPROM.read(2);
      high_score2 = EEPROM.read(3) + EEPROM.read(4) + EEPROM.read(5);
      
      Serial.begin(9600);
      
      unsigned char n = 0;
      tasks[n].state = Init;
      tasks[n].period = period;
      tasks[n].elapsedTime = 0;
      tasks[n].TickFct = &LED_Matrix;
      ++n;
      tasks[n].state = Init_B;
      tasks[n].period = 50;
      tasks[n].elapsedTime = 0;
      tasks[n].TickFct = &Buttons;
      ++n;
      tasks[n].state = Start_LCD;
      tasks[n].period = 250;
      tasks[n].elapsedTime = 0;
      tasks[n].TickFct = &LCD;
      ++n;
      tasks[n].state = Start_Joy;
      tasks[n].period = 250;
      tasks[n].elapsedTime = 0;
      tasks[n].TickFct = &Joystick;
      ++n;
      tasks[n].state = Init_EEPROM;
      tasks[n].period = 250;
      tasks[n].elapsedTime = 0;
      tasks[n].TickFct = &HighScore;
}

void loop() {

//        EEPROM.write(0, 0);
//        EEPROM.write(1, 0);
//        EEPROM.write(2, 0);
//        EEPROM.write(3, 0);
//        EEPROM.write(4, 0);
//        EEPROM.write(5, 0);

//          Serial.print("EEPROM0: ");
//          Serial.print(EEPROM.read(0));
//          Serial.print(" | EEPROM1: ");
//          Serial.print(EEPROM.read(1));
//          Serial.print(" | EEPROM2: ");
//          Serial.print(EEPROM.read(2));
//
//          Serial.print(" | high_score1: ");
//          Serial.print(high_score1);
//          Serial.print(" | score1: ");
//          Serial.println(score);
//
//
///////
//
         Serial.print("EEPROM3: ");
         Serial.print(EEPROM.read(3));
         Serial.print(" | EEPROM4: ");
         Serial.print(EEPROM.read(4));
         Serial.print(" | EEPROM5: ");
         Serial.print(EEPROM.read(5));
//
         Serial.print(" | high_score2: ");
         Serial.print(high_score2);
         Serial.print(" | score2: ");
         Serial.print(score);
         Serial.print(" | end: ");
         Serial.println(end);

         Serial.println("");
  
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
