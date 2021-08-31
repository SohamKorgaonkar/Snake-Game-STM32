/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306.h"
#include "stdlib.h"
#include "stdbool.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
int draw_grid[16][8];
int snake_speed[2]={1,0};
int size_of_snake=2;
int snake_pos[2][20];
int old_poshx;
int old_poshy;
int pos_head[2];
bool sw_up;
bool sw_down;
bool sw_right;
bool sw_left;
int score;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
void init_grid(void);
void update_grid(void);
void draw_snake(int x,int y);
void draw_food(int x,int y);
void clear_block(int x,int y);
void move_snake(void);
void re_init(void);
void spawn_food(void);
int rand_no(int m);
void check_collision(void);
void wait_check(void);
void check_food(void);
void game_over_screen(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  /* USER CODE BEGIN 2 */
	init_grid();
	draw_grid[0][0]=2;
	draw_grid[1][0]=2;
	snake_pos[0][0]=0;
	snake_pos[1][0]=0;
	snake_pos[0][1]=1;
	snake_pos[1][1]=0;
	size_of_snake=2;
	snake_speed[0]=1;
	snake_speed[1]=0;
	sw_up=true;
	sw_down=true;
	sw_right=true;
	sw_left=true;
	score=0;
	spawn_food();
	//int test=0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//update_grid();
		update_grid();
		check_food();
		move_snake();
		check_collision();
		HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_12);
		//HAL_Delay(250);
		wait_check();
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

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
  hi2c1.Init.ClockSpeed = 400000;
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
	ssd1306_Init();
  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA1 PA2 PA3 PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

void draw_snake(int x,int y){
	int x0=x*8;
	int y0=y*8;
	int x1=x0+8;
	int y1=y0+8;
	for(int i=x0;i<x1;i++){
		for(int j=y0;j<y1;j++){
			ssd1306_DrawPixel(i,j,0x01);
		}
	}
}
void draw_food(int x,int y){
	int x0=x*8;
	int y0=y*8;
	int x1=x0+8;
	int y1=y0+8;
	for(int i=x0;i<x1;i++){
		for(int j=y0;j<y1;j++){
			ssd1306_DrawPixel(i,j,0x01);
		}
	}
}

void clear_block(int x,int y){
	int x0=x*8;
	int y0=y*8;
	int x1=x0+8;
	int y1=y0+8;
	for(int i=x0;i<x1;i++){
		for(int j=y0;j<y1;j++){
			ssd1306_DrawPixel(i,j,0x00);
		}
	}
}

void update_grid(){
	SSD1306_ClearScreen();
	for(int i=0;i<16;i++){
		for(int j=0;j<8;j++){
			if(draw_grid[i][j]==2){
				draw_snake(i,j);
			}
			else if(draw_grid[i][j]==1){
				draw_food(i,j);
			}
			else{
				clear_block(i,j);
			}
		}
	}
	ssd1306_UpdateScreen();
}

//Fill grid with 0
void init_grid(void){
	for(int i=0; i<16;i++){
		for(int j=0;j<8;j++){
			draw_grid[i][j]=0;
		}
	}
}

void move_snake(){
		for(int i=size_of_snake-1;i>=0;i--){
			draw_grid[snake_pos[0][i]][snake_pos[1][i]]=0;
		}
		old_poshx=snake_pos[0][size_of_snake-1];
		old_poshy=snake_pos[1][size_of_snake-1];
		snake_pos[0][size_of_snake-1]=snake_pos[0][size_of_snake-1]+snake_speed[0];
		snake_pos[1][size_of_snake-1]=snake_pos[1][size_of_snake-1]+snake_speed[1];
		pos_head[0]=snake_pos[0][size_of_snake-1];
		pos_head[1]=snake_pos[1][size_of_snake-1];
		for(int i=size_of_snake-2;i>=0;i--){
			int nx = snake_pos[0][i];
			int ny = snake_pos[1][i];
			snake_pos[0][i]=old_poshx;
			snake_pos[1][i]=old_poshy;
			old_poshx=nx;
			old_poshy=ny;
		}
		for(int i=size_of_snake-1;i>=0;i--){
			draw_grid[snake_pos[0][i]][snake_pos[1][i]]=2;
		}
}

void re_init(void){
	init_grid();
	draw_grid[0][0]=2;
	draw_grid[1][0]=2;
	snake_pos[0][0]=0;
	snake_pos[1][0]=0;
	snake_pos[0][1]=1;
	snake_pos[1][1]=0;
	size_of_snake=2;
	snake_speed[0]=1;
	snake_speed[1]=0;
	sw_up=true;
	sw_down=true;
	sw_right=true;
	sw_left=true;
	score=0;
	spawn_food();
}

void spawn_food(void){
	int xf=rand_no(15);
	int yf=rand_no(7);
	while(draw_grid[xf][yf]==2){
		xf=rand_no(15);
		yf=rand_no(7);
	}
	draw_grid[xf][yf]=1;
}

int rand_no(int m){
	int num = (rand()%(m+1));
	return num;
}

void check_collision(void){
	if(pos_head[0]>=16 || pos_head[0]<0){
		game_over_screen();
		re_init();
	}
	if(pos_head[1]>=8 || pos_head[1]<0){
		game_over_screen();
		re_init();
	}
	if(draw_grid[pos_head[0]+snake_speed[0]][pos_head[1]+snake_speed[1]]==2){
		game_over_screen();
		re_init();
	}
}

void wait_check(void){
	for(int i=0;i<45;i++){
		sw_down=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1);
		if(sw_down==false && snake_speed[1]==0){
			snake_speed[0]=0;
			snake_speed[1]=1;
			break;
		}
		sw_up=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4);
		if(sw_up==false && snake_speed[1]==0){
			snake_speed[0]=0;
			snake_speed[1]=-1;
			break;
		}
		sw_right=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2);
		if(sw_right==false && snake_speed[0]==0){
			snake_speed[0]=1;
			snake_speed[1]=0;
			break;
		}
		sw_left=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_3);
		if(sw_left==false && snake_speed[0]==0){
			snake_speed[0]=-1;
			snake_speed[1]=0;
			break;
		}
		HAL_Delay(5);
	}
}
void check_food(void){
	if(draw_grid[pos_head[0]+snake_speed[0]][pos_head[1]+snake_speed[1]]==1){
		score++;
		size_of_snake++;
		snake_pos[0][size_of_snake-1]=pos_head[0]+snake_speed[0];
		snake_pos[1][size_of_snake-1]=pos_head[1]+snake_speed[1];
		spawn_food();
	}
}

void game_over_screen(void){
	SSD1306_ClearScreen();
	ssd1306_SetCursor(16,20);
	ssd1306_WriteString("Game Over", Font_11x18, 0x01);
	ssd1306_SetCursor(16,40);
	ssd1306_WriteString("Score: ", Font_11x18, 0x01);
	ssd1306_SetCursor(80,40);
	char sc1=score+'0';
	ssd1306_WriteChar(sc1, Font_11x18,0x01);
	ssd1306_UpdateScreen();
	HAL_Delay(1000);
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
