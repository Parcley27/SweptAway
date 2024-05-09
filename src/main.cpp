/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Pierce Oxley                                              */
/*    Created:      8/5/2024                                                  */
/*    Description:  Timely Robotics Code                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

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
    if (Controller1.ButtonL1.pressing()) {
      FirstStage.setVelocity(50.0, pct);
    } else if (Controller1.ButtonL2.pressing()) {
      FirstStage.setVelocity(-15.0, pct);
    } else {
      FirstStage.setVelocity(0.0, pct);
    }

    if (Controller1.ButtonR1.pressing()) {
      SecondStage.setVelocity(50.0, pct);
    } else if (Controller1.ButtonR2.pressing()) {
      SecondStage.setVelocity(-50.0, pct);
    } else {
      SecondStage.setVelocity(0.0, pct);
    }

    if (Controller1.ButtonA.pressing()) {
      Claw.setVelocity(50.0, pct);
    } else if (Controller1.ButtonB.pressing()) {
      Claw.setVelocity(-50.0, pct);
    } else {
      Claw.setVelocity(0.0, pct);
    }

    FirstStage.spin(forward);
    SecondStage.spin(forward);
    Claw.spin(forward);

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
