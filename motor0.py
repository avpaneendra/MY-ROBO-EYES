import RPi.GPIO as GPIO
import time

# Motor A (Left)
A1A = 17
A1B = 27

# Motor B (Right)
B1A = 22
B1B = 23

# Setup GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setup(A1A, GPIO.OUT)
GPIO.setup(A1B, GPIO.OUT)
GPIO.setup(B1A, GPIO.OUT)
GPIO.setup(B1B, GPIO.OUT)

# Set up PWM for speed control
pwm_A1A = GPIO.PWM(A1A, 100)
pwm_A1B = GPIO.PWM(A1B, 100)
pwm_B1A = GPIO.PWM(B1A, 100)
pwm_B1B = GPIO.PWM(B1B, 100)

pwm_A1A.start(0)
pwm_A1B.start(0)
pwm_B1A.start(0)
pwm_B1B.start(0)

speedVal = 50  # 0-100 for PWM in Python

def stopMotors():
    pwm_A1A.ChangeDutyCycle(0)
    pwm_A1B.ChangeDutyCycle(0)
    pwm_B1A.ChangeDutyCycle(0)
    pwm_B1B.ChangeDutyCycle(0)

def forward():
    pwm_A1A.ChangeDutyCycle(speedVal)
    pwm_A1B.ChangeDutyCycle(0)
    pwm_B1A.ChangeDutyCycle(speedVal)
    pwm_B1B.ChangeDutyCycle(0)

def backward():
    pwm_A1A.ChangeDutyCycle(0)
    pwm_A1B.ChangeDutyCycle(speedVal)
    pwm_B1A.ChangeDutyCycle(0)
    pwm_B1B.ChangeDutyCycle(speedVal)

def left():
    pwm_A1A.ChangeDutyCycle(0)
    pwm_A1B.ChangeDutyCycle(speedVal)
    pwm_B1A.ChangeDutyCycle(speedVal)
    pwm_B1B.ChangeDutyCycle(0)

def right():
    pwm_A1A.ChangeDutyCycle(speedVal)
    pwm_A1B.ChangeDutyCycle(0)
    pwm_B1A.ChangeDutyCycle(0)
    pwm_B1B.ChangeDutyCycle(speedVal)

try:
    stopMotors()
    print("Started: Type command (w/u=forward, s/n=backward, a/l=left, d/r=right, x=stop, 1-4=speed)")
    while True:
        cmd = input("CMD: ").strip().lower()
        if not cmd:
            continue
        c = cmd[0]
        if c in ['u', 'w']:
            forward()
            print("FORWARD")
        elif c in ['n', 's']:
            backward()
            print("BACKWARD")
        elif c in ['l', 'a']:
            left()
            print("LEFT")
        elif c in ['r', 'd']:
            right()
            print("RIGHT")
        elif c == 'x':
            stopMotors()
            print("STOP")
        elif c == '1':
            speedVal = 65
        elif c == '2':
            speedVal = 80
        elif c == '3':
            speedVal = 100
        elif c == '4':
            speedVal = 120
        else:
            print("Unknown command")
finally:
    stopMotors()
    GPIO.cleanup()
