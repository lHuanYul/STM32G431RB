#ifndef MY_MYCODE_H
#define MY_MYCODE_H     //使mycode不重複 include



/* +Includes ------------------------------------------------------------------*/
#include "common.h"
#include "hall_detection.h"
#include "motor.h"
#include "PI_control.h"
#include "vehicle.h"

#include "packet.h"
#include "vec.h"
#include "my_uart.h"

/* -Includes ------------------------------------------------------------------*/



/* +general -------------------------------------------------------------------*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void MCmain(void);

/* -general -------------------------------------------------------------------*/



#endif
