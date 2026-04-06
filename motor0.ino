#include <SoftwareSerial.h>

// ESP32 → Arduino
//SoftwareSerial espSerial;//(10, 11); // RX, TX

// Motor A (Left)
#define A1A 5
#define A1B 6

// Motor B (Right)
#define B1A 9
#define B1B 10

int speedVal = 120; // slow speed (0–255)

void setup() {
  Serial.begin(115200);      // Debug (PC)
 // espSerial.begin(9600);   // Match ESP32 baud!
Serial.print("Started: if CMD: prompt is not seen arduino is not doing TX");
   pinMode(A1A, OUTPUT);
  pinMode(A1B, OUTPUT);
  pinMode(B1A, OUTPUT);
  pinMode(B1B, OUTPUT);

  stopMotors();
}

// ================= MOTOR CONTROL =================

void forward() {
  analogWrite(A1A, speedVal);
  analogWrite(A1B, 0);

  analogWrite(B1A, speedVal);
  analogWrite(B1B, 0);
}

void backward() {
  analogWrite(A1A, 0);
  analogWrite(A1B, speedVal);

  analogWrite(B1A, 0);
  analogWrite(B1B, speedVal);
}

void left() {
  analogWrite(A1A, 0);
  analogWrite(A1B, speedVal);

  analogWrite(B1A, speedVal);
  analogWrite(B1B, 0);
}

void right() {
  analogWrite(A1A, speedVal);
  analogWrite(A1B, 0);

  analogWrite(B1A, 0);
  analogWrite(B1B, speedVal);
}

void stopMotors() {
  analogWrite(A1A, 0);
  analogWrite(A1B, 0);
  analogWrite(B1A, 0);
  analogWrite(B1B, 0);
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

 char c = cmd.charAt(0);
 Serial.println(c);
 switch (c) {

      case 'u': forward();  Serial.println("FORWARD");Serial.println("UP"); break;
      case 'w': forward();  Serial.println("FORWARD"); Serial.println("UP");break;
      
      case 'l': left(); Serial.println("LEFT");break;
      case 'a': left(); Serial.println("LEFT");break;      
      
      case 'n': backward(); Serial.println("BACKWARD");Serial.println("DOWN"); break;      
      case 's': backward(); Serial.println("BACKWARD"); Serial.println("DOWN");break;  
      
      case 'r': right(); Serial.println("RIGHT");break;
      case 'd': right();Serial.println("RIGHT"); break;
      
      case 'x': stopMotors();  Serial.println("STOP");break;

      case 'q': speedVal = 80; break; // faster
      case 'z': speedVal = 65; break;  // slower

      default: break;
    }
    /*
    if (cmd == 'l') 
    else if (cmd == 'r') Serial.println("RIGHT");
    else if (cmd == 'u')
    else if (cmd == 'd') Serial.println("DOWN");
    else if (cmd == 'w')
    else if (cmd == 's') 
    else if (cmd == 'x')
    */
  }
}
