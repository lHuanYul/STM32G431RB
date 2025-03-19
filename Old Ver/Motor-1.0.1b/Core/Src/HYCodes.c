/*
  ******************************************************************************
  * @file    HYCodes.c
  * @brief   HY Codes file
  ******************************************************************************
*/
/* Includes (Copy from main.c) ----------------------------------------------- */
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Local Var ----------------------------------------------------------------- */
#define ABOVE_ROAD_MIN 10
#define ABOVE_ROAD_MAX ABOVE_ROAD_MIN+9
#define ABOVE_CROSS_MIN 20
#define ABOVE_CROSS_MAX ABOVE_CROSS_MIN+9

#define LOCATION_NUMBERS 16
LOCATION locations[LOCATION_NUMBERS] = {0};
LOCATION_INPUT locations_input[LOCATION_NUMBERS] = {
    {0, {{0, 0}}},
    {1, {
        {2, 1},
        {3, 1},
        {0, 0}}},
    {2, {
        {1, 1},
        {0, 0}}},
    {3, {
        {1, 1},
        {4, 1},
        {0, 0}}},
    {4, {
        {3, 1},
        {0, 0}}}
};

#define ADC0_buffer_LEN 8
#define ADC1_buffer_LEN 8
uint16_t ADC0_buffer[ADC0_buffer_LEN] = {0};
uint16_t ADC1_buffer[ADC1_buffer_LEN] = {0};

// LED(A5) Button(C13)
MOTOR motor2 = {
    // AH BH CH (PA0 PA1 PB10)
    &htim2,
    {TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3},
    // AL BL CL
    {GPIOB, GPIOB, GPIOB},
    {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5},
    // H1 H2 H3 
    {GPIOB, GPIOB, GPIOB},
    {GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15},
    // Speed inited
    0
};

MOTOR motor3 = {
    // AH BH CH (PA6 PA4 PC8)
    &htim3,
    {TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3},
    // AL BL CL
    {GPIOB, GPIOB, GPIOB},
    {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5},
    // H1 H2 H3
    {GPIOB, GPIOB, GPIOB},
    {GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15},
    // Speed inited
    0
};

int gpio_pin_state_map[12][6] = {
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

/* Init ---------------------------------------------------------------------- */
void HYCodes_Init(void) {
    CreateMap();
}

/* Main ---------------------------------------------------------------------- */
void HYCodes_Main(void) {
    MotorPWMStart(motor2);
    MotorSpeedSet(&motor2, 0);
    
    MotorSpin(motor2);
    while(true) {
        if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)) {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
            MotorSpeedSet(&motor2, 0);
        } else {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
            MotorSpeedSet(&motor2, 50);
        }
    }
}
/* Interrupt ----------------------------------------------------------------- */
void HYCodes_TIM2_IRQHandler_Before(void) {
    MotorSpin(motor2);
}
void HYCodes_TIM3_IRQHandler_Before(void) {
    MotorSpin(motor2);
}
void HYCodes_EXTI15_10_Before(void) {
    MotorSpin(motor2);

    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}
void HYCodes_EXTI15_10_After(void) {
    
}
/* Function ------------------------------------------------------------------ */
// Map create
bool CreateMap(void) {
    int i, j;
    uint32_t _id, _locate, _length;
    // Init map
    for(i = 0; i < LOCATION_NUMBERS; i++) {
        locations[i].id = i;
        for(j = 0; j < MAX_CONNECTIONS; j++) {
            locations[i].connect[j].locate = NULL;
            locations[i].connect[j].length = 0;
        }
    }
    // Write map
    for(i = 0; i < LOCATION_NUMBERS; i++) {
        _id = locations_input[i].id;
        // Invalid index
        if(_id >= LOCATION_NUMBERS) return false;

        for(j = 0; j < MAX_CONNECTIONS; j++) {
            _locate = locations_input[i].connect[j].locate;
            _length = locations_input[i].connect[j].length;
            
            // Input finished
            if(_locate == 0) break;
            // Invalid index
            if(_locate >= LOCATION_NUMBERS) return false;
            
            locations[_id].connect[j].locate = &(locations[_locate]);
            locations[_id].connect[j].length = _length;
        }
    }

    return true;
}

// Start motor PWM
void MotorPWMStart(MOTOR motor) {
    for(int i = 0; i < 3; i++) {
        HAL_TIM_PWM_Start_IT(motor.htim, motor.Channel[i]);
    }
}

// Motor detect & control
void MotorSpin(MOTOR motor) {
    // Hall sensor pins read
    int state = (
        (HAL_GPIO_ReadPin(motor.D_GPIOx[0], motor.D_GPIO_Pin_x[0]) << 2) |
        (HAL_GPIO_ReadPin(motor.D_GPIOx[1], motor.D_GPIO_Pin_x[1]) << 1) |
        (HAL_GPIO_ReadPin(motor.D_GPIOx[2], motor.D_GPIO_Pin_x[2]) << 0)
    );
    // Renew state
    if(motor.speed >= 0) {
        // Clockwise
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
            __HAL_TIM_SET_COMPARE(motor.htim, motor.Channel[i], motor.speed);
        } else {
            __HAL_TIM_SET_COMPARE(motor.htim, motor.Channel[i], 0);
        }
        // L set/clear
        HAL_GPIO_WritePin(motor.L_GPIOx[i], motor.L_GPIO_Pin_x[i], gpio_pin_state_map[state][i*2+1]? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}

//Motor speed set
void MotorSpeedSet(MOTOR* motor, short speed) {
    if(speed >= -100 && speed <= 100) motor->speed = Motor_TIM_AutoReload * speed / 100;
}

//Calculate ADC average value
int ADCAvgCal(int *buffer, int bufferLen) {
    int total = 0;
    for(int i = 0; i < bufferLen; i++) {
        total += buffer[i]; 
    }
    return total;
}

// Stop vehicle
void VehicleStop(void) {
    MotorSpeedSet(&motor2, 0);
    MotorSpeedSet(&motor3, 0);
}

// Vehicle move straight forward/backward
void VehicleMoveStraight(short speed) {
    MotorSpeedSet(&motor2, speed);
    MotorSpeedSet(&motor3, speed);
}

// Vehicle turnning
void VehicleMoveTurn(direction _direction, short speed) {
    if(_direction > 0) {
        MotorSpeedSet(&motor2, speed);
        MotorSpeedSet(&motor3, -speed);
    } else {
        MotorSpeedSet(&motor2, -speed);
        MotorSpeedSet(&motor3, speed);
    }
}

void StateRun(int adc_value) {
    if((adc_value > ABOVE_ROAD_MIN) && (adc_value > ABOVE_ROAD_MAX)) {
        VehicleMoveStraight(50);
    }
    
}
