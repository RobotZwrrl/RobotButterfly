// ------------------------------------
// --------- Sensor Callbacks ---------
// ------------------------------------
void sensorLightChangeCallback(struct Sensor *s, bool trigger_dir) {

  if(trigger_dir == false) {
    Serial << "---> Light on!";
  } else {
    Serial << "---> Light off!";
  }
  Serial << " (" << s->trig_count << ")" << endl;

}

void sensorLightAmbientChangeCallback(struct Sensor *s, int change) {
  
  Serial << "\r\n\r\n" << endl;
  
  Serial << "-------- Ambient change detected! -------- " << change << " ";

  if(change < 0) {
    Serial << "---> Light ambient brighter!" << endl;
  } else {
    Serial << "---> Light ambient darker!" << endl;
  }

  Serial << "\r\n\r\n" << endl;

}

void sensorTemperatureChangeCallback(struct Sensor *s, bool trigger_dir) {

  if(trigger_dir == false) {
    Serial << "---> Temperature warmer!";
  } else {
    Serial << "---> Temperature colder!";
  }
  Serial << " (" << s->trig_count << ")" << endl;

}

void sensorTemperatureAmbientChangeCallback(struct Sensor *s, int change) {
  
  Serial << "\r\n\r\n" << endl;
  
  Serial << "-------- Ambient change detected! -------- " << change << " ";

  if(change < 0) {
    Serial << "---> Temperature ambient warmer" << endl;
  } else {
    Serial << "---> Temperature ambient colder" << endl;
  }

  Serial << "\r\n\r\n" << endl;

}
// ------------------------------------


// ------------------------------------
// --------- Sensor Getters -----------
// ------------------------------------
// the most recent raw data reading
uint16_t getSensorRaw(struct Sensor *s) {
  return s->raw;
}

// sensor value through a moving average filter
uint16_t getSensorVal(struct Sensor *s) {
  return s->val;
}

// ambient value averaged over the course of minutes
uint16_t getSensorAmbient(struct Sensor *s) {
  return s->ambient;
}
// ------------------------------------

