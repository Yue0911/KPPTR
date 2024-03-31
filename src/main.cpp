#include <Arduino.h>

const int digitPins[] = {10, 11, 12, 13};  // 替換為實際連接的引腳
const int segmentPins[] = {3, 4, 5, 6, 7, 8, 9};  // 替換為實際連接的引腳
const int buttonPin = 2;  // 替換為實際連接的按鈕引腳
const int buzzerPin = A2;  // 替換為實際連接的蜂鳴器引腳

int randomNumber;

const int digitPatterns[][7] = {
    {0, 0, 0, 0, 0, 0, 1},  // 0
    {1, 0, 0, 1, 1, 1, 1},  // 1
    {0, 0, 1, 0, 0, 1, 0},  // 2
    {0, 0, 0, 0, 1, 1, 0},  // 3
    {1, 0, 0, 1, 1, 0, 0},  // 4
    {0, 1, 0, 0, 1, 0, 0},  // 5
    {0, 1, 0, 0, 0, 0, 0},  // 6
    {0, 0, 0, 1, 1, 1, 1},  // 7
    {0, 0, 0, 0, 0, 0, 0},  // 8
    {0, 0, 0, 0, 1, 0, 0}   // 9
  };

  void displayDigit(int digit) {

  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], digitPatterns[digit][i]);
  }
}

void displayNumber(int number) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(digitPins[i], HIGH);  // 關閉所有數位
  }

  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], HIGH);  // 關閉所有數段（這裡使用 LOW 因為是共陽極）
  }

  // 顯示百位
  displayDigit(number / 1000);
  digitalWrite(digitPins[3], LOW);
  delay(1);
  digitalWrite(digitPins[3], HIGH);

  // 顯示十位
  displayDigit((number / 100) % 10);
  digitalWrite(digitPins[2], LOW);
  delay(1);
  digitalWrite(digitPins[2], HIGH);

  // 顯示個位
  displayDigit((number / 10) % 10);
  digitalWrite(digitPins[1], LOW);
  delay(1);
  digitalWrite(digitPins[1], HIGH);

  // 顯示個位
  displayDigit(number % 10);
  digitalWrite(digitPins[0], LOW);
  delay(1);
  digitalWrite(digitPins[0], HIGH);
}

void playDiceSound() {
  unsigned long previousMillis = 0;
  const int soundDuration = 150;  // 聲音持續時間（毫秒）
  const int sound1Frequency = 400;  // 第一個聲音的頻率
  const int sound2Frequency = 30;  // 第二個聲音的頻率
  static int soundState = 0;  // 0表示播放第一個聲音，1表示播放第二個聲音

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= soundDuration) {
    previousMillis = currentMillis;

    // 選擇要播放的聲音
    int frequency = (soundState == 0) ? sound1Frequency : sound2Frequency;

    // 播放聲音
    tone(buzzerPin, frequency, 50);
    // 切換到下一個聲音
    soundState = 1 - soundState;  // 切換0和1
  }
}

void setup() {
  // 設定數位引腳為輸出
  for (int i = 0; i < 4; i++) {
    pinMode(digitPins[i], OUTPUT);
  }

  // 設定數段引腳為輸出
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }

  // 設定按鈕引腳為輸入
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  // 初始化亂數種子
  randomSeed(analogRead(0));
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    randomNumber = random(0, 10000);  // 產生 0 到 9999 的隨機數
    playDiceSound();
  }
  displayNumber(randomNumber);
  delay(10);
}
