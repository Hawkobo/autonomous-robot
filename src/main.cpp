#include <Arduino.h>
#include <Servo.h>
#include "motor_controller.h"
//Define pins
#define LEFT_TRACKING_SENSOR A0
#define MIDDLE_TRACKING_SENSOR A1
#define RIGHT_TRACKING_SENSOR 13
#define ULTRASOUND_TRIGGER A2
#define ULTRASOUND_ECHO A3
// States contants
const String LINE_TRACKING = "LINE_TRACKING";
const String OBSTACLE_AVOIDANCE = "OBSTACLE_AVOIDANCE";
// Moves contants
const String MOVE_FORWARD = "FORWARD";
const String MOVE_LEFT = "LEFT";
const String MOVE_RIGHT = "RIGHT";
const String MOVE_STOP = "STOP";
// Servo angles constants
const unsigned short int SERVO_FULL_LEFT_ANGLE = 180;
const unsigned short int SERVO_LEFT_30_ANGLE = 150;
const unsigned short int SERVO_LEFT_60_ANGLE = 120;
const unsigned short int SERVO_FORWARD_ANGLE = 90;
const unsigned short int SERVO_RIGHT_60_ANGLE = 60;
const unsigned short int SERVO_RIGHT_30_ANGLE = 30;
const unsigned short int SERVO_FULL_RIGHT_ANGLE = 0;
const unsigned short int NUMBER_OF_SERVO_ANGLES = 7;
const unsigned short int SERVO_ANGLES[NUMBER_OF_SERVO_ANGLES] = {
  SERVO_FULL_LEFT_ANGLE,
  SERVO_LEFT_30_ANGLE,
  SERVO_LEFT_60_ANGLE,
  SERVO_FORWARD_ANGLE,
  SERVO_RIGHT_60_ANGLE,
  SERVO_RIGHT_30_ANGLE,
  SERVO_FULL_RIGHT_ANGLE,
};
// Initialize variables
MotorController motorController(3, 4, 1, 2);
String state = LINE_TRACKING;
String lastMove = MOVE_FORWARD;
Servo distanceSensorNeck;
unsigned long manouverStartTime;
unsigned short int distances[] = { 0, 0, 0, 0, 0, 0, 0 };
unsigned short int currentDistanceMeasurementIndex = 0;

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

int readPing() {
  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(ULTRASOUND_TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASOUND_TRIGGER, HIGH);
  delayMicroseconds(5);
  digitalWrite(ULTRASOUND_TRIGGER, LOW);
  long duration = pulseIn(ULTRASOUND_ECHO, HIGH);
  // convert the time into a distance
  long cm = microsecondsToCentimeters(duration);
  return cm;
}

void moveTrack(void) {
  int left = digitalRead(LEFT_TRACKING_SENSOR);
  int center = digitalRead(MIDDLE_TRACKING_SENSOR);
  int right = digitalRead(RIGHT_TRACKING_SENSOR);
  distanceSensorNeck.write(SERVO_FORWARD_ANGLE);

  if (center == 1) {
    lastMove = MOVE_FORWARD;
    motorController.moveForward();
    return;
  }
  if (left == 1 && center == 0) {
    lastMove = MOVE_LEFT;
    motorController.turnLeft();

    return;
  }
  if (right == 1 && center == 0) {
    lastMove = MOVE_RIGHT;
    motorController.turnRight();
    return;
  }
  if (left == 0 && right == 0) {
    if (lastMove == MOVE_FORWARD) {
      lastMove = MOVE_STOP;
      motorController.stop();
      return;
    }
    if (lastMove == MOVE_LEFT) {
      motorController.turnLeft();
      return;
    }
    motorController.turnRight();
  }
}

void moveNeck(void) {
  distanceSensorNeck.write(SERVO_ANGLES[currentDistanceMeasurementIndex]);
}

unsigned short int getShortestDistanceIndex() {
  unsigned short int shortest = 999;
  unsigned short int shortestIndex = -1;
  for (unsigned short int i = 0; i < NUMBER_OF_SERVO_ANGLES; i++) {
    if (distances[i] < shortest) {
      shortest = distances[i];
      shortestIndex = i;
    }
  }
  return shortestIndex;
}

void setup() {
  pinMode(LEFT_TRACKING_SENSOR, INPUT_PULLUP);
  pinMode(MIDDLE_TRACKING_SENSOR, INPUT_PULLUP);
  pinMode(RIGHT_TRACKING_SENSOR, INPUT_PULLUP);
  pinMode(ULTRASOUND_TRIGGER, OUTPUT);
  pinMode(ULTRASOUND_ECHO, INPUT);
  distanceSensorNeck.attach(10);
  Serial.begin(115200);
}

void loop() {
  if (state == LINE_TRACKING) {
    int distanceCm = readPing();
    if (distanceCm > 15) {
      // safe to roll
      moveTrack();
    } else {
      // start turning
      motorController.stop();
      currentDistanceMeasurementIndex = 0;
      state = OBSTACLE_AVOIDANCE;
    }
    return;
  }
  if (state == OBSTACLE_AVOIDANCE) {
    if (currentDistanceMeasurementIndex < NUMBER_OF_SERVO_ANGLES) {
      motorController.stop();
      delay(100);
      moveNeck();
      delay(100);
      int distance = readPing();
      distances[currentDistanceMeasurementIndex] = distance;
      Serial.print(SERVO_ANGLES[currentDistanceMeasurementIndex]);
      Serial.print(" - ");
      Serial.print(distance);
      Serial.print("\n");
      currentDistanceMeasurementIndex++;
      return;
    }
    if (currentDistanceMeasurementIndex >= NUMBER_OF_SERVO_ANGLES) {
      unsigned short int shortestIndex = getShortestDistanceIndex();
      switch (shortestIndex) {
        case 0:
        case 1:
        case 2: {
          // shortest from left
          unsigned short int distance = distances[shortestIndex];
          if (distance > 15) {
            Serial.println("left");
            motorController.turnLeft();
          } else {
            Serial.println("right");
            motorController.turnRight();
          }
          break;
        }
        case 3: {
          // shortest in front
          Serial.println("forward, right");
          motorController.turnRight();
          break;
        }
        default: {
          // shortest from right
          Serial.println("right left");
          motorController.turnLeft();
        }
      }
      delay(500);
      currentDistanceMeasurementIndex = 0;
    }
  }
}
