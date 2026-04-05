#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define MAX_SNOW 20

int snowX[MAX_SNOW];
int snowY[MAX_SNOW];

void initSnow() {
  for (int i = 0; i < MAX_SNOW; i++) {
    snowX[i] = random(0, 128);
    snowY[i] = random(0, 64);
  }
}
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ===== Motion =====
float eyeX = 0, eyeY = 0;
float targetX = 0, targetY = 0;
unsigned long lastRandomMove = 0;
bool manualControl = false;

// ===== Pupil =====
float pupilSize = 15;
float targetPupil = 10;

// ===== Blink =====
bool blink = false;
int blinkTimer = 0;

// ===== Frame =====
int cornerRadius = 18;   // ellipse-like corners
int borderThickness = 10; // thick frame

// ================= DRAW FRAME =================
void drawRoundedEyeFrame() {

  // Outer filled rounded rectangle
  display.fillRoundRect(0, 0, 128, 64, cornerRadius, WHITE);

  // Inner cut (black) → creates thick border
  display.fillRoundRect(
    borderThickness,
    borderThickness,
    128 - 2 * borderThickness,
    64 - 2 * borderThickness,
    cornerRadius - 4,
    BLACK
  );
}

void randomEyeIdle0() {

  // Run every ~80 ms (safe for OLED)
  if (millis() - lastRandomMove > 80) {

    // small random offsets (micro movement)
    targetX += random(-2, 3);
    targetY += random(-2, 3);

    // limit movement (so it doesn't go outside)
    targetX = constrain(targetX, -20, 20);
    targetY = constrain(targetY, -12, 12);

    lastRandomMove = millis();
  }
}
void randomEyeIdle() {

  static unsigned long lastMove = 0;

  // Run every ~120 ms (safe + smooth)
  if (millis() - lastMove > 120) {

    int r = random(0, 5);

    switch (r) {
      case 0: processCommand("left"); break;
      case 1: processCommand("right"); break;
      case 2: processCommand("up"); break;
      case 3: processCommand("down"); break;
      case 4: processCommand("center"); break;
    }

    lastMove = millis();
  }
}
// ================= DRAW EYE =================
void drawEye() {
  int cx = 64;
  int cy = 32;

  if (blink) {
    // eyelid
    display.fillRect(10, cy - 2, 108, 10, WHITE);
    display.fillRect(cx - 5, 0, 2, 64, WHITE);
    return;
  }

  // Small pupil
  display.fillCircle(cx + eyeX, cy + eyeY, pupilSize, WHITE);

  // reflection
  display.fillCircle(cx + eyeX - 2, cy + eyeY - 2, 1, BLACK);
}

// ================= SMOOTH =================
void updateMotion() {
  eyeX += (targetX - eyeX) * 0.15;
  eyeY += (targetY - eyeY) * 0.15;
  pupilSize += (targetPupil - pupilSize) * 0.1;
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

// ================= COMMAND =================
void processCommand(String cmd) {
  cmd.trim();

  // Movement
  if (cmd == "l") targetX = -40;
  else if (cmd == "r") targetX = 40;
  else if (cmd == "u") targetY = -10;
  else if (cmd == "d") targetY = 10;
  else if (cmd == "c") {
    targetX = 0;
    targetY = 0;
  }
    else if (cmd == "z") {
    randomEyeIdle();
  }
    else if (cmd == "b") {
blastAnimation(64, 32); // center of OLED
dblastAnimation(64, 32); // center of OLED

} else if (cmd == "s") {
display.clearDisplay();

drawRoundedEyeFrame();
drawEye();

// ❄️ snow overlay
snowFall();

drawBottomText("hello");

display.display();

}else if (cmd == "?")
{
  Serial.println("l for left");
  Serial.println("r for right");
  Serial.println("u for up");
  Serial.println("d for down");
  Serial.println("c for center");
  
  Serial.println("z for random");
  Serial.println("b for center blast circles");
  Serial.println("s for sintilations");
  Serial.println("? for for this");
  
}
  

  // Focus
  else if (cmd == "focus") targetPupil = 3;
  else if (cmd == "relax") targetPupil = 6;
}
void drawBottomText(String txt) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  int textWidth = txt.length() * 6;   // approx width (6px per char)
  int x = (128 - textWidth) / 2;      // center align
  int y = 54;                         // bottom position

  display.setCursor(x, y);
  display.print(txt);
}


void dblastAnimation(int cx, int cy) {

  for (int r = 0; r < 64; r += 3) {

    display.clearDisplay();

    // multiple rings
    display.drawCircle(cx, cy, r, WHITE);
    display.drawCircle(cx, cy, r - 6, WHITE);
    display.drawCircle(cx, cy, r - 12, WHITE);

    display.display();
    delay(20);
  }
}
void blastAnimation(int cx, int cy) {

  for (int r = 0; r < 64; r += 4) {

    display.clearDisplay();

    // expanding filled circle
    display.fillCircle(cx, cy, r, WHITE);

    // hollow center (shockwave look)
    display.fillCircle(cx, cy, r - 3, BLACK);

    display.display();
    delay(15);
  }
}
void snowFall() {

  for (int i = 0; i < MAX_SNOW; i++) {

    // draw snow particle
    display.drawPixel(snowX[i], snowY[i], WHITE);

    // move down
    snowY[i] += random(1, 3);

    // slight horizontal drift
    snowX[i] += random(-1, 2);

    // reset when out of screen
    if (snowY[i] > 63) {
      snowY[i] = 0;
      snowX[i] = random(0, 128);
    }

    // keep inside bounds
    if (snowX[i] < 0) snowX[i] = 0;
    if (snowX[i] > 127) snowX[i] = 127;
  }
}
// ================= SETUP =================
void setup() {
  Serial.begin(115200);
initSnow();
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }
}


// ================= LOOP =================
void loop() {
  if (Serial.available()) {
       String cmd = Serial.readStringUntil('\n');
    processCommand(cmd);
  }
 updateMotion();
  updateBlink();
  display.clearDisplay();
  drawRoundedEyeFrame(); // 👈 main   frame
  drawEye();             // 👁 pupil inside
  drawBottomText("hello Hi 😊");
    display.display();
  delay(25);
}
