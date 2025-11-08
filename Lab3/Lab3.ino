/* Define shift register pins used for seven segment display */
#include <LiquidCrystal.h>
#define LATCH_DIO D15   
#define CLK_DIO D14
#define DATA_DIO 2        // D2 is OK, too

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
/* Segment byte maps for numbers 0 to 9 , A, b , C*/ // MSB: dp g f e d c b a :LSB
const byte SEGMENT_MAP[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0X7F,0X6F,0X77,0X7C,0X39,0x0};
char KeyValue[]={'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};
const byte DP = 0x40;   // Digit Point

/* Byte maps to select digit 1 to 4 */ // E: 1110  D: 1101  B: 1011   7: 0111    // the 0's place
const byte SEGMENT_SELECT[] = {0x0E,0x0D,0x0B,0x07};   // {0xFE,0xFD,0xFB,0xF7};  is OK, too.
const byte buttonBT[] = {BT1, BT2, BT3, BT4};
const byte buttonA[] = {A0, A1, A2, A3};
void setup ()
{
    /* Set DIO pins to outputs */
    pinMode(LATCH_DIO,  OUTPUT);
    pinMode(CLK_DIO,  OUTPUT);
    pinMode(DATA_DIO, OUTPUT);
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
    lcd.begin(16, 2);              // start the library
    lcd.setCursor(0,0);
    Serial.begin(9600);
}

/* Main program */
void loop()
{
  int key = keyScan();
  if (key != -1) {
    if (key != 3 && key != 7 && key != 11 && key != 12 && key != 14) {

      delay(300);
      
    }
  }
  if (!digitalRead(buttonBT[0])) {
    int arr[] = {13, 13, 13, 13};
    showFrame4(arr[0], arr[1], arr[2], arr[3], 500);
    for (int i = 0; i < 8; ++i) {
      arrLeftShift(arr, i < 4 ? num[i] : 13);
      showFrame4(arr[0], arr[1], arr[2], arr[3], 500);
    }
  }
  if (!digitalRead(buttonBT[1])) {
    int arr[] = {13, 13, 13, 13};
    showFrame4(arr[0], arr[1], arr[2], arr[3], 500);
    for (int i = 3; i >= -4; --i) {
      arrRightShift(arr, i >= 0 ? num[i] : 13);
      showFrame4(arr[0], arr[1], arr[2], arr[3], 500);
    }
  }
  
//  Serial.println(key);
  showFrame4(num[0], num[1], num[2], num[3], 1);
}

void showAll(int arr[4]) {
  for (int i = 0; i < 4; ++i) {
    WriteNumberToSegment(i, arr[i]);
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
          Serial.print(i);
          Serial.print(' ');
          Serial.println(j);
          digitalWrite(buttonA[i], HIGH);
          return((j - 10) * 4 + i);
      }
    }
  }
  return -1;
}

/* Write a decimal number between 0 and 9 to one of the 4 digits of the display */
void WriteNumberToSegment(byte Segment, byte Value)
{
    digitalWrite(LATCH_DIO, LOW);
    // The following order cannot be changed. MAP first and then SELECT.
    shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_MAP[Value]);        
    shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_SELECT[Segment] );

    digitalWrite(LATCH_DIO,HIGH);
}

void showFrame4(int d0, int d1, int d2, int d3, int ms) {
  unsigned long t0 = millis();
  while ((int)(millis() - t0) < ms) {
    WriteNumberToSegment(0, d0);
    delayMicroseconds(500);
    WriteNumberToSegment(1, d1);
    delayMicroseconds(500);
    WriteNumberToSegment(2, d2);
    delayMicroseconds(500);
    WriteNumberToSegment(3, d3);
    delayMicroseconds(500);
  }
}
