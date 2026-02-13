#include "PID_CONFIG.h"
#include "RELAY_CONFIG.h"

//-----------------------------------------------------------------
//PID TUNING PARAMETERS--------------------------------------------
//-----------------------------------------------------------------

// PID tuning parameters for pump dosing control
double P = 4.0;   // Proportional gain
double I = 0.2;   // Integral gain (small value to prevent overshoot)
double D = 22.0;  // Derivative gain

//-----------------------------------------------------------------
//PID VARIABLES----------------------------------------------------
//-----------------------------------------------------------------

// Volume control variables
double VOLUME_SETPOINT = 0;     // Target volume in ml
double CURRENT_VOLUME = 0;      // Current volume dispensed in ml
double outputPump = 0;          // PID output (0-255 for PWM)

//-----------------------------------------------------------------
//PID CONTROLLER INSTANCE------------------------------------------
//-----------------------------------------------------------------

// Initialize PID controller for pump dosing
// Parameters: Input, Output, Setpoint, Kp, Ki, Kd, Mode
PID pidPump(&CURRENT_VOLUME, &outputPump, &VOLUME_SETPOINT, P, I, D, DIRECT);

//-----------------------------------------------------------------
//FUNCTION FOR INITIALIZING PID------------------------------------
//-----------------------------------------------------------------
void initPID() {
  // Set the PID controller mode to automatic
  pidPump.SetMode(AUTOMATIC);
  
  // Set output limits (0-255 for PWM control)
  pidPump.SetOutputLimits(0, 255);
  
  Serial.println("PID Controller: Initialized!");
  Serial.print("PID Tuning - P: ");
  Serial.print(P);
  Serial.print(", I: ");
  Serial.print(I);
  Serial.print(", D: ");
  Serial.println(D);
}

//-----------------------------------------------------------------
//FUNCTION FOR COMPUTING PID---------------------------------------
//-----------------------------------------------------------------
void computePID() {
  // Compute the PID output value
  pidPump.Compute();
  
  // Handle pump control logic based on PID output
  if (outputPump > 0) {
    // Turn on pump with PWM based on PID output
    operatePUMP_PWM((uint8_t)outputPump);
  } else {
    // Turn off pump
    operatePUMP(false);
  }
}

//-----------------------------------------------------------------
//FUNCTION FOR SETTING PUMP SETPOINT-------------------------------
//-----------------------------------------------------------------
void setPumpSetpoint(double targetVolume) {
  VOLUME_SETPOINT = targetVolume;
  Serial.print("PID Setpoint updated to: ");
  Serial.print(targetVolume);
  Serial.println(" ml");
}

//-----------------------------------------------------------------
//FUNCTION FOR UPDATING CURRENT VOLUME-----------------------------
//-----------------------------------------------------------------
void updateCurrentVolume(double volume) {
  CURRENT_VOLUME = volume;
}

//-----------------------------------------------------------------
//FUNCTION FOR RESETTING PID---------------------------------------
//-----------------------------------------------------------------
void resetPID() {
  CURRENT_VOLUME = 0;
  outputPump = 0;
  VOLUME_SETPOINT = 0;
  pidPump.SetMode(MANUAL);
  pidPump.SetMode(AUTOMATIC);
  Serial.println("PID Controller: Reset!");
}
