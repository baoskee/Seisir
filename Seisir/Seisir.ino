#include <LiquidCrystal.h>
#include <BMI160.h>
#include <CurieImu.h>

#include "CurieImu.h"

LiquidCrystal lcd(12,11,5,4,3,2);

    
int16_t ax, ay, az;         // accelerometer values
int16_t gx, gy, gz;         // gyrometer values
int tempo = 150;
const int buzzerPin = 7;


const int ledPin = 13;      // activity LED pin
boolean blinkState = false; // state of the LED

void setup() {
    lcd.begin(16,2);
    lcd.clear();
    pinMode(buzzerPin, OUTPUT);
    Serial.begin(9600); // initialize Serial communication
    while (!Serial);    // wait for the serial port to open
    
    // initialize device
//    Serial.println("Initializing IMU device...");
    CurieImu.initialize();
    
    // verify connection
//    Serial.println("Testing device connections...");
    if (CurieImu.testConnection()) {
//        Serial.println("CurieImu connection successful");
    } else {
//        Serial.println("CurieImu connection failed");
    }
    
    // use the code below to calibrate accel/gyro offset values
//    Serial.println("Internal sensor offsets BEFORE calibration...");
//    Serial.print(CurieImu.getXAccelOffset());
//    Serial.print("\t"); // -76
//    Serial.print(CurieImu.getYAccelOffset());
//    Serial.print("\t"); // -235
//    Serial.print(CurieImu.getZAccelOffset());
//    Serial.print("\t"); // 168
//    Serial.print(CurieImu.getXGyroOffset());
//    Serial.print("\t"); // 0
//    Serial.print(CurieImu.getYGyroOffset());
//    Serial.print("\t"); // 0
//    Serial.println(CurieImu.getZGyroOffset());
    
    
    
    
    
    
    // To manually configure offset compensation values,
    // use the following methods instead of the autoCalibrate...() methods below
    //    CurieImu.setXGyroOffset(220);
    //    CurieImu.setYGyroOffset(76);
    //    CurieImu.setZGyroOffset(-85);
    //    CurieImu.setXAccelOffset(-76);
    //    CurieImu.setYAccelOffset(-235);
    //    CurieImu.setZAccelOffset(168);
    
//    Serial.println("About to calibrate. Make sure your board is stable and upright");
    delay(2000);
    
    // The board must be resting in a horizontal position for
    //  motionRecently += amountMoved;
    //  if(motionRecently >= threshold) {do stuff}
    //  motionRecently -=
    // the following calibration procedure to work correctly!
//    Serial.print("Starting Gyroscope calibration...");
    CurieImu.autoCalibrateGyroOffset();
//    Serial.println(" Done");
//    Serial.print("Starting Acceleration calibration...");
    CurieImu.autoCalibrateXAccelOffset(0);
    CurieImu.autoCalibrateYAccelOffset(0);
    CurieImu.autoCalibrateZAccelOffset(1);
//    Serial.println(" Done");
//    
//    Serial.println("Internal sensor offsets AFTER calibration...");
//    Serial.print(CurieImu.getXAccelOffset());
//    Serial.print("\t"); // -76
//    Serial.print(CurieImu.getYAccelOffset());
//    Serial.print("\t"); // -2359
//    Serial.print(CurieImu.getZAccelOffset());
//    Serial.print("\t"); // 1688
//    Serial.print(CurieImu.getXGyroOffset());
//    Serial.print("\t"); // 0
//    Serial.print(CurieImu.getYGyroOffset());
//    Serial.print("\t"); // 0
//    Serial.println(CurieImu.getZGyroOffset());
//    
//    Serial.println("Enabling Gyroscope/Acceleration offset compensation");
    CurieImu.setGyroOffsetEnabled(true);
    CurieImu.setAccelOffsetEnabled(true);
    
    // configure Arduino LED for activity indicator
    pinMode(ledPin, OUTPUT);
}
int currMovementMag = 0;
int total = 0;
int lastCountTotalChanged = 0;
int count = 0;

void loop() {
    // read raw accel/gyro measurements from device
    CurieImu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    // these methods (and a few others) are also available
    //CurieImu.getAcceleration(&ax, &ay, &az);
    //CurieImu.getRotation(&gx, &gy, &gz);
    
    // display tab-separated accel/gyro x/y/z values
    // Serial.print("a/g:\t");
    // Serial.println(ax);
    //  Serial.print("\t");
    //  Serial.print(ay);
    //  Serial.print("\t");
    //  Serial.print(az);
    //  Serial.print("\t");

    int pastMovementMag = currMovementMag;
    currMovementMag = abs(ax)+abs(ay)+abs(az);

    if(pastMovementMag > 55000 && currMovementMag < 55000) {
      total = total + 1;
      lastCountTotalChanged = count;
    } else if(pastMovementMag < 55000 && currMovementMag > 55000) {
      total = total + 1;
      lastCountTotalChanged = count;
    } else {
      if(count - 70 == lastCountTotalChanged) {
        total = 0;
      }
    }

    if(total > 75) {
        int i, duration;

        for (i = 0; i < 10; i++) {
            duration = 2 * tempo;  // length of note/rest in ms

             lcd.print("SEIZURE DETECTED");
             tone(buzzerPin, 445, 750);
             delay(duration);            // wait for tone to finish
 
        }
    }
  
    count++;
//  Serial.println(currMovementMag);
    Serial.println(total);
//  Serial.println(gx);
//  Serial.println(gy);
//  Serial.println(gz);
    delay(5);

 
}
