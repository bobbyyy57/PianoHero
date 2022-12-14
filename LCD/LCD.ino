
#include <LiquidCrystal.h>

typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
} task;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

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

int song_choice = 0;


unsigned char j = 0;
unsigned char c = 0;

long score = 0;

enum States_Joy{Start_Joy};
int Joystick(int state) {
    switch(state) {
      case Start_Joy: 

            
            xPosition = analogRead(VRx);
            yPosition = analogRead(VRy);
            SW_state = digitalRead(SW);
            mapX = map(xPosition, 0, 1023, -512, 512);
            mapY = map(yPosition, 0, 1023, -512, 512);


                        Serial.print("X: ");
            Serial.print(mapX);
            Serial.print(" | Y: ");
            Serial.print(mapY);
            Serial.print(" | Button: ");
            Serial.println(SW_state);


            if (mapX <= -450 && mapY < 20) {
                up = 1;
            }
            else if (mapX >= 450 && mapY < 20) {
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

int countdown_finish, end = 1;

enum States_LCD{Start_LCD, Menu_1, Menu_2, Menu_3, Countdown, Score, End};
int LCD(int state) {
    switch(state) {
       case Start_LCD: 
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("   PIANO HERO");
              lcd.setCursor(0, 1);
              lcd.print(" click to start");

               reset = 0;

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
              lcd.print("Bad Romance");

              if (reset == 1) {
                state = Start_LCD;
              }
              else if (down == 1) {
                state = Menu_2;
              }
              else if (butt == 1) {
                song_choice = 1;
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
              lcd.print("Bad Romance");

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

               if (c < 4) {
                 lcd.setCursor(4, 0);
                 lcd.print("3");   
                 ++c; 
                 
               }
               else if (c < 8) {
                 lcd.setCursor(8, 1);
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
              else if (j < 40 && butt == 0) {
                  lcd.setCursor(0,0);
                  lcd.print(" YOU DID GREAT! ");
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

const unsigned short tasksNum = 2;
task tasks[tasksNum];

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(RESET, INPUT);
  pinMode(SW, INPUT_PULLUP); 

  unsigned char n = 0;
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

  Serial.begin(9600); 
  
}


void loop() {

  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
    if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
      tasks[i].state = tasks[i].TickFct(tasks[i].state);
      tasks[i].elapsedTime = millis(); // Last time this task was ran

    }
  }
  delay(100);
  
  // print the number of seconds since reset:
  //lcd.print(millis() / 1000);
}
