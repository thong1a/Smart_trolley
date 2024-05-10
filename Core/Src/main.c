/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tagid.h"
#include "tag_display.h"
#include "uart_cli.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define Read_Menu_Button()			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_3)
#define	Read_Select_Button()		HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
typedef enum
{
	STATE_MENU=1,
	STATE_BILL=2,
	STATE_CHECKOUT=3,
	STATE_DISCOUNT=4
}Menu_Typedef;
typedef enum
{
	AGREE,
	REJECT,
	ERROR_STATE
}Terminal_State;
typedef enum
{
	UP_BUTTON,
	DOWN_BUTTON,
	MENU_BUTTON,
	SELECT_BUTTON,
	NO_BUTTON
}Button_Typedef;
typedef enum
{
		EVENT=0,
		NO_EVENT
}Event_Typedef;
Menu_Typedef	Menu_state=STATE_MENU,Last_Menu_State=STATE_MENU; 
uint8_t cusor=1;
uint8_t id[5];
Item_Typedef list[100];
uint8_t bill[50][2];
BUTTON_Name UP,DOWN;
uint8_t page=0;
uint8_t test=0;
uint8_t data_rv;
uint8_t button_check=0;
Terminal_State final_state;
char select_choose=0;
int Button_Handle(void);
void Menu_Handle_Mode(void);
void Checkout_Handle_Mode(Terminal_State *state);
void	Bill_Handle_Mode(void);
void Discount_Handle_Mode(void);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
Tag_Init(&hspi2,GPIOB,GPIO_PIN_12);
Tag_Display_Init(&hi2c1);
BUTTON_Init(&UP,GPIOA,GPIO_PIN_0);
BUTTON_Init(&DOWN,GPIOA,GPIO_PIN_1);
uart_init(&huart1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		if(button_check==1)
			Button_Handle();
		else
		{
			handle_data();
			if(Last_Menu_State!=Menu_state)
			{
				Last_Menu_State=Menu_state;
				Tag_Clear();
			}
			switch(Menu_state)
			{
				case STATE_MENU:
					if(button_check==0)
					if(Tag_Handle_Item(list,id)==TAG_OK)
					Tag_Convert_Bill(list,bill);
					if(button_check==0)
					Tag_Menu_Display();
					Menu_Handle_Mode();
					break;
				case	STATE_DISCOUNT:
					if(button_check==0)
					Discount_Handle_Mode();
					break;
				case	STATE_BILL:
					if(button_check==0)
					Bill_Handle_Mode();
					break;
				case	STATE_CHECKOUT:
					if(button_check==0)
					Tag_Checkout_Display(bill);
					if(button_check==0)
					Checkout_Handle_Mode(&final_state);
					break;
			}
		}
		
  }

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 10, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
			if(GPIO_Pin==GPIO_PIN_4)
				button_check=1;
}
int Button_Handle(void)
{
		button_check=0;
		if(BUTTON_Read(&UP)!=NO_CLICK)
		{
				if(Menu_state!=STATE_BILL)
				{
					cusor++;
					if(cusor>3)
					cusor=1;
				}
				else
				{
					page+=3;
				}
		}
		if(BUTTON_Read(&DOWN)!=NO_CLICK)
		{
				if(Menu_state!=STATE_BILL)
				{
					cusor--;
					if(cusor<1)
					cusor=3;
				}
				else
					page-=3;
		}
		if(Read_Menu_Button()==0)
		{
			Menu_state=STATE_MENU;
			page=0;
			cusor=1;
			select_choose=0;
		}
		if(Read_Select_Button()==0)
			select_choose=1;
		if(Menu_state!=STATE_BILL)
		Tag_Cusor_Display(cusor);
		if(Read_Select_Button()==0&&Menu_state==STATE_MENU)
			return 1;
		return 0;
		
}
void Menu_Handle_Mode(void)
{
		Tag_Cusor_Display(cusor);
		if(select_choose==1)
		{
			switch(cusor)
			{
				case 1:
					select_choose=0;
					Menu_state=STATE_CHECKOUT;
					break;
				case 2:
					select_choose=0;
					Menu_state=STATE_BILL;
					break;
				case 3:
					select_choose=0;
					Menu_state=STATE_DISCOUNT;
					break;
			}
			cusor=2;
			page=0;
		}
		if(button_check==0)
		if(Tag_Handle_Item(list,id)==TAG_OK)
				Tag_Convert_Bill(list,bill);
}
void Checkout_Handle_Mode(Terminal_State *state)
{
		Tag_Cusor_Display(cusor);
		if(select_choose==1)
		{
				cusor=1;
				page=0;
				select_choose=0;
				switch(cusor)
				{
					case 1:
						*state=AGREE;
						break;
					case 2:
						*state=REJECT;
						break;
					default:
						*state=ERROR_STATE;
						return;
				}
		}
}
void	Bill_Handle_Mode(void)
{
		if(button_check==0)
		if(Tag_Handle_Item(list,id)==TAG_OK)
			Tag_Convert_Bill(list,bill);
		if(button_check==0)
		Tag_Bill_Display(bill,page);
}
void Discount_Handle_Mode(void)
{
		static uint8_t flag=0;
		if(Tag_Read_Item(id)==TAG_OK||flag==1)
		{
				Tag_Discount_Display(id);
				flag=1;
		}
		else
				Tag_Discount_Display(NULL);
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if(huart->Instance == huart1.Instance){
        recieve_data(data_rv);
			HAL_UART_Receive_IT(&huart1,&data_rv,1);
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
