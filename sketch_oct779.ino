const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
const int buttonPin = 2;

const bool commonAnode = true;
const int pressedLevel = HIGH;

int mood = 10;

int lastReading = LOW;
int stableButton = LOW;
unsigned long lastChangeTime = 0;
const unsigned long debounceTime = 30;

unsigned long lastPetTime = 0;
unsigned long lastDropTime = 0;

const unsigned long lonelyTime = 5000;
const unsigned long dropInterval = 1000;

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buttonPin, INPUT);

  Serial.begin(9600);

  lastReading = digitalRead(buttonPin);
  stableButton = lastReading;

  lastPetTime = millis();
  lastDropTime = lastPetTime;

  showMood();
  Serial.println(mood);
}

void loop() {
  unsigned long now = millis();

  int reading = digitalRead(buttonPin);

  if (reading != lastReading) {
    lastChangeTime = now;
    lastReading = reading;
  }

  if (now - lastChangeTime >= debounceTime) {
    if (reading != stableButton) {
      stableButton = reading;

      if (stableButton == pressedLevel) {
        mood = min(mood + 1, 20);

        lastPetTime = now;
        lastDropTime = now;

        Serial.println(mood);
      }
    }
  }
  if (now - lastPetTime >= lonelyTime &&
      now - lastDropTime >= dropInterval) {

    mood = max(mood - 1, 0);
    lastDropTime = now;

    Serial.println(mood);
  }

  showMood();
}

void showMood() {
  int red;
  int green;
  int blue;

  if (mood <= 10) {
    red = map(mood, 0, 10, 255, 0);
    green = map(mood, 0, 10, 0, 255);
    blue = 0;
  } else {
    red = 0;
    green = map(mood, 10, 20, 255, 0);
    blue = map(mood, 10, 20, 0, 255);
  }

  if (commonAnode) {
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  }

  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}