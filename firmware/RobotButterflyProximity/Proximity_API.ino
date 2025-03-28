void proximityTriggerCallback(struct Proximity *p) {
  Serial << "----> Close proximity detected!" << endl;
}

// from 0-7 inclusive for feeding neopixel strip
uint8_t getProximity8(struct Proximity *p) {
  return map(p->raw, PROXIMITY_MIN, PROXIMITY_MAX, 0, 7);
}

// from 0-9 inclusive for feeding servo range animation
uint8_t getProximity10(struct Proximity *p) {
  return map(p->raw, PROXIMITY_MIN, PROXIMITY_MAX, 0, 9);
}


