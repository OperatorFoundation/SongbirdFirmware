/// Create a continuously varying (in frequency) sine wave
/// Note: Uses excessive CPU time.
/// The frequency actually changes in discrete steps every 128 samples (2.9 ms).
void startTonesweep()
{
  // specifies the amplitude,
  // start and end frequencies (which can sweep up or down)
  // and the length of time of the sweep.
  float sweepAmplitude = 0.02;
  int sweepLo = 10;
  int sweepHi = 50;
  float sweepTime = 0.02; // Length of time for the sweep in seconds

  // play(level, lowFreq, highFreq, time);
  // Start generating frequency sweep output. The time is specified in seconds. Level is 0 to 1.0
  tonesweep.play(sweepAmplitude, sweepLo, sweepHi, sweepTime);
}

void maintainTonesweep()
{
  if (!tonesweep.isPlaying())
  {
    startTonesweep();
  }
}
