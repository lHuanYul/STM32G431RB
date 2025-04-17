#ifndef MY_TIM_H
#define MY_TIM_H

#include <stdint.h>
#include "tim.h"
#include "myIncCodes/motor.h"

#define APB1_TIM_CLOCK 170 // MHz
#define APB2_TIM_CLOCK 170 // MHz
#define TIM2_AUTORELOAD MOTOR_TIM_AUTORELOAD
#define TIM2_DUTYRATIO 50
#define TIM3_AUTORELOAD MOTOR_TIM_AUTORELOAD
#define TIM3_DUTYRATIO 50
#define TIM6_AUTORELOAD 50000

#define TIMER6_COUNT 10 // 50ms * x

extern uint16_t timer6_count;

#endif
