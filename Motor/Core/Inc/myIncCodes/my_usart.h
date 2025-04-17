#ifndef MY_USART_H
#define MY_USART_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <usart.h>
#include <stm32g431xx.h>
#include <stm32g4xx_hal_uart.h>

#define USART1_BAUDRATE 115200
#define USART2_BAUDRATE 115200

#define UART_TIME_OUT 100
#define TRANSFER_BUFFER_SIZE 8

typedef struct {
    uint8_t function;
    uint8_t data[8];
    uint8_t end;
} UART_BUFFER;

extern uint8_t uart_buffer_t[];
extern uint8_t uart_buffer_r[10];
extern uint8_t buffer[];
extern UART_BUFFER transfer_buffer[];

void UARTBufferInit(void);

#endif
