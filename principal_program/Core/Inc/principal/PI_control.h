#ifndef PI_CONTROL_H
#define PI_CONTROL_H



/* +Includes ------------------------------------------------------------------*/
#include "common.h"
#include "vehicle.h"
#include "motor.h"

/* -Includes ------------------------------------------------------------------*/



/* +subprogram -----------------------------------------------------------------*/
void mycode_EXTI9_5_IRQHandler(void);
void mycode_EXTI3_IRQHandler(void);
void r_exit_rpm_count(void);
void l_exit_rpm_count(void);
void TIM1_UP_TIM16_IRQHandler_use(void);
int time_rpm(int temp_pem_Value, int rpm_count, double *integral, float *present_speed);
int PI_Controller(int temp_pem_Value, double measurement, double *integral);
void PI_timer_setup(void);

/* -subprogram -----------------------------------------------------------------*/



#endif
