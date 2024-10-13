/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    HYCodes.c
  * @brief   HY Codes file
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
/* Global Var ----------------------------------------------------------------*/
int frequency = MNote_C4;
int duty_ratio = 50;
int selecter[6];
int sheet[6];
int timflag;
int test;
uint8_t rx_data[1];
uint8_t tx_data[1] = "H";
/* Init ----------------------------------------------------------------------*/
void HYCodes_Init(void){
    frequency = MNote_C4;
    duty_ratio = 50;
    test = 87;
}
/* Main ----------------------------------------------------------------------*/
void HYCodes_Main(void){
    //Normal Timer Test
    /*
    HAL_TIM_Base_Start_IT(&htim1);
    */
    while(true){
        //PWM Music Test
        /*
        int music_sheet[] = {MNote_C4, MNote_D4, MNote_E4, MNote_F4, MNote_G4, MNote_A4, MNote_B4};
        for(int i = 0; i < 7; i+=1){
            frequency = music_sheet[i];
            uint32_t frequency_c1 = 1000000 / frequency;
            TIM2->CNT = 0;
            TIM2->ARR = frequency_c1;
            TIM2->CCR1 = frequency_c1 * duty_ratio / 100;
            HAL_Delay(200);
        }
        */
        //Motor & Detector Test
        /*
        uint16_t input = (GPIOB->IDR >> 6) & 0x0007;
        uint16_t output = GPIOB->ODR & 0xFFC0;
        output |= Motor(input);
        GPIOB->ODR = output;
        */
        //USART Test
        /*
        if(HAL_UART_Receive(&huart1, rx_data, 1, HAL_MAX_DELAY) == HAL_OK){

        }
        if(rx_data[0] == 'H'){
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
            rx_data[0] = 'Y';
        }
        */
    }
}
/* Interrupt -----------------------------------------------------------------*/
void HYCodes_ItButtonPC13(void){
    //Button Control Tim2 Enable
    
    if (TIM2->CR1 & TIM_CR1_CEN) {
        HAL_TIM_PWM_Stop_IT(&htim2,TIM_CHANNEL_1);
        GPIOA->BSRR = (1 << 5);
        //TIM2->CR1 &= ~TIM_CR1_CEN;
    } else {
        HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
        GPIOA->BSRR = (1 << (5+16));
        //TIM2->CR1 |= TIM_CR1_CEN;
    }
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    
    //USART Test
    /*
    HAL_UART_Transmit(&huart1, tx_data, 1, HAL_MAX_DELAY);
    */
}
void HYCodes_ItTim1(void){
    
}
void HYCodes_ItTim2(void){

}
void HYCodes_ItUART2(void){
    
}
/* Function ------------------------------------------------------------------*/
uint16_t Motor(uint16_t input){
    /* 外部使用
    uint16_t input = (GPIOB->IDR >> 6) & 0x0007;
    uint16_t output = GPIOB->ODR & 0xFFC0;
    output |= Motor(input);
    GPIOB->ODR = output;
    */
    switch (input)
    {
    case 0:
        return 0x30;
    case 1:
        return 0x18;
    case 2:
        return 0x0C;
    case 3:
        return 0x06;
    case 4:
        return 0x03;
    case 5:
        return 0x21;
    default:
        return 0x00;
    }
}
