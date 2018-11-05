#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H
#include <UCMotor.h>

class MotorController
{
  private:
    UC_DCMotor leftMotor1;
    UC_DCMotor rightMotor1;
    UC_DCMotor leftMotor2;
    UC_DCMotor rightMotor2;
  public:
    static const int MAX_SPEED;
    static const int TURN_SPEED;
    static const int STOP_SPEED;

    MotorController(int left1, int right1, int left2, int right2);

    void turnLeft(void);

    void turnRight(void);

    void stop(void);

    void moveForward(void);
};

#endif
