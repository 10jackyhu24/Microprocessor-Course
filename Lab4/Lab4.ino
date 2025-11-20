#include <LiquidCrystal.h>

LiquidCrystal lcd (8, 9, 4, 5, 6, 7);
const int speakerPin = D3;
const byte buttonBT[] = {BT1, BT2, BT3, BT4};
const byte buttonA[] = {A0, A1, A2, A3};
char KeyValue[] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};

int tempo = 300;

char pre_recorded_notes[] = {8, 5, 3, 5, 8, 11, 8, 9, 8, 5, 8};
unsigned long pre_recorded_beats[] = {1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3};

int recorded[100];
int recorded_end_index = 0;
bool is_record;
bool is_play;
int pitch = 0;
String syllables[] = {"Do", "Re", "Mi", "Fa", "So", "La", "Si"};
String pitch_levels[] = {"LOW", "Middle", "High"};
int cur_pitch = -1;

void setup() {
  is_record = false;
  is_play = false;
  pinMode(speakerPin, OUTPUT);
  pinMode(BT1, INPUT);
  pinMode(BT2, INPUT);
  pinMode(BT3, INPUT);
  pinMode(BT4, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  
  for (int i = 0; i < 4; i++) {
    pinMode(buttonA[i], OUTPUT);
    digitalWrite(buttonA[i], HIGH);
  }
  
  lcd.begin(16,2);
  lcd.setCursor(13,0);
  lcd.print("Low");
  pitch = 0; // 初始設為低音
}

void loop() {
  int key = keyScan();
  if (key != -1) {
    char k = KeyValue[key];
    pitchKeypress(k);
    soundKeypress(k);
    if (is_record && '1' <= k && k <= '7') {
      recorded[recorded_end_index++] = pitch + (k - '0');
    }
  }

  if (!digitalRead(buttonBT[0])) {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Recording");
    is_record = true;
    recorded_end_index = 0;
    delay(300); // 防抖動
  }
  else if (!digitalRead(buttonBT[1])) {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("No Recording");
    is_record = false;
    delay(300);
  }
  else if (!digitalRead(buttonBT[2])) {
    playRecorded();
    delay(300);
  }
  else if (!digitalRead(buttonBT[3])) {
    playPreRecorded();
    delay(300);
  }
}

void pitchKeypress(char k) {
  if (k < 'A' || k > 'C') return;
  
  lcd.clear();
  // 顯示最後一個按的音
  if(cur_pitch != -1){
    lcd.setCursor(0,0);
    int note_index = (cur_pitch - 1) % 7;
    lcd.print(syllables[note_index]);
  }
  
  // 顯示狀態
  if(is_record){
    lcd.setCursor(0, 1);
    lcd.print("Recording");
  }
  if(is_play){
    lcd.setCursor(0, 1);
    lcd.print("Playing");
  }
  
  // 設定音高和顯示
  if (k == 'A') {
    lcd.setCursor(12,0);
    lcd.print("High");
    pitch = 14;
  }
  else if (k == 'B') {
    lcd.setCursor(10,0);
    lcd.print("Middle");
    pitch = 7;
  }
  else if (k == 'C') {
    lcd.setCursor(13,0);
    lcd.print("Low");
    pitch = 0;
  }
}

void soundKeypress(char k) {
  if (k < '1' || k > '7') return;
  
  is_play = false;
  lcd.clear();
  
  if(is_record){
    lcd.setCursor(0, 1);
    lcd.print("Recording ");
  }
  
  // 顯示音高
  if(pitch == 14){
    lcd.setCursor(12,0);
    lcd.print("High");
  }
  else if(pitch == 7){
    lcd.setCursor(10,0);
    lcd.print("Middle");
  }
  else {
    lcd.setCursor(13,0);
    lcd.print("Low");
  }
  
  // 顯示音符名稱
  lcd.setCursor(0, 0);
  int note_index = (k - '1');
  lcd.print(syllables[note_index]);
  
  // 播放音符
  playNote(speakerPin, pitch + (k - '0'), tempo);
  
  // 紀錄目前的音高
  cur_pitch = pitch + (k - '0');
}

void playRecorded() {
  is_play = true;
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Playing");
  
  for (int i = 0; i < recorded_end_index; i++) {
    lcd.clear();
    // 顯示音高
    lcd.setCursor(0, 1);
    lcd.print("Playing");
    int pitch_temp = recorded[i];
    if(1 <= pitch_temp && pitch_temp <= 7){
      lcd.setCursor(13, 0);
      lcd.print("Low");
    }
    else if(8 <= pitch_temp && pitch_temp <= 14){
      lcd.setCursor(10, 0);
      lcd.print("Middle");
    }
    else{
      lcd.setCursor(12, 0);
      lcd.print("High");
    }
    
    // 顯示音符名稱
    lcd.setCursor(0, 0);
    int note_value = recorded[i] - 1;
    if (note_value >= 0 && note_value < 21) {
      int syllable_index = note_value % 7;
      lcd.print(syllables[syllable_index]);
    }
    
    // 播放音符
    if (recorded[i] == 0) {
      delay(tempo);
    }
    else {
      playNote(speakerPin, recorded[i], 350);
      delay(tempo/20);
    }
  }
  is_play = false;
}

void playPreRecorded() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Family");
  lcd.setCursor(0, 1);
  lcd.print("Playing a Song.");
  
  for (int i = 0; i < sizeof(pre_recorded_notes); i++) {
    if (pre_recorded_notes[i] == 0) {
      delay(pre_recorded_beats[i] * tempo);
    }
    else {
      playNote(speakerPin, pre_recorded_notes[i], pre_recorded_beats[i] * tempo);
      delay(tempo/20);
    }
  }
}

void playNote(int OutputPin, char note, unsigned long duration) {
  char names[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};
  int tones[] = {
    262, 294, 330, 349, 392, 440, 494,    // 低音
    523, 587, 659, 698, 784, 880, 988,    // 中音  
    1046, 1175, 1318, 1397, 1568, 1760, 1976  // 高音
  };
  
  for (int i = 0; i < 21; i++) {
    if (names[i] == note) {
      tone(OutputPin, tones[i], duration);
      delay(duration);
      noTone(OutputPin);
      break;
    }
  }
}

int keyScan() {
  for (int col = 0; col < 4; col++) {
    // 設置當前列為LOW，其他為HIGH
    for (int i = 0; i < 4; i++) {
      digitalWrite(buttonA[i], (i == col) ? LOW : HIGH);
    }
    delayMicroseconds(100);
    
    // 讀取行
    for (int row = 0; row < 4; row++) {
      int pin = 10 + row;
      if(digitalRead(pin) == LOW) {
        digitalWrite(buttonA[col], HIGH);
        return row * 4 + col;
      }
    }
  }
  return -1;
}
