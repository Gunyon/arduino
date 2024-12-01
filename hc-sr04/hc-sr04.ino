const int triggerPin = 7; // Pinul de trigger pentru senzorul HC-SR04
const int echoPin = 6;    // Pinul de echo pentru senzor
const int ledPin = 9;     // Pinul PWM pentru banda LED

unsigned long previousMillis = 0;
const long interval = 20; // Intervalul pentru schimbarea intensității
const long delayMillis = 2000; // Timpul de asteptare inainte de a schimba intensitatea
unsigned long startMillis = 0;
int ledIntensity = 0;
int ledIntensityStep = 3;
bool ledIncreasing = false;
bool inZone = false;
const int proximityThreshold = 100; // Distanța de 1 metru
bool delayPassed = false;

void setup() {
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  long duration;
  int distance;

  // Trimit semnal de trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  // Citire durata semnal
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; // Calculez distanța în cm

  Serial.print("distance: ");
  Serial.println(distance);

  // Verific dacă cineva este la mai puțin de 1 metru
  if (distance <= proximityThreshold) {
    if (!inZone && (ledIntensity == 0 || ledIntensity == 255)) {
      startMillis = millis(); // Începem cronometrul de 5 secunde
      delayPassed = false;
    }
    inZone = true;
  } else {
    if (inZone && (ledIntensity == 0 || ledIntensity == 255)) {
      startMillis = millis(); // Începem cronometrul de <delayMillis> secunde
      delayPassed = false; // Resetăm temporizatorul dacă nu mai este în zonă
    }
    inZone = false;
  }

  // Verificăm dacă au trecut <delayMillis> secunde
  if (!delayPassed && (millis() - startMillis >= delayMillis)) {
    delayPassed = true; // Setăm că au trecut <delayMillis> secunde
  }

  // Actualizez intensitatea luminii doar după ce au trecut <delayMillis> secunde
  if (delayPassed) {
    Serial.println("delayPassed");
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      if (inZone && ledIntensity < 255) {
        ledIntensity = constrain(ledIntensity + ledIntensityStep, 0, 255);
        analogWrite(ledPin, ledIntensity);
        Serial.println("Aprindem");
      } else if (!inZone && ledIntensity > 0) {
        Serial.println("Stringem");
        ledIntensity = constrain(ledIntensity - ledIntensityStep, 0, 255);
        analogWrite(ledPin, ledIntensity);
      }
    }
  }

  Serial.print("ledIntensity: ");
  Serial.println(ledIntensity);

  delay(1);
}
