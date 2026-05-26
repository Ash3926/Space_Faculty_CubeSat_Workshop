#include <SoftwareSerial.h>
SoftwareSerial obc(3,4); 

void setup() {
  Serial.begin(9600);
  obc.begin(9600);

  pinMode(3, INPUT); // RX pin for OBC to ADCS communication
  pinMode(4, OUTPUT); // TX pin for OBC to ADCS communication
  while (!Serial); // Wait for serial monitor to open
}

void loop() {
  
  if (Serial.available()) {

    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "TEMPADCS") {

      obc.println("TEMPADCS");

      while (!obc.available());

      String temperature = obc.readStringUntil('\n');

      Serial.print("ADCS Temperature: ");
      Serial.print(temperature);
      Serial.println(" C");
}
}
