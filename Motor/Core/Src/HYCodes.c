/** 
  ******************************************************************************
  * @file           : HYCodes.c
  * @brief          : HY Codes file
  ******************************************************************************
  */
// Includes (Copy from main.c) -----------------------------------------------
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
// Local Var -----------------------------------------------------------------
#define ABOVE_ROAD_MIN 10
#define ABOVE_ROAD_MAX ABOVE_ROAD_MIN+9
#define ABOVE_CROSS_MIN 20
#define ABOVE_CROSS_MAX ABOVE_CROSS_MIN+9

#define LOCATION_NUMBERS 16
LOCATION locations[LOCATION_NUMBERS] = {0};
uint8_t paths[LOCATION_NUMBERS];
uint8_t origine_pos;

#define ADC0_BUFFER_LEN 8
#define ADC1_BUFFER_LEN 8
uint16_t ADC0_buffer[ADC0_BUFFER_LEN] = {0};
uint16_t ADC1_buffer[ADC1_BUFFER_LEN] = {0};

#define TIMER6_COUNT 10 // 50ms * x
uint16_t timer6_count = 0;
float rps_calculate = 20 / TIMER6_COUNT;
float derivative_t, integral;
int motor2_rpm_setpoint, motor3_rpm_setpoint;

#define UART_TIME_OUT 100
uint8_t uart_buffer_t[] = "Running\r\n";
uint8_t uart_buffer_r[10] = {0};
uint8_t buffer[20] = {0};

#define TRANSFER_BUFFER_SIZE 8
UART_BUFFER transfer_buffer[TRANSFER_BUFFER_SIZE];

uint16_t running = 0;
uint16_t test01 = 0;
uint16_t test02 = 0;
uint16_t test03 = 0;

// LED(A5) Button(C13)
MOTOR motor2 = {
    // Speed inited value
    0,
    // AH BH CH (PA0 PA1 PB10)
    &htim2,
    {TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3},
    // AL BL CL
    {GPIOB, GPIOB, GPIOB},
    {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5},
    // H1 H2 H3 
    {GPIOB, GPIOB, GPIOB},
    {GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15},
    0, 0, 0
};

MOTOR motor3 = {
    // Speed inited value
    0,
    // AH BH CH (PA6 PA4 PC8)
    &htim3,
    {TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3},
    // AL BL CL
    {GPIOB, GPIOB, GPIOB},
    {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5},
    // H1 H2 H3
    {GPIOB, GPIOB, GPIOB},
    {GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15},
    0, 0, 0
};

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

// Interrupt -----------------------------------------------------------------
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

// Function ------------------------------------------------------------------
// Map create
bool MapCreate(void) {
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
    // Input map
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
            
            locations[_id].connect[j].locate = _locate;
            locations[_id].connect[j].length = _length;
        }
    }
    return true;
}

// UART buffer init
bool UARTBufferInit(void) {
    int i, j;
    for (i = 0; i < TRANSFER_BUFFER_SIZE; i++) {
        transfer_buffer[i].function = 0;
        for (j = 0; j < 8; j++) {
            transfer_buffer[i].data[j] = 0;
        }
        transfer_buffer[i].end = 0x0A; // "\n"
    }
}

// Find path
bool MapFindPath(uint32_t target) {
    int i, j;
    bool visited[LOCATION_NUMBERS] = {false};
    uint32_t dist[LOCATION_NUMBERS];
    uint32_t prev[LOCATION_NUMBERS] = {0};
    VehicleStop();
    for(i = 0; i < LOCATION_NUMBERS; i++) {
        dist[i] = UINT32INF;
    }
    dist[origine_pos] = 0;
    visited[origine_pos] = true;
    // Dijkstra 演算法
    /*for (i = 0; i < LOCATION_NUMBERS; i++) {
        // 找出目前未訪問、且距離最小的節點
        uint32_t min_dist = UINT32INF, u = UINT32INF;
        for (j = 0; j < LOCATION_NUMBERS; j++) {
            if (!visited[j] && dist[j] < min_dist) {
                min_dist = dist[j];
                u = j;
            }
        }

        // 如果所有可到達的點都處理完，或找到終點 target
        if (u == UINT32INF || u == target) break;

        // 標記 u 為已訪問
        visited[u] = true;

        // 更新 u 的所有鄰居 v
        for (j = 0; j < MAX_CONNECTIONS; j++) {
            if (locations[u].connect[j].locate == NULL) break;
            uint32_t v = locations[u].connect[j].locate->id;
            uint32_t weight = locations[u].connect[j].length;

            if (!visited[v] && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
            }
        }
    }*/
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

//Motor speed set
void MotorSpeedSet(MOTOR* motor, short speed) {
    if(speed >= -100 && speed <= 100) motor->arr_trigger_value = MOTOR_TIM_AUTORELOAD * speed / 100;
}

void MotorSpeedInitTest(void) {
    MotorSpeedSet(&motor2, 100);
    MotorSpeedSet(&motor3, 100);
    HAL_Delay(1000);
    MotorSpeedSet(&motor2, 0);
    MotorSpeedSet(&motor3, 0);
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

/*void PI_Controller(double measurement) {
    double error = setpoint - measurement;  // 計算誤差
    integral += error * dt;  // 積分項累積
    double output = Kp * error + Ki * integral;  // 計算 P I 控制輸出

    pwmValue += output * 0.1;

    // 限制 PWM 占空比範圍
    if (output > MAX_DUTY) output = MAX_DUTY;
    if (output < MIN_DUTY) output = MIN_DUTY;
}*/

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        test01++;
        memset(uart_buffer_r, 0, sizeof(uart_buffer_r));
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        test02++;
        HAL_UART_Transmit_DMA(&huart1, uart_buffer_r, sizeof(uart_buffer_r));
        HAL_UART_Receive_DMA(&huart1, uart_buffer_r, sizeof(uart_buffer_r));
    }
}
