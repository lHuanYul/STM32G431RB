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
#define TIM2_AutoReload 1000 // Timer 2 (PWM) auto reload register
#define TIM2_DutyRatio 50 // Timer 2 (PWM) duty ratio
#define BOOL int
#define true 1
#define false 0

// Global Var ----------------------------------------------------------------

// Private function prototypes -----------------------------------------------
void HYCodes_Init(void);
void HYCodes_Main(void);
void HYCodes_ItButtonPC13(void);
void HYCCodes_ItMotorDetector(void);
void MotorSpin(void);
void MotorSpeed(int);
