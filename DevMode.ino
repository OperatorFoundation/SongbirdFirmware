boolean dev_mode = false;

boolean isDevModeEnabled()
{
  return dev_mode;
}

void toggleDevMode()
{
  if (dev_mode)
  {
    dev_mode = false;
    Serial.println("Production mode");
  }
  else
  {
    dev_mode = true;
    Serial.println("Dev mode");
  }

  setProductionDevMixer();

  refreshDisplay();
}
