#include <SPI.h>
#include <LoRa.h>

const int csPin = 10;
const int resetPin = 9;
const int irqPin = 2;

void setup() {
  Serial.begin(115200); // Must match Transmitter and Python
  while (!Serial);

  LoRa.setPins(csPin, resetPin, irqPin);
  if (!LoRa.begin(434E6)) {
    while (1);
  }
  LoRa.setSyncWord(0x77); // Must match Transmitter
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    while (LoRa.available()) {
      // Use Serial.write for binary data, NOT Serial.print
      Serial.write(LoRa.read());
    }
  }
}