#ifndef	_COMMAND_H
#define	_COMMAND_H
#include "main.h"
#include "cli_type.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
extern const cli_command_entry table[];
const cli_command_entry* find_command_entry(char *cmd);
static UART_HandleTypeDef *uart_command;
void Command_Init(UART_HandleTypeDef *UART);
uint8_t get_command(char buffer[][20],char *command);
void command_excutive(char buffer[][20],uint8_t argnum);
void uart_print(const char* str,...);
#endif