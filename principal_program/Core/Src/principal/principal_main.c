#include "principal/principal_main.h"



/* +Main ------------------------------------------------------------*/
void MCmain(void) {
    motor_setup(motor_right, motor_left);
    hall_detection_adc_setup();
    PI_timer_setup();
    uartInit();

    updateMotorStep(&motor_right);
    updateMotorStep(&motor_left);
    while (1) {
        commutateMotor(motor_right);
        commutateMotor(motor_left);
        adc_renew();
        /*if() {
            Straight_mode();
        }
        else if() {
            Rotate_in_place();
        }*/

    }
}

/* -Main ------------------------------------------------------------*/
