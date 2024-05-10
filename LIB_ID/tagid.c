#include "tagid.h"
extern uint8_t quantity_item_of_table;
extern Good_Information item_table[]; 
void Tag_Init(SPI_HandleTypeDef *hspi,GPIO_TypeDef* PORT,uint16_t CS)
{
		SPI=hspi;
		CS_PIN=CS;
		CS_PORT=PORT;
		MFRC522_Init(&rfid,SPI,CS_PORT,CS_PIN);
}
void Tag_Convert_Item(uint8_t *id,Item_Typedef *item)
{		
		for(uint8_t i=0;i<3;i++)
		item->id[i]=*(id+i+2);
}
Tag_status	Tag_Read_Item(uint8_t *id)
{
		if(MFRC522_Check(&rfid,id)==MI_OK)
			return	TAG_OK;
		else
		{
			return TAG_ERROR;
		}
}
Tag_status	Tag_Add_Item(Item_Typedef *list,Item_Typedef item)
{
		
		(list+item_quantity)->item_id=item.item_id;
		item_quantity++;
		return	TAG_OK;
}

uint8_t Tag_get_quantity(void)
{
		return	item_quantity;
}
Tag_status	Tag_Remove_Item(Item_Typedef *list,Item_Typedef item,uint8_t index)
{
		(list+index)->item_id=0;
		if(index!=item_quantity-1)
		{
			for(uint8_t i=index;i<item_quantity-1;i++)
			{
					(list+i)->item_id=(list+i+1)->item_id;
			}
			(list+item_quantity-1)->item_id=0;
		}
		item_quantity--;
		return TAG_OK;
}
Tag_status	Tag_Handle_Item(Item_Typedef *list,uint8_t *id)
{
		static uint32_t last_time=0;
		if(HAL_GetTick()-last_time>500)
		{
			last_time=HAL_GetTick();
			if(Tag_Read_Item(id)==TAG_OK)
			{
					Item_Typedef	item;
					uint8_t index;
					Tag_Convert_Item(id,&item);
					if(Tag_Search_Item(list,item,&index)==NO_FOUNDED)
					{
						if(Tag_Add_Item(list,item)==TAG_OK)
						return TAG_OK;
					}
					else
					{	
						if(Tag_Remove_Item(list,item,index)==TAG_OK)
						return TAG_OK;
					}
						return TAG_ERROR;
			}
		}
			return TAG_HANDLING;
}

Search_Typedef Tag_Search_Item(Item_Typedef *list,Item_Typedef item,uint8_t *index)
{
		for(uint8_t	i=0;i<item_quantity;i++)
		{
				if((list+i)->item_id==item.item_id)
				{
					*index=i;	
					return FOUNDED;
				}
		}
		return NO_FOUNDED;
}
Tag_status	Tag_Convert_Bill(Item_Typedef	*list,uint8_t bill[][2])
{
		volatile uint8_t flag_index[50]={0};
		uint8_t index=1;
		for(uint8_t i=0;i<10;i++)
		for(uint8_t j=0;j<2;j++)
		bill[i][j]=0;
		for(uint8_t i=0;i<item_quantity;i++)
		{
				for(uint8_t j=0;j<quantity_item_of_table;j++)
				{
						if(list[i].item_id==item_table[j].id)
						{
								if(flag_index[j]==0)
								{
										bill[index][0]=j;
										bill[index][1]++;
										flag_index[j]=index;
								}
								else
								{
										bill[flag_index[j]][1]++;
								}
						}
				}
		}
		return TAG_OK;
}


