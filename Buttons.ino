#include <Bounce2.h>            // https://www.pjrc.com/teensy/td_libs_Bounce.html

// Button pins
#define SW_LEFT_PIN 26
#define SW_RIGHT_PIN 29
#define SW_UP_PIN 28
#define SW_DOWN_PIN 27

const int numButtons = 4;
const int buttonPins[numButtons] = {SW_LEFT_PIN, SW_RIGHT_PIN, SW_UP_PIN, SW_DOWN_PIN};
Button buttons[numButtons];

void setupButtons()
{
  // Buttons
  // pull-ups are required, LOW = button pressed, HIGH = not pressed
  for (int index = 0; index < numButtons; index++) 
  {
    buttons[index].attach(buttonPins[index], INPUT_PULLUP);  // Attach the switch pin to the Bounce object
    buttons[index].interval(5);            // Set debounce interval (in milliseconds)

    // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
    buttons[index].setPressedState(LOW);
  }
}

void handleButtons()
{
  for (int index = 0; index < numButtons; index++) 
  {
    buttons[index].update();
  }  

  if (buttons[0].fallingEdge())
  {
    Serial.println("B0");

    incrementCurrentTest();
  }

  if (buttons[1].fallingEdge())
  {
    Serial.println("B1");

    incrementMode();
  }

  if (buttons[2].fallingEdge())
  {
    Serial.println("B2");

    toggleDevMode();
  }

  if (buttons[3].fallingEdge())
  {
    Serial.println("B3");
  }  
}
