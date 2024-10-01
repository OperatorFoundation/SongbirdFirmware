boolean dev_mode = true;

boolean isDevModeEnabled()
{
  return dev_mode;
}

void toggleDevMode()
{
  if (dev_mode)
  {
    dev_mode = false;
  }
  else
  {
    dev_mode = true;
  }

  setProductionDevMixer();
}
