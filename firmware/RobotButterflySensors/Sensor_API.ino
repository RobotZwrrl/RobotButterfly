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

void sensorTemperatureChangeCallback(struct Sensor *s, bool light_off) {

  if(light_off == false) {
    Serial << "warmer!" << endl;
  } else {
    Serial << "colder!" << endl;
  }

}

void sensorTemperatureAmbientChangeCallback(struct Sensor *s, int change) {
  
  Serial << "\r\n\r\n" << endl;
  
  Serial << "--------> 5 min ambient change detected! " << change << " ";

  if(change < 0) {
    Serial << "warmer" << endl;
  } else {
    Serial << "colder" << endl;
  }

  Serial << "\r\n\r\n" << endl;

}
// ------------------------------------


// ------------------------------------
// --------- Sensor Getters -----------
// ------------------------------------
uint16_t getSensorRaw(struct Sensor *s) {
  return s->raw;
}

uint16_t getSensorVal(struct Sensor *s) {
  return s->val;
}

uint16_t getSensorAmbient(struct Sensor *s) {
  return s->ambient;
}
// ------------------------------------

