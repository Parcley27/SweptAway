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

// A global instance of competition
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
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main function where everything is run from
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
