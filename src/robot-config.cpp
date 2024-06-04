/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       robot-config.cpp                                          */
/*    Author:       Pierce Oxley                                              */
/*    Created:      8/5/2024                                                  */
/*    Description:  Configure robot settings and devices                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// Include required library for VEX
#include "vex.h"

// Use VEX namespace for code objects
using namespace vex;

// Declare brain
brain Brain;

// Declare controller
controller Controller1 = controller(primary);

// Declare sensors
inertial Gyro = inertial(PORT11);

// Declare motors
// Correct for physical orientation
const bool forwards = true;
const bool backwards = false;

// Drive motors
motor FrontRight = motor(PORT1, ratio18_1, forwards);
motor BackRight = motor(PORT2, ratio18_1, forwards);
motor BackLeft = motor(PORT3, ratio18_1, backwards);
motor FrontLeft = motor(PORT4, ratio18_1, backwards);

motor_group RightMotors = motor_group(FrontRight, BackRight);
motor_group LeftMotors = motor_group(FrontLeft, BackLeft);

// Accessory motors
motor FirstStageRight = motor(PORT5, ratio18_1, backwards);
motor FirstStageLeft = motor(PORT6, ratio18_1, forwards);
motor_group FirstStage = motor_group(FirstStageLeft, FirstStageRight);

motor SecondStage = motor(PORT7, ratio18_1, backwards);

motor Claw = motor(PORT8, ratio18_1, forwards);

// Initialize robot for use
// Includes object setup and sensor calibration
void vexcodeInit(void) {
  // Set motor stopping modes
  FrontRight.setStopping(brake);
  BackRight.setStopping(brake);
  BackLeft.setStopping(brake);
  FrontLeft.setStopping(brake);

  FirstStage.setStopping(hold);
  SecondStage.setStopping(hold);

  Claw.setStopping(hold);

  // Calibrate inertial sensor
  Gyro.calibrate();
  
  // Wait for sensor to calibrate
  while (Gyro.isCalibrating()) {
    wait(10, msec);

  }
}