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

void sensorLightAmbientChangeCallback(struct Sensor *s, int change) {
  
  Serial << "\r\n\r\n" << endl;
  
  Serial << "--------> 5 min ambient change detected! " << change << " ";

  if(change < 0) {
    Serial << "brighter" << endl;
  } else {
    Serial << "darker" << endl;
  }

  Serial << "\r\n\r\n" << endl;

}
// ------------------------------------


// ------------------------------------
// --------- Sensor Getters -----------
// ------------------------------------
uint16_t getSensorTemperatureRaw(struct Sensor *s) {
  return s->raw;
}

uint16_t getSensorTemperatureVal(struct Sensor *s) {
  return s->val;
}

uint16_t getSensorTemperatureAmbient(struct Sensor *s) {
  return s->ambient;
}
// ------------------------------------

