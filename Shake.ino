#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

int IN1 = 9;
int IN2 = 10;
int MP = 6;

uint16_t BNO055_SAMPLERATE_DELAY_MS = 100;


Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

void setup()
{
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  while (!Serial) delay(10);  

  Serial.println("Orientation Sensor Test"); Serial.println("");


  if (!bno.begin())
  {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }

  delay(1000);
}

void loop()
{
  String incoming = Serial.readStringUntil('\n');
  incoming.trim();

  if (incoming == "IMUDATA")
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

    int8_t boardTemp = bno.getTemp();
    Serial.println();
    Serial.print(F("temperature: "));
    Serial.println(boardTemp);

    uint8_t system, gyro, accel, mag = 0;
    bno.getCalibration(&system, &gyro, &accel, &mag);
    Serial.println();
    Serial.print("Calibration: Sys=");
    Serial.print(system);
    Serial.print(" Gyro=");
    Serial.print(gyro);
    Serial.print(" Accel=");
    Serial.print(accel);
    Serial.print(" Mag=");
    Serial.println(mag);

  

    Serial.println("--");
    delay(BNO055_SAMPLERATE_DELAY_MS);
  }
  shake();
}

void printEvent(sensors_event_t* event) {
  double x = -1000000, y = -1000000 , z = -1000000; //dumb values, easy to spot problem
  if (event->type == SENSOR_TYPE_ACCELEROMETER) {
    Serial.print("Accl:");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else if (event->type == SENSOR_TYPE_ORIENTATION) {
    Serial.print("Orient:");
    x = event->orientation.x;
    y = event->orientation.y;
    z = event->orientation.z;
  }
  else if (event->type == SENSOR_TYPE_MAGNETIC_FIELD) {
    Serial.print("Mag:");
    x = event->magnetic.x;
    y = event->magnetic.y;
    z = event->magnetic.z;
  }
  else if (event->type == SENSOR_TYPE_GYROSCOPE) {
    Serial.print("Gyro:");
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
  }
  else if (event->type == SENSOR_TYPE_ROTATION_VECTOR) {
    Serial.print("Rot:");
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
  }
  else if (event->type == SENSOR_TYPE_LINEAR_ACCELERATION) {
    Serial.print("Linear:");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else if (event->type == SENSOR_TYPE_GRAVITY) {
    Serial.print("Gravity:");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else {
    Serial.print("Unk:");
  }

  Serial.print("\tx= ");
  Serial.print(x);
  Serial.print(" |\ty= ");
  Serial.print(y);
  Serial.print(" |\tz= ");
  Serial.println(z);
}

void shake()
{
  sensors_event_t linearData;

  bno.getEvent(&linearData, Adafruit_BNO055::VECTOR_LINEARACCEL);

  float x = linearData.acceleration.x;
  float y = linearData.acceleration.y;
  float z = linearData.acceleration.z;

  if (abs(x) > 0.5 || abs(y) >  0.5 || abs(z) > 0.5)
  {
    Serial.println("SHAKE DETECTED");

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

    delay(2000);
  }

}

