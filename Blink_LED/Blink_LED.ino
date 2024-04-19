void setup() {
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
}

void turnOnLED(bool LEDSTATE){
  digitalWrite(D3, LEDSTATE);
  digitalWrite(D4, !LEDSTATE);
  delay(1000);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
}