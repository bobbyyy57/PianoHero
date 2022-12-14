#include "pitches.h"

typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
} task;

// notes in the melody:
int melody[] = {

  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {

  4, 8, 8, 4, 4, 4, 4, 4
};

unsigned char thisNote = 0;
enum States{Init, Start, Release, Clear} state;
int Tick(int state) {
    switch(state) {
    case Init: 


        if (digitalRead(A0) == 1) {
          tone(2, NOTE_C4);
        }
        else if (digitalRead(A1) == 1) {
          tone(2, NOTE_G3);
        }
        else if (digitalRead(A2) == 1) {
          tone(2, NOTE_A3);
        }
        else if (digitalRead(6) == 1) {
          tone(2, NOTE_C3);
        }
        else if (digitalRead(7) == 1) {
          tone(2, NOTE_A2);
        }
        else{
          noTone(2);
        }

        // iterate over the notes of the melody:
      
//        for (int thisNote = 0; thisNote < 8; thisNote++) {
//      
//          // to calculate the note duration, take one second divided by the note type.
//      
//          //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
//      
//          int noteDuration = 1000 / noteDurations[thisNote];
//      
//          tone(8, melody[thisNote], noteDuration);
//      
//          // to distinguish the notes, set a minimum time between them.
//      
//          // the note's duration + 30% seems to work well:
//      
//          int pauseBetweenNotes = noteDuration * 1.30;
//      
//          delay(pauseBetweenNotes);
//      
//          // stop the tone playing:
//      
//          noTone(8);

       
//  }
  state = Init;
          break;
}
return state;
}

const unsigned short tasksNum = 1;
task tasks[tasksNum];

void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  
  Serial.begin(9600);

  unsigned char n = 0;
  tasks[n].state = Init;
  tasks[n].period = 200;
  tasks[n].elapsedTime = 0;
  tasks[n].TickFct = &Tick;
}


void loop() {
  // put your main code here, to run repeatedly:
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
    if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
      tasks[i].state = tasks[i].TickFct(tasks[i].state);
      tasks[i].elapsedTime = millis(); // Last time this task was ran
      //Serial.println(tasks[i].state);
    }
  }
  delay(100);
}
