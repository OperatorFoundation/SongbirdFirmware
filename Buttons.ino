// Wondering which Bounce2 library to install? It's included in the Teensy core.
// https://www.pjrc.com/teensy/td_libs_Bounce.html
#include <Bounce2.h> 

// Button pins
#define SW_LEFT_PIN 6
#define SW_RIGHT_PIN 3
#define SW_UP_PIN 5
#define SW_DOWN_PIN 4

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

  if (buttons[0].fallingEdge()) // LEFT
  {
    Serial.println("LEFT Button");
    previousMode();
    // playNextFile();
  }

  if (buttons[1].fallingEdge()) // RIGHT
  {
    Serial.println("RIGHT Button");
    incrementMode();
  }

  if (buttons[2].fallingEdge()) // TOP
  {
    Serial.println("TOP Button");
    passthroughMode();
  }

  if (buttons[3].fallingEdge()) // MIDDLE
  {
    Serial.println("MIDDLE Button");
    toggleDevMode();
  }  
}
