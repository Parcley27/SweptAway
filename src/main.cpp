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
  while (1) {
    int forwards = Controller1.Axis3.position();
    int horizontal = Controller1.Axis4.position();
    int rotation = Controller1.Axis1.position();

    /* Standard Drive
    double FrontRightSpeed = forwards - horizontal - rotation;
    double BackRightSpeed = forwards + horizontal - rotation;
    double BackLeftSpeed = forwards - horizontal + rotation;
    double FrontLeftSpeed = forwards + horizontal + rotation;
    */

    ///* Arcade Drive
    double currentRotation = Gyro.rotation();

    const double pi = 3.14159265;
    double currentRotationRadians = currentRotation * pi / 180.0;

    double adjustedForward = forwards * cos(currentRotationRadians) + horizontal * sin(currentRotationRadians);
    double adjustedHorizontal = -forwards * sin(currentRotationRadians) + horizontal * cos(currentRotationRadians);   
    
    double FrontRightSpeed = adjustedForward - adjustedHorizontal - rotation;
    double BackRightSpeed = adjustedForward + adjustedHorizontal - rotation;
    double BackLeftSpeed = adjustedForward - adjustedHorizontal + rotation;
    double FrontLeftSpeed = adjustedForward + adjustedHorizontal + rotation;
    //*/

    FrontRight.spin(forward, FrontRightSpeed, pct);
    BackRight.spin(forward, BackRightSpeed, pct);
    BackLeft.spin(forward, BackLeftSpeed, pct);
    FrontLeft.spin(forward, FrontLeftSpeed, pct);

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
