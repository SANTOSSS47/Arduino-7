#include <Adafruit_NeoPixel.h>

#define LED_PIN 2
#define NUM_LEDS 48

#define BTN_BRAKE 3
#define BTN_LEFT 4
#define BTN_RIGHT 5

Adafruit_NeoPixel leds(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

#define BRAKE_START 0
#define BRAKE_END 39
#define LEFT_BLINKER_START 40
#define LEFT_BLINKER_END 43
#define RIGHT_BLINKER_START 44
#define RIGHT_BLINKER_END 47

unsigned long leftBlinkerLastTime = 0;
unsigned long rightBlinkerLastTime = 0;
bool leftBlinkerActive = false;
bool rightBlinkerActive = false;

void setup() {
  leds.begin();
  leds.clear();
  leds.show();

  pinMode(BTN_BRAKE, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(BTN_BRAKE) == LOW) {
    setSegment(BRAKE_START, BRAKE_END, leds.Color(255, 0, 0));
  } else {
    clearSegment(BRAKE_START, BRAKE_END);
  }

  handleBlinker(BTN_LEFT, LEFT_BLINKER_START, LEFT_BLINKER_END, leftBlinkerLastTime, leftBlinkerActive);
  handleBlinker(BTN_RIGHT, RIGHT_BLINKER_START, RIGHT_BLINKER_END, rightBlinkerLastTime, rightBlinkerActive);
}

void handleBlinker(int buttonPin, int start, int end, unsigned long &lastTime, bool &isActive) {
  if (digitalRead(buttonPin) == LOW) {
    isActive = true;
    lastTime = millis();
  }

  if (isActive) {
    unsigned long currentTime = millis();

    if ((currentTime / 500) % 2 == 0) {
      setSegment(start, end, leds.Color(255, 165, 0));
    } else {
      clearSegment(start, end);
    }

    if (currentTime - lastTime > 3000) {
      isActive = false;
      clearSegment(start, end);
    }
  }
}

void setSegment(int start, int end, uint32_t color) {
  for (int i = start; i <= end; i++) {
    leds.setPixelColor(i, color);
  }
  leds.show();
}

void clearSegment(int start, int end) {
  for (int i = start; i <= end; i++) {
    leds.setPixelColor(i, 0);
  }
  leds.show();
}
