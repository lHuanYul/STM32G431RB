#include "usart.h"
#include "gpio.h"
#include "myIncCodes/it.h"
#include "myIncCodes/motor.h"
#include "myIncCodes/my_tim.h"

float rps_calculate = 20 / TIMER6_COUNT;

void HYCodes_TIM2_IRQHandler_Before(void) {
    MotorSpin(motor2);
}

void HYCodes_TIM3_IRQHandler_Before(void) {
    MotorSpin(motor2);
}

void HYCodes_TIM6_IRQHandler_Before(void) {
    timer6_count++;
    if (timer6_count >= TIMER6_COUNT) {
        motor2.rps = motor2.rotate_count * rps_calculate / 6;
        motor3.rps = motor3.rotate_count * rps_calculate / 6;
        motor2.rotate_count = 0;
        motor3.rotate_count = 0;
        timer6_count = 0;
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    }
}

void HYCodes_EXTI15_10_Before(void) {
    motor2.rotate_count++;
    motor3.rotate_count++;
    MotorSpin(motor2);

    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}

void HYCodes_USART1_IRQHandler_Before(void) {}
