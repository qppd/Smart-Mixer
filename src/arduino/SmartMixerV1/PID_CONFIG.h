#ifndef PID_CONFIG_H
#define PID_CONFIG_H

#include <Arduino.h>
#include <PID_v1.h>

// PID tuning parameters for pump dosing control
extern double P;
extern double I;
extern double D;

// PID variables for volume control
extern double VOLUME_SETPOINT;      // Target volume in ml
extern double CURRENT_VOLUME;       // Current volume dispensed
extern double outputPump;           // PID output (0-255 for PWM)

// PID controller instance
extern PID pidPump;

void initPID();
void computePID();
void setPumpSetpoint(double targetVolume);
void updateCurrentVolume(double volume);
void resetPID();

#endif
