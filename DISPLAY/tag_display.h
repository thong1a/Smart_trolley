#ifndef	TAG_DISPLAY_H
#define	TAG_DISPLAY_H
#include "CLCD_I2C.h"
#include "tagid.h"
#include <stdio.h>
static CLCD_I2C_Name LCD;
#define	Cusor_Display	0x7E
#define DISCOUNT_10	0xF9AD9FB0
#define	DISCOUNT_MILK	0x1234
#define	MILK_PRICE	200
#define	SUGAR_PRICE	100
#define	PORFUME_PRICE	50
#define	BAG_PRICE	250
#define	DRINK_PRICE	20
typedef enum
{
	DISCOUNT_TENPERCENT =2,
	DISCOUNT_MILK_PERCENT=3,
	NO_DISCOUNT
}Discount_Typedef;
void Tag_Display_Init(I2C_HandleTypeDef* hi2c_CLCD);
uint32_t Get_ItemTotal(uint16_t quantity,uint16_t price);
uint32_t Get_BillTotal(uint8_t	bill[][2]);
Discount_Typedef Tag_Discount_Defination(uint8_t	*Discount_ID);
void Tag_Menu_Display(void);
void Tag_Checkout_Display(uint8_t bill[][2]);
void Tag_Discount_Display(uint8_t *ID);
void Tag_Display_Item(uint8_t	bill[][2], char *name,uint16_t price,uint8_t index,uint8_t page);
void Tag_Bill_Display(uint8_t	bill[][2],uint8_t page);
void Tag_Cusor_Display(uint8_t row);
void Tag_Clear(void);
#endif
