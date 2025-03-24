// ------------------------------------
// --------- Sensor Callbacks ---------
// ------------------------------------
void sensorLightChangeCallback(struct Sensor *s, bool light_off) {

  if(light_off == false) {
    Serial << "light on!" << endl;
  } else {
    Serial << "light off!" << endl;
  }

}

// ------------------------------------


// ------------------------------------
// --------- Sensor Getters -----------
// ------------------------------------


// ------------------------------------

