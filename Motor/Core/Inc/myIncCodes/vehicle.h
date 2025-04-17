#ifndef MY_VEHICLE_H
#define MY_VEHICLE_H

#include "stm32g4xx_nucleo.h"
#include "myIncCodes/motor.h"

#define direction char
#define RIGHT -1
#define LEFT 1

void VehicleStop(void);
void VehicleMoveStraight(short);
void VehicleMoveTurn(direction, short);

#endif
