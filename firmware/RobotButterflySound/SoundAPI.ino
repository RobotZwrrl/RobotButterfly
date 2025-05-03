void soundDoneCallback(uint8_t id) {
  if(DEBUG_BUZZER) Serial << "done sound (" << id << ")" << endl;
}


void playSound(uint8_t id) {
  CURRENT_SOUND = id;
}
