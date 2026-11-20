#include <ESP32Servo.h>

#define TRIG_PIN  12
#define ECHO_PIN  13
#define SERVO_PIN 14

#define DETECT_CM     5
#define CLOSE_DELAY   2000
#define DOOR_CLOSED   0
#define DOOR_OPEN     90

Servo myServo;
bool doorOpen = false;
unsigned long lastDetectedTime = 0;

float readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return -1;
  return duration * 0.0343 / 2;
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  myServo.setPeriodHertz(50);
  myServo.attach(SERVO_PIN, 500, 2400);
  myServo.write(DOOR_CLOSED);
}

void loop() {
  float distance = readDistanceCm();
  bool detected = (distance > 0 && distance <= DETECT_CM);

  if (detected) {
    lastDetectedTime = millis();

    if (!doorOpen) {
      myServo.write(DOOR_OPEN);
      doorOpen = true;
      Serial.println("Door OPEN");
    }
  }


  if (doorOpen && !detected && (millis() - lastDetectedTime >= CLOSE_DELAY)) {
    myServo.write(DOOR_CLOSED);
    doorOpen = false;
    Serial.println("Door CLOSED");
  }

  delay(50);
}