#define CLOCK_PIN 3
#define SPEED_UP 0
#define SPEED_DOWN 1
long del = 60770;

void setup() {
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(SPEED_UP, INPUT);
  pinMode(SPEED_DOWN, INPUT);
}

void loop() {
  digitalWrite(CLOCK_PIN, HIGH);
  delay(1030);
  digitalWrite(CLOCK_PIN, LOW);
  delay(del);               

  if(digitalRead(UP) == HIGH) del += 5;
  if(digitalRead(DOWN) == HIGH) del -= 5;
}
