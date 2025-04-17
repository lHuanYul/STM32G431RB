/** 
  ******************************************************************************
  * @file           : HYCodes.c
  * @brief          : HY Codes file
  ******************************************************************************
  */
// Includes (Copy from main.c) -----------------------------------------------
#include "tim.h"
#include "myIncCodes/motor.h"
#include "myIncCodes/my_usart.h"
#include "myIncCodes/my_tim.h"
// Local Var -----------------------------------------------------------------
#define ABOVE_ROAD_MIN 10
#define ABOVE_ROAD_MAX ABOVE_ROAD_MIN+9
#define ABOVE_CROSS_MIN 20
#define ABOVE_CROSS_MAX ABOVE_CROSS_MIN+9

float derivative_t, integral;
int motor2_rpm_setpoint, motor3_rpm_setpoint;

// LED(A5) Button(C13)

// Init ----------------------------------------------------------------------
void HYCodes_Init(void) {
    MapCreate();
    UARTBufferInit();
    derivative_t = 50 * TIMER6_COUNT / 1000;
}

// Main ----------------------------------------------------------------------
void HYCodes_Main(void) {
    MotorPWMStart(motor2);
    HAL_TIM_Base_Start_IT(&htim6);
    HAL_UART_Receive_DMA(&huart1, uart_buffer_r, sizeof(uart_buffer_r));

    MotorSpin(motor2);
    while(true) {
        /*if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)) {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
            MotorSpeedSet(&motor2, 0);
        } else {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
            MotorSpeedSet(&motor2, 50);
        }*/
        
        //running++;
    }
}

void StateRun(int adc_value) {
    if((adc_value > ABOVE_ROAD_MIN) && (adc_value > ABOVE_ROAD_MAX)) {
        //VehicleMoveStraight(50);
    }
    
}

/*void PI_Controller(double measurement) {
    double error = setpoint - measurement;  // 計算誤差
    integral += error * dt;  // 積分項累積
    double output = Kp * error + Ki * integral;  // 計算 P I 控制輸出

    pwmValue += output * 0.1;

    // 限制 PWM 占空比範圍
    if (output > MAX_DUTY) output = MAX_DUTY;
    if (output < MIN_DUTY) output = MIN_DUTY;
}*/

