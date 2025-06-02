
float minNoiseAmplitude = 0.2;
float maxNoiseAmplitude = 1.0; 
float noiseAmplitude = 0.0;

int minNoiseSpan = 9; //milliseconds;
int maxNoiseSpan = 13; // milliseconds;
unsigned long noiseSpan = 0;
unsigned long noiseMaintenanceTimestamp = 0;

void startNoise()
{
  setNoiseLevels();

  effectsMixer.gain(PASSTHROUGHCHANNEL, audioLevelPassthrough);
  effectsMixer.gain(NOISECHANNEL, audioLevelNoise); // Turn on the noise channel
  effectsMixer.gain(WAVEFORMCHANNEL, 0);
  effectsMixer.gain(PITCHSHIFTCHANNEL, 0);
}

void maintainNoise()
{
  unsigned long now = millis();

  if ((now - noiseMaintenanceTimestamp) >= noiseSpan)
  {
    setNoiseLevels();
  }
}

void setNoiseLevels()
{
  // Set the maintenance timestamp
  noiseMaintenanceTimestamp = millis();

  // Set the noiseSpan
  noiseSpan = random(minNoiseSpan, maxNoiseSpan);

  // Get an amplitude
  noiseAmplitude = randomFloat(minNoiseAmplitude, maxNoiseAmplitude);

  // Set the output peak level, from 0 (off) to 1.0. The default is off. Noise is generated only after setting to a non-zero level.
  pinkNoise.amplitude(noiseAmplitude); // FIXME
}

void stopNoise()
{
  pinkNoise.amplitude(0);

  effectsMixer.gain(PASSTHROUGHCHANNEL, audioLevelPassthrough);
  effectsMixer.gain(NOISECHANNEL, 0); // Turn off the noise channel
  effectsMixer.gain(WAVEFORMCHANNEL, 0);
  effectsMixer.gain(PITCHSHIFTCHANNEL, 0);
}

float randomFloat(float min, float max)
{
  int randInt = random(0, 256);
  float range = max - min;
  float result = ((randInt/256) * range) + min;

  return result;
}
