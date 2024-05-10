#include "tag_display.h"
extern uint8_t quantity_item_of_table;
extern Good_Information item_table[]; 
Discount_Typedef Tag_Discount_Defination(uint8_t	*Discount_ID)
{
		Item_Typedef	item;
		Tag_Convert_Item(Discount_ID,&item);
		switch((uint32_t)item.item_id)
		{
			case DISCOUNT_10:
				return	DISCOUNT_TENPERCENT;
			case DISCOUNT_MILK:
				return	DISCOUNT_MILK_PERCENT;
			default:
				return NO_DISCOUNT;
		}
}
void Tag_Display_Init(I2C_HandleTypeDef* hi2c_CLCD)
{
		CLCD_I2C_Init(&LCD,hi2c_CLCD,0x4e,20,4);
}
void Tag_Menu_Display(void)
{
		CLCD_I2C_SetCursor(&LCD, 8, 0);
		CLCD_I2C_WriteString(&LCD,"MENU");
		CLCD_I2C_SetCursor(&LCD, 2, 1);
		CLCD_I2C_WriteString(&LCD,"THANH TOAN");
		CLCD_I2C_SetCursor(&LCD, 2, 2);
		CLCD_I2C_WriteString(&LCD,"HOA DON");
		CLCD_I2C_SetCursor(&LCD, 2, 3);
		CLCD_I2C_WriteString(&LCD,"MA GIAM GIA");
		HAL_Delay(5);
}

void Tag_Checkout_Display(uint8_t bill[][2])
{
		uint32_t total=Get_BillTotal(bill);
		char total_print[10];
		CLCD_I2C_SetCursor(&LCD, 4, 0);
		CLCD_I2C_WriteString(&LCD,"TOTAL");
		CLCD_I2C_SetCursor(&LCD, 4, 1);
		sprintf(total_print,"%u",total);
		CLCD_I2C_WriteString(&LCD,total_print);
		CLCD_I2C_SetCursor(&LCD, 2, 2);
		CLCD_I2C_WriteString(&LCD,"XAC NHAN");
		CLCD_I2C_SetCursor(&LCD, 2, 3);
		CLCD_I2C_WriteString(&LCD,"HUY BO");
		HAL_Delay(50);
}
void Tag_Discount_Display(uint8_t *ID)
{
		if(ID==NULL)
		{
				CLCD_I2C_SetCursor(&LCD, 4, 0);
				CLCD_I2C_WriteString(&LCD,"MA GIAM GIA");
				CLCD_I2C_SetCursor(&LCD, 7, 1);
				CLCD_I2C_WriteString(&LCD,"QUET QR");
				CLCD_I2C_SetCursor(&LCD, 2, 2);
				CLCD_I2C_WriteString(&LCD,"XAC NHAN");
				CLCD_I2C_SetCursor(&LCD, 2, 3);
				CLCD_I2C_WriteString(&LCD,"HUY BO");
				HAL_Delay(50);
		}
		else
		{
				Discount_Typedef discount;
				CLCD_I2C_SetCursor(&LCD, 4, 0);
				CLCD_I2C_WriteString(&LCD,"MA GIAM GIA");
				CLCD_I2C_SetCursor(&LCD, 0, 1);
				discount=Tag_Discount_Defination(ID);
				switch(discount)
				{
					case DISCOUNT_TENPERCENT:
						CLCD_I2C_WriteString(&LCD,"GIAM 10%              ");
						break;
					case DISCOUNT_MILK_PERCENT:
						CLCD_I2C_WriteString(&LCD,"GIAM 10% MILK         ");
						break;
					case NO_DISCOUNT:
						CLCD_I2C_WriteString(&LCD,"KHONG HOP LE          ");
						break;
				}
				CLCD_I2C_SetCursor(&LCD, 2, 2);
				CLCD_I2C_WriteString(&LCD,"XAC NHAN");
				CLCD_I2C_SetCursor(&LCD, 2, 3);
				CLCD_I2C_WriteString(&LCD,"HUY BO");
				HAL_Delay(50);
		}
}
void Tag_Display_Item(uint8_t	bill[][2], char *name,uint16_t price,uint8_t index,uint8_t page)
{
		char quantity[10];
		char price_string[7];
		uint16_t total;
		CLCD_I2C_SetCursor(&LCD, 0, index);
		CLCD_I2C_WriteString(&LCD,name);
		CLCD_I2C_SetCursor(&LCD,9, index);
		sprintf(quantity,"%u",bill[page+index][1]);
		CLCD_I2C_WriteString(&LCD,quantity);
		total=bill[page+index][1]*price;
		CLCD_I2C_SetCursor(&LCD,14, index);
		sprintf(price_string,"%u",total);
		CLCD_I2C_WriteString(&LCD,price_string);
}
void Tag_Bill_Display(uint8_t	bill[][2],uint8_t page)
{
		static uint8_t last_page=0;
		if(last_page!=page)
		{
				Tag_Clear();
				last_page=page;
		}
		CLCD_I2C_SetCursor(&LCD, 0, 0);
		CLCD_I2C_WriteString(&LCD,"ITEM");
		CLCD_I2C_SetCursor(&LCD,8, 0);
		CLCD_I2C_WriteString(&LCD,"NUMBER");
		CLCD_I2C_SetCursor(&LCD,15, 0);
		CLCD_I2C_WriteString(&LCD,"TOTAL");
		for(uint8_t	i=1;i<=3;i++)
		{
				Tag_Display_Item(bill,item_table[bill[page+i][0]].name,item_table[bill[page+i][0]].price,i,page);
		}
}
void Tag_Cusor_Display(uint8_t row)
{
		for(uint8_t i=1;i<4;i++)
		{
				CLCD_I2C_SetCursor(&LCD, 0, i);
				CLCD_I2C_WriteString(&LCD," ");
				CLCD_I2C_SetCursor(&LCD, 1, i);
				CLCD_I2C_WriteString(&LCD," ");
		}
		CLCD_I2C_SetCursor(&LCD, 0, row);
		CLCD_I2C_WriteChar(&LCD,(char)Cusor_Display);
}
void Tag_Clear(void)
{
		CLCD_I2C_Clear(&LCD);
		HAL_Delay(2);
}
uint32_t Get_ItemTotal(uint16_t quantity,uint16_t price)
{
		return (uint32_t)quantity*price;
}
uint32_t Get_BillTotal(uint8_t	bill[][2])
{
		int i=1;
		uint32_t total=0;
		while(bill[i][0]!=0)
		{
					total+=Get_ItemTotal(bill[i][1],item_table[bill[0][i]].price);
					i++;
		}
		return total;
}
