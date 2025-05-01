/*
 * LCD_Driver.c
 *
 *  Created on: Sep 28, 2023
 *      Author: Xavion
 */

#include "LCD_Driver.h"


/**
  * @brief LTDC Initialization Function
  * @param None
  * @retval None
  */

static LTDC_HandleTypeDef hltdc;
static RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
static FONT_t *LCD_Currentfonts;
static uint16_t CurrentTextColor   = 0xFFFF;


/*
 * fb[y*W+x] OR fb[y][x]
 * Alternatively, we can modify the linker script to have an end address of 20013DFB instead of 2002FFFF, so it does not place variables in the same region as the frame buffer. In this case it is safe to just specify the raw address as frame buffer.
 */
//uint32_t frameBuffer[(LCD_PIXEL_WIDTH*LCD_PIXEL_WIDTH)/2] = {0};		//16bpp pixel format. We can size to uint32. this ensures 32 bit alignment


//Someone from STM said it was "often accessed" a 1-dim array, and not a 2d array. However you still access it like a 2dim array,  using fb[y*W+x] instead of fb[y][x].
uint16_t frameBuffer[LCD_PIXEL_WIDTH*LCD_PIXEL_HEIGHT] = {0};			//16bpp pixel format.


void LCD_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable the LTDC clock */
  __HAL_RCC_LTDC_CLK_ENABLE();

  /* Enable GPIO clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /* GPIO Config
   *
    LCD pins
   LCD_TFT R2 <-> PC.10
   LCD_TFT G2 <-> PA.06
   LCD_TFT B2 <-> PD.06
   LCD_TFT R3 <-> PB.00
   LCD_TFT G3 <-> PG.10
   LCD_TFT B3 <-> PG.11
   LCD_TFT R4 <-> PA.11
   LCD_TFT G4 <-> PB.10
   LCD_TFT B4 <-> PG.12
   LCD_TFT R5 <-> PA.12
   LCD_TFT G5 <-> PB.11
   LCD_TFT B5 <-> PA.03
   LCD_TFT R6 <-> PB.01
   LCD_TFT G6 <-> PC.07
   LCD_TFT B6 <-> PB.08
   LCD_TFT R7 <-> PG.06
   LCD_TFT G7 <-> PD.03
   LCD_TFT B7 <-> PB.09
   LCD_TFT HSYNC <-> PC.06
   LCDTFT VSYNC <->  PA.04
   LCD_TFT CLK   <-> PG.07
   LCD_TFT DE   <->  PF.10
  */

  /* GPIOA configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6 |
                           GPIO_PIN_11 | GPIO_PIN_12;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
  GPIO_InitStructure.Alternate= GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

 /* GPIOB configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_8 | \
                           GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

 /* GPIOC configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_10;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

 /* GPIOD configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_3 | GPIO_PIN_6;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

 /* GPIOF configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_10;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);

 /* GPIOG configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_6 | GPIO_PIN_7 | \
                           GPIO_PIN_11;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

  /* GPIOB configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  GPIO_InitStructure.Alternate= GPIO_AF9_LTDC;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* GPIOG configuration */
  GPIO_InitStructure.Pin = GPIO_PIN_10 | GPIO_PIN_12;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);
}

void LTCD_Layer_Init(uint8_t LayerIndex)
{
	LTDC_LayerCfgTypeDef  pLayerCfg;

	pLayerCfg.WindowX0 = 0;	//Configures the Window HORZ START Position.
	pLayerCfg.WindowX1 = LCD_PIXEL_WIDTH;	//Configures the Window HORZ Stop Position.
	pLayerCfg.WindowY0 = 0;	//Configures the Window vertical START Position.
	pLayerCfg.WindowY1 = LCD_PIXEL_HEIGHT;	//Configures the Window vertical Stop Position.
	pLayerCfg.PixelFormat = LCD_PIXEL_FORMAT_1;  //INCORRECT PIXEL FORMAT WILL GIVE WEIRD RESULTS!! IT MAY STILL WORK FOR 1/2 THE DISPLAY!!! //This is our buffers pixel format. 2 bytes for each pixel
	pLayerCfg.Alpha = 255;
	pLayerCfg.Alpha0 = 0;
	pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
	pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
	if (LayerIndex == 0){
		pLayerCfg.FBStartAdress = (uintptr_t)frameBuffer;
	}
	pLayerCfg.ImageWidth = LCD_PIXEL_WIDTH;
	pLayerCfg.ImageHeight = LCD_PIXEL_HEIGHT;
	pLayerCfg.Backcolor.Blue = 0;
	pLayerCfg.Backcolor.Green = 0;
	pLayerCfg.Backcolor.Red = 0;
	if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, LayerIndex) != HAL_OK)
	{
		LCD_Error_Handler();
	}
}

void clearScreen(void)
{
  LCD_Clear(0,LCD_COLOR_WHITE);
}

void LTCD__Init(void)
{
	hltdc.Instance = LTDC;
	/* Configure horizontal synchronization width */
	hltdc.Init.HorizontalSync = ILI9341_HSYNC;
	/* Configure vertical synchronization height */
	hltdc.Init.VerticalSync = ILI9341_VSYNC;
	/* Configure accumulated horizontal back porch */
	hltdc.Init.AccumulatedHBP = ILI9341_HBP;
	/* Configure accumulated vertical back porch */
	hltdc.Init.AccumulatedVBP = ILI9341_VBP;
	/* Configure accumulated active width */
	hltdc.Init.AccumulatedActiveW = 269;
	/* Configure accumulated active height */
	hltdc.Init.AccumulatedActiveH = 323;
	/* Configure total width */
	hltdc.Init.TotalWidth = 279;
	/* Configure total height */
	hltdc.Init.TotalHeigh = 327;
	/* Configure R,G,B component values for LCD background color */
	hltdc.Init.Backcolor.Red = 0;
	hltdc.Init.Backcolor.Blue = 0;
	hltdc.Init.Backcolor.Green = 0;

	/* LCD clock configuration */
	/* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
	/* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 192 Mhz */
	/* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 192/4 = 48 Mhz */
	/* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_8 = 48/4 = 6Mhz */

	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
	PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
	PeriphClkInitStruct.PLLSAI.PLLSAIR = 4;
	PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_8;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
	/* Polarity */
	hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
	hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
	hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
	hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;

	LCD_GPIO_Init();

	if (HAL_LTDC_Init(&hltdc) != HAL_OK)
	 {
	   LCD_Error_Handler();
	 }

	ili9341_Init();
}

/* START Draw functions */


/*
 * This is really the only function needed.
 * All drawing consists of is manipulating the array.
 * Adding input sanitation should probably be done.
 */
void LCD_Draw_Pixel(uint16_t x, uint16_t y, uint16_t color)
{
	frameBuffer[y*LCD_PIXEL_WIDTH+x] = color;  //You cannot do x*y to set the pixel.
}

/*
 * These functions are simple examples. Most computer graphics like OpenGl and stm's graphics library use a state machine. Where you first call some function like SetColor(color), SetPosition(x,y), then DrawSqure(size)
 * Instead all of these are explicit where color, size, and position are passed in.
 * There is tons of ways to handle drawing. I dont think it matters too much.
 */
void LCD_Draw_Circle_Fill(uint16_t Xpos, uint16_t Ypos, uint16_t radius, uint16_t color)
{
    for(int16_t y=-radius; y<=radius; y++)
    {
        for(int16_t x=-radius; x<=radius; x++)
        {
            if(x*x+y*y <= radius*radius)
            {
            	LCD_Draw_Pixel(x+Xpos, y+Ypos, color);
            }
        }
    }
}

void LCD_Draw_Vertical_Line(uint16_t x, uint16_t y, uint16_t len, uint16_t color)
{
  for (uint16_t i = 0; i < len; i++)
  {
	  LCD_Draw_Pixel(x, i+y, color);
  }
}

void LCD_Clear(uint8_t LayerIndex, uint16_t Color)
{
	if (LayerIndex == 0){
		for (uint32_t i = 0; i < LCD_PIXEL_WIDTH * LCD_PIXEL_HEIGHT; i++){
			frameBuffer[i] = Color;
		}
	}
  // TODO: Add more Layers if needed
}

//This was taken and adapted from stm32's mcu code
void LCD_SetTextColor(uint16_t Color)
{
  CurrentTextColor = Color;
}

//This was taken and adapted from stm32's mcu code
void LCD_SetFont(FONT_t *fonts)
{
  LCD_Currentfonts = fonts;
}

//This was taken and adapted from stm32's mcu code
void LCD_Draw_Char(uint16_t Xpos, uint16_t Ypos, const uint16_t *c)
{
  uint32_t index = 0, counter = 0;
  for(index = 0; index < LCD_Currentfonts->Height; index++)
  {
    for(counter = 0; counter < LCD_Currentfonts->Width; counter++)
    {
      if((((c[index] & ((0x80 << ((LCD_Currentfonts->Width / 12 ) * 8 ) ) >> counter)) == 0x00) && (LCD_Currentfonts->Width <= 12)) || (((c[index] & (0x1 << counter)) == 0x00)&&(LCD_Currentfonts->Width > 12 )))
      {
      }
      else
      {
    	  LCD_Draw_Pixel(counter + Xpos,index + Ypos,CurrentTextColor);
      }
    }
  }
}

void LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii)
{
  Ascii -= 32;
  LCD_Draw_Char(Xpos, Ypos, &LCD_Currentfonts->table[Ascii * LCD_Currentfonts->Height]);
}

void LCD_DrawMenuScreen(void) {
    LCD_Clear(0, COLOR_BG);

    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_SetFont(&Font16x24);
    LCD_DisplayChar(50, 40, '1');
    LCD_DisplayChar(50, 60, '-');
    LCD_DisplayChar(50, 80, 'P');
    LCD_DisplayChar(50, 100, 'L');
    LCD_DisplayChar(50, 120, 'A');
    LCD_DisplayChar(50, 140, 'Y');
    LCD_DisplayChar(50, 160, 'E');
    LCD_DisplayChar(50, 180, 'R');

    LCD_DisplayChar(170, 40, '2');
    LCD_DisplayChar(170, 60, '-');
    LCD_DisplayChar(170, 80, 'P');
    LCD_DisplayChar(170, 100, 'L');
    LCD_DisplayChar(170, 120, 'A');
    LCD_DisplayChar(170, 140, 'Y');
    LCD_DisplayChar(170, 160, 'E');
    LCD_DisplayChar(170, 180, 'R');

    LCD_Draw_Vertical_Line(120, 40, 240, LCD_COLOR_BLACK);
}

void LCD_DrawGameBoard(uint8_t board[6][7]) {
    LCD_Clear(0, COLOR_BG);

    for (uint8_t row = 0; row < BOARD_ROWS; row++) {
        for (uint8_t col = 0; col < BOARD_COLS; col++) {
            uint16_t x = BOARD_X_START + col * (2 * CELL_RADIUS + CELL_SPACING);
            uint16_t y = BOARD_Y_START + row * (2 * CELL_RADIUS + CELL_SPACING);
            uint16_t color = COLOR_BG;

            if (board[row][col] == 1)
            	color = COLOR_PLAYER1;
            else if (board[row][col] == 2)
            	color = COLOR_PLAYER2;

            LCD_Draw_Circle_Fill(x, y, CELL_RADIUS, COLOR_GRID);
            LCD_Draw_Circle_Fill(x, y, CELL_RADIUS - 3, color);
        }
    }
}

void LCD_ClearFloatingBand(void) {
    for (uint16_t y = FLOATING_COIN_Y0; y < FLOATING_COIN_H; y++) {
      for (uint16_t x = 0; x < LCD_PIXEL_WIDTH; x++) {
        LCD_Draw_Pixel(x, y, COLOR_BG);
      }
    }
}

void draw_elapsed_time(uint8_t t) {
    char buf[8];
    sprintf(buf, "%lu", t);

    uint16_t text_w = strlen(buf) * 8;
    uint16_t text_h = 12;
    uint16_t x0 = (LCD_PIXEL_WIDTH - text_w) / 2;
    uint16_t y0 = LCD_PIXEL_HEIGHT - text_h - 2;

    for (uint16_t i = 0; i < strlen(buf); i++) {
        LCD_Draw_Circle_Fill(x0 + i * 12, y0 + 4, 10, COLOR_BG);
    }

    LCD_SetFont(&Font12x12);
    LCD_SetTextColor(LCD_COLOR_BLACK);
    for (uint16_t i = 0; buf[i]; i++) {
        LCD_DisplayChar(x0 + i * 8, y0, buf[i]);
    }
}

void LCD_DrawGameOverScreen(uint8_t winner, uint8_t redWins, uint8_t yellowWins, uint8_t elapsedTime) {
    LCD_Clear(0, COLOR_BG);
    LCD_SetFont(&Font16x24);

    if (winner == 1) {
        LCD_SetTextColor(LCD_COLOR_RED);
        LCD_DisplayChar(40, 100, 'R');
        LCD_DisplayChar(60, 100, 'E');
        LCD_DisplayChar(80, 100, 'D');
        LCD_DisplayChar(100, 100, ' ');
        LCD_DisplayChar(120, 100, 'W');
        LCD_DisplayChar(140, 100, 'I');
        LCD_DisplayChar(160, 100, 'N');
        LCD_DisplayChar(180, 100, 'S');
    }
    else if (winner == 2) {
        LCD_SetTextColor(LCD_COLOR_YELLOW);
        LCD_DisplayChar(20, 100, 'Y');
        LCD_DisplayChar(40, 100, 'E');
        LCD_DisplayChar(60, 100, 'L');
        LCD_DisplayChar(80, 100, 'L');
        LCD_DisplayChar(100, 100, 'O');
        LCD_DisplayChar(120, 100, 'W');
        LCD_DisplayChar(140, 100, ' ');
        LCD_DisplayChar(160, 100, 'W');
        LCD_DisplayChar(180, 100, 'I');
        LCD_DisplayChar(200, 100, 'N');
        LCD_DisplayChar(220, 100, 'S');
    }
    else {
        LCD_SetTextColor(LCD_COLOR_BLACK);
        LCD_DisplayChar(100, 100, 'T');
        LCD_DisplayChar(120, 100, 'I');
        LCD_DisplayChar(140, 100, 'E');
    }

    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_DisplayChar(60, 180, 'R');
    LCD_DisplayChar(80, 180, 'E');
    LCD_DisplayChar(100, 180, 'S');
    LCD_DisplayChar(120, 180, 'T');
    LCD_DisplayChar(140, 180, 'A');
    LCD_DisplayChar(160, 180, 'R');
    LCD_DisplayChar(180, 180, 'T');

    LCD_SetFont(&Font12x12);
    LCD_SetTextColor(LCD_COLOR_BLACK);

    char buffer[16];
    uint16_t y = 210;
    uint16_t x;

    // Red Wins
    sprintf(buffer, "Red Wins: %d", redWins);
    x = 60;
    for (uint8_t i = 0; buffer[i] != '\0'; i++) {
        LCD_DisplayChar(x + i * 8, y, buffer[i]);
    }

    // Yellow Wins
    sprintf(buffer, "Yellow Wins: %d", yellowWins);
    y += 14;
    for (uint8_t i = 0; buffer[i] != '\0'; i++) {
        LCD_DisplayChar(x + i * 8, y, buffer[i]);
    }

    // Elapsed Time
    sprintf(buffer, "Time: %ds", elapsedTime);
    y += 14;
    for (uint8_t i = 0; buffer[i] != '\0'; i++) {
        LCD_DisplayChar(x + i * 8, y, buffer[i]);
    }
}





void LCD_DrawFloatingCoin(uint8_t column, uint16_t color) {
	uint16_t x = BOARD_X_START + column * (2 * CELL_RADIUS + CELL_SPACING);
	uint16_t y = BOARD_Y_START - 30;

    LCD_Draw_Circle_Fill(x, y, CELL_RADIUS, color);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void LCD_Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

// Touch Functionality   //

#if COMPILE_TOUCH_FUNCTIONS == 1

void InitializeLCDTouch(void)
{
  if(STMPE811_Init() != STMPE811_State_Ok)
  {
	 for(;;); // Hang code due to error in initialzation
  }
}

STMPE811_State_t returnTouchStateAndLocation(STMPE811_TouchData * touchStruct)
{
	return STMPE811_ReadTouch(touchStruct);
}

void DetermineTouchPosition(STMPE811_TouchData * touchStruct)
{
	STMPE811_DetermineTouchPosition(touchStruct);
}

uint8_t ReadRegisterFromTouchModule(uint8_t RegToRead)
{
	return STMPE811_Read(RegToRead);
}

void WriteDataToTouchModule(uint8_t RegToWrite, uint8_t writeData)
{
	STMPE811_Write(RegToWrite, writeData);
}

#endif // COMPILE_TOUCH_FUNCTIONS
