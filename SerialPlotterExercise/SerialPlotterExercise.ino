#include <math.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial); // Wait for serial monitor to open

  Serial.println("Ready to plot data");
}

void loop() {
  // Calculate values
  float obc_temp = analogRead(A1)*5 / (1024*0.01);
  float eps_temp = analogRead(A2)*5 / (1024*0.01);
  float vbat = analogRead(A0)*5 / 1024;

  // Prints comma-separated values to monitor and plotter
  Serial.println("TEMPOBC:" + String(obc_temp) + ",TEMPEPS:" + String(eps_temp) + ",VBAT:" + String(vbat));

  //highkey lazy to do the variance stuff, see how ltr
}

