#ifndef MY_MOTOR_H
#define MY_MOTOR_H



/* +Includes ------------------------------------------------------------------*/
#include "common.h"

/* -Includes ------------------------------------------------------------------*/



/* +struct --------------------------------------------------------------------*/
typedef struct{
    double integral_record;
    int rpm_count;
    uint32_t adc_value;
    int pwmValue;
    int pwmValue_temp;
    float present_speed;
    bool clockwise;
    int currentStep;

    GPIO_TypeDef* Hall_GPIOx[3];
    uint16_t Hall_GPIO_Pin_x[3];

    GPIO_TypeDef* M_GPIOx[6];
    uint16_t M_GPIO_Pin_x[6];

    TIM_HandleTypeDef* TIMx[3];
    uint32_t TIM_CHANNEL_x[3];

} MOTOR_PARAMETER;

/* -struct --------------------------------------------------------------------*/



/* +variable ------------------------------------------------------------------*/
extern MOTOR_PARAMETER motor_right;
extern MOTOR_PARAMETER motor_left;

/* -variable ------------------------------------------------------------------*/



/* +subprogram -----------------------------------------------------------------*/
void motor_setup(MOTOR_PARAMETER Motor_R, MOTOR_PARAMETER Motor_L );
void updateMotorStep(MOTOR_PARAMETER *motor);
void commutateMotor(MOTOR_PARAMETER motor);

/* -subprogram -----------------------------------------------------------------*/



#endif
