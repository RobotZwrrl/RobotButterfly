// uses the raw value to trigger the close proximity
// this is called at intervals defined by PROXIMITY_TRIGGER_FREQ
void proximityTriggerCallback(struct Proximity *p) {
  Serial << "----> Close proximity detected!" << endl;
}

// from 0-7 inclusive for feeding neopixel strip
// based on the moving average value
uint8_t getProximity8(struct Proximity *p) {
  return map(p->val, PROXIMITY_MIN, PROXIMITY_MAX, 0, 7);
}

// from 0-9 inclusive for feeding servo range animation
// based on the moving average value
uint8_t getProximity10(struct Proximity *p) {
  return map(p->val, PROXIMITY_MIN, PROXIMITY_MAX, 0, 9);
}


