/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Pierce Oxley                                              */
/*    Created:      8/5/2024                                                  */
/*    Description:  Timely Robotics Code                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// Setup
// Any included libraries, namespaces, instances, etc
#include "vex.h"
#include "math.h"

const int counterclockwise = -1;
const int clockwise = 1;

using namespace vex;

// Declare competition instance
competition Competition;

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*                      Run vexcodeInit to set up robot                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

}

// Normalize any given angle to 0 ... 360
double normalizeAngle(double angle) {
    while (angle < 0) {
        angle += 360;

    }

    while (angle >= 360) {
        angle -= 360;

    }

    return angle;
}

// Rotate to a provided angle (degrees)
void rotateTo(int angle, int direction = 0) {
  // -1 = counterclockwise, 0 = any, 1 = clockwise
  const int any = 0;

  RightMotors.stop();
  LeftMotors.stop();

  if (direction == any) {
    while (normalizeAngle(Gyro.rotation() + 1) < angle) {
    LeftMotors.spin(forward, 60, pct);
    RightMotors.spin(forward, -60, pct);

    }

    while (normalizeAngle(Gyro.rotation() - 360) > angle) {
    LeftMotors.spin(forward, -60, pct);
    RightMotors.spin(forward, 60, pct);

    }
  }

  if (direction == counterclockwise) {
    while (normalizeAngle(Gyro.rotation() - 360) > angle) {
    LeftMotors.spin(forward, -40, pct);
    RightMotors.spin(forward, 40, pct);

    }
  }

  if (direction == clockwise) {
    while (normalizeAngle(Gyro.rotation()) < angle) {
    LeftMotors.spin(forward, 40, pct);
    RightMotors.spin(forward, -40, pct);

    }
  }

  LeftMotors.stop();
  RightMotors.stop();

}

void arcadeDrive(int x, int y, int rotation = 0) {
  const double pi = 3.14159265;

  double currentRotation = Gyro.rotation();
  double currentRotationRadians = (currentRotation) * pi / 180.0;

  double forwardSpeed = x * cos(currentRotationRadians) + y * sin(currentRotationRadians);
  double horizontalSpeed = -x * sin(currentRotationRadians) + y * cos(currentRotationRadians);

  double FrontRightSpeed = forwardSpeed - horizontalSpeed - rotation;
  double BackRightSpeed = forwardSpeed + horizontalSpeed - rotation;
  double BackLeftSpeed = forwardSpeed - horizontalSpeed + rotation;
  double FrontLeftSpeed = forwardSpeed + horizontalSpeed + rotation;
  
  FrontRight.spin(forward, FrontRightSpeed, pct);
  BackRight.spin(forward, BackRightSpeed, pct);
  BackLeft.spin(forward, BackLeftSpeed, pct);
  FrontLeft.spin(forward, FrontLeftSpeed, pct);

}

// Drive in a given direction at a given speed
// Direction is specified as heading on the unit circle
void drive(int angle, int speed, double time) {
  const double pi = 3.14159265;

  angle = normalizeAngle(angle);
  double radianAngle = angle * pi / 180.0;

  int digitalX = speed * sin(radianAngle);
  int digitalY = (speed * speed) - (digitalX * digitalX);
  digitalY = sqrt(digitalY);

  double currentRotation = Gyro.rotation();
  double currentRotationRadians = (currentRotation) * pi / 180.0;

  if (angle > 90 && angle < 270) {
    digitalY = -digitalY;

  }

  arcadeDrive(digitalX, digitalY);

  wait(time * 1000, msec);

  FrontRight.stop();
  BackRight.stop();
  BackLeft.stop();
  FrontLeft.stop();

}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                            Autonomous Portion                             */
/*                                                                           */
/*               Used for the autonomous portion of each game                */
/*                                                                           */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // Use drive() and rotateTo()

  // Turn to first ball
  rotateTo(225);

  // Collect first ball
  // Prepare mechanism
  FirstStage.setVelocity(50, pct);
  Claw.setVelocity(-100, pct);

  FirstStage.spin(forward);
  Claw.spin(forward);

  wait(1, seconds);

  FirstStage.stop();
  Claw.stop();

  drive(225, 60, 0.5);

  // Grab ball
  FirstStage.setVelocity(-50, pct);
  FirstStage.spinFor(1, seconds);

  Claw.setVelocity(100, pct);
  Claw.spinFor(0.75, seconds);

  drive(45, 50, 1.0);

  // Turn to wall
  rotateTo(1);

  // Move arm up
  FirstStage.spin(forward, 100, pct);
  SecondStage.spin(forward, 100, pct);

  wait(1, seconds);
  
  FirstStage.stop();
  SecondStage.stop();

  // Drive to wall
  drive(90, 60, 0.7);
  drive(90, 20, 0.4);

  // Drop ball
  Claw.setVelocity(-100, pct);
  Claw.spinFor(0.5, seconds);

  // Return to middle
  drive(270, 55, 0.6);

  SecondStage.setVelocity(-100, pct);
  SecondStage.spinFor(1.0, seconds);

  // Go to second ball
  rotateTo(315, -1);

  LeftMotors.stop();
  RightMotors.stop();

  drive(135, 60, 1.0);

  // Grab ball
  FirstStage.setVelocity(-50, pct);
  FirstStage.spinFor(1.0, seconds);

  Claw.setVelocity(100, pct);
  Claw.spinFor(0.8, seconds);

  // Go back to middle
  drive(315, 40, 1.0);

  // Turn to wall
  rotateTo(1);

  // Go to wall
  FirstStage.spin(forward, 100, pct);
  SecondStage.spin(forward, 100, pct);

  wait(1, seconds);
  
  FirstStage.stop();
  SecondStage.stop();

  drive(90, 60, 1.0);

  // Drop ball
  Claw.setVelocity(-100, pct);
  Claw.spinFor(1.0, seconds);

  // Prepare for driver control
  drive(270, 60, 1.0);

  SecondStage.setVelocity(-100, pct);
  SecondStage.spinFor(1.0, seconds);

}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                User Control                               */
/*                                                                           */
/*              Used for the user control portion of each game               */
/*                                                                           */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  bool sensitive = false;

  while (1) {
    int forwards = Controller1.Axis3.position();
    int horizontal = Controller1.Axis4.position();
    int rotation = Controller1.Axis1.position();

    if (sensitive) {
      rotation = rotation * 0.45;

    } else {
      rotation = rotation * 0.9;

    }

    arcadeDrive(forwards, horizontal, rotation);

    if (Controller1.ButtonL1.pressing()) {
      FirstStage.spin(forward, 50.0, pct);

    } else if (Controller1.ButtonL2.pressing()) {
      FirstStage.spin(forward, -15.0, pct);

    } else {
      FirstStage.stop();

    }

    if (Controller1.ButtonR1.pressing()) {
      SecondStage.spin(forward, 50.0, pct);

    } else if (Controller1.ButtonR2.pressing()) {
      SecondStage.spin(forward, -50.0, pct);

    } else {
      SecondStage.stop();

    }

    if (Controller1.ButtonB.pressing()) {
      Claw.spin(forward, 100.0, pct);

    } else if (Controller1.ButtonA.pressing()) {
      Claw.spin(forward, -100.0, pct);

    } else {
      Claw.stop();

    }

    if (Controller1.ButtonDown.pressing()) {
      Gyro.calibrate();

    }

    if (Gyro.isCalibrating()) {
      Controller1.rumble(".");
      
    }

    if (Controller1.ButtonUp.pressing()) {
      //drive(90, 100, 1);
      //rotateTo(0);
      //RightMotors.spin(forward);

    }

    if (Controller1.ButtonRight.pressing()) {
      sensitive = !sensitive;
      Controller1.rumble(".");
      wait(0.25, seconds);

    }

    // Sleep for a short while to prevent wasted resources.
    wait(20, msec);

  }
}

//
// Main function where everything runs from
//

int main() {
  // Declare which parts of the program are for each game mode
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run pre_auton() to set up robot for each game
  pre_auton();

  // Run main forever
  while (true) {
    wait(100, msec);
  }
}
