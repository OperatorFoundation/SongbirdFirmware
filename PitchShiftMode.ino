void setupPitchShift()
{
  iqmixer.frequency(frequencyLO);   // Frequency shift, Hz
  sum.gain(0, gain*sign);           // Set gains
  sum.gain(1, gain);
}

void startPitchShift()
{
  effectsMixer.gain(PITCHSHIFTCHANNEL, audioLevelPitchshift); // Turn on pitch-shifted audio
  effectsMixer.gain(PASSTHROUGHCHANNEL, 0); // Turn off pass-through audio
  effectsMixer.gain(NOISECHANNEL, 0);
  effectsMixer.gain(WAVEFORMCHANNEL, 0);
}

void maintainShift()
{
}

void stopPitchShift()
{
  effectsMixer.gain(PASSTHROUGHCHANNEL, audioLevelPassthrough); // Turn on pass-through audio
  effectsMixer.gain(PITCHSHIFTCHANNEL, 0); // Turn off pitch-shifted audio
  effectsMixer.gain(NOISECHANNEL, 0);
  effectsMixer.gain(WAVEFORMCHANNEL, 0);
}
