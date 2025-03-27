void setupPitchShift()
{
  iqmixer.frequency(frequencyLO);   // Frequency shift, Hz
  sum.gain(0, gain*sign);           // Set gains
  sum.gain(1, gain);
}

void startPitchShift()
{
  effectsMixer.gain(0, 3.0); // Turn on pitch-shifted audio
  effectsMixer.gain(3, 0); // Turn off pass-through audio
}

void maintainShift()
{
}

void stopPitchShift()
{
  effectsMixer.gain(0, 0); // Turn off pitch-shifted audio
  effectsMixer.gain(3, 0.5); // Turn on pass-through audio
}
