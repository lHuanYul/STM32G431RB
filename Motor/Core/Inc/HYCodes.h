/**
  ******************************************************************************
  * @file           : HYCodes.h
  * @brief          : HY Codes Header file
  ******************************************************************************
  */
// Includes ------------------------------------------------------------------
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stm32g431xx.h>
// Define --------------------------------------------------------------------
#define APB1_TIM_CLOCK 170 // MHz
#define APB2_TIM_CLOCK 170 // MHz
#define MOTOR_TIM_AUTORELOAD 1000
#define TIM2_AUTORELOAD MOTOR_TIM_AUTORELOAD
#define TIM2_DUTYRATIO 50
#define TIM3_AUTORELOAD MOTOR_TIM_AUTORELOAD
#define TIM3_DUTYRATIO 50
#define TIM6_AUTORELOAD 50000
#define USART1_BAUDRATE 115200
#define USART2_BAUDRATE 115200
#define Hello hello

#define direction char
#define RIGHT -1
#define LEFT 1

#define MAX_CONNECTIONS 8
#define UINT32INF 4294967295
typedef struct {
    uint32_t locate;
    uint32_t length;
} CONNECT;
typedef struct {
    uint32_t locate;
    uint32_t length;
} CONNECT_INPUT;
typedef struct LOCATION {
    uint32_t id;
    CONNECT connect[MAX_CONNECTIONS];
} LOCATION;
typedef struct {
    uint32_t id;
    CONNECT_INPUT connect[MAX_CONNECTIONS];
} LOCATION_INPUT;

typedef struct {
    short pwm_setpoint;
    TIM_HandleTypeDef* htim;
    uint32_t Channel[3];
    GPIO_TypeDef* L_GPIOx[3];
    uint16_t L_GPIO_Pin_x[3];
    GPIO_TypeDef* D_GPIOx[3];
    uint16_t D_GPIO_Pin_x[3];
    uint32_t arr_trigger_value;
    uint16_t rotate_count;
    float rps;
} MOTOR;

typedef struct {
    uint8_t function;
    uint8_t data[8];
    uint8_t end;
} UART_BUFFER;


/* Global Var ---------------------------------------------------------------- */

// Private function prototypes -----------------------------------------------
void HYCodes_Init(void);
void HYCodes_Main(void);
void HYCodes_TIM2_IRQHandler_Before(void);
void HYCodes_TIM3_IRQHandler_Before(void);
void HYCodes_TIM6_IRQHandler_Before(void);
void HYCodes_EXTI15_10_Before(void);
void HYCodes_USART1_IRQHandler_Before(void);
bool MapCreate(void);
bool UARTBufferInit(void);
void MotorPWMStart(MOTOR);
void MotorSpin(MOTOR);
void MotorSpeedSet(MOTOR*, short);
int  ADCAvgCal(int *, int);
void VehicleStop(void);
void VehicleMoveStraight(short);
void VehicleMoveTurn(direction, short);
