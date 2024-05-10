#include "cli_implement_command.h"
extern Good_Information item_table[];
extern uint8_t quantity_item_of_table;
void get_revenue(char argv[][20],uint8_t argnum)
{
		//This function will be developed in the near future
}
void add_new_item(char argv[][20],uint8_t argnum)
{
	if(argnum<4)
	{
			uart_print("Don't have enough argnum for this function\n");
	}
	else
	{
			if(item_quantity<MAX_ITEM)
			{
				strcpy(item_table[quantity_item_of_table].name,argv[1]);
				item_table[quantity_item_of_table].price=atoi(argv[2]);
				item_table[quantity_item_of_table].id=atoi(argv[3]);
			}
			else
			{
					uart_print("Storage of Item table is full\n");
			}
	}
}