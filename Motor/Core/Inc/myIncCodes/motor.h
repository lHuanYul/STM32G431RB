#ifndef MY_MOTOR_H
#define MY_MOTOR_H

#include <stdint.h>
#include <stdbool.h>
#include <stm32g431xx.h>
#include <stm32g4xx_hal_tim.h>

#define MOTOR_TIM_AUTORELOAD 1000

typedef struct MOTOR {
    short pwm_setpoint;
    TIM_HandleTypeDef* htim;
    uint32_t Channel[3];
    GPIO_TypeDef* L_GPIOx[3];
    uint16_t L_GPIO_Pin_x[3];
    GPIO_TypeDef* D_GPIOx[3];
    uint16_t D_GPIO_Pin_x[3];
    int arr_trigger_value;
    uint16_t rotate_count;
    float rps;
} MOTOR;

extern MOTOR motor2, motor3;

void MotorPWMStart(MOTOR);
void MotorSpin(MOTOR);
void MotorSpeedSet(MOTOR*, short);

#endif
