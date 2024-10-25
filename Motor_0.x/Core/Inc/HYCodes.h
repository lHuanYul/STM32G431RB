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
// Define --------------------------------------------------------------------
#define APB1_TIM_Clock 170 // MHz
#define APB2_TIM_Clock 170 // MHz
#define TIM1_AutoReload 1000 // Timer 2 (PWM) auto reload register
#define BOOL int
#define true 1
#define false 0

// Global Var ----------------------------------------------------------------
extern int TIM1_DutyRatio; // Timer 2 (PWM) duty ratio

// Private function prototypes -----------------------------------------------
void HYCodes_Init(void);
void HYCodes_Main(void);
void HYCodes_EXTI15_10(void);
void MotorSpin(void);
void MotorSpeed(void);
