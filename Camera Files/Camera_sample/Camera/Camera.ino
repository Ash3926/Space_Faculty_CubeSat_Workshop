#include <Adafruit_VC0706.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <LoRa.h>

// Camera Pins
SoftwareSerial cameraconnection(5, 6); // TX, RX
Adafruit_VC0706 cam = Adafruit_VC0706(&cameraconnection);

// LoRa Pins
const int csPin = 10;          
const int resetPin = 9;       
const int irqPin = 2;         

void setup() {
  // 1. Initialize Serial and Hardware
  Serial.begin(115200); 
  while (!Serial);

  LoRa.setPins(csPin, resetPin, irqPin);
  if (!LoRa.begin(434E6)) { 
    Serial.println("LoRa Failed!");
    while (1);
  }
  LoRa.setSyncWord(0x77); 

  if (!cam.begin()) {
    Serial.println("Camera Not Found!");
    while (1);
  }
  
  // Set resolution (160x120 is recommended for first test)
  cam.setImageSize(VC0706_160x120); 
  delay(3000); // Give camera time to settle

  // 2. Take the Photo
  Serial.println("Taking snapshot...");
  if (!cam.takePicture()) {
    Serial.println("Failed to snap!");
    return;
  }

  uint32_t jpglen = cam.frameLength();
  Serial.print("Sending "); Serial.print(jpglen); Serial.println(" bytes...");

  // 3. Send START marker to Ground Station
  LoRa.beginPacket();
  LoRa.print("---START_IMAGE---");
  LoRa.endPacket();

  // 4. Send Image Data in chunks
  while (jpglen > 0) {
    uint8_t *buffer;
    uint8_t bytesToRead = min((uint32_t)64, jpglen); 
    buffer = cam.readPicture(bytesToRead);

    LoRa.beginPacket();
    LoRa.write(buffer, bytesToRead);
    LoRa.endPacket();

    jpglen -= bytesToRead;
    
    // Crucial: Small delay to prevent the receiver from being overwhelmed
    delay(100); 
  }

  // 5. Send END marker
  LoRa.beginPacket();
  LoRa.print("---END_IMAGE---");
  LoRa.endPacket();
  
  Serial.println("Transmission Complete. System Idle.");
}

void loop() {
  // Do nothing. To take another photo, press the RESET button on the Arduino.
}