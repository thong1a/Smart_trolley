#ifndef	_UART_H
#define	_UART_H
#include "main.h"
#include "command_cli.h"
#define	MAX_DATA_UART		20
static uint8_t buffer_uart[MAX_DATA_UART];
static uint8_t buffer_len=0;
static UART_HandleTypeDef *uart_cli;
static uint8_t uart_flag=0;
void uart_init(UART_HandleTypeDef *UART);
void recieve_data(uint8_t data);
void handle_data(void);
#endif