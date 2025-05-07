# include "principal/vehicle.h"



#define MAX_DUTY  100                                          // PWM 最大占空比
#define MIN_DUTY  0                                            // PWM 最小占空比

const uint32_t trans_value = 16*16*16 + 16*16 + 16 + 1;        // 判斷是否轉灣大小



/* AGV一般循跡功能 --------------------------------------------------*/
void Straight_mode(void) {
    // 限制 PWM 占空比範圍
    if (motor_right.pwmValue_temp > MAX_DUTY) motor_right.pwmValue_temp = MAX_DUTY;
    if (motor_right.pwmValue_temp < MIN_DUTY) motor_right.pwmValue_temp = MIN_DUTY;
    if (motor_left.pwmValue_temp > MAX_DUTY) motor_left.pwmValue_temp = MAX_DUTY;
    if (motor_left.pwmValue_temp < MIN_DUTY) motor_left.pwmValue_temp = MIN_DUTY;



    if(motor_right.adc_value>=trans_value) {
        motor_right.pwmValue = motor_right.pwmValue_temp;
        motor_left.pwmValue = MIN_DUTY;
    }
    else if(motor_left.adc_value>=trans_value) {
        motor_left.pwmValue = motor_left.pwmValue_temp;
        motor_right.pwmValue = MIN_DUTY;
    }
    else {
        motor_right.pwmValue = motor_right.pwmValue_temp;
        motor_left.pwmValue = motor_left.pwmValue_temp;
    }
}

/* AGV一般循跡功能 --------------------------------------------------*/




/* AGV原地旋轉功能 --------------------------------------------------*/
void Rotate_in_place(void) {

}


/* AGV原地旋轉功能 --------------------------------------------------*/
