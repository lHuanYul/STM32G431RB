#include "myIncCodes/adc.h"

#define ADC0_BUFFER_LEN 8
#define ADC1_BUFFER_LEN 8
uint16_t ADC0_buffer[ADC0_BUFFER_LEN] = {0};
uint16_t ADC1_buffer[ADC1_BUFFER_LEN] = {0};

//Calculate ADC average value
int ADCAvgCal(int *buffer, int bufferLen) {
    int total = 0;
    for(int i = 0; i < bufferLen; i++) {
        total += buffer[i]; 
    }
    return total;
}
