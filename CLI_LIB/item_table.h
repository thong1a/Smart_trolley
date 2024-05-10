#ifndef	_ITEM_TABLE_H
#define	_ITEM_TABLE_H
#include "main.h"
#include "tagid.h"
#define		MAX_QUANTITY	50
uint8_t quantity_item_of_table=5;
Good_Information item_table[MAX_QUANTITY]=
{
		{"MILK",50,0x00307F7B},
		{"SUGAR",20,0x00BE9FA9},
		{"PORFUME",200,0x00DFB50A},
		{"BAG",220,0x005A2175},
		{"DRINK",5,0x00A6A229}
};
#endif