// 定義接腳
const int buttonPin = 22;    // 按鈕輸入 (按下為 HIGH)
const int com1 = 30;         // COM1 控制腳 (接 PNP 電晶體基極)
const int segments[] = {38, 39, 40, 41, 42, 43, 44}; // A, B, C, D, E, F, G

// 共陽極七段顯示器 0-9 編碼 (0代表亮, 1代表暗)
const byte numbers[10][7] = {
  {0, 0, 0, 0, 0, 0, 1}, // 0
  {1, 0, 0, 1, 1, 1, 1}, // 1
  {0, 0, 1, 0, 0, 1, 0}, // 2
  {0, 0, 0, 0, 1, 1, 0}, // 3
  {1, 0, 0, 1, 1, 0, 0}, // 4
  {0, 1, 0, 0, 1, 0, 0}, // 5
  {0, 1, 0, 0, 0, 0, 0}, // 6
  {0, 0, 0, 1, 1, 1, 1}, // 7
  {0, 0, 0, 0, 0, 0, 0}, // 8
  {0, 0, 0, 0, 1, 0, 0}  // 9
};

int lastButtonState = LOW;   // 上次按鈕狀態
bool isCounting = false;     // 是否正在計數顯示
int currentNum = 0;          // 目前顯示數字
unsigned long lastUpdateTime = 0;   // 上次更新數字的時間
const int updateInterval = 800;     // 數字跳動速度 (毫秒)

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(com1, OUTPUT);
  
  // 初始狀態：根據電路圖 PNP 電晶體給 HIGH 會「關閉」顯示器
  digitalWrite(com1, HIGH); 

  // 初始化 A-G 接腳為不亮 (HIGH)
  for (int i = 0; i < 7; i++) {
    pinMode(segments[i], OUTPUT);
    digitalWrite(segments[i], HIGH); 
  }
}

void loop() {
  // 讀取按鈕狀態
  int buttonRead = digitalRead(buttonPin);

  // 偵測按鈕按下的瞬間 (從 LOW 變成 HIGH)
  if (buttonRead == HIGH && lastButtonState == LOW) {
    delay(50); // 簡易去彈跳
    if (digitalRead(buttonPin) == HIGH) {
      isCounting = true;  // 啟動顯示
      currentNum = 0;     // 每次按下都從 0 開始
      displayNumber(currentNum); // 立即更新顯示
      lastUpdateTime = millis(); // 重設計時器
    }
  }
  lastButtonState = buttonRead;

  // 如果已經開始計數
  if (isCounting) {
    // 開啟 COM1 (PNP 電晶體給 LOW 才會導通電源)
    digitalWrite(com1, LOW);

    // 檢查是否到達更新時間 (非阻塞式計時，不會卡住按鈕偵測)
    if (millis() - lastUpdateTime >= updateInterval) {
      currentNum++;
      if (currentNum > 9) {
        currentNum = 0; // 數到 9 後循環回 0
      }
      displayNumber(currentNum);
      lastUpdateTime = millis();
    }
  }
}

// 顯示數字的副程式
void displayNumber(int num) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segments[i], numbers[num][i]);
  }
}
