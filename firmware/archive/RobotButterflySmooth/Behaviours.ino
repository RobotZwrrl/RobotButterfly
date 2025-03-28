
void distanceFlap() {
  
  uint8_t dist_min = 5;  // distance range = 5 - 20 cm
  uint8_t dist_max = 20;
  uint16_t offset = 600; // offset from wings up position

  double *distances = HCSR04.measureDistanceCm();
  double dist_cm = distances[0];
  if(dist_cm < 0) return;

  int avg = (int)floor( ultrasonicAvg.reading(dist_cm) );
  if(avg > dist_max) avg = dist_max;
  if(avg < dist_min) avg = dist_min;
  uint16_t neo_map = (int)map(avg, dist_min, dist_max, 0, NEOPIXEL_COUNT);

  Serial << "avg = " << avg << " cm";
  Serial << " map = " << neo_map << endl;

  if(millis()-last_dist_servo >= 100) {
    uint16_t left_map = map(avg, dist_min, dist_max, SERVO_LEFT_UP, SERVO_LEFT_HOME+offset);
    uint16_t right_map = map(avg, dist_min, dist_max, SERVO_RIGHT_UP, SERVO_RIGHT_HOME-offset);
    wing_left.current_pos = left_map;
    wing_right.current_pos = right_map;
    Serial << avg << " : wing_left = " << wing_left.current_pos << ", wing_right = " << wing_right.current_pos << endl;
    wing_left.motor.writeMicroseconds( wing_left.current_pos );
    wing_right.motor.writeMicroseconds( wing_right.current_pos );
    last_dist_servo = millis();
  }

  for(int i=0; i<strip.numPixels(); i++) {
    if(i<neo_map) {
      patternPurplePink(i);
      neoSparkle();
    } else {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
    if(neo_map == 0) {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
  }
  strip.show();

}


void gentleFlap() {

  uint8_t speed = 30; // degrees per second

  uint16_t resolution = 500; // how smoothe the movement is based on the scale of mapping the resolution to the movement positions
  uint8_t step = 10; // how many microseconds to advance each iteration ... to make it go faster, increase this
  uint16_t offset = 500; // offset from up position
  uint16_t del_closed = 250; // how long wings are closed in milliseconds
  uint16_t del_opened = 100; // how long wings are opened in milliseconds

  if(FLAP_STEP == STEP1 && millis()-last_step >= del_closed) {

    // up to down
    wing_left.current_pos = SERVO_LEFT_HOME+offset;
    wing_right.current_pos = SERVO_RIGHT_HOME-offset;
    wing_left.motor.startEaseTo(wing_left.current_pos, speed);
    wing_right.motor.startEaseTo(wing_right.current_pos, speed);
    
    while(ServoEasing::areInterruptsActive()) {
      // mini updates
      neoUpdate();
      buttonUpdate();
    }

    FLAP_STEP = STEP2;
    last_step = millis();
  }

  if(FLAP_STEP == STEP2 && millis()-last_step >= del_opened) {

    // down to up
    wing_left.current_pos = SERVO_LEFT_UP;
    wing_right.current_pos = SERVO_RIGHT_UP;
    wing_left.motor.startEaseTo(wing_left.current_pos, speed);
    wing_right.motor.startEaseTo(wing_right.current_pos, speed);
    
    while(ServoEasing::areInterruptsActive()) {
      // mini updates
      neoUpdate();
      buttonUpdate();
    }

    FLAP_STEP = STEP1;
    last_step = millis();
  }

}


