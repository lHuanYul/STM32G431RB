/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    HYCodes.c
  * @brief   HY Codes file
  ******************************************************************************
  */
/* USER CODE END Header */
// Includes (Copy from main.c) -----------------------------------------------
#include "main.h"
#include "tim.h"
#include "gpio.h"
// Global Var ----------------------------------------------------------------
int state;
int TIM1_DutyRatio = 100;

// Local Var -----------------------------------------------------------------
typedef struct { // GPIO Pin struct
    GPIO_TypeDef* GPIOx;
    uint16_t GPIO_Pin_x;
} GPIO_PinConfig;
typedef struct { // PWM Pin struct
    TIM_HandleTypeDef* htim;
    uint32_t Channel;
} TIM_ChannelConfig;
GPIO_PinConfig gpio_pins_detector[] = { // Detector pin define
    {GPIOB, GPIO_PIN_13}, // H1
    {GPIOB, GPIO_PIN_14}, // H2
    {GPIOB, GPIO_PIN_15}  // H3
};
TIM_ChannelConfig tim_pins_motor[] = {
    {&htim1, TIM_CHANNEL_1}, //AH-PC0
    {&htim1, TIM_CHANNEL_2}, //BH-PC1
    {&htim1, TIM_CHANNEL_3}  //CH-PC2
};
GPIO_PinConfig gpio_pins_motor[] = { // Motor pin define
    {GPIOB, GPIO_PIN_3}, // AL
    {GPIOB, GPIO_PIN_4}, // BL
    {GPIOB, GPIO_PIN_5}  // CL
};
// Timer 2 (PWM) output on PA0
int gpio_pin_state_map[6][6] = { // Motor pin output (state = H1*1 + H2*2 + H3*4) 546231
    {1, 0, 0, 0, 0, 1},
    {0, 0, 1, 0, 0, 1},
    {0, 1, 1, 0, 0, 0},
    {0, 1, 0, 0, 1, 0},
    {0, 0, 0, 1, 1, 0},
    {1, 0, 0, 1, 0, 0}
};
// Init ----------------------------------------------------------------------
void HYCodes_Init(void){
    
}
// Main ----------------------------------------------------------------------
/* Main function */
void HYCodes_Main(void){
    MotorSpeed(); // Motor speed set
    MotorSpin(); // Motor first output set
    while(true){
        
    }
}
// Interrupt -----------------------------------------------------------------
/* EXTI15_10 Interrupt */
void HYCodes_EXTI15_10(void){
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
    MotorSpin();
}
// Function ------------------------------------------------------------------
/* Motor detect & control */
void MotorSpin(void){
    state = 0;
    for(int i = 0; i < 3; i++) { // Detector Pins read (根據i的值動態加上4,2,1)
        if(HAL_GPIO_ReadPin(gpio_pins_detector[i].GPIOx, gpio_pins_detector[i].GPIO_Pin_x) == GPIO_PIN_SET) {
            state += (1 << (2-i));
        }
    }
    switch (state)
    {
    case 5:
        state = 0;
        break;
    case 4:
        state = 1;
        break;
    case 6:
        state = 2;
        break;
    case 2:
        state = 3;
        break;
    case 3:
        state = 4;
        break;
    case 1:
        state = 5;
        break;
    default:
        return;
    }
    for(int i = 0; i < 6; i+=2) { // Motor pins write (根據狀態變化輸出)
        int i2 = i/2;
        if (gpio_pin_state_map[state][i]) {
            HAL_TIM_PWM_Start(tim_pins_motor[i2].htim, tim_pins_motor[i2].Channel);
        } else {
            HAL_TIM_PWM_Stop(tim_pins_motor[i2].htim, tim_pins_motor[i2].Channel);
        }
        HAL_GPIO_WritePin(gpio_pins_motor[i+1].GPIOx, gpio_pins_motor[i+1].GPIO_Pin_x, gpio_pin_state_map[state][i+1]? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}
/* Motor speed control */
void MotorSpeed(void){
    if(TIM1_DutyRatio < 0 || TIM1_DutyRatio > 100) TIM1_DutyRatio = 50;
    int speed = TIM1_AutoReload * TIM1_DutyRatio / 100;
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, speed);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, speed);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, speed);
}
