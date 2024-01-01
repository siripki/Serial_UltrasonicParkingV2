#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#define t1 2
#define e1 3
#define t2 4
#define e2 5
#define t3 6
#define e3 7

#define t4 10
#define e4 11
#define t5 12
#define e5 13

Servo myservo;

int distanceThreshold = 100;

int parkingAvailable = 3;
int barrierState = 0;

void setup() {
  pinMode(t1, OUTPUT);
  pinMode(t2, OUTPUT);
  pinMode(t3, OUTPUT);
  pinMode(t4, OUTPUT);
  pinMode(t5, OUTPUT);
  pinMode(e1, INPUT);
  pinMode(e2, INPUT);
  pinMode(e3, INPUT);
  pinMode(e4, INPUT);
  pinMode(e5, INPUT);
  
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  myservo.attach(9);
  myservo.write(0);
}

long readDistance(int triggerPin, int echoPin) {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  return pulseIn(echoPin, HIGH);
}


void loop() {
  float d1 = 0.01723 * readDistance(t1, e1);
  float d2 = 0.01723 * readDistance(t2, e2);
  float d3 = 0.01723 * readDistance(t3, e3);
  float d4 = 0.01723 * readDistance(t4, e4);
  float d5 = 0.01723 * readDistance(t5, e5);
  //get all sensor readings

  Serial.println("d1 = " + String(d1) + "cm");
  Serial.println("d2 = " + String(d2) + "cm");
  Serial.println("d3 = " + String(d3) + "cm");
  Serial.println("d4 = " + String(d4) + "cm");
  Serial.println("d5 = " + String(d5) + "cm");
  //display all sensor distance on serial monitor

  if (barrierState == 0) {
    if (d4 < 100 && d5 >= 100 && parkingAvailable > 0) {
      parkingAvailable -= 1;
      barrierState = -1;
      myservo.write(90);
    }
    if (d4 >= 100 && d5 < 100 && parkingAvailable < 3) {
      parkingAvailable += 1;
      barrierState = 1;
      myservo.write(90);
    }
  } else if (barrierState == -1) {
    if (d4 >= 100 && d5 < 100) {
      barrierState = -2;
      myservo.write(0);
    }
  } else if (barrierState == 1) {
    if (d5 >= 100 && d4 < 100) {
      barrierState = 2;
      myservo.write(0);
    }
  } else if (barrierState == -2) {
    if (d5 >= 100) {
      barrierState = 0;
    }
  } else if (barrierState == 2) {
    if (d4 >= 100) {
      barrierState = 0;
    }
  }

  lcd.setCursor(0, 0);
  if (parkingAvailable == 0) {
    lcd.print("Parking Full  ");
  } else {
    lcd.print("Parking left ");
    lcd.print(parkingAvailable);
  }



  if (d1 > 100 & d2 > 100 & d3 > 100) {
    lcd.setCursor(0, 1);
    lcd.print("Slot 1 2 3 Free");
    delay(500);
  } else if ((d1 > 100 & d2 > 100) | (d2 > 100 & d3 > 100) | (d3 > 100 & d1 > 100)) {
    lcd.setCursor(0, 1);
    if (d1 > 100 & d2 > 100)
      lcd.print("Slot 1 & 2 Free");
    else if (d1 > 100 & d3 > 100)
      lcd.print("Slot 1 & 3 Free");
    else
      lcd.print("Slot 2 & 3 Free");
  } else if (d1 < 100 & d2 < 100 & d3 < 100) {
    lcd.setCursor(0, 1);
    lcd.print("Parking Full   ");
  } else if ((d1 < 100 & d2 < 100) | (d2 < 100 & d3 < 100) | (d3 < 100 & d1 < 100)) {
    lcd.setCursor(0, 1);
    if (d1 > 100)
      lcd.print("Slot 1 is Free ");
    else if (d2 > 100)
      lcd.print("Slot 2 is Free ");
    else
      lcd.print("Slot 3 is Free ");
  }
  delay(100);
}