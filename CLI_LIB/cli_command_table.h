#ifndef _CLI_TABLE_H
#define	_CLI_TABLE_H
#include "cli_type.h"
#include "cli_implement_command.h"
cli_command_infor command_infor_getrevenue=
{
		get_revenue,
		"get revenue of this trolley"
};
cli_command_infor	command_infor_additem=
{
		add_new_item,
		"add new your items into system"
};
const cli_command_entry table[]=
{
	{"getrevenue",&command_infor_getrevenue},
	{"additem",&command_infor_additem},
	{NULL,NULL}
};
#endif