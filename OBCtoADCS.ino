#include <SoftwareSerial.h>
SoftwareSerial obc(3,4); 

void setup() {
  Serial.begin(9600);
  obc.begin(9600);

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
