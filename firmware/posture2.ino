#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

const int flexPin = A0;
const int motorPin = 8;
const int buttonPin = 2;

const int ledR = 6;
const int ledG = 5;
const int ledB = 3;

//adjust threshold if not sensitive enough (make smaller) forward tilt
//if too sensitive make larger
float tiltThreshold = 7.5; //degrees from baseline posture
float downThreshold = -0.7;
int flexThreshold = 60; 
const unsigned long badPostureDelayMs = 5000; //grace period before buzz

float tiltBaseline = 0;
float yBaseline = 9.8;
int flexBaseline = 0;

bool systemOn = false;

unsigned long buttonStartTime = 0;
bool buttonWasPressed = false;
bool longPress = false;

unsigned long badPostureStartTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(motorPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  if (!mpu.begin()) {
    Serial.println("Can't find mpu");
    while(1);
  }
  delay(500);
  recalibrateSensors(); //starts off with recalibration so must be sitting straight when plugged in
}

void loop() {
  button();
  if (!systemOn) {
    digitalWrite(motorPin, LOW);
    ledStatus(false, false);
    return;
  }
  bool buzz = checkPosture();
  digitalWrite(motorPin, buzz ? HIGH : LOW);
  ledStatus(true, buzz);
  delay(100);
}

void button() {
  bool currentlyPressed = (digitalRead(buttonPin) == LOW); // active LOW
  if (currentlyPressed && !buttonWasPressed) {
    buttonStartTime = millis();
    longPress = false;
  }
  if (!currentlyPressed && buttonWasPressed) {
    unsigned long pressTime = millis() - buttonStartTime;

    //some short press turns on/off system
    if (pressTime < 1000 && !longPress) {
      systemOn = !systemOn;
      Serial.print("System: ");
      Serial.println(systemOn ? "ON" : "OFF");
      coinVibrator(1, 100);
    }
  }
  //long press >3s recalibrates
  if (currentlyPressed && !longPress && (millis() - buttonStartTime >= 3000)) {
    recalibrateSensors();
    coinVibrator(2, 150);
    longPress = true;
  }
  buttonWasPressed = currentlyPressed;
}

bool checkPosture() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  float pitch = atan2(a.acceleration.z, sqrt(a.acceleration.x * a.acceleration.x + a.acceleration.y * a.acceleration.y)) * (180.0 / PI);
  //changes from baseline
  float tiltChange = pitch - tiltBaseline;
  float yChange = a.acceleration.y - yBaseline;
  //flex (added sensor)
  int flexVal = analogRead(flexPin);
  int flexChange = flexVal - flexBaseline;

  bool badFlex = flexChange > flexThreshold;
  bool badForward = tiltChange > tiltThreshold; //forward tilt
  bool badDown = yChange < downThreshold;  //down measurment (shoulders/torso drops)
  bool badTilt = badForward || badDown;
  bool badPosture = badTilt || badFlex;

  //delay 
  if (badTilt) {
   if (badPostureStartTime == 0) badPostureStartTime = millis();
  } else {
   badPostureStartTime = 0;
  }
  bool buzz = badTilt && (millis() - badPostureStartTime >= badPostureDelayMs);

  Serial.print("Pitch: "); Serial.print(pitch);
  Serial.print(" tilt: "); Serial.print(tiltChange);
  Serial.print(" Y: "); Serial.print(a.acceleration.y);
  Serial.print(" flex: "); Serial.print(flexVal);
  Serial.print(" flexdiff: "); Serial.print(flexChange);
  Serial.print(" badForward: "); Serial.print(badForward);
  Serial.print(" badDown: "); Serial.print(badDown);
  Serial.print(" badFlex: "); Serial.print(badFlex);
  Serial.print(" Buzz: "); Serial.println(buzz ? "ON" : "OFF");
  return buzz;
}

void recalibrateSensors() {
  Serial.println("Recalibrating...");
  sensors_event_t a, g, temp;
  float sumPitch = 0;
  float sumY = 0;
  long flexSum = 0;
  //show recalib by blue blink
  for (int i = 0; i < 3; i++){
    digitalWrite(ledB, HIGH);
    delay(150);
    digitalWrite(ledB, LOW);
    delay(150);
  }
  for (int i = 0; i < 50; i++) {
    mpu.getEvent(&a, &g, &temp);
    float pitch = atan2(a.acceleration.z, sqrt(a.acceleration.x * a.acceleration.x + a.acceleration.y * a.acceleration.y)) * (180.0 / PI);
    sumPitch += pitch;
    sumY+=a.acceleration.y;
    delay(10);
  }
  tiltBaseline = sumPitch /50.0;
  yBaseline = sumY /50.0;
  Serial.print("New baseline tilt: ");
  Serial.println(tiltBaseline);
  Serial.print("New baseline Y: ");
  Serial.println(yBaseline);
  Serial.print("New flex baseline: ");
  Serial.println(flexBaseline);
  coinVibrator(1, 300);
}

void coinVibrator(int numOfBuzzes, int buzzLength) {
  for (int i = 0; i < numOfBuzzes; i++) {
    digitalWrite(motorPin, HIGH);
    delay(buzzLength);
    digitalWrite(motorPin, LOW);
    delay(150);
  }
}

void ledStatus(bool systemOn, bool badPosture) {
  if (!systemOn) {
    digitalWrite(ledR, LOW);
    digitalWrite(ledG, LOW);
    digitalWrite(ledB, LOW);
    return;
  }
  if (badPosture){ 
    //red
    digitalWrite(ledR, HIGH);
    digitalWrite(ledG, LOW);
    digitalWrite(ledB, LOW);
  }else{         
    //green
    digitalWrite(ledR, LOW);
    digitalWrite(ledG, HIGH);
    digitalWrite(ledB, LOW);
  }
}