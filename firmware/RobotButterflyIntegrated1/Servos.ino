void initServos() {

  if(initialised_servos) return;

  wing_left.name = "LEFT";
  wing_left.motor = s1;
  if(SERVO_LEFT_HOME == -99) {
    Serial << "!! Servo LEFT needs calibration" << endl;
    wing_left.calibrated = false;
  }
  wing_left.home_pos = SERVO_LEFT_HOME;
  wing_left.up_pos = SERVO_LEFT_UP;
  wing_left.down_pos = SERVO_LEFT_DOWN;
  wing_left.current_pos = SERVO_LEFT_UP;

  wing_left.motor.attach(SERVO_LEFT_PIN, SERVO_LEFT_UP, DEFAULT_MICROSECONDS_FOR_0_DEGREE, DEFAULT_MICROSECONDS_FOR_180_DEGREE);
  wing_left.active = true;
  wing_left.motor.setEasingType(EASE_QUADRATIC_IN_OUT);

  // ---

  wing_right.name = "RIGHT";
  wing_right.motor = s2;
  if(SERVO_RIGHT_HOME == -99) {
    Serial << "!! Servo RIGHT needs calibration" << endl;
    wing_right.calibrated = false;
  }
  wing_right.home_pos = SERVO_RIGHT_HOME;
  wing_right.up_pos = SERVO_RIGHT_UP;
  wing_right.down_pos = SERVO_RIGHT_DOWN;
  wing_right.current_pos = SERVO_RIGHT_UP;

  wing_right.motor.attach(SERVO_RIGHT_PIN, SERVO_RIGHT_UP, DEFAULT_MICROSECONDS_FOR_0_DEGREE, DEFAULT_MICROSECONDS_FOR_180_DEGREE);
  wing_right.active = true;
  wing_right.motor.setEasingType(EASE_QUADRATIC_IN_OUT);
  
  // ---

  wing_left.motor.startEaseTo( SERVO_LEFT_UP, 10 );
  wing_right.motor.startEaseTo( SERVO_RIGHT_UP, 10 );

  initialised_servos = true;

}

