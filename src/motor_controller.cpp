#include <UCMotor.h>

class MotorController
{
  private:
    UC_DCMotor leftMotor1;
    UC_DCMotor rightMotor1;
    UC_DCMotor leftMotor2;
    UC_DCMotor rightMotor2;
  public:
    static const int MAX_SPEED = 255;
    static const int TURN_SPEED = 255;
    static const int STOP_SPEED = 5;
    static const int TURN_ARC_SPEED = 30;

    MotorController(int left1, int right1, int left2, int right2);

    void turnLeft(void);
    void turnLeft(uint8_t speed);
    void turnLeftArc(void);

    void turnRight(void);
    void turnRight(uint8_t speed);
    void turnRightArc(void);

    void stop(void);

    void moveForward(void);
    void moveForward(uint8_t speed);

    void moveBackward(void);
};

MotorController::MotorController(int left1, int right1, int left2, int right2) :
  leftMotor1(left1, MOTOR34_64KHZ),
  rightMotor1(right1, MOTOR34_64KHZ),
  leftMotor2(left2, MOTOR34_64KHZ),
  rightMotor2(right2, MOTOR34_64KHZ)
{}

void MotorController::turnLeft(void) {
  leftMotor1.run(BACKWARD);
  rightMotor1.run(FORWARD);
  leftMotor2.run(BACKWARD);
  rightMotor2.run(FORWARD);
  leftMotor1.setSpeed(TURN_SPEED);
  rightMotor1.setSpeed(TURN_SPEED);
  leftMotor2.setSpeed(TURN_SPEED);
  rightMotor2.setSpeed(TURN_SPEED);
}

void MotorController::turnLeft(uint8_t speed) {
  leftMotor1.run(BACKWARD);
  rightMotor1.run(FORWARD);
  leftMotor2.run(BACKWARD);
  rightMotor2.run(FORWARD);
  leftMotor1.setSpeed(speed);
  rightMotor1.setSpeed(speed);
  leftMotor2.setSpeed(speed);
  rightMotor2.setSpeed(speed);
}

void MotorController::turnLeftArc(void) {
  leftMotor1.run(FORWARD);
  rightMotor1.run(FORWARD);
  leftMotor2.run(FORWARD);
  rightMotor2.run(FORWARD);
  leftMotor1.setSpeed(TURN_ARC_SPEED);
  rightMotor1.setSpeed(MAX_SPEED);
  leftMotor2.setSpeed(TURN_ARC_SPEED);
  rightMotor2.setSpeed(MAX_SPEED);
}

void MotorController::turnRight(void) {
  leftMotor1.run(FORWARD);
  rightMotor1.run(BACKWARD);
  leftMotor2.run(FORWARD);
  rightMotor2.run(BACKWARD);
  leftMotor1.setSpeed(TURN_SPEED);
  rightMotor1.setSpeed(TURN_SPEED);
  leftMotor2.setSpeed(TURN_SPEED);
  rightMotor2.setSpeed(TURN_SPEED);
}

void MotorController::turnRight(uint8_t speed) {
  leftMotor1.run(FORWARD);
  rightMotor1.run(BACKWARD);
  leftMotor2.run(FORWARD);
  rightMotor2.run(BACKWARD);
  leftMotor1.setSpeed(speed);
  rightMotor1.setSpeed(speed);
  leftMotor2.setSpeed(speed);
  rightMotor2.setSpeed(speed);
}

void MotorController::turnRightArc(void) {
  leftMotor1.run(FORWARD);
  rightMotor1.run(FORWARD);
  leftMotor2.run(FORWARD);
  rightMotor2.run(FORWARD);
  leftMotor1.setSpeed(MAX_SPEED);
  rightMotor1.setSpeed(TURN_ARC_SPEED);
  leftMotor2.setSpeed(MAX_SPEED);
  rightMotor2.setSpeed(TURN_ARC_SPEED);
}

void MotorController::stop(void) {
  leftMotor1.run(STOP_SPEED);
  rightMotor1.run(STOP_SPEED);
  leftMotor2.run(STOP_SPEED);
  rightMotor2.run(STOP_SPEED);
}

void MotorController::moveForward(void) {
  leftMotor1.run(FORWARD);
  rightMotor1.run(FORWARD);
  leftMotor2.run(FORWARD);
  rightMotor2.run(FORWARD);
  leftMotor1.setSpeed(MAX_SPEED);
  rightMotor1.setSpeed(MAX_SPEED);
  leftMotor2.setSpeed(MAX_SPEED);
  rightMotor2.setSpeed(MAX_SPEED);
}

void MotorController::moveForward(uint8_t speed) {
  leftMotor1.run(FORWARD);
  rightMotor1.run(FORWARD);
  leftMotor2.run(FORWARD);
  rightMotor2.run(FORWARD);
  leftMotor1.setSpeed(speed);
  rightMotor1.setSpeed(speed);
  leftMotor2.setSpeed(speed);
  rightMotor2.setSpeed(speed);
}

void MotorController::moveBackward(void) {
  leftMotor1.run(BACKWARD);
  rightMotor1.run(BACKWARD);
  leftMotor2.run(BACKWARD);
  rightMotor2.run(BACKWARD);
  leftMotor1.setSpeed(MAX_SPEED);
  rightMotor1.setSpeed(MAX_SPEED);
  leftMotor2.setSpeed(MAX_SPEED);
  rightMotor2.setSpeed(MAX_SPEED);
}
