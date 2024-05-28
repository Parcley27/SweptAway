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

// Will rotate the robot to a given angle (degrees)
void rotateTo(int angle) {
  if (normalizeAngle(Gyro.rotation()) < angle) {
    while (normalizeAngle(Gyro.rotation()) < angle) {
    LeftMotors.spin(forward, 30, pct);
    RightMotors.spin(forward, -30, pct);

    }

  } else if (normalizeAngle(Gyro.rotation()) > angle) {
    while (normalizeAngle(Gyro.rotation()) > angle) {
    LeftMotors.spin(forward, -30, pct);
    RightMotors.spin(forward, 30, pct);

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

// Will drive the robot at a specified direction at a specified speed
// Direction is specified as heading on the unit circle
void drive(int angle, int speed, int time) {
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

  wait(time, seconds);

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
  // Autonomous here

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
