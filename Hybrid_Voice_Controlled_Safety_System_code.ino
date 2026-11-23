#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// === Servo Motors ===
Servo servo1;
Servo servo2;
#define SERVO1_PIN 3
#define SERVO2_PIN 5

int openPos = 240;   // adjust for your isolators
int closePos = 60;   // adjust for your isolators
int currentPos = 30; // start CLOSED

// === LCD ===
LiquidCrystal_I2C lcd(0x27, 16, 2);  // use 0x3F if 0x27 doesn't work

// === Bluetooth ===
SoftwareSerial BT(10, 11);  // RX = 10, TX = 11

// === Push Button ===
#define BUTTON_PIN 8
bool lastButtonState = HIGH;
bool isolatorOpen = false;

void setup() {
  // Attach servos
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo1.write(closePos);
  servo2.write(closePos);

  // LCD setup
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Isolator System");
  lcd.setCursor(0, 1);
  lcd.print("Status: CLOSED");

  // Bluetooth setup
  BT.begin(9600);

  // Button setup
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // --- Push Button ---
  bool buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == LOW && lastButtonState == HIGH) {
    delay(50);
    if (!isolatorOpen) {
      openIsolator();
    } else {
      closeIsolator();
    }
  }
  lastButtonState = buttonState;

  // --- Bluetooth Commands ---
  if (BT.available()) {
    String command = BT.readStringUntil('\n');
    command.trim();
    command.toUpperCase();

    if (command == "OPEN") {
      openIsolator();
    } else if (command == "CLOSE") {
      closeIsolator();
    } else if (command == "STOP") {
      stopIsolator();
    }
  }
}

// === Functions ===
void openIsolator() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Isolator System");
  lcd.setCursor(0, 1);
  lcd.print("Opening...");

  for (int pos = currentPos; pos <= openPos; pos++) {
    servo1.write(pos);
    servo2.write(pos);
    delay(15);
  }
  currentPos = openPos;
  isolatorOpen = true;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Isolator System");
  lcd.setCursor(0, 1);
  lcd.print("Status: OPEN");
}

void closeIsolator() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Isolator System");
  lcd.setCursor(0, 1);
  lcd.print("Closing...");

  for (int pos = currentPos; pos >= closePos; pos--) {
    servo1.write(pos);
    servo2.write(pos);
    delay(15);
  }
  currentPos = closePos;
  isolatorOpen = false;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Isolator System");
  lcd.setCursor(0, 1);
  lcd.print("Status: CLOSED");
}

void stopIsolator() {
  servo1.detach();
  servo2.detach();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Isolator System");
  lcd.setCursor(0, 1);
  lcd.print("EMERGENCY STOP");
}
