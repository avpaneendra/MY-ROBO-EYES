# MY-ROBO-EYES
🤖 My OWN-Style OLED Eye Animation Engine (ESP32)  A dynamic ROBO OLED-inspired robotic eye UI system built using ESP32 + SSD1306 OLED (128x64). This project simulates a living robotic eye with smooth motion, blinking, animations, and particle effects.


✨ Features
👁️ Eye Rendering Engine
Full-screen rounded rectangular visor eye
Thick border with smooth curved corners
Real-time pupil movement with smoothing
Reflection highlight for realism
🎯 Motion System
Smooth interpolation (no jerky movement)
Direction control via serial commands:
l → Left
r → Right
u → Up
d → Down
c → Center
😴 Blinking System
Automatic blinking with timing control
Horizontal + vertical eyelid effect
Natural EMO-style eye closure
🎲 Idle Intelligence
Random eye movement mode (z)
Simulates curiosity and autonomous behavior
🎆 Animation Effects
💥 Blast Animation
Expanding shockwave from screen center
Multiple styles:
Filled blast
Multi-ring ripple effect

❄️ Scintilation Effect
Particle-based falling snow simulation
Random drift + reset logic
Ambient visual mode

💬 Text System
Bottom-centered text display

| Command | Action               |
| ------- | -------------------- |
| l       | Move left            |
| r       | Move right           |
| u       | Move up              |
| d       | Move down            |
| c       | Center eye           |
| z       | Random idle movement |
| b       | Blast animation      |
| s       | Snow effect          |
| ?       | Help menu            |

⚙️ Hardware Requirements
ESP32 (Dev Module)
SSD1306 OLED Display (128x64, I2C)
Connections:
SDA → GPIO 21
SCL → GPIO 22
📦 Libraries Used
Adafruit GFX
Adafruit SSD1306
Wire (I2C)
🚀 Key Concepts
Real-time animation rendering on OLED
Particle systems (snow, effects)
State-based behavior system
Smooth motion using interpolation
Embedded UI design for robots
🔥 Future Enhancements
Emotion system (happy, angry, sleepy)
Voice-controlled interaction
Sensor-based eye tracking
Dual-eye synchronization
Particle engine (rain, fire, hearts)
💡 Inspiration

Inspired by  robot UI design, aiming to replicate:
1. Pin Mapping
You need to map Arduino pins (5, 6, 9, 10) to Pi GPIO pins. Let's assume:

A1A = GPIO 17
A1B = GPIO 27
B1A = GPIO 22
B1B = GPIO 23
You can adjust these if you wire differently.


Expressive robotic eyes
Minimalist OLED animations
Autonomous behavior
👨‍💻 Author: A V Paneendra
(ESP32 + AI + Robotics Enthusiast)

⭐ If you like this project

Give it a star ⭐ and build your own robot!



