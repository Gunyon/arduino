#define BUTTON 3
#define LED_PIN A0

bool button_state = false;
bool led_state = false;

void setup() {
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  bool button = !digitalRead(BUTTON);

  if (button == 1 && button_state == false) {
    button_state = true;
    Serial.println("Button Pressed");
    delay(100);
  } else if (button == 0 && button_state == true) {
    button_state = false;
    Serial.println("Button Released");
    led_state = !led_state;
    delay(100);
  }

  digitalWrite(LED_PIN, led_state);
}