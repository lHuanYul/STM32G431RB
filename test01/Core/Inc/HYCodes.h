/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : HYCodes.h
  * @brief          : Header for Codes file.
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Define --------------------------------------------------------------------*/
#define APB1_TIM_Clock 170//MHz
#define APB2_TIM_Clock 170//MHz
#define USART1_BaudRate 115200
#define BOOL int
#define true 1
#define false 0
//Define Pin
#define SET_PA5 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
#define CLR_PA5 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
//Define Music Node
//3
#define MNote_C3 131
#define MNoteS_L1 131
#define MNote_CS3 139
#define MNote_D3 147
#define MNoteS_L2 147
#define MNote_DS3 156
#define MNote_E3 165
#define MNoteS_L3 165
#define MNote_F3 175
#define MNoteS_L4 175
#define MNote_FS3 185
#define MNote_G3 196
#define MNoteS_L5 196
#define MNote_GS3 208
#define MNote_A3 220
#define MNoteS_L6 220
#define MNote_AS3 233
#define MNote_B3 247
#define MNoteS_L7 247
//4
#define MNote_C4 262
#define MNoteS_N1 262
#define MNote_CS4 277
#define MNote_D4 294
#define MNoteS_N2 294
#define MNote_DS4 311
#define MNote_E4 330
#define MNoteS_N3 330
#define MNote_F4 349
#define MNoteS_N4 349
#define MNote_FS4 370
#define MNote_G4 392
#define MNoteS_N5 392
#define MNote_GS4 415
#define MNote_A4 440
#define MNoteS_N6 440
#define MNote_AS4 466
#define MNote_B4 494
#define MNoteS_N7 494
//5
#define MNote_C5 523
#define MNoteS_H1 523
#define MNote_CS5 554
#define MNote_D5 587
#define MNoteS_H2 587
#define MNote_DS5 622
#define MNote_E5 659
#define MNoteS_H3 659
#define MNote_F5 698
#define MNoteS_H4 698
#define MNote_FS5 740
#define MNote_G5 784
#define MNoteS_H5 784
#define MNote_GS5 831
#define MNote_A5 880
#define MNoteS_H6 880
#define MNote_AS5 932
#define MNote_B5 988
#define MNoteS_H7 988

/* Global Var ----------------------------------------------------------------*/
extern int frequency;
extern int duty_ratio;
extern int timflag;
extern int test;
extern int selecter[];
extern int sheet[];

/* Private function prototypes -----------------------------------------------*/
void HYCodes_Init(void);
void HYCodes_Main(void);
void HYCodes_ItButtonPC13(void);
void HYCodes_ItTim1(void);
void HYCodes_ItTim2(void);
void HYCodes_ItUART2(void);
uint16_t Motor(uint16_t input);
