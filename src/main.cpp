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

// Use VEX namespace for code objects
using namespace vex;

// Declare competition instance
competition Competition;

// Set up robot to run program and prepare for competition
void pre_auton(void) {
  // Prepare the robot to run the program
  // Object setup, calibration, etc all in here
  vexcodeInit();

}

// Normalize any given angle to 0 ... 359
double normalizeAngle(double angle) {
  // If less than 0, add 360 deg
  while (angle < 0) {
      angle += 360;

  }

  // If more than or equal to 360, subtract 360 deg
  while (angle >= 360) {
      angle -= 360;

  }

  return angle;

}

// Find most efficient turn direction based on two input angles
int findTurnDirection(double currentAngle, double targetAngle) {
  const int counterclockwise = -1;
  const int clockwise = 1;

  // Find the diffirence between the angles, and normalize to 0 ... 359
  double delta = normalizeAngle(targetAngle - currentAngle);

  // Return -1 for counterclockwise or 1 for clockwise
  return (delta > 180) ? counterclockwise : clockwise;

}

// Check if a value is within a maximum range of a middle value
bool isInRange(double middleValue, double maximumRange, double value) {
  // Return true if in range, otherwise return false
  return (abs(middleValue - value) <= maximumRange) ? true : false;

}

// Rotate to a provided angle (degrees)
void rotateTo(int targetAngle, int direction = 0, int turnSpeed = 40) {
  // -1 = counterclockwise, 0 = any, 1 = clockwise
  const int any = 0;

  const double degreeOfError = 1.0;

  // Stopping the motors before trying to turn helps turning be more accurate
  RightMotors.stop();
  LeftMotors.stop();

  // Figure out direction to turn if none provided
  if (direction == any) {
    direction = findTurnDirection(normalizeAngle(Gyro.rotation()), targetAngle);

  }

  // Direction always is 1 (clockwise) or -1 (counterclockwise)
  // If 1 then nothing happens, if -1 then it switches the two speeds around to always turn in the right direction
  while (!isInRange(targetAngle, degreeOfError, normalizeAngle(Gyro.rotation()))) {
    LeftMotors.spin(forward, direction * turnSpeed, pct);
    RightMotors.spin(forward, direction * -turnSpeed, pct);

  }

  LeftMotors.stop();
  RightMotors.stop();

}

// Drive in an absolute heading relative to starting position
void arcadeDrive(int x, int y, int rotation = 0) {
  const double pi = 3.14159265;

  // Convert current rotation to radians
  // math.h sin() and cos() use radians not degrees
  double currentRotation = Gyro.rotation();
  double currentRotationRadians = (currentRotation) * pi / 180.0;

  // Solve general speeds accounting for current rotation
  double forwardSpeed = x * cos(currentRotationRadians) + y * sin(currentRotationRadians);
  double horizontalSpeed = -x * sin(currentRotationRadians) + y * cos(currentRotationRadians);

  // Solve each individual motor speed
  double FrontRightSpeed = forwardSpeed - horizontalSpeed - rotation;
  double BackRightSpeed = forwardSpeed + horizontalSpeed - rotation;
  double BackLeftSpeed = forwardSpeed - horizontalSpeed + rotation;
  double FrontLeftSpeed = forwardSpeed + horizontalSpeed + rotation;
  
  // Turn motors
  FrontRight.spin(forward, FrontRightSpeed, pct);
  BackRight.spin(forward, BackRightSpeed, pct);
  BackLeft.spin(forward, BackLeftSpeed, pct);
  FrontLeft.spin(forward, FrontLeftSpeed, pct);

}

// Drive in a given direction at a given speed, used for autonomous
// Direction is specified as heading on the unit circle
void drive(int angle, int speed, double time) {
  const double pi = 3.14159265;

  // Convert intended angle to radians between 0 ... 359 degrees
  angle = normalizeAngle(angle);
  double radianAngle = angle * pi / 180.0;

  // Solve forwards and horizntal speeds
  int digitalX = speed * sin(radianAngle);
  int digitalY = (speed * speed) - (digitalX * digitalX);
  digitalY = sqrt(digitalY);

  // Account for "reverse" movements
  if (angle > 90 && angle < 270) {
    digitalY = -digitalY;

  }

  // Drive in same fashion as user control, but for a predetermined time only
  arcadeDrive(digitalX, digitalY);

  wait(time * 1000, msec);

  // Stop motors once finished
  FrontRight.stop();
  BackRight.stop();
  BackLeft.stop();
  FrontLeft.stop();

}

// Autonomous code section during competition
void autonomous(void) {
  // Same values as those used in findTurnDirection()
  // Unfortunately didn't work as global varialbes so they're stuck here
  const int counterclockwise = -1;
  const int clockwise = 1;

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
  SecondStage.spinFor(0.75, seconds);

}

// User control section during competition
void usercontrol(void) {
  // Initialize sensitive mode turned off
  bool sensitive = false;

  while (1) {
    // Fetch robot direction input from controller
    int forwards = Controller1.Axis3.position();
    int horizontal = Controller1.Axis4.position();
    int rotation = Controller1.Axis1.position();

    // Change rotation sensitivity
    // Can also be done with other inputs/outputs in wanted later
    rotation = rotation * (sensitive ? 0.45 : 0.9);

    // Drive with paramaters taken from robot controller
    arcadeDrive(forwards, horizontal, rotation);

    // Control robot first stage
    if (Controller1.ButtonL1.pressing()) {
      FirstStage.spin(forward, 50.0, pct);

    } else if (Controller1.ButtonL2.pressing()) {
      FirstStage.spin(forward, -25.0, pct);

    } else {
      FirstStage.stop();

    }

    // Control robot second stage
    if (Controller1.ButtonR1.pressing()) {
      SecondStage.spin(forward, 75.0, pct);

    } else if (Controller1.ButtonR2.pressing()) {
      SecondStage.spin(forward, -75.0, pct);

    } else {
      SecondStage.stop();

    }

    // Control robot claw
    if (Controller1.ButtonB.pressing()) {
      Claw.spin(forward, 100.0, pct);

    } else if (Controller1.ButtonA.pressing()) {
      Claw.spin(forward, -100.0, pct);

    } else {
      Claw.stop();

    }

    // Recalibrate gyro sensor incase it gets confused
    if (Controller1.ButtonDown.pressing()) {
      Gyro.calibrate();

    }

    // Haptic feedback to know when it's not safe to drive
    if (Gyro.isCalibrating()) {
      Controller1.rumble(".");
      
    }

    if (Controller1.ButtonUp.pressing()) {
      //drive(90, 100, 1);
      //rotateTo(0);
      //RightMotors.spin(forward);

    }

    // Toggle "sensitive mode"
    // Delay is added to allow time for button to be depressed
    if (Controller1.ButtonLeft.pressing()) {
      sensitive = !sensitive;
      Controller1.rumble(".");
      wait(0.25, seconds);

    }

    // Sleep for a short while to prevent wasted resources.
    wait(20, msec);

  }
}

// Run and loop robot program
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
