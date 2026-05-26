#include <SoftwareSerial.h>
SoftwareSerial adcs(3,2); 
int LM35 = A6;

void setup() {
  Serial.begin(9600);
  adcs.begin(9600);
  pinMode(3, INPUT);
  pinMode(2, OUTPUT);    
}

void loop() {
  if (adcs.available()) {

    String command = adcs.readStringUntil('\n');
    command.trim();

    if (command == "TEMPADCS") {

      int sensorValue = analogRead(LM35);

      float voltage = sensorValue * (5.0 / 1023.0);
      float temperature = voltage * 100.0;

      adcs.println(temperature);

    }
  }
}
