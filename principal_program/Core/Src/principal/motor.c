#include "principal/motor.h"



/* +variable ------------------------------------------------------------------*/
const int SEQUENCE[6][3] = {                             // Commutation right_SEQUENCE for 120 degree control
  { 1, -1,  0},
  { 1,  0, -1},
  { 0,  1, -1},
  {-1,  1,  0},
  {-1,  0,  1},
  { 0, -1,  1}
};

/* -variable ------------------------------------------------------------------*/



/* +struct right or left motor --------------------------------------*/
MOTOR_PARAMETER motor_right = {
    0.0,                                                 //integral_record   PI積分累積儲存
    0,                                                   //rpm_count  計數motor speed
    0,                                                   //adc_value
    15,                                                  //pwmValue ; Current PWM value (adjustable)
    0,                                                   //pwmValue_temp  圈/s
    0,                                                   //present_speed
    0,                                                   //clockwise ; 1 = clockwise, 0 = counter clockwise
    -1,                                                  //currentStep

    {GPIOC, GPIOC, GPIOC },                              //Hall sensor pins
    {GPIO_PIN_1,GPIO_PIN_2,GPIO_PIN_3 },

    //right Pin definitions for the three phases
    {GPIOA, GPIOA, GPIOB, GPIOB, GPIOB, GPIOB },
    {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_10, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15 },
    //AH BH CH AL BL CL

    {&htim2, &htim2, &htim2 },
    {TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3 }

};

MOTOR_PARAMETER motor_left = {
    0.0,                                                  //integral_record   PI積分累積儲存
    0,                                                    //rpm_count  計數motor speed
    0,                                                    //adc_value
    15,                                                   //pwmValue ; Current PWM value (adjustable)
    0,                                                    //pwmValue_temp
    0,                                                    //present_speed
    0,                                                    //clockwise ; 1 = clockwise, -1 = counter clockwise
    -1,                                                   //currentStep

    {GPIOC, GPIOC, GPIOC },                               //Hall sensor pins
    {GPIO_PIN_5,GPIO_PIN_6,GPIO_PIN_8 },

    //left Pin definitions for the three phases
    {GPIOA, GPIOA, GPIOB, GPIOC, GPIOC, GPIOC  },
    {GPIO_PIN_6, GPIO_PIN_4, GPIO_PIN_0, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12 },
    //AH BH CH AL BL CL

    {&htim3, &htim3, &htim3 },
    {TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3 }

};

/* -struct right or left motor --------------------------------------*/



// +Basis set -------------------------------------------------------*/
void motor_setup(MOTOR_PARAMETER Motor_R, MOTOR_PARAMETER Motor_L ) {
    //right_set
    HAL_TIM_PWM_Start(Motor_R.TIMx[0], Motor_R.TIM_CHANNEL_x[0]);
    HAL_TIM_PWM_Start(Motor_R.TIMx[1], Motor_R.TIM_CHANNEL_x[1]);
    HAL_TIM_PWM_Start(Motor_R.TIMx[2], Motor_R.TIM_CHANNEL_x[2]);

    //left_set
    HAL_TIM_PWM_Start(Motor_L.TIMx[0], Motor_L.TIM_CHANNEL_x[0]);
    HAL_TIM_PWM_Start(Motor_L.TIMx[1], Motor_L.TIM_CHANNEL_x[1]);
    HAL_TIM_PWM_Start(Motor_L.TIMx[2], Motor_L.TIM_CHANNEL_x[2]);
}

/* -Basis set -------------------------------------------------------*/



/* +renew motor speed and hall signal -------------------------------*/
void commutateMotor(MOTOR_PARAMETER motor) {
    for (int i = 0; i < 3; i++) {
        if (SEQUENCE[motor.currentStep][i] == 1) {
            __HAL_TIM_SET_COMPARE(motor.TIMx[i], motor.TIM_CHANNEL_x[i], motor.pwmValue);
            HAL_GPIO_WritePin(motor.M_GPIOx[i + 3], motor.M_GPIO_Pin_x[i + 3],  GPIO_PIN_RESET);
        } else if (SEQUENCE[motor.currentStep][i] == -1) {
            __HAL_TIM_SET_COMPARE(motor.TIMx[i], motor.TIM_CHANNEL_x[i], 0);
            HAL_GPIO_WritePin(motor.M_GPIOx[i + 3], motor.M_GPIO_Pin_x[i + 3],  GPIO_PIN_SET);
        } else {
            __HAL_TIM_SET_COMPARE(motor.TIMx[i], motor.TIM_CHANNEL_x[i], 0);
            HAL_GPIO_WritePin(motor.M_GPIOx[i + 3], motor.M_GPIO_Pin_x[i + 3],  GPIO_PIN_RESET);
        }
    }
}

/* +renew motor speed and hall signal -------------------------------*/



/* +hall senser and decide clockwise --------------------------------*/
void updateMotorStep(MOTOR_PARAMETER *motor) {
    int hallState =
           (HAL_GPIO_ReadPin(motor->Hall_GPIOx[0], motor->Hall_GPIO_Pin_x[0]) << 2)
         | (HAL_GPIO_ReadPin(motor->Hall_GPIOx[1], motor->Hall_GPIO_Pin_x[1]) << 1)
         | (HAL_GPIO_ReadPin(motor->Hall_GPIOx[2], motor->Hall_GPIO_Pin_x[2]));
    if (motor->clockwise == 0) {          //逆
        switch(hallState) {
            case 2: motor->currentStep = 0; break;
            case 3: motor->currentStep = 1; break;
            case 1: motor->currentStep = 2; break;
            case 5: motor->currentStep = 3; break;
            case 4: motor->currentStep = 4; break;
            case 6: motor->currentStep = 5; break;
        }
    }
    else if(motor->clockwise == 1) {        //順
        switch(hallState) {
            case 5: motor->currentStep = 0; break;
            case 4: motor->currentStep = 1; break;
            case 6: motor->currentStep = 2; break;
            case 2: motor->currentStep = 3; break;
            case 3: motor->currentStep = 4; break;
            case 1: motor->currentStep = 5; break;
        }
    }
}

/* -hall senser and decide clockwise --------------------------------*/
