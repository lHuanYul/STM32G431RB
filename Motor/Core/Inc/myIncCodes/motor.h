#ifndef MY_MOTOR_H
#define MY_MOTOR_H

#include <stdint.h>
#include <stdbool.h>
#include <stm32g431xx.h>
#include <stm32g4xx_hal_tim.h>

#define MOTOR_TIM_AUTORELOAD 1000

typedef struct MOTOR {
    // PWM setpoint value
    short pwm_setpoint;
    // PWM realtime value
    short pwm_realtime;
    // PWM timer
    TIM_HandleTypeDef* htim;
    uint32_t Channel[3];
    // AL BL CL
    GPIO_TypeDef* L_GPIOx[3];
    uint16_t L_GPIO_Pin_x[3];
    // AH BH CH
    GPIO_TypeDef* D_GPIOx[3];
    uint16_t D_GPIO_Pin_x[3];
    // ARR value from realtime
    int arr_trigger_value;
    // Rotate count (+3 per circle)
    uint16_t rotate_count;
    // RPM(But second) value
    float rps;
} MOTOR;

extern MOTOR motor2, motor3;

void MotorPWMStart(MOTOR);
void MotorSpin(MOTOR);
void MotorPWMSetpointSet(MOTOR*, short);
void MotorPWMRealtimeSet(MOTOR*);

#endif
