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

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void);
