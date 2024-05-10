#include "command_cli.h"

void Command_Init(UART_HandleTypeDef *UART)
{
		uart_command=UART;
}
void uart_print(const char* str,...)
{
		char string_transmit[100];
	va_list args;
	va_start(args,str);
	vsprintf(string_transmit,str,args);
	va_end(args);
	HAL_UART_Transmit(uart_command,(uint8_t *)string_transmit,strlen(string_transmit),100);
}
uint8_t get_command(char buffer[][20],char *command)
{
		char *p;
		uint8_t argnum=0;
	//Tach chuoi con lan dau tien
		p=strtok(command," ");
		strcpy((char *)buffer[argnum++],p);
		while(p!=NULL)
		{
				p=strtok(NULL," ");
				if(p!=NULL)
					strcpy((char*)buffer[argnum++],p);
		}
		return argnum;
}
const cli_command_entry* find_command_entry(char *cmd)
{
		const cli_command_entry *command_entry=table;
		while(command_entry->command_name!=NULL)
		{
				if(strcmp(cmd,command_entry->command_name)==0)
				{
						return command_entry;
				}
				command_entry++;
		}
		return NULL;
}
void command_excutive(char buffer[][20],uint8_t argnum)
{
		const cli_command_entry *command;
		command=find_command_entry(buffer[0]);
		if(command!=NULL)
		{
				command->command_infor->function(buffer,argnum);
		}
		else
			uart_print("This function doesn't exist");
}