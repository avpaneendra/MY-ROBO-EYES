#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <QMC5883LCompass.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;


// ================= OBJECTS =================
QMC5883LCompass compass;
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// ================= GLOBAL =================
uint8_t orientation = 0;
bool tiltMode = false;
bool snowMode = false;
String command;

#define MAX_SNOW 20
int snowX[MAX_SNOW];
int snowY[MAX_SNOW];

// Tilt offsets
int offsetX = 0;
int offsetY = 0;
int offsetZ = 0;
#define LEAN_THRESHOLD 80

// Eye motion
float eyeX = 0, eyeY = 0;
float targetX = 0, targetY = 0;

// Pupil
float pupilSize = 10;
float targetPupil = 10;

// Blink
bool blink = false;
int blinkTimer = 0;

// Frame
int cornerRadius = 18;
int borderThickness = 10;

// ================= ORIENTATION =================
void applyOrientation() {
  display.setRotation(orientation);
}

// ================= SNOW =================
void initSnow() {
  for (int i = 0; i < MAX_SNOW; i++) {
    snowX[i] = random(0, 128);
    snowY[i] = random(0, 64);
  }
}

void snowFall() {
  for (int i = 0; i < MAX_SNOW; i++) {
    display.drawPixel(snowX[i], snowY[i], WHITE);

    snowY[i] += random(1, 3);
    snowX[i] += random(-1, 2);

    if (snowY[i] > 63) {
      snowY[i] = 0;
      snowX[i] = random(0, 128);
    }

    snowX[i] = constrain(snowX[i], 0, 127);
  }
}

// ================= DRAW =================
void drawRoundedEyeFrame() {
  display.fillRoundRect(0, 0, 128, 64, cornerRadius, WHITE);
  display.fillRoundRect(borderThickness, borderThickness,
                        128 - 2 * borderThickness,
                        64 - 2 * borderThickness,
                        cornerRadius - 4, BLACK);
}

void drawEye() {
  int cx = 64;
  int cy = 32;

  if (blink) {
    display.fillRect(10, cy - 2, 108, 10, WHITE);
    display.fillRect(cx - 5, 0, 2, 64, WHITE);
    return;
  }

  display.fillCircle(cx + eyeX, cy + eyeY, pupilSize, WHITE);
  display.fillCircle(cx + eyeX - 2, cy + eyeY - 2, 1, BLACK);
}

// ================= MOTION =================
void updateMotion() {
  eyeX += (targetX - eyeX) * 0.15;
  eyeY += (targetY - eyeY) * 0.15;
}

// ================= BLINK =================
void updateBlink() {
  blinkTimer++;
  if (blinkTimer > 1200) blink = true;
  if (blinkTimer > 1250) {
    blink = false;
    blinkTimer = 0;
  }
}

// ================= RANDOM =================
void randomEyeIdle() {
  static unsigned long lastMove = 0;

  if (millis() - lastMove > 120) {
    int r = random(0, 5);

    if (r == 0) targetX = -40;
    if (r == 1) targetX = 40;
    if (r == 2) targetY = -10;
    if (r == 3) targetY = 10;
    if (r == 4) {
      targetX = 0;
      targetY = 0;
    }

    lastMove = millis();
  }
}

// ================= TEXT =================
void drawBottomText(String txt) {
  display.setTextSize(1);
  display.setTextColor(BLACK);

  int textWidth = txt.length() * 6;
  int x = (128 - textWidth) / 2;

  display.setCursor(x, 56);
  display.print(txt);
}

// ================= BLAST =================
void blastAnimation(int cx, int cy) {
  for (int r = 0; r < 64; r += 4) {
    display.clearDisplay();
    display.fillCircle(cx, cy, r, WHITE);
    display.fillCircle(cx, cy, r - 3, BLACK);
    display.display();
    delay(15);
  }
}

// ================= TILT =================
void updateOrientationFromTilt() {

  compass.read();

  int x = compass.getX() - offsetX;
  int y = compass.getY() - offsetY;

  uint8_t newOrientation = orientation;

  if (abs(y) > abs(x)) {
    if (y > LEAN_THRESHOLD) newOrientation = 1;
    else if (y < -LEAN_THRESHOLD) newOrientation = 3;
  } else {
    if (x > LEAN_THRESHOLD) newOrientation = 0;
    else if (x < -LEAN_THRESHOLD) newOrientation = 2;
  }

  if (newOrientation != orientation) {
    orientation = newOrientation;
    applyOrientation();
  }
}

// ================= COMMAND =================
void processCommand(String cmd) {

  if (cmd == "l" || cmd == "left"  ) targetX = -40;
  else if (cmd == "r") targetX = 40;
  else if (cmd == "u") targetY = -10;
  else if (cmd == "n") targetY = 10;
    if (cmd == "a" || cmd == "left"  ) targetX = -40;
  else if (cmd == "d") targetX = 40;
  else if (cmd == "w") targetY = -10;
  else if (cmd == "s") targetY = 10;
    else if (cmd == "x") {
    blastAnimation(64, 32);
  }

  else if (cmd == "c") {
    targetX = 0;
    targetY = 0;
  }
  else if (cmd == "z") {
    randomEyeIdle();
  }
  else if (cmd == "b") {
    blastAnimation(64, 32);
  }
  else if (cmd == "y") {
    snowMode = !snowMode;
  }
  else if (cmd == "o") {
    orientation = (orientation + 1) % 4;
    applyOrientation();
  }
  else if (cmd == "O") {
    tiltMode = !tiltMode;
  }
}

// ================= SETUP =================
void setup() {

  Serial.begin(115200);
  SerialBT.begin("ROBOT"); // Bluetooth name
  compass.init();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }

  applyOrientation();
  initSnow();

  // Calibration
  long sumX = 0, sumY = 0, sumZ = 0;

  for (int i = 0; i < 20; i++) {
    compass.read();
    sumX += compass.getX();
    sumY += compass.getY();
    sumZ += compass.getZ();
    delay(50);
  }

  offsetX = sumX / 20;
  offsetY = sumY / 20;
  offsetZ = sumZ / 20;
}
String cmd1;
// ================= LOOP =================
void loop() {



    if (SerialBT.available()) {
      /*
    String cmd1 = SerialBT.readStringUntil('\n');
    
    // Send to Arduino
    
    */
     char c = SerialBT.read();

   cmd1 = String(c);
    cmd1.trim();
    cmd1.toLowerCase();
 command=cmd1;
    // Also process locally (eye control)
    processCommand(cmd1);
  }
    if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.toLowerCase();cmd.trim(); Serial.println(command);
    processCommand(cmd);
  }

  if (tiltMode) {
    updateOrientationFromTilt();
  }

  updateMotion();
  updateBlink();

  display.clearDisplay();

  drawRoundedEyeFrame();
  drawEye();

  if (snowMode) {
    snowFall();
  }

  drawBottomText("l r u d c b s z o O");

  display.display();

  delay(25);
}
