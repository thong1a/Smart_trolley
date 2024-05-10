#ifndef	_CLI_TYPE_H
#define	_CLI_TYPE_H
#include "main.h"
#include "cli_implement_command.h"
typedef void(*command_function)(char argv[][20],uint8_t argnum);
typedef struct
{
		command_function function;
		char							*help;
}cli_command_infor;
typedef struct
{
		char						*command_name;
		cli_command_infor *command_infor;
}cli_command_entry;
#endif