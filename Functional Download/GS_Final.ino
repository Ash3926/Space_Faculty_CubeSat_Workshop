#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Initialize LoRa module at 434 MHz
  if (!LoRa.begin(434E6)) 
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  String input = "";

  input = Serial.readStringUntil('\n'); // Read input from Serial until newline character
  input.trim(); // Remove any leading/trailing whitespace

  // Send command, if any, to LoRa module
  LoRa.beginPacket();
  LoRa.print(input);
  LoRa.endPacket();

  // try to parse packet if received
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet and send to serial (laptop connection)
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }
  }
}
