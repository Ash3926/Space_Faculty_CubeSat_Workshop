#include "Adafruit_VC0706.h"
#include <SoftwareSerial.h>
#include <SPI.h>
#include <LoRa.h>

SoftwareSerial cameraconnection(6, 5);
Adafruit_VC0706 cam = Adafruit_VC0706(&cameraconnection);
String request = "";

void setup() {
  Serial.begin(9600); //Match the default baud rate of the camera
  while (!Serial); // Wait for serial monitor to open

  //Initialize pins for comms with camera
  pinMode(5, INPUT);
  pinMode(6, OUTPUT);

  // Set up LoRa for transmission of image
  if (!LoRa.begin(434E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.beginPacket();
  LoRa.println("Ready to begin with Camera VC0706. Type SENDIMAGE_G1 to receive image.");
  LoRa.endPacket();
}

void loop() {
  if (request == "SENDIMAGE_G1\n")
  {
    if (cam.begin()) {
      LoRa.beginPacket();
      LoRa.print(" [G1] Camera Found:");
    } 
    else {
      LoRa.beginPacket();
      LoRa.println(" [G1] No camera found?");
      LoRa.endPacket();
      return;
    }

    // Set Image size
    //cam.setImageSize(VC0706_640x480);        // biggest
    //cam.setImageSize(VC0706_320x240);        // medium
    cam.setImageSize(VC0706_160x120);          // small
    char *reply = cam.getVersion();
    if (reply == 0) {
      LoRa.print("Failed to get version");
    } else {
      LoRa.println("-----------------");
      LoRa.print(reply);
      LoRa.println("-----------------");
    }

    if (!cam.takePicture()) 
    {
      LoRa.println(" [G1] Failed to snap!");
      LoRa.endPacket();
      return;
    }
    else {
      LoRa.println(" [G1] Picture taken!"); 
      LoRa.endPacket();}

    // Retrieve size of image/frame taken
    uint32_t jpglen = cam.frameLength();
    // Read and send image 32 bytes at a time
    LoRa.beginPacket();
    LoRa.println("G1 sending image of " + String(jpglen) + "bytes length.");
    LoRa.endPacket();

    LoRa.beginPacket();
    LoRa.println("---START_IMAGE---");
    LoRa.endPacket();

    // Keep sending 32 byte chunks of the image until the whole image has been sent, then send a final packet to indicate the end of the image
    while (jpglen > 0){
      uint8_t bytesToRead = min((uint32_t)32, jpglen);
      uint8_t *buf = cam.readPicture(bytesToRead);
      LoRa.beginPacket();
      LoRa.write(buf, bytesToRead);
      LoRa.endPacket();
      jpglen -= bytesToRead;
    }

    LoRa.beginPacket();
    LoRa.println("---END_IMAGE---");
    LoRa.endPacket();
  }

  // Listen for incoming image capture requests from the receiver
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
