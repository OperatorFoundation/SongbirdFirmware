#define GRANULAR_MEMORY_SIZE 12800  // enough for 290 ms at 44.1 kHz
int16_t granularMemory[GRANULAR_MEMORY_SIZE];

// More than 1.03 sounds too high and less than 0.98 sounds too low for the voice to be recognizable as that person, 
// lower than 0.98 could work nicely for something that sounds human but not the like the same person
float pitchShiftRatio = 0.98; 
float pitchShiftGrainLength = 23; // grainLength is specified in milliseconds, up to one third of the memory from begin()

void setupPitchShift()
{
  // TODO: Tweak the sample sizes based on relevant sizes: 1 msec, 3 msec, 25 msec
  // float someValue = 0.15;
  // float msec = 1.0 + (someValue * 99.0);
  // pitchShiftGrainLength = msec;
  
  // the Granular effect requires memory to operate
  pitchShifter.begin(granularMemory, GRANULAR_MEMORY_SIZE);
}

void setPitchShiftLevels()
{
  // TODO: We may want to do this based on some algorithmic value, the example is based on user input
  // // Continuously adjust the speed, based on the A3 pot
  // float ratio;
  // ratio = powf(2.0, knobA2 * 2.0 - 1.0); // 0.5 to 2.0
  // //ratio = powf(2.0, knobA2 * 6.0 - 3.0); // 0.125 to 8.0 -- uncomment for far too much range!
  // lPitchShift.setSpeed(ratio);
  // rPitchShift.setSpeed(ratio);
  pitchShifter.setSpeed(pitchShiftRatio);
}

// Pitch shift by continuously sampling grains and playing them at altered speed. The grainLength is specified in milliseconds, up to one third of the memory from begin();
void startPitchShift()
{
  /// 1. Pitch - Pitch Shifting
  /// 2. Pitch Velocity - How fast we are changing the pitch 
  /// 3. Pitch Acceleration - How fast we are changing the way we are changing the pitch
  setPitchShiftLevels();

  pitchShifter.beginPitchShift(pitchShiftGrainLength);
}

void maintainShift()
{
  setPitchShiftLevels();
}

void stopPitchShift()
{
  pitchShifter.stop();
}
