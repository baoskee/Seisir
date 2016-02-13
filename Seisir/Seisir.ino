#include "CurieImu.h"

int16_t ax, ay, az;         // accelerometer values
int16_t gx, gy, gz;         // gyrometer values


// Philosophy: Minimize false positives

const int ledPin = 13;      // activity LED pin
boolean blinkState = false; // state of the LED

void setup() {
    Serial.begin(9600); // initialize Serial communication
    while (!Serial);    // wait for the serial port to open
    
    // initialize device
    Serial.println("Initializing IMU device...");
    CurieImu.initialize();
    
    // verify connection
    Serial.println("Testing device connections...");
    if (CurieImu.testConnection()) {
        Serial.println("CurieImu connection successful");
    } else {
        Serial.println("CurieImu connection failed");
    }
    
    // use the code below to calibrate accel/gyro offset values
    Serial.println("Internal sensor offsets BEFORE calibration...");
    Serial.print(CurieImu.getXAccelOffset());
    Serial.print("\t"); // -76
    Serial.print(CurieImu.getYAccelOffset());
    Serial.print("\t"); // -235
    Serial.print(CurieImu.getZAccelOffset());
    Serial.print("\t"); // 168
    Serial.print(CurieImu.getXGyroOffset());
    Serial.print("\t"); // 0
    Serial.print(CurieImu.getYGyroOffset());
    Serial.print("\t"); // 0
    Serial.println(CurieImu.getZGyroOffset());
    
    
    
    
    
    
    // To manually configure offset compensation values,
    // use the following methods instead of the autoCalibrate...() methods below
    //    CurieImu.setXGyroOffset(220);
    //    CurieImu.setYGyroOffset(76);
    //    CurieImu.setZGyroOffset(-85);
    //    CurieImu.setXAccelOffset(-76);
    //    CurieImu.setYAccelOffset(-235);
    //    CurieImu.setZAccelOffset(168);
    
    Serial.println("About to calibrate. Make sure your board is stable and upright");
    delay(2000);
    
    // The board must be resting in a horizontal position for
    //  motionRecently += amountMoved;
    //  if(motionRecently >= threshold) {do stuff}
    //  motionRecently -=
    // the following calibration procedure to work correctly!
    Serial.print("Starting Gyroscope calibration...");
    CurieImu.autoCalibrateGyroOffset();
    Serial.println(" Done");
    Serial.print("Starting Acceleration calibration...");
    CurieImu.autoCalibrateXAccelOffset(0);
    CurieImu.autoCalibrateYAccelOffset(0);
    CurieImu.autoCalibrateZAccelOffset(1);
    Serial.println(" Done");
    
    Serial.println("Internal sensor offsets AFTER calibration...");
    Serial.print(CurieImu.getXAccelOffset());
    Serial.print("\t"); // -76
    Serial.print(CurieImu.getYAccelOffset());
    Serial.print("\t"); // -2359
    Serial.print(CurieImu.getZAccelOffset());
    Serial.print("\t"); // 1688
    Serial.print(CurieImu.getXGyroOffset());
    Serial.print("\t"); // 0
    Serial.print(CurieImu.getYGyroOffset());
    Serial.print("\t"); // 0
    Serial.println(CurieImu.getZGyroOffset());
    
    Serial.println("Enabling Gyroscope/Acceleration offset compensation");
    CurieImu.setGyroOffsetEnabled(true);
    CurieImu.setAccelOffsetEnabled(true);
    
    // configure Arduino LED for activity indicator
    pinMode(ledPin, OUTPUT);
}

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
    int length = 10; // Modify array length here
    int xValues[length];
    int yValues[length];
    int zValues[length];
    for (i = 0; i < length; i += 1) {
        xValues[i] = ax;
        yValues[i] = ay;
        zValues[i] = az;
    }
    
    FILE *gnuplot = popen("gnuplot", "w");
    fprintf(gnuplot, "plot '-'\n");
    for (i = 0; i < count; i++)
        fprintf(gnuplot, "%g %g\n", xValues[i], yValues[i]);
    fprintf(gnuplot, "e\n");
    fflush(gnuplot);
    
    //  Serial.print(gx);
    //  Serial.print("\t");
    //  Serial.print(gy);
    //  Serial.print("\t");
    //  Serial.println(gz);
    
    
