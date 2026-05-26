int IN1 = 9;
int IN2 = 10;
int MP = 6;
int s1 = 0, s2 = 0, s3 = 0;
// Initialize time check variables
unsigned long prevtime = 0;
unsigned long rotatetime = 0;
int rotatecommand = 1;

void setup() {
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(MP, OUTPUT);

  prevtime = millis();
  rotatetime = millis();
  }

void loop() {

  // 5s sample time, checks that time since last sample is >= 5s before resampling
  if (millis() - prevtime >= 5000)
  {
    s1 = analogRead(A0);
    s2 = analogRead(A1);
    s3 = analogRead(A2);
    Serial.print("S1 Reading: ");
    Serial.println(s1);
    Serial.print("S2 Reading: ");
    Serial.println(s2);
    Serial.print("S3 Reading: ");
    Serial.println(s3);
    prevtime = millis();
    rotatecommand = 1;
  }

  // If previous rotate command has run for 1s, stoprotate
  if (millis() - rotatetime > 1000)
  {
    stoprotate();
    rotatecommand = 0;
  }

  if (rotatecommand == 1)
  {
    // Checks the sun sensor readings, attempts to rotate clockwise or counterclockwise according to the current sunlight facing
    if ((s1-s2) < 50 && (s3-s2) < 50){ // Back sensor has most sun - correct orientation, no further action
      stoprotate();    
      rotatetime = millis();
    }
    else if ((s1-s2) > 50){ // Left sensor has more sun, motor spins right to rotate the satellite left
      rotateR();
      rotatetime = millis(); // Saves time of rotation for comparison
    }

    else{ // Right sensor more sun, motor spins left to rotate the satellite right
      rotateL();
      rotatetime = millis(); // Saves time of rotation for comparison
    }
  }
}

void rotateR(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(MP, 200);
  }

void rotateL(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(MP, 200);
}

void stoprotate(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(MP, 200);
}
