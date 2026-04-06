#include <SoftwareSerial.h>

// ESP32 → Arduino
//SoftwareSerial espSerial;//(10, 11); // RX, TX

void setup() {
  Serial.begin(115200);      // Debug (PC)
 // espSerial.begin(9600);   // Match ESP32 baud!
}

// ================= LOOP =================
void loop() {

  if (Serial.available()) {

//    char cmd = Serial.read();
//    cmd = tolower(cmd);

    String cmd = Serial.readStringUntil('\n');
    cmd.toLowerCase();cmd.trim();
    
    Serial.print("CMD: ");
    Serial.println(cmd);

    if (cmd == 'l') Serial.println("LEFT");
    else if (cmd == 'r') Serial.println("RIGHT");
    else if (cmd == 'u') Serial.println("UP");
    else if (cmd == 'd') Serial.println("DOWN");
    else if (cmd == 'w') Serial.println("FORWARD");
    else if (cmd == 's') Serial.println("BACKWARD");
    else if (cmd == 'x') Serial.println("STOP");
  }
}
