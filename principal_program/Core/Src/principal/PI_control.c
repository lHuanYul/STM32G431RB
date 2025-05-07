#include "principal/PI_control.h"



/* +variable ------------------------------------------------------------------*/
// PI 控制參數
#define Kp  1.5                                          // 比例增益
#define Ki  0.8                                          // 積分增益
#define dt 0.5f                                          // 取樣時間
#define PI_feedbacck 0.1                                 // PI feedbacck 增益值

#define setpoint 20                                      // 速度恆定目標
/* -variable ------------------------------------------------------------------*/


void PI_timer_setup(void){
    HAL_TIM_Base_Start_IT(&htim1);
}



/* hall exit count --------------------------------------------------*/
void mycode_EXTI3_IRQHandler(void) {
    updateMotorStep(&motor_right);
    r_exit_rpm_count();
}

void mycode_EXTI9_5_IRQHandler(void) {
    updateMotorStep(&motor_left);
    l_exit_rpm_count();
}

void r_exit_rpm_count() {
    motor_right.rpm_count++;
}

void l_exit_rpm_count() {
    motor_left.rpm_count++;
}

/* hall exit count -------------------------------------------------*/



/* +PI speed control ------------------------------------------------*/
void TIM1_UP_TIM16_IRQHandler_use(void) {                //計時到，進行temp_pwm更新
    if(motor_right.adc_value >= trans_value) {
        motor_right.pwmValue_temp = time_rpm(motor_right.pwmValue_temp, motor_right.rpm_count, &motor_right.integral_record, &motor_right.present_speed);
    }
    if(motor_left.adc_value >= trans_value) {
        motor_left.pwmValue_temp = time_rpm(motor_left.pwmValue_temp, motor_left.rpm_count, &motor_left.integral_record, &motor_left.present_speed);
    }

    motor_left.rpm_count = 0;                           //將rpm計速器歸零
    motor_right.rpm_count = 0;
    updateMotorStep(&motor_left);
    updateMotorStep(&motor_right);

}

int time_rpm(int pwmValue_temp, int rpm_count, double *integral_record, float *present_speed) {
    float real_speed = rpm_count/6;
    real_speed/=dt;
    *present_speed = real_speed;                        // 紀錄當前速度

    int pwmValue_renew = PI_Controller(pwmValue_temp, real_speed, integral_record);
    return pwmValue_renew;
}

int PI_Controller(int temp_pwm_Value, double measurement, double *integral) {
    double error = setpoint - measurement;              // 計算誤差
    *integral += error * dt;                            // 積分項累積
    int output_pwm_Value = Kp * error + Ki * *integral; // 計算 P I 控制輸出
    temp_pwm_Value += output_pwm_Value * PI_feedbacck;

    return temp_pwm_Value;
}

/* PI speed control ------------------------------------------------*/
