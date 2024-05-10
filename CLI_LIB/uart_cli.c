#include "uart_cli.h"
void uart_init(UART_HandleTypeDef *UART)
{
			uart_cli=UART;
			Command_Init(UART);
}
void recieve_data(uint8_t data)
{
		if(data=='\n') //Nhan xong chuoi
		{
				buffer_uart[buffer_len++]='\0'; //Tranh truong hop con luu gia tri truoc
				uart_flag=1;
		}
		else
		{
				buffer_uart[buffer_len]=data;
				buffer_len++;
		}
}
void handle_data(void)
{
		char command[4][20];
		uint8_t argnum=0;
		if(uart_flag==1)
		{	
			//HAL_UART_Transmit(uart_cli,buffer_uart,buffer_len,100);
			argnum=get_command(command,(char*)buffer_uart);
			command_excutive(command,argnum);
//			for(uint8_t i=0;i<number_of_parameter;i++)
//			{
//				HAL_UART_Transmit(uart_cli,(uint8_t *)command[i],sizeof(command[i]),1000);
//				HAL_UART_Transmit(uart_cli,(uint8_t *)"\n",sizeof("\n"),100);
//			}	
			buffer_len=0;
			uart_flag=0;
		}	
		
}