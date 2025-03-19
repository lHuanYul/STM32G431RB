/*
  ******************************************************************************
  * @file    HYCodes.c
  * @brief   HY Codes file
  ******************************************************************************
*/
/* Includes (Copy from main.c) ----------------------------------------------- */
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Global Var ---------------------------------------------------------------- */
int clockwise;

int test00;
int test01;
int test02;

/* Local Var ----------------------------------------------------------------- */
// LED(A5) Button(C13)
MOTOR motor2 = {
    // AH BH CH (PA0 PA1 PB10)
    &htim2,
    {TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3},
    // AL BL CL
    {GPIOB, GPIOB, GPIOB},
    {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5},
    // H1 H2 H3 
    {GPIOB, GPIOB, GPIOB},
    {GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15},
    // Speed inited
    0
};

MOTOR motor3 = {
    // AH BH CH (PA6 PA4 PC8)
    &htim3,
    {TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3},
    // AL BL CL
    {GPIOB, GPIOB, GPIOB},
    {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5},
    // H1 H2 H3
    {GPIOB, GPIOB, GPIOB},
    {GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15},
    // Speed inited
    0
};

int gpio_pin_state_map[12][6] = {
    // AH AL BH BL CH CL
    // Motor pin output (Original_state H1*4 +H2*2 +H3*1 -> New_state At MotorSpin)
    // Clockwise
    {1, 0, 0, 1, 0, 0},
    {1, 0, 0, 0, 0, 1},
    {0, 0, 1, 0, 0, 1},
    {0, 1, 1, 0, 0, 0},
    {0, 1, 0, 0, 1, 0},
    {0, 0, 0, 1, 1, 0},
    // Counter Clockwise
    {0, 0, 0, 1, 1, 0},
    {0, 1, 0, 0, 1, 0},
    {0, 1, 1, 0, 0, 0},
    {0, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 0, 0}
};

/* Init ---------------------------------------------------------------------- */
void HYCodes_Init(void) {
    test01 = 0;
    test02 = 0;
}

/* Main ---------------------------------------------------------------------- */
void HYCodes_Main(void) {
    MotorPWMStart(motor2);
    MotorSpeedSet(&motor2, 0);
    
    MotorSpin(motor2);
    while(true) {
        if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)) {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
            MotorSpeedSet(&motor2, 0);
        } else {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
            MotorSpeedSet(&motor2, 50);
        }
    }
}
/* Interrupt ----------------------------------------------------------------- */
void HYCodes_TIM2_IRQHandler_Before() {
    MotorSpin(motor2);
}
void HYCodes_TIM3_IRQHandler_Before() {
    MotorSpin(motor2);
}
void HYCodes_EXTI15_10_Before(void) {
    MotorSpin(motor2);

    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}
void HYCodes_EXTI15_10_After(void) {
    
}
/* Function ------------------------------------------------------------------ */
void MotorPWMStart(MOTOR motor) {
    for(int i = 0; i < 3; i++) {
        HAL_TIM_PWM_Start_IT(motor.htim, motor.Channel[i]);
    }
}

// Motor detect & control
void MotorSpin(MOTOR motor) {
    // HDetector Pins read
    int state = (
        (HAL_GPIO_ReadPin(motor.D_GPIOx[0], motor.D_GPIO_Pin_x[0]) << 2) |
        (HAL_GPIO_ReadPin(motor.D_GPIOx[1], motor.D_GPIO_Pin_x[1]) << 1) |
        (HAL_GPIO_ReadPin(motor.D_GPIOx[2], motor.D_GPIO_Pin_x[2]) << 0)
    );
    test00 = state;
    // Renew state
    if(motor.pwm_value) {
        // Clockwise
        switch(state) {
            case 5: state = 0; break;
            case 4: state = 1; break;
            case 6: state = 2; break;
            case 2: state = 3; break;
            case 3: state = 4; break;
            case 1: state = 5; break;
            default: state = 0; break;
        }
    } else {
        // Counter Clockwise
        switch(state) {
            case 2: state = 0; break;
            case 3: state = 1; break;
            case 1: state = 2; break;
            case 5: state = 3; break;
            case 4: state = 4; break;
            case 6: state = 5; break;
            default: state = 0; break;
        }
    }
    // Motor pins set A->B->C
    for(int i = 0; i < 3; i++) {
        // H set/clear
        if (gpio_pin_state_map[state][i*2]) {
            __HAL_TIM_SET_COMPARE(motor.htim, motor.Channel[i], motor.pwm_value);
        } else {
            __HAL_TIM_SET_COMPARE(motor.htim, motor.Channel[i], 0);
        }
        // L set/clear
        HAL_GPIO_WritePin(motor.L_GPIOx[i], motor.L_GPIO_Pin_x[i], gpio_pin_state_map[state][i*2+1]? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}

//Motor speed set
void MotorSpeedSet(MOTOR* motor, int speed) {
    if(speed >= -100 && speed <= 100) motor->pwm_value = Motor_TIM_AutoReload * speed / 100;
}
