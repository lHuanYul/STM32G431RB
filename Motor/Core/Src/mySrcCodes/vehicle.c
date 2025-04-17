#include "myIncCodes/vehicle.h"

// Stop vehicle
void VehicleStop(void) {
    MotorSpeedSet(&motor2, 0);
    MotorSpeedSet(&motor3, 0);
}

// Vehicle move straight forward/backward
void VehicleMoveStraight(short speed) {
    MotorSpeedSet(&motor2, speed);
    MotorSpeedSet(&motor3, speed);
}

// Vehicle turnning
void VehicleMoveTurn(direction _direction, short speed) {
    if(_direction > 0) {
        MotorSpeedSet(&motor2, speed);
        MotorSpeedSet(&motor3, -speed);
    } else {
        MotorSpeedSet(&motor2, -speed);
        MotorSpeedSet(&motor3, speed);
    }
}
