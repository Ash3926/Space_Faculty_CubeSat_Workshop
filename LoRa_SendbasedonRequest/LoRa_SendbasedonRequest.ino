#include <SPI.h>
#include <LoRa.h>

String request = "\0";

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for serial monitor to open

  Serial.println("Initializing LoRa...");

  // Only initialize the LoRa chip ONCE here
  if (!LoRa.begin(434E6)) {
    Serial.println("Starting LoRa failed! Check your wiring.");
    while (1); // Freeze here only if hardware is missing/miswired
  }

  Serial.println("LoRa Initialization Successful!");

  LoRa.beginPacket();
  LoRa.println("[Group 1] | COMMANDS LIST");
  LoRa.println("-------------------------");
  LoRa.println(" - TEMPOBC\n - TEMPEPS\n - TIME\n - TEMPCOMPARISON\n - VBAT\n - ALL");
  LoRa.endPacket();
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

  Serial.println("Sending Time");

  LoRa.beginPacket();
  LoRa.print("[Group 1] | TIME = ");
  LoRa.print(m_now);
  LoRa.print("m");
  LoRa.print(s_now);
  LoRa.println("s");
  LoRa.endPacket();
  return;
}

void temp_comparison() {
  float obc_temp = analogRead(A1)*5 / (1023*0.01);
  float eps_temp = analogRead(A2)*5 / (1023*0.01); 
  float diff = (obc_temp > eps_temp ? obc_temp-eps_temp : eps_temp-obc_temp);

  String larger = (obc_temp > eps_temp ? "TEMPOBC is higher than TEMPEPS by " : "TEMPEPS is higher than TEMPOBC by ");

  LoRa.beginPacket();
  LoRa.print(larger);
  LoRa.print(diff);
  LoRa.println("degrees Celsius");
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

void loop() {
  // --- 1. TRANSMIT SECTION ---
  if (request == "TEMPOBC\n")
  {
    OBC_temp();
  }
  if (request == "TEMPEPS\n")
  {
    EPS_temp();
  }
  if (request == "TIME\n")
  {
    time_LoRa();
  }
  if (request == "TEMPCOMPARISON\n")
  {
    temp_comparison();
  }
  if (request == "VBAT\n")
  {
    vbat_send();
  }
  if (request == "ALL\n")
  {
    OBC_temp();
    EPS_temp();
    time_LoRa();
    temp_comparison();
    vbat_send();
  }

  // --- 2. RECEIVE / LISTEN SECTION ---
  Serial.println("Listening for 5 seconds...");
  
  unsigned long startListenTime = millis();
  
  // Instead of delay(5000), we loop and check for incoming packets for 5 seconds
  while (millis() - startListenTime < 5000) {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      String request = "";
      Serial.print("Received string: ");
      
      // Read and print the incoming message
      while (LoRa.available()) {
        request += (char)LoRa.read();
      }
      Serial.print(request);
      Serial.println(); // New line after message
    }
  }
}