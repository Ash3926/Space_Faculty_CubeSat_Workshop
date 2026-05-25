#include <SPI.h>
#include <LoRa.h>


void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Initializing LoRa...");


  if (!LoRa.begin(430E6))
  {
    Serial.println("Starting LoRa failed! Check your wiring.");
    while (1);
  }

  Serial.println("LoRa Initialization Successful!");
}

void loop() {
  if (Serial.available() > 0) {
    String incoming = Serial.readStringUntil("\n");
    incoming.trim();

    int obc_temp = (analogRead(A1) * 5.0) / (1024.0 * 0.01); 
    int EPS_temp = (analogRead(A2) * 5.0) / (1024.0 * 0.01);

    if (incoming == "TEMPOBC") 
    {
      LoRa.beginPacket();
      LoRa.print("Group 1 | TEMPOBC = "); 
      LoRa.print(obc_temp);               
      LoRa.endPacket();
      Serial.println("TEMPOBC Sent");
      incoming = "/0";
    }
    else if (incoming == "TEMPEPS") 
    {
      LoRa.beginPacket();
      LoRa.print("Group 1 | TEMPEPS = ");
      LoRa.print(EPS_temp);
      LoRa.endPacket();
      Serial.println("TEMPEPS Sent");
      incoming = "/0";
    }
    else if (incoming == "TIME") 
    {
      LoRa.beginPacket();
      LoRa.print("Group 1 | TIME = ");
      LoRa.print(millis());
      LoRa.endPacket();
      Serial.println("TIME Sent");
      incoming = "/0";
    }
    else
    {
      LoRa.beginPacket();
      LoRa.print(incoming);
      LoRa.endPacket();
    }
  }
 
  else
  {
    Serial.println("Listening...");
    
    unsigned long startListenTime = millis();
    
    while (millis() - startListenTime < 5000) 
    {
      int packetSize = LoRa.parsePacket();
      if (packetSize) 
      {
        Serial.print("Received packet: ");
        
      
        while (LoRa.available()) {
          Serial.print((char)LoRa.read());
        }
        Serial.println(); 
      }
    }
  }
}