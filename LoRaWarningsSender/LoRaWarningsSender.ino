#include <SPI.h>
#include <LoRa.h>

String request = "";

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Warnings Sender");

  if (!LoRa.begin(434E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

bool warning_temp(){
  float tempobc = analogRead(A1)*5 / (1024*0.01);
  float tempeps = analogRead(A2)*5 / (1024*0.01);

  int secs = millis()/1000 % 60;
  int mins = millis()/1000 / 60;

  if (tempobc > 30) {
    LoRa.beginPacket();
    LoRa.print("[" + String(mins) + "m " + String(secs) + "s]" + "[Group 1] WARNING! TEMPOBC is too high: " + String(tempobc, 2) + "degrees Celsius");
    LoRa.endPacket();
  }
  if (tempeps > 30) {
    LoRa.beginPacket();
    LoRa.print("[" + String(mins) + "m " + String(secs) + "s]" + "[Group 1] WARNING! TEMPEPS is too high: " + String(tempeps, 2) + "degrees Celsius");
    LoRa.endPacket();
  }

  if (tempobc <= 30 && tempeps <= 30){
    return true;
  }
  return false;
}

bool warning_vbat(){
  float vbat = analogRead(A0)*5 / (1024);

  int secs = millis()/1000 % 60;
  int mins = millis()/1000 / 60;
  
  if (vbat < 3.6){
    LoRa.beginPacket();
    LoRa.print("[" + String(mins) + "m " + String(secs) + "s]" + "[Group 1] WARNING! VBAT too low: " + String(vbat, 2) + "V");
    LoRa.endPacket();
    return false;
  }
  return true;
}

void loop() {

  //Call the individual check functions for different possible errors
  if (warning_temp() && warning_vbat() && request = "STATUS\n")
  {
    LoRa.beginPacket();
    LoRa.println("[Group 1] STATUS = NOMINAL");
    LoRa.endPacket();
  }

  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    request = "";
    Serial.print("Received string: ");
    
    // Read and print the incoming message
    while (LoRa.available()) {
      request += (char)LoRa.read();
    }
    Serial.print(request);
    Serial.println(); // New line after message
  }
}
