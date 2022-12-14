 #include <EEPROM.h>

 typedef struct task
{
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
} task;


int high(int pos) {

  int count = 0;
  
  for (int i = 0; i < pos; ++i) {
     count += EEPROM.read(i);
  }

  return count;
}
 
int score1 = 0;
int score2 = 0; 
int high_score1 = 0; 
int high_score2 = 0; 
int choice = 0; 
enum States_EEPROM{Init_EEPROM};
  int HighScore(int state) {
    switch(state) {
      case Init_EEPROM: 

//        EEPROM.write(0, 0);
//        EEPROM.write(1, 0);
//        EEPROM.write(2, 0);
//        EEPROM.write(3, 0);
//        EEPROM.write(4, 0);
//        EEPROM.write(5, 0);

     
       
       if(digitalRead(A2)==1) {
          choice = 1;
       }

       if(digitalRead(6)==1) {
          choice = 2;
       }

      
//      
        if (choice == 1) {
           if(digitalRead(A0)==1) score1 = score1 + 1;
              if (score1 > high_score1) {



                    if (score1 > 510) {
                      EEPROM.write(2, score1 - 510);
                    }
                    else if (score1 > 255) {
                      EEPROM.write(1, score1 - 255);
                    }
                    else {
                      EEPROM.write(0, score1);
                    }
                    
                    high_score1 = EEPROM.read(0) + EEPROM.read(1) + EEPROM.read(2);
      
                }
        }
        else if (choice == 2) {
          if(digitalRead(A1)==1) score2 = score2 + 1;
              if (score2 > high_score2) {
      

      
                    if (score2 > 510) {
                      EEPROM.write(5, score2 - 510);
                    }
                    else if (score2 > 255) {
                      EEPROM.write(4, score2 - 255);
                    }
                    else {
                      EEPROM.write(3, score2);
                    }
                    
                    high_score2 = EEPROM.read(3) + EEPROM.read(4) + EEPROM.read(5);
      
                }
        }

          Serial.print("EEPROM0: ");
          Serial.print(EEPROM.read(0));
          Serial.print(" | EEPROM1: ");
          Serial.print(EEPROM.read(1));
          Serial.print(" | EEPROM2: ");
          Serial.print(EEPROM.read(2));

          Serial.print(" | high_score1: ");
          Serial.print(high_score1);
          Serial.print(" | score1: ");
          Serial.println(score1);


/////

          Serial.print("EEPROM3: ");
          Serial.print(EEPROM.read(3));
          Serial.print(" | EEPROM4: ");
          Serial.print(EEPROM.read(4));
          Serial.print(" | EEPROM5: ");
          Serial.print(EEPROM.read(5));

          Serial.print(" | high_score2: ");
          Serial.print(high_score2);
          Serial.print(" | score2: ");
          Serial.print(score2);
          Serial.print(" | choice: ");
          Serial.println(choice);

          Serial.println("");
          
          state = Init_EEPROM;
    }
    return state; 
  }

  
  const unsigned short tasksNum = 1;
  task tasks[tasksNum];
  
  void setup() {
    Serial.begin(9600);
    pinMode(A1,INPUT);
    pinMode(A2,INPUT);
    pinMode(A3,INPUT);   
    
    high_score1 = EEPROM.read(0) + EEPROM.read(1) + EEPROM.read(2);
    high_score2 = EEPROM.read(3) + EEPROM.read(4) + EEPROM.read(5);
      
    unsigned char n = 0;
    tasks[n].state = Init_EEPROM;
    tasks[n].period = 250;
    tasks[n].elapsedTime = 0;
    tasks[n].TickFct = &HighScore;
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
//    delay(100);
}
