#include "Adafruit_VC0706.h"
#include <SoftwareSerial.h>
#include <SPI.h>

SoftwareSerial cameraconnection(5, 6);
Adafruit_VC0706 cam = Adafruit_VC0706(&cameraconnection);
String request = "";

void setup() {
  Serial.begin(9600); 
  while (!Serial); // Wait for serial monitor to open

  //Initialize pins for comms with camera
  pinMode(5, INPUT);
  pinMode(6, OUTPUT);

  Serial.println("Ready to begin with Camera VC0706. Type PHOTO to receive image.");
}

void loop() {
  delay(30000); // Delay to allow time for the image to be sent before the next request is processed
  if (request == "PHOTO\n"); 
  // Remove semicolon if using a receiver arduino to read raw bytes instead of image_receiver.py
  // If using a receiver arduino to read raw bytes,
  // image will be printed to the serial monitor as a stream of bytes
  {
    if (cam.begin()) {
      Serial.print("Camera Found: ");
    } 
    else {
      Serial.println("No camera found?");
      return;
    }
    char *reply = cam.getVersion();
    if (reply == 0) {
      Serial.print("Failed to get version");
    } else {
      Serial.println("-----------------");
      Serial.print(reply);
      Serial.println("-----------------");
    }
    // Set Image size
    cam.setImageSize(VC0706_640x480);        // biggest
    //cam.setImageSize(VC0706_320x240);        // medium
    //cam.setImageSize(VC0706_160x120);          // small

    if (!cam.takePicture()) 
    {
      Serial.println("Failed to snap!");
      return;
    }
    else {
      Serial.print("---START_IMAGE---"); }

    // Retrieve size of image/frame taken
    uint32_t jpglen = cam.frameLength();
    // Read and send image 32 bytes at a time
    while (jpglen > 0) 
    {
      uint8_t bytesToRead = min((uint32_t)32, jpglen);
      uint8_t *buf = cam.readPicture(bytesToRead);
      Serial.write(buf, bytesToRead);
      jpglen -= bytesToRead;
    }
    Serial.print("---END_IMAGE---");
  }

  request = "";
  Serial.print("Received string: ");
  
  // Read and print the incoming message
  request = Serial.readString();
  Serial.print(request);
  Serial.println(); // New line after message
}
