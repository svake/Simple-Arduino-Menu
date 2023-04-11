#include <U8x8lib.h>

#define BUTTON_PIN 1
#define LONG_PRESS_TIME 1000 // in milliseconds
#define DEBOUNCE_TIME 50 // in milliseconds

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

// Menu Items
const char* menuItems[3][3] = {
  {"Menu 1", "Menu 2", "Menu 3"},
  {"Number 1", "Number 2", "Boolean 1"},
  {"Option 1", "Option 2", "Option 3"}
};

int currentLevel = 0;
int currentSelection = 0;
int currentValue = 0;
bool currentBoolean = false;
bool buttonPressed = false;
unsigned long buttonPressStart = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  u8x8.begin();
  u8x8.setFont(u8x8_font_7x14_1x2_r);
}

void loop() {
  checkButton();
  displayMenu();
}

void checkButton() {
  int buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW && !buttonPressed) {
    buttonPressed = true;
    buttonPressStart = millis();
  }

  if (buttonState == HIGH && buttonPressed) {
    buttonPressed = false;
    unsigned long buttonPressDuration = millis() - buttonPressStart;
    if (buttonPressDuration < DEBOUNCE_TIME) {
      return;
    }
    if (buttonPressDuration >= LONG_PRESS_TIME) {
      onLongPress();
    } else {
      onShortPress();
    }
  }
}

void onShortPress() {
  if (currentLevel == 0) {
    currentSelection++;
    if (currentSelection > 2) {
      currentSelection = 0;
    }
  } else {
    switch (currentSelection) {
      case 0:
        currentValue++;
        if (currentValue > 9) {
          currentValue = 0;
        }
        break;
      case 1:
        currentValue++;
        if (currentValue > 9) {
          currentValue = 0;
        }
        break;
      case 2:
        currentBoolean = !currentBoolean;
        break;
    }
  }
}

void onLongPress() {
  if (currentLevel == 0) {
    currentLevel = 1;
  } else {
    // Register the value and return to the menu function
    currentLevel = 0;
  }
}

void displayMenu() {
  
  u8x8.clear();
  if (currentLevel == 0) {
    // First level menu
    u8x8.setCursor(0, 0);
    u8x8.print(menuItems[0][currentSelection]);
  } else {
    // Second level menu
    u8x8.setCursor(0, 0);
    u8x8.print(menuItems[1][currentSelection]);
    u8x8.setCursor(0, 1);
    switch (currentSelection) {
      case 0:
        u8x8.print(currentValue);
        break;
      case 1:
        u8x8.print(currentValue);
        break;
      case 2:
        if (currentBoolean) {
          u8x8.print("YES");
        } else {
          u8x8.print("NO");
        }
        break;
    }
  }
}
