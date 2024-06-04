/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       robot-config.h                                            */
/*    Author:       Pierce Oxley                                              */
/*    Created:      8/5/2024                                                  */
/*    Description:  Share objects around robot                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// Use VEX namespace for code objects
using namespace vex;

// Include brain for main.cpp
extern brain Brain;

// Include controller for main.cpp
extern controller Controller1;

// Include sensors for main.cpp
extern inertial Gyro;

// Include drive motors for main.cpp
extern motor FrontRight;
extern motor BackRight;
extern motor BackLeft;
extern motor FrontLeft;

extern motor_group RightMotors;
extern motor_group LeftMotors;

// Include accessory motors for main.cpp
extern motor_group FirstStage;
extern motor SecondStage;
extern motor Claw;

// Include vexcodeInit() for main.cpp
void vexcodeInit(void);
