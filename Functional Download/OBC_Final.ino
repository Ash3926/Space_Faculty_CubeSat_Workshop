#include <SPI.h>
#include <LoRa.h>
#include <SoftwareSerial.h>
#include "Adafruit_VC0706.h"

SoftwareSerial obc(3,4); // RX, TX pins for OBC to ADCS communication
SoftwareSerial cameraconnection(6, 5); // RX, TX pins for comms with camera
Adafruit_VC0706 cam = Adafruit_VC0706(&cameraconnection);

String command = "";

void setup() {
    Serial.begin(9600);
    obc.begin(9600);
    while (!Serial); // Wait for serial monitor to open

    // Initialize pins for comms with ADCS
    pinMode(3, INPUT); 
    pinMode(4, OUTPUT); 

    // Initialize pins for comms with camera
    pinMode(5, INPUT);
    pinMode(6, OUTPUT);


    // Only initialize the LoRa chip ONCE here
    if (!LoRa.begin(434E6)) {
        while (1); // Freeze here only if hardware is missing/miswired. Should not be used for normal operation.
    }

    // Sends the command list to the GS at the start of the program.
    sendCommandList();
}

void loop() {
    
    if (obc.available()) {
        // Read the incoming data from ADCS and send it over LoRa
        String obc_data = obc.readStringUntil('\n'); // Read until newline character
        LoRa.beginPacket();
        LoRa.print(obc_data);
        LoRa.endPacket();
    }

    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        command = "";
        
        // Read and save the incoming command
        while (LoRa.available()) {
            command += (char)LoRa.read();
        }
        // Handle the command
        CommandHandler(command);
    }
}

void sendCommandList() 
{
    LoRa.beginPacket();
    LoRa.println("[Group 1] | COMMANDS LIST");
    LoRa.println("-------------------------");
    LoRa.println("- ALL : Returns data of the satellite in the form `[timestamp] [team name] TEMPOBC | TEMPEPS | TEMPADCS | VBAT | Camera_pointing_status`");
    LoRa.println("- TEMPOBC : Returns the temperature of the OBC board");
    LoRa.println("- TEMPEPS : Returns the temperature of the EPS (Electrical Power System) board");
    LoRa.println("- TEMPADCS : Returns the temperature of the ADCS board");
    LoRa.println("- VBAT : Returns the remaining battery charge in V");
    LoRa.println("- PHOTO : After 10s, takes a photo from the satellite and transmits the raw data back in bytes. Open the image_receiver.py file and be ready to switch port access.");
    LoRa.println("- POS_State : Returns the camera pointing status");
    LoRa.println("- SUN : Triggers rotation in response to an incorrect pointing status");
    LoRa.println("- IMUDATA : Returns the data of the IMU");
    LoRa.println("- DETUMBLE : Triggers the detumbling of the satellite");
    LoRa.println("- HELP : Returns the command list");
    LoRa.println("-------------------------");
    LoRa.endPacket();
}

void CommandHandler(String command) {
    /*
    Handles TEMPOBC, TEMPEPS, VBAT and PHOTO commands locally
    Forwards ADCS commands to adcs board
    */
    if (command == ""){
        return;
    }
    time_LoRa(); // Send timestamp for all commands
    if (command == "ALL") {
        time_LoRa();
        print_all();
    }
    else if (command == "TEMPOBC") {
        OBC_temp();
    }
    else if (command == "TEMPEPS") {
        EPS_temp();
    }
    else if (command == "TEMPADCS") {
        LoRa.beginPacket();
        LoRa.print("[Group 1] | TEMPADCS = ");
        LoRa.endPacket();
        obc.print("TEMPADCS");
    }
    else if (command == "VBAT") {
        vbat_send();
    }
    else if (command == "PHOTO") {
        PHOTO();
    }
    else if (command == "POS_State") {
        LoRa.beginPacket();
        LoRa.print("[Group 1] | POS_State = ");
        LoRa.endPacket();
        obc.print("POS_State");
    }
    else if (command == "SUN") {
        obc.print("SUN");
    }
    else if (command == "IMUDATA") {
        LoRa.beginPacket();
        LoRa.print("[Group 1] | IMUDATA = ");
        LoRa.endPacket();
        obc.print("IMUDATA");
    }
    else if (command == "DETUMBLE") {
        obc.print("DETUMBLE");
    }
    else if (command == "HELP") {
        sendCommandList();
    }
}

void OBC_temp() {
  /* Read voltage, convert to degrees at 10mV/°C*/
  float obc_temp = analogRead(A1)*5 / (1024*0.01); 

  Serial.println("Sending OBC_temp");

  LoRa.beginPacket();
  LoRa.print("[Group 1] | TEMPOBC = ");
  LoRa.print(obc_temp);
  LoRa.println("degrees Celsius");
  LoRa.endPacket();
  return;
}

void EPS_temp() {
  /* Read voltage, convert to degrees at 10mV/°C*/
  float eps_temp = analogRead(A2)*5 / (1024*0.01); 

  Serial.println("Sending EPS_temp");

  LoRa.beginPacket();
  LoRa.print("[Group 1] | TEMPEPS = ");
  LoRa.print(eps_temp);
  LoRa.println("degrees Celsius");
  LoRa.endPacket();
  return;
}

void time_LoRa() {
  int s_now = millis() / 1000;
  int m_now = s_now / 60;
  s_now = s_now % 60;

  LoRa.beginPacket();
  LoRa.print("[");
  LoRa.print(m_now);
  LoRa.print("m");
  LoRa.print(s_now);
  LoRa.print("s");
  LoRa.print("]");
  LoRa.endPacket();
  return;
}

void vbat_send() {
  float vbat = analogRead(A0)*5 / 1023;

  LoRa.beginPacket();
  LoRa.print("[Group 1] | VBAT = ");
  LoRa.print(vbat);
  LoRa.println("V");
  LoRa.endPacket();
  return;
}

void PHOTO() {
    delay(10000); // Delay 10s to allow GS to open image receiver program

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

    // Set Image size manually
    //cam.setImageSize(VC0706_640x480);        // biggest
    //cam.setImageSize(VC0706_320x240);        // medium
    cam.setImageSize(VC0706_160x120);          // small

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
