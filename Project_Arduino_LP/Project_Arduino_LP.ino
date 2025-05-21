#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2); // LCD 설정
SoftwareSerial mySerial(12, 13);    // RX (11), TX (10)
DFRobotDFPlayerMini myDFPlayer;

const int buttonPin_L = 3;  // 왼쪽 버튼
const int buttonPin_C = 4;  // 확인 버튼
const int buttonPin_R = 5;  // 오른쪽 버튼

const int stepsPerRevolution = 2048;
Stepper myStepper(stepsPerRevolution,11,9,10,8);  

int count = 1;              // 현재 선택된 트랙 (1부터 시작)
int totalTracks = 8;        // 총 트랙 수
bool isPlaying = false;     // 현재 재생 중인지 상태 확인

// 트랙 데이터
String tracks[] = {
  "Nocturne No2 inE",
  "Little Girl",
  "Jailhouse Rock",
  "Hey Jude",
  "The Swan",
  "Merry Christmas, Mr.Lawrence",
  "River flows in you",
  "Rockin Around The Christmas Tree"
};

String Who[] = {
  "Chopin",
  "Lee mun sae",
  "Elvis Presley",
  "Beatles",
  "Saint Saens",
  "Ryuichi Sakamoto",
  "Yiruma",
  "Brenda Lee"
};

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  myStepper.setSpeed(10);

  pinMode(buttonPin_L, INPUT);
  pinMode(buttonPin_C, INPUT);
  pinMode(buttonPin_R, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Not Playing");
  lcd.setCursor(0, 1);
  lcd.print("Select Track");

  delay(1000); // 초기화 대기

  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("DFPlayer Mini 초기화 실패");
    while (true); // 초기화 실패 시 무한 대기
  }

  Serial.println("DFPlayer Mini 초기화 성공!");
}

void loop() {
  int reading_L = digitalRead(buttonPin_L);
  int reading_C = digitalRead(buttonPin_C);
  int reading_R = digitalRead(buttonPin_R);

  // 왼쪽 버튼: 트랙 감소
  if (reading_L == HIGH && !isPlaying) {
    count--;
    if (count < 1) count = totalTracks;
    updateLCD();
    delay(300);
  }

  // 오른쪽 버튼: 트랙 증가
  if (reading_R == HIGH && !isPlaying) {
    count++;
    if (count > totalTracks) count = 1;
    updateLCD();
    delay(300);
  }

  // 확인 버튼: 재생/중지
  if (reading_C == HIGH) {
    if (isPlaying) {
      // 음악과 모터 중지
      isPlaying = false;
      myDFPlayer.stop();
      lcd.setCursor(0, 0);
      lcd.print("Not Playing");
    } else {
      // 음악과 모터 재생
      click_C();
    }
    delay(300);
  }

  // 음악이 재생 중이면 모터를 계속 회전
  if (isPlaying) {
    myStepper.step(stepsPerRevolution / 64); // 모터 한 단계씩 회전
  }
}

void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Track: ");
  lcd.print(count);
  lcd.setCursor(0, 1);
  lcd.print(Who[count - 1]);
}

void click_C() {
  myDFPlayer.volume(20);

  isPlaying = true; // 재생 상태로 변경
  if (count == 1 || count == 5 || count == 6) {
    myDFPlayer.EQ(DFPLAYER_EQ_CLASSIC);
  }
   else if (count == 2 || count == 7) {
    if (count == 7){
    myDFPlayer.volume(17); }
    myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  } else if (count == 3 || count == 4 || count == 8) {
    myDFPlayer.EQ(DFPLAYER_EQ_JAZZ);
  }

  myDFPlayer.playMp3Folder(count); // 해당 트랙 실행

  lcd.setCursor(0, 0);
  lcd.print("Playing....");
  scrollText(tracks[count - 1]);

  // 음악이 재생되는 동안 모터를 계속 회전
  while (isPlaying) {
    int reading_C = digitalRead(buttonPin_C); // 중지 버튼 상태 확인

    // 확인 버튼이 눌리면 중지
    if (reading_C == HIGH) {
      isPlaying = false;
      myDFPlayer.stop();
      lcd.setCursor(0, 0);
      lcd.print("Not Playing");
      break; // 루프 종료
    }
    // 모터 회전
    myStepper.step(stepsPerRevolution / 64);
  }
}
void scrollText(String title) {
  int len = title.length();

  if (len > 16) {
    for (int i = 0; i <= len - 16; i++) {
      if (!isPlaying) break; // 재생 중지 상태라면 스크롤 멈춤
      lcd.setCursor(0, 1);
      lcd.print(title.substring(i, i + 16));
      delay(500);
    }
  }
  else {
    lcd.setCursor(0, 1);
    lcd.print(tracks[count - 1]);
  }
}