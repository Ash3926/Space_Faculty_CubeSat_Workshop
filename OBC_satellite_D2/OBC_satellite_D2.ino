// Arduino code to run CubeSat circuit
// C++ Code

#include <string.h>
#include <SPI.h>
#include <LoRa.h>

int elapsed_seconds = 0;
int elapsed_min = 0;
int reset_min = 0;
int reset_s = 0;

void setup()
{
	// put your setup code here, to run once:
   	Serial.begin(9600);
    Serial.print("Input Functions:\n TIMEOBC - Print time since system start\n RESETTIME - Saves reset point for RTIMEOBC\n RTIMEOBC - Print time since RESETTIME\n TEMPOBC - Print temperature sensor reading\n");
}

void loop()
{
  elapsed_seconds = (millis() / 1000) - (60 * elapsed_min);
  
  if (elapsed_seconds >= 60)
  {
    elapsed_min += 1;
    elapsed_seconds = elapsed_seconds - 60;
  }
  
  String incoming = Serial.readString();
  
  if (incoming == "TIMEOBC\n")
  {
    /*
    Prints time since system start
    */
    Serial.print("TIMEOBC = ");
    Serial.print(elapsed_min);
    Serial.print("m ");
    Serial.print(elapsed_seconds);
    Serial.print("s\n");
  }

  if (incoming == "RESETTIME\n")
  {
    /*
    Saves reset point for RTIMEOBC 
    */
    reset_min = elapsed_min;
    reset_s = elapsed_seconds;
  }
  
  if (incoming == "RTIMEOBC\n")
  {
    /*
    Prints time since last reset
    */
    Serial.print("RTIMEOBC = ");
    if (elapsed_seconds < reset_s)
      {
        elapsed_seconds += 60;
        elapsed_min -= 1;
      }
    Serial.print(elapsed_min - reset_min);
    Serial.print("m ");
    Serial.print(elapsed_seconds - reset_s);
    Serial.print("s\n");
  }

  if (incoming == "TEMPOBC\n")
  {
    /* 
    Reads and outputs the temperature of the OBC
    */
    int temp_val = analogRead(A1) / 10 ;	/* Read voltage, convert to degrees at 10mV/°C*/

    Serial.print("TEMPOBC = ");
    Serial.print(temp_val);
    Serial.print("°C\n");
  }
}