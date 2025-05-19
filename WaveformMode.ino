/// Create a continuously varying (in frequency) sine wave
/// Note: Uses excessive CPU time.
/// The frequency actually changes in discrete steps every 128 samples (2.9 ms).

bool waveformPlaying = true;
unsigned long lastUpdate = 0;
const float AMPLITUDE = 1.0;


// Constants for frequency parameters - Full human voice range
const float BASE_FREQUENCY = 185;      // Center frequency in Hz (near middle of human voice range 85-420 Hz)
const float FREQUENCY_RANGE = 100;     // How much the frequency varies (+/-)
                                         // This gives a full range from 85Hz to 420Hz
                                         // Human voice generally ranges from ~85Hz (lower registers) to ~420Hz (higher registers)

// Slow Modulation
const float SLOW_MOD_FREQUENCY = 0.5; // Hz
const float SLOW_MOD_AMPLITUDE = 0.6; // 60% contribution

// Medium Modulation
const float MED_MOD_FREQUENCY = 1.7; // Hz
const float MED_MOD_AMPLITUDE = 0.3; // 30% contribution

// Fast Modulation
const float FAST_MOD_FREQUENCY = 4.3; // Hz
const float FAST_MOD_AMPLITUDE = 0.1; // 10% Contribution

const unsigned long UPDATE_INTERVAL = 10; // The human ear can't detect frequency changes faster than about 20ms, so updating at 10ms ensures smooth sound while reducing processing load


void setupWaveform()
{
  waveform.frequency(BASE_FREQUENCY);
  waveform.amplitude(AMPLITUDE);
}

void startWaveform()
{    
    // Restore amplitude
    waveform.amplitude(AMPLITUDE);
    
    // Restore mixer gain
    effectsMixer.gain(2, 0.1);
    
    waveformPlaying = true;
}

void maintainWaveform()
{
  if (waveformPlaying)
  {
    updateFrequency();
  }
}

void stopWaveform()
{
  if (waveformPlaying)
  {
    // Set amplitude to 0 to stop the sound
    waveform.amplitude(0);
    
    // Update mixer gain to 0
    effectsMixer.gain(2, 0);
    
    waveformPlaying = false;
  }
}

/* 
 * FREQUENCY MODULATION EXPLANATION:
 * 
 * This code creates a complex, human-like frequency variation by combining 
 * three separate sine wave modulation patterns that operate at different speeds:
 * 
 * 1. Slow modulation (0.5 Hz) - creates long, sweeping changes (2 second cycle)
 * 2. Medium modulation (1.7 Hz) - adds medium variations (0.6 second cycle)
 * 3. Fast modulation (4.3 Hz) - adds micro-variations (0.23 second cycle)
 * 
 * These three patterns combine mathematically into a single modulation factor,
 * which is then used to calculate ONE new frequency at each update.
 * 
 * Example calculation:
 * If modulation1 = 0.3, modulation2 = -0.15, modulation3 = 0.05
 * Then: modulationFactor = 0.3 + (-0.15) + 0.05 = 0.2
 * newFrequency = 252.5 + (0.2 * 167.5) = 286 Hz
 * 
 * While the sine wave only plays one frequency at a time, the complexity
 * comes from how that frequency changes over time in a non-repeating pattern.
 * 
 * The frequency range covers the full human vocal range from approximately 
 * 85Hz (lower vocal registers) to 420Hz (higher vocal registers).
 */

void updateFrequency()
{
  unsigned long currentTime = millis();

  // CPU optimization: only update frequency every UPDATE_INTERVAL milliseconds
  if (currentTime - lastUpdate < UPDATE_INTERVAL) { return; }

  // Save the current time for the next comparison
  lastUpdate = currentTime;

  // Convert time to seconds for modulation calculations
  float timeInSeconds = currentTime / 1000.0;

  // Calculate modulation factors for the three components
  float modulation1 = sin(TWO_PI * SLOW_MOD_FREQUENCY * timeInSeconds) * SLOW_MOD_AMPLITUDE;
  float modulation2 = sin(TWO_PI * MED_MOD_FREQUENCY * timeInSeconds) * MED_MOD_AMPLITUDE;
  float modulation3 = sin(TWO_PI * FAST_MOD_FREQUENCY * timeInSeconds) * FAST_MOD_AMPLITUDE;

  // Combine into a single modulation factor
  float modulationFactor = modulation1 + modulation2 + modulation3;

  // Calculate and apply the new frequency
  float newFrequency = BASE_FREQUENCY + (modulationFactor * FREQUENCY_RANGE);
  waveform.frequency(newFrequency);
}
