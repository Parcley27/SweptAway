#include "vex.h"

using namespace vex;

// Brain
brain Brain;

// Controller
controller Controller1 = controller(primary);

// Sensors
inertial Gyro = inertial(PORT11);

// Drive Motors
motor FrontRight = motor(PORT1, ratio18_1, true);
motor BackRight = motor(PORT2, ratio18_1, true);
motor BackLeft = motor(PORT3, ratio18_1, false);
motor FrontLeft = motor(PORT4, ratio18_1, false);

// Accessory Motors
motor FirstStageRight = motor(PORT5, ratio18_1, true);
motor FirstStageLeft = motor(PORT6, ratio18_1, true);
motor_group FirstStage = motor_group(FirstStageLeft, FirstStageRight);

motor SecondStage = motor(PORT7, ratio18_1, true);

motor Claw = motor(PORT8, ratio18_1, true);

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  // Set motor stopping modes
  FrontRight.setStopping(brake);
  BackRight.setStopping(brake);
  BackLeft.setStopping(brake);
  FrontLeft.setStopping(brake);

  FirstStage.setStopping(brake);
  SecondStage.setStopping(brake);

  Claw.setStopping(brake);

  // Calibrate inertial sensor
  Gyro.calibrate();
  
  while (Gyro.isCalibrating()) {
    // Do nothing
  }

}