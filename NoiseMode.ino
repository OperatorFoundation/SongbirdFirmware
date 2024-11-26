float minNoiseAmplitude = 0.1;
float maxNoiseAmplitude = 1.0; 
float noiseAmplitude = 0.0;

int minNoiseSpan = 9; //milliseconds;
int maxNoiseSpan = 13; // milliseconds;
unsigned long noiseSpan = 0;
unsigned long noiseMaintenanceTimestamp = 0;

void startNoise()
{
  setNoiseLevels();
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
  pinkNoise.amplitude(noiseAmplitude);
}

void stopNoise()
{
  pinkNoise.amplitude(0);
}

float randomFloat(float min, float max)
{
  int randInt = random(0, 256);
  float range = max - min;
  float result = ((randInt/256) * range) + min;

  return result;
}
