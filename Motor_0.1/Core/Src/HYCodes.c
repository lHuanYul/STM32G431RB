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

// Local Var -----------------------------------------------------------------
typedef struct { // Pin struct
    GPIO_TypeDef* GPIOx;
    uint16_t GPIO_Pin_x;
} GPIO_PinConfig;
GPIO_PinConfig gpio_pins_detector[] = { // Detector pin define
    {GPIOB, GPIO_PIN_6}, // H1
    {GPIOB, GPIO_PIN_7}, // H2
    {GPIOB, GPIO_PIN_8}  // H3
};
GPIO_PinConfig gpio_pins_motor[] = { // Motor pin define
    {GPIOB, GPIO_PIN_0}, // AH
    {GPIOB, GPIO_PIN_1}, // AL
    {GPIOB, GPIO_PIN_2}, // BH
    {GPIOB, GPIO_PIN_3}, // BL
    {GPIOB, GPIO_PIN_5}, // CH
    {GPIOB, GPIO_PIN_5}  // CL
};
// Timer 2 (PWM) output on PA0
int gpio_pin_state_map[6][6] = { // Motor pin output (state = -1 + H1*1 + H2*2 + H3*4)
    {1, 0, 0, 0, 0, 1},
    {0, 1, 1, 0, 0, 0},
    {0, 0, 1, 0, 0, 1},
    {0, 0, 0, 1, 1, 0},
    {1, 0, 0, 1, 0, 0},
    {0, 1, 0, 0, 1, 0}
};
// Init ----------------------------------------------------------------------
void HYCodes_Init(void){
    
}
// Main ----------------------------------------------------------------------
/* Main function */
void HYCodes_Main(void){
    HAL_TIM_Base_Start(&htim2); // Timer 2 (PWM) start
    Motor(); // Motor first output set
    while(true){
        
    }
}
// Interrupt -----------------------------------------------------------------
/* PC13 Interrupt */
void HYCodes_ItButtonPC13(void){
    Motor();
}
/* PB6/7/8 Interrupt (Spin Motor)*/
void HYCCodes_ItMotorDetector(void){
    Motor();
}
// Function ------------------------------------------------------------------
/* Motor detect & control */
void Motor(void){
    int state = -1;
    for(int i = 0; i < 3; i++) { // Detector Pins read (根據i的值動態加上1,2或4)
        if(HAL_GPIO_ReadPin(gpio_pins_detector[i].GPIOx, gpio_pins_detector[i].GPIO_Pin_x) == GPIO_PIN_SET) {
            state += (1 << i);
        }
    }
    if(state < 0 || state > 5) return;
    for(int i = 0; i < 6; i++) { // Motor pins write (根據狀態變化輸出)
        HAL_GPIO_WritePin(gpio_pins_motor[i].GPIOx, gpio_pins_motor[i].GPIO_Pin_x, gpio_pin_state_map[state][i]? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}
