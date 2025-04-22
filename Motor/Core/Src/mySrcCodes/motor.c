#include "tim.h"
#include "gpio.h"
#include "myIncCodes/motor.h"

bool gpio_pin_state_map[12][6] = {
    // AH AL BH BL CH CL
    // Motor pin output (Original_state H1*4 +H2*2 +H3*1 -> New_state At MotorSpin)
    // Clockwise
    {1, 0, 0, 1, 0, 0},
    {1, 0, 0, 0, 0, 1},
    {0, 0, 1, 0, 0, 1},
    {0, 1, 1, 0, 0, 0},
    {0, 1, 0, 0, 1, 0},
    {0, 0, 0, 1, 1, 0},
    // Counter Clockwise
    {0, 0, 0, 1, 1, 0},
    {0, 1, 0, 0, 1, 0},
    {0, 1, 1, 0, 0, 0},
    {0, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 0, 0}
};

MOTOR motor2 = {
    // Speed value setpoint/realtime
    0, 0,
    // AH BH CH (PA0 PA1 PB10)
    &htim2,
    {TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3},
    {GPIOB,         GPIOB,          GPIOB},
    {GPIO_PIN_3,    GPIO_PIN_4,     GPIO_PIN_5},
    {GPIOB,         GPIOB,          GPIOB},
    {GPIO_PIN_13,   GPIO_PIN_14,    GPIO_PIN_15},
    0, 0, 0
};

MOTOR motor3 = {
    0, 0,
    // AH BH CH (PA6 PA4 PC8)
    &htim3,
    {TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3},
    // AL BL CL
    {GPIOB,         GPIOB,          GPIOB},
    {GPIO_PIN_3,    GPIO_PIN_4,     GPIO_PIN_5},
    // H1 H2 H3
    {GPIOB,         GPIOB,          GPIOB},
    {GPIO_PIN_13,   GPIO_PIN_14,    GPIO_PIN_15},
    0, 0, 0
};

/// Start motor PWM
void MotorPWMStart(MOTOR motor) {
    for(int i = 0; i < 3; i++) {
        HAL_TIM_PWM_Start_IT(motor.htim, motor.Channel[i]);
    }
}

/// Motor detect & control
void MotorSpin(MOTOR motor) {
    // Hall sensor pins read
    int state = (
        (HAL_GPIO_ReadPin(motor.D_GPIOx[0], motor.D_GPIO_Pin_x[0]) << 2) |
        (HAL_GPIO_ReadPin(motor.D_GPIOx[1], motor.D_GPIO_Pin_x[1]) << 1) |
        (HAL_GPIO_ReadPin(motor.D_GPIOx[2], motor.D_GPIO_Pin_x[2]) << 0)
    );
    // Renew state
    int _pwm;
    if(motor.arr_trigger_value >= 0) {
        // Clockwise
        _pwm = motor.arr_trigger_value;
        switch(state) {
            case 5: state = 0; break;
            case 4: state = 1; break;
            case 6: state = 2; break;
            case 2: state = 3; break;
            case 3: state = 4; break;
            case 1: state = 5; break;
            default: state = 0; break;
        }
    } else {
        _pwm = -motor.arr_trigger_value;
        // Counter Clockwise
        switch(state) {
            case 2: state = 0; break;
            case 3: state = 1; break;
            case 1: state = 2; break;
            case 5: state = 3; break;
            case 4: state = 4; break;
            case 6: state = 5; break;
            default: state = 0; break;
        }
    }
    // Motor pins set A->B->C
    for(int i = 0; i < 3; i++) {
        // H set/clear
        if (gpio_pin_state_map[state][i*2]) {
            __HAL_TIM_SET_COMPARE(motor.htim, motor.Channel[i], _pwm);
        } else {
            __HAL_TIM_SET_COMPARE(motor.htim, motor.Channel[i], 0);
        }
        // L set/clear
        HAL_GPIO_WritePin(motor.L_GPIOx[i], motor.L_GPIO_Pin_x[i], gpio_pin_state_map[state][i*2+1]? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}

/// Motor speed set
void MotorPWMSetpointSet(MOTOR* motor, short speed) {
    if(speed >= -100 && speed <= 100) motor->pwm_setpoint = speed;
    MotorPWMRealtimeSet(motor); // TEMP
}

/// Motor pwm set
void MotorPWMRealtimeSet(MOTOR* motor) {
    // +PI Control
    short speed = motor->pwm_setpoint;
    if(speed >= -100 && speed <= 100) motor->arr_trigger_value = MOTOR_TIM_AUTORELOAD * speed / 100;
}

void MotorSpeedInitTest(void) {
    MotorPWMSetpointSet(&motor2, 100);
    MotorPWMSetpointSet(&motor3, 100);
    HAL_Delay(1000);
    MotorPWMSetpointSet(&motor2, 0);
    MotorPWMSetpointSet(&motor3, 0);
}
