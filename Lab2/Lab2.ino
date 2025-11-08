/* Define shift register pins used for seven segment display */
#define LATCH_DIO D15   
#define CLK_DIO D14
#define DATA_DIO 2        // D2 is OK, too

/* Segment byte maps for numbers 0 to 9 , A, b , C*/ // MSB: dp g f e d c b a :LSB
const byte SEGMENT_MAP[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0X7F,0X6F,0X77,0X7C,0X39,0x0};
char KeyValue[]={'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};
const byte DP = 0x40;   // Digit Point

/* Byte maps to select digit 1 to 4 */ // E: 1110  D: 1101  B: 1011   7: 0111    // the 0's place
const byte SEGMENT_SELECT[] = {0x0E,0x0D,0x0B,0x07};   // {0xFE,0xFD,0xFB,0xF7};  is OK, too.
int num[4] = {13, 13, 13, 13};
const byte buttonBT[] = {BT1, BT2, BT3, BT4};
const byte buttonA[] = {A0, A1, A2, A3};
void setup ()
{
    /* Set DIO pins to outputs */
    pinMode(LATCH_DIO,  OUTPUT);
    pinMode(CLK_DIO,  OUTPUT);
    pinMode(DATA_DIO, OUTPUT);
}

/* Main program */
void loop()
{
  int key = keyScan();
  if (~key) {
    if (key != 3 && key != 7 && key != 10 && key != 11 && key != 12 && key != 14) {
      arrLeftShift(num, KeyValue[key] - '0');
    }
  }
  if (!digitalRead(buttonBT[0])) {
    int arr[] = {13, 13, 13, 13};
    showAll(arr);
    delay(300);
    for (int i = 0; i < 8; ++i) {
      arrLeftShift(arr, i < 3 ? num[i] : 13);
      delay(300);
    }
  }
  if (!digitalRead(buttonBT[1])) {
    int arr[] = {13, 13, 13, 13};
    showAll(arr);
    delay(300);
    for (int i = 3; i >= -4; --i) {
      arrRightShift(arr, i >= 0 ? num[i] : 13);
      delay(300);
    }
  }
  
  Serial.println(key);
  showAll(num);
  delay(300);
}

void showAll(int arr[4]) {
  for (int i = 0; i < 4; ++i) {
    WriteNumberToSegment(i, arr[i]);
  }
}

void arrLeftShift(int arr[4], int input) {
  for (int i = 0; i < 3; ++i) {
    arr[i] = arr[i + 1];
  }
  arr[3] = input;
}

void arrRightShift(int arr[4], int input) {
  for (int i = 3; i >= 0; --i) {
    arr[i] = arr[i - 1];
  }
  arr[0] = input;
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
          return(i * 4 + j - 10);
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
