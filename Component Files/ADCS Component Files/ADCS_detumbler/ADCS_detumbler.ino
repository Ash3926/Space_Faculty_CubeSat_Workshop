

int IN1 = 9;
int IN2 = 10;
int MP = 6;

void setup()
{
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  while (!Serial) 
  {
    delay(10);  
  }

  Serial.println("Detumbler code"); 
  
  delay(1000);
}

void detumbling()
{
  Serial.println("DETUMBLING STARTED");

  for (int i = 0; i < 2; i++)
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(MP, 200);
    delay(2000);

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(MP, 200);
    delay(2000);
  }

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(MP, 200);
  delay(2000);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(MP, 0);

  Serial.println("DETUMBLING COMPLETE");
}