// ------------------------------------
// --------- Button Callbacks ---------
// ------------------------------------

// give user feedback that they have held the
// button and its time to to release the button
void buttonHoldNotificationCallback(uint8_t n) {
  Serial << "hiya!" << endl;
}

// do an action here when the button is clicked
void buttonClickCallback(uint8_t n) {
  switch(n) {
    case BUTTON_BOTH:
    break;
    case BUTTON_LEFT:
    break;
    case BUTTON_RIGHT:
    break;
  }
}

void buttonReleaseCallback(uint8_t n) {

}

