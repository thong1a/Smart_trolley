#ifndef	TAGID_H
#define	TAGID_H
#include "stm32f1xx_hal.h"
#include "RC522.h"
#include "BUTTON.h"
#define	MILK			0x00307F7B
#define	SUGAR			0x00BE9FA9
#define	PORFUME		0x00DFB50A
#define	BAG				0x005A2175
#define	DRINK			0x00A6A229
static MFRC522_Name	rfid;
static SPI_HandleTypeDef *SPI;
static uint16_t CS_PIN;
static GPIO_TypeDef*	CS_PORT;
static	uint8_t item_quantity=0;
static uint8_t	flag=0;
extern BUTTON_Name UP,DOWN;
extern int Button_Handle(void);

typedef enum
{
	TAG_OK=0,
	TAG_ERROR,
	TAG_HANDLING
}Tag_status;
typedef enum
{
	FOUNDED,
	NO_FOUNDED
	
}Search_Typedef;
typedef union
{
	uint8_t	id[4];
	uint32_t item_id;
}Item_Typedef;
typedef	enum
{
		NEWCARD,
		OLDCARD,
		NO_FOUND
}Card_Typedef;
typedef struct
{
		char name[50];
		uint8_t	price;
		uint32_t	id;
}Good_Information;
void Tag_Init(SPI_HandleTypeDef *hspi,GPIO_TypeDef* PORT,uint16_t CS);
Tag_status	Tag_Read_Item(uint8_t *id);
void Tag_Convert_Item(uint8_t *id,Item_Typedef *item);
Search_Typedef Tag_Search_Item(Item_Typedef *list,Item_Typedef item,uint8_t *index);
Tag_status	Tag_Add_Item(Item_Typedef *list,Item_Typedef item);
Tag_status	Tag_Remove_Item(Item_Typedef *list,Item_Typedef item,uint8_t index);
uint8_t Tag_get_quantity(void);
Tag_status	Tag_Handle_Item(Item_Typedef *list,uint8_t *id);
Tag_status	Tag_Convert_Bill(Item_Typedef	*list,uint8_t bill[][2]);


#endif
