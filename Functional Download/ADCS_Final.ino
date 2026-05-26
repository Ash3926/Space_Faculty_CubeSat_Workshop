// Necessary libraries for serial communication and sensor reading
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

SoftwareSerial adcs(3,2); // RX, TX, SoftwareSerial object for communication with OBC
int LM35 = A6; // LM35 temperature sensor connected to analog pin A6

 // Motor driver input pins
int IN1 = 9;
int IN2 = 10;
int MP = 6; // PWM pin for motor speed control

// Sun sensor readings for three sensors
// s1 = left sensor, s2 = back sensor, s3 = right sensor
int s1 = 0, s2 = 0, s3 = 0; 

// Initialize time check variables and flag for rotation command and data reset
unsigned long prevtime = 0;
unsigned long rotatetime = 0;
int rotatecommand = 1;
int resetvals = 0;
int rotatefeedback = 0; // Variable to store feedback for OBC on whether rotation is needed and in which direction (0 = no rotation, 1 = rotate left, 2 = rotate right)

uint16_t BNO055_SAMPLERATE_DELAY_MS = 1000; // Sample rate delay for BNO055 sensor
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire); // Initialize BNO055 sensor object

// Initialize variables for event data from BNO055 sensor
sensors_event_t orientationData , angVelocityData , linearAccelData, magnetometerData, accelerometerData, gravityData;

void setup() {
    // Start serial communication for debugging and communication with OBC
    Serial.begin(9600);
    adcs.begin(9600);
    bno.begin(); // Initialize BNO055 sensor


    // Set pin modes for motor control and sensor reading
    pinMode(3, INPUT);
    pinMode(2, OUTPUT);   
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(MP, OUTPUT);

    // Initialize time variables for sampling and rotation timing
    prevtime = millis();
    rotatetime = millis(); 
}

void loop() {

    // Passive checking the sun sensor readings
    // 5s sample time, checks that time since last sample is >= 5s before resampling
    if (millis() - prevtime >= 5000)
    {
        s1 = analogRead(A0);
        s2 = analogRead(A1);
        s3 = analogRead(A2);
        prevtime = millis();
        resetvals = 1; // Sets flag for new sun sensor data to be checked in sunsensorcheck function
    }

    // If previous rotate command has run for 1s, stoprotate
    if (millis() - rotatetime > 1000 && rotatecommand == 1)
    {
        stoprotate();
        rotatecommand = 0;
    }

    // If this is a new set of sun sensor readings, check if rotation is needed and set feedback for OBC
    if (resetvals == 1)
    {
        rotatefeedback = sunsensorcheck();
    }

    // Takes a serial command from the OBC, and responds accordingly.
    if (adcs.available()) {
        String command = adcs.readStringUntil('\n');
        command.trim();
        CommandResponse(command);
    }

    // Checks for shake events and responds accordingly
    // Does not check for shake if a rotation command has been running in the last 1s to prevent interference with rotation testing
    if (millis() - last_shake_time > BNO055_SAMPLERATE_DELAY_MS && rotatecommand == 0) // Checks that least sample delay has passed since last shake to prevent multiple triggers from one shake event
    {
        shake();
    }
}

// Command response functions for OBC commands
void CommandResponse(String command){
    if (command == "TEMPADCS") { // Command requesting ADCS temperature
        float temperature = readTemperature();
        adcs.println(temperature);
    }
    
    if (command == "SUN"){ // Command triggering rotation
        rotatenow(rotatefeedback);
    }

    if (command == "POS_State"){ // Command requesting sun sensor feedback
        if (rotatefeedback == 0){
            adcs.println("THE CAMERA IS OBSERVING EARTH");
        }
        else {
            adcs.println("THE SATELLITE POSITION NEEDS TO BE ADJUSTED");
        }
    }

    if (command == "IMUDATA"){ // Command requesting IMU data
        IMU_data();
    }
    
    if (command == "DETUMBLE"){ // Command triggering detumbling function
        detumbling();
    }
}

// Detumbler function: Spins the motor in each direction for 2s, then stops. 
void detumbling()
{
    for (int i = 0; i < 2; i++)
    {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(MP, 200);
        delay(2000);

        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        analogWrite(MP, 200);
        delay(2000);
    }

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(MP, 200);
    delay(2000);

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(MP, 0);

    adcs.println("DETUMBLING COMPLETE"); // Sends detumbling complete message to OBC
}

// Temperature read function
float readTemperature() {
    int sensorValue = analogRead(LM35);
    float voltage = sensorValue * (5.0 / 1023.0);
    float temperature = voltage * 100.0;
    return temperature;
}

// Function to check sun sensor readings and determine if rotation is needed, and in which direction
// Minimum difference of 50 between sensors is needed to trigger rotation, to prevent over-sensitivity to small changes in light levels
int sunsensorcheck(){
    // Checks the sun sensor readings, sets flag for data to be sent to OBC
    if ((s1-s2) < 50 && (s3-s2) < 50){ // Back sensor has most sun - correct orientation, no further action needed
        rotatefeedback = 0;
        stoprotate();
    }
    else if ((s1-s2) > 50){ // Left sensor has more sun, position needs to be adjusted
        rotatefeedback = 1;
    }
    else{ // Right sensor more sun, position needs to be adjusted
        rotatefeedback = 2;
    }
    resetvals = 0; // Resets flag for new sun sensor data
    return rotatefeedback;
}

// Rotate functions for motor control
void rotatenow(int feedback){
    if (feedback == 1){ // motor rotates left, momentum will cause satellite to rotate right
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(MP, 200);
    }
    else if (feedback == 2){ // motor rotates right, momentum will cause satellite to rotate left
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        analogWrite(MP, 200);
    }
    if (feedback != 0){
        rotatetime = millis(); // Saves time of rotation for comparison
        rotatecommand = 1;
    }
}

void stoprotate(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(MP, 200);
}

void IMU_data()
{
    sensors_event_t orientationData , angVelocityData , linearAccelData, magnetometerData, accelerometerData, gravityData;
    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
    bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
    bno.getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
    bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
    bno.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);

    printEvent(&orientationData);
    printEvent(&angVelocityData);
    printEvent(&linearAccelData);
    printEvent(&magnetometerData);
    printEvent(&accelerometerData);
    printEvent(&gravityData);
}

// helper function to print data from BNO055 sensor events for debugging purposes
void printEvent(sensors_event_t* event) {
    double x = -1000000, y = -1000000 , z = -1000000; //dumb values, easy to spot problem
    if (event->type == SENSOR_TYPE_ACCELEROMETER) {
        adcs.print("Accl:");
        x = event->acceleration.x;
        y = event->acceleration.y;
        z = event->acceleration.z;
    }
    else if (event->type == SENSOR_TYPE_ORIENTATION) {
        adcs.print("Orient:");
        x = event->orientation.x;
        y = event->orientation.y;
        z = event->orientation.z;
    }
    else if (event->type == SENSOR_TYPE_MAGNETIC_FIELD) {
        adcs.print("Mag:");
        x = event->magnetic.x;
        y = event->magnetic.y;
        z = event->magnetic.z;
    }
    else if (event->type == SENSOR_TYPE_GYROSCOPE) {
        adcs.print("Gyro:");
        x = event->gyro.x;
        y = event->gyro.y;
        z = event->gyro.z;
    }
    else if (event->type == SENSOR_TYPE_ROTATION_VECTOR) {
        adcs.print("Rot:");
        x = event->gyro.x;
        y = event->gyro.y;
        z = event->gyro.z;
    }
    else if (event->type == SENSOR_TYPE_LINEAR_ACCELERATION) {
        adcs.print("Linear:");
        x = event->acceleration.x;
        y = event->acceleration.y;
        z = event->acceleration.z;
    }
    else if (event->type == SENSOR_TYPE_GRAVITY) {
        adcs.print("Gravity:");
        x = event->acceleration.x;
        y = event->acceleration.y;
        z = event->acceleration.z;
    }
    else {
        adcs.print("Unk:");
    }

    adcs.print("\tx= ");
    adcs.print(x);
    adcs.print(" |\ty= ");
    adcs.print(y);
    adcs.print(" |\tz= ");
    adcs.println(z);
}

// Shake command to check if there is a shake, and rotates in each direction for 0.5s if shake is detected, then stops for 2s to prevent multiple triggers from one shake event
void shake()
{
    sensors_event_t linearData;

    bno.getEvent(&linearData, Adafruit_BNO055::VECTOR_LINEARACCEL);

    float x = linearData.acceleration.x;
    float y = linearData.acceleration.y;
    float z = linearData.acceleration.z;

    // If the absolute value of acceleration in any direction exceeds 0.5, a shake is detected
    if (abs(x) > 0.5 || abs(y) >  0.5 || abs(z) > 0.5)
    {
        adcs.println("WARNING: ANOMALY IN ACCELERATION"); //Sends shake message to OBC

        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(MP,200);
        delay(500);

        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        analogWrite(MP,200);
        delay(500);

        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        analogWrite(MP,0);
    }
    last_shake_time = millis(); // Saves time of shake for comparison
}