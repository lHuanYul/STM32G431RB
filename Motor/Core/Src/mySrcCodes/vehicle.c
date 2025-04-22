#include "myIncCodes/vehicle.h"

// Stop vehicle
void VehicleStop(void) {
    MotorPWMSetpointSet(&motor2, 0);
    MotorPWMSetpointSet(&motor3, 0);
}

// Vehicle move straight forward/backward
void VehicleMoveStraight(short speed) {
    MotorPWMSetpointSet(&motor2, speed);
    MotorPWMSetpointSet(&motor3, speed);
}

// Vehicle turnning
void VehicleMoveTurn(direction _direction, short speed) {
    if(_direction > 0) {
        MotorPWMSetpointSet(&motor2, speed);
        MotorPWMSetpointSet(&motor3, -speed);
    } else {
        MotorPWMSetpointSet(&motor2, -speed);
        MotorPWMSetpointSet(&motor3, speed);
    }
}
