// TODO: the packet has to be sent to topic /data
// TODO: add mqtt param to neoanim?

#include <RobotButterfly.h>

#define SENDER 1 // use this to set which program is uploaded

String sender_mqtt_id = ""; // receiver robot needs to know this

RobotButterfly robotbutterfly;

void myiotConnectedCallback();
void myiotDisconnectedCallback();
void myiotMessageReceivedCallback(String topic, String payload);
void myiotMessagePublishedCallback(String topic, String payload);
void myonClickCallback(uint8_t p);

void myNeoAnimStartCallback(struct NeoAnimation *a);
void myNeoAnimDoneCallback(struct NeoAnimation *a);
void myNeoAnimLoopCallback(struct NeoAnimation *a);


void setup() {
  Serial.begin(9600);
  robotbutterfly = RobotButterfly();
  
  // start up robot butterfly with some settings:
  // 1st param true = library will initialise the servos automatically
  // 2nd param false = library will not increment state machine on button click automatically
  robotbutterfly.init(true, false);

  // implement callbacks
  robotbutterfly.onClickCallback_client = myonClickCallback;
  robotbutterfly.onIOTMessageReceivedCallback_client = myiotMessageReceivedCallback;
  robotbutterfly.onIOTMessagePublishedCallback_client = myiotMessagePublishedCallback;
  robotbutterfly.onIOTConnectedCallback_client = myiotConnectedCallback;
  robotbutterfly.onIOTDisconnectedCallback_client = myiotDisconnectedCallback;
  robotbutterfly.onNeoAnimStartCallback_client = myNeoAnimStartCallback;
  robotbutterfly.onNeoAnimDoneCallback_client = myNeoAnimDoneCallback;
  robotbutterfly.onNeoAnimLoopCallback_client = myNeoAnimLoopCallback;

  stopAllNeoAnimation();

  Serial << "Welcome to Robot Butterfly!" << endl;
  setSoundTaskPriority(PRIORITY_SOUND_HIGH);
  playSound(SOUND_ALERT_STARTUP);
  setSoundTaskPriority(PRIORITY_SOUND_MID);

  robotbutterfly.enableMQTT();
}

void loop() {
  robotbutterfly.update();
}

