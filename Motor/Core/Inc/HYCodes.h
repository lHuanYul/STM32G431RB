/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : HYCodes.h
  * @brief          : Header for Codes file.
  ******************************************************************************
  */
/* USER CODE END Header */
// Includes ------------------------------------------------------------------
#include <stdint.h>
#include <stdint.h>
#include <stm32g431xx.h>
// Define --------------------------------------------------------------------
#define APB1_TIM_Clock 170 // MHz
#define APB2_TIM_Clock 170 // MHz
#define Motor_TIM_AutoReload 1000
#define TIM2_AutoReload Motor_TIM_AutoReload
#define TIM2_DutyRatio 50
#define TIM3_AutoReload Motor_TIM_AutoReload
#define TIM3_DutyRatio 50

#define BOOL int
#define true 1
#define false 0

typedef struct { //Motor
    TIM_HandleTypeDef* htim;
    uint32_t Channel[3];
    GPIO_TypeDef* L_GPIOx[3];
    uint16_t L_GPIO_Pin_x[3];
    GPIO_TypeDef* D_GPIOx[3];
    uint16_t D_GPIO_Pin_x[3];
    int pwm_value;
} MOTOR;
// Global Var ----------------------------------------------------------------

// Private function prototypes -----------------------------------------------
void HYCodes_Init(void);
void HYCodes_Main(void);
void HYCodes_TIM2_IRQHandler_Before(void);
void HYCodes_TIM3_IRQHandler_Before(void);
void HYCodes_EXTI15_10_Before(void);
void HYCodes_EXTI15_10_After(void);
void MotorPWMStart(MOTOR);
void MotorSpin(MOTOR);
void MotorSpeedSet(MOTOR*, int);
