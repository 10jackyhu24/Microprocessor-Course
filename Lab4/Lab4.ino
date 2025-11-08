#include <LiquidCrystal.h>

LiquidCrystal lcd (8, 9, 4, 5, 6, 7);
const int speakerPin = D3;
const byte buttonBT[] = {BT1, BT2, BT3, BT4};
const byte buttonA[] = {A0, A1, A2, A3};
char KeyValue[] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};

int pre_recorded_notes[] = {};
int pre_recorded_beats[] = {};

String hi[]={"LOW","LOW","LOW","LOW","LOW","LOW","LOW","Middle","Middle","Middle","Middle","Middle","Middle","Middle","High","High","High","High","High","High","High"};  //對照音高

void setup() {
  pinMode(speakerPin, OUTPUT);
  pinMode(BT1, INPUT);
  pinMode(BT2, INPUT);
  pinMode(BT3, INPUT);
  pinMode(BT4, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);
  lcd.begin(16,2);
  lcd.setCursor(13,0);
  lcd.print("Low");
}

void loop() {
  int key = keyScan();
  if (key != -1) {

  }

  if (!digitalRead(buttonBT[0])) {
    
  }
  if (!digitalRead(buttonBT[1])) {
    
  }
  if (!digitalRead(buttonBT[2])) {
    
  }
  if (!digitalRead(buttonBT[3])) {
    
  }
}

int keyScan() {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (i == j) {
        digitalWrite(buttonA[j], LOW);
      }
      else {
        digitalWrite(buttonA[j], HIGH);
      }
    }
    delayMicroseconds(100);
    for (int j = 10; j < 14; ++j) {
      if(digitalRead(j) == LOW) {
          digitalWrite(buttonA[i], HIGH);
          return((j - 10) * 4 + i);
      }
    }
  }
  return -1;
}
