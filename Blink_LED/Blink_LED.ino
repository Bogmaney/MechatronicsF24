const int EN=9; //H-Br0 Enable
const int MC1=3; //Motor Control 1
const int MC2=2; //Motor Control 2

int velocity = 255; //Hastighed (0-255)



void setup() {
  // put your setup code here, to run once:
  pinMode(EN, OUTPUT);
  pinMode(MC1, OUTPUT);
  pinMode(MC2, OUTPUT);
  brake();
  Serial.begin(9600);
}

void loop()
{
  digitalWrite(EN, HIGH);
  digitalWrite(MC1, HIGH);
  digitalWrite(MC2, LOW);
  analogWrite(EN, velocity);
  delay(5000);
}

void forward(int rate) //Motor kører frem ved værdi mellem (0-255)
{
  digitalWrite(EN, LOW);
  digitalWrite(MC1, HIGH);
  digitalWrite(MC2, LOW);
  analogWrite(EN, rate);
}

void reverse(int rate) //Motor kører baglæns ved værdi mellem (0-255)
{
  digitalWrite(EN, LOW);
  digitalWrite(MC1, LOW);
  digitalWrite(MC2, HIGH);
  analogWrite(EN, rate);
}

void brake() //Motor bremser 
{
  digitalWrite(EN, LOW);
  digitalWrite(MC1, LOW);
  digitalWrite(MC2, LOW);
  digitalWrite(EN, HIGH);
}