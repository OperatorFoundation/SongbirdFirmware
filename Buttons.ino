void setupButtons()
{
  // Buttons
  // pull-ups are required, LOW = button pressed, HIGH = not pressed
  for (int index = 0; index < numButtons; index++) 
  {
    // pinMode(buttonPins[index], INPUT_PULLUP); // Set pins as INPUT with internal pull-up resistor
    buttons[index].attach(buttonPins[index], INPUT_PULLUP);  // Attach the switch pin to the Bounce object
    buttons[index].interval(5);            // Set debounce interval (in milliseconds)

    // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
    buttons[index].setPressedState(LOW);
  }
}
