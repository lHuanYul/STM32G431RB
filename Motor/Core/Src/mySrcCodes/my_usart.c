#include "myIncCodes/my_usart.h"

uint8_t uart_buffer_t[] = "Running\r\n";
uint8_t uart_buffer_r[10] = {0};
uint8_t buffer[20] = {0};

UART_BUFFER transfer_buffer[TRANSFER_BUFFER_SIZE];

// UART buffer init
void UARTBufferInit(void) {
    int i, j;
    for (i = 0; i < TRANSFER_BUFFER_SIZE; i++) {
        transfer_buffer[i].function = 0;
        for (j = 0; j < 8; j++) {
            transfer_buffer[i].data[j] = 0;
        }
        transfer_buffer[i].end = 0x0A; // "\n"
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        memset(uart_buffer_r, 0, sizeof(uart_buffer_r));
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        HAL_UART_Transmit_DMA(&huart1, uart_buffer_r, sizeof(uart_buffer_r));
        HAL_UART_Receive_DMA(&huart1, uart_buffer_r, sizeof(uart_buffer_r));
    }
}
