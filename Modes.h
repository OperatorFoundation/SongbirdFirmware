#ifndef _MODES_H_
#define _MODES_H_

enum SoundMode
{
  NOISE,
  PITCHSHIFT,
  TONESWEEP,
  PARTY,
  NONE
};

SoundMode currentMode = NONE;

// In order to have a function that returns an enum type, the enum type definition and the function definition must be in a .h file. This is a side effect of Arduino code reordering.
SoundMode getCurrentMode()
{
  return currentMode;
}

#endif
