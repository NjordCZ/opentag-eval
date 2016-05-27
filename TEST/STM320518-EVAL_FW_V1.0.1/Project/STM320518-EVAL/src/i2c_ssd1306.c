/**	
 *  SSD1306 I2C  STM32F0 Njord
 */


#include "i2c_ssd1306.h"


/* Absolute value */
#define ABS(x)   ((x) > 0 ? (x) : -(x))


/* SSD1306 data buffer */
static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

/* Private SSD1306 structure */
typedef struct {
	uint16_t CurrentX;
	uint16_t CurrentY;
	uint8_t Inverted;
	uint8_t Initialized;
} SSD1306_t;

/* Private variable */
static SSD1306_t SSD1306;





static void SSD1306_WRITECOMMAND(uint8_t command)
{
  
__disable_irq();   
 while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY) == SET); // wait if the bus is busy?
  
 I2C_TransferHandling(I2C2, SSD1306_I2C_ADDR, 2, I2C_AutoEnd_Mode, I2C_CR2_START); // Configure slave address, nbytes, reload, end mode and start or stop generation
   
 while(I2C_GetFlagStatus(I2C2, I2C_ISR_TXIS) == RESET){};  // Wait until TXIS flag is set, waiting ACK
   
 I2C_SendData(I2C2, 0x00);// Send Register address
  
 while(I2C_GetFlagStatus(I2C2, I2C_FLAG_TXE) == RESET); // 
  
  I2C_SendData(I2C2, command);// Send data          
  
  while(I2C_GetFlagStatus(I2C2, I2C_ISR_STOPF) == RESET){};// Wait until STOPF flag is set
   
  I2C_ClearFlag(I2C2, I2C_ICR_STOPCF); // Clear STOPF flag
 __enable_irq(); 
  
    
}



static void SSD1306_WRITEMULTI(uint8_t* data, uint16_t count) 
{
 __disable_irq(); 
 //Wait until I2C isn't busy
while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY) == SET);
  
I2C_TransferHandling(I2C2, SSD1306_I2C_ADDR, count + 1, I2C_AutoEnd_Mode, I2C_CR2_START); // Configure slave address, nbytes, reload, end mode and start or stop generation
   
 while(I2C_GetFlagStatus(I2C2, I2C_ISR_TXIS) == RESET){};  // Wait until TXIS flag is set, waiting ACK
   
 I2C_SendData(I2C2, 0x40);// Send Register address

 while(I2C_GetFlagStatus(I2C2, I2C_FLAG_TXE) == RESET);
 
 
 for (uint8_t i = 0; i < count; i++) 
  {
    
   
    I2C_SendData(I2C2, data[i]); // Send data  
    while(I2C_GetFlagStatus(I2C2, I2C_FLAG_TXE) == RESET);
    
    
  }
 

 I2C_GenerateSTOP(I2C2, ENABLE);
 
 while(I2C_GetFlagStatus(I2C2, I2C_ISR_STOPF) == RESET){};// Wait until STOPF flag is set
   
 I2C_ClearFlag(I2C2, I2C_ICR_STOPCF); // Clear STOPF flag
  
   __enable_irq(); 

 	
}




uint8_t I2C_SSD1306_Init(void) {

    I2C_InitTypeDef  I2C_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
  
  
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

    /* Configure the I2C clock source. The clock is derived from the HSI */
  //  RCC_I2CCLKConfig(RCC_I2C1CLK_HSI);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_1); // SCL
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_1); // SDA


    //Configure pins: SCL and SDA ------------------
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
    I2C_InitStructure.I2C_DigitalFilter = 0x00;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00; // MPU6050 7-bit adress = 0x68, 8-bit adress = 0xD0;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Disable; //I2C_Ack_Enable;
    //I2C_InitStructure.I2C_Timing = 0xA0120227;
    I2C_InitStructure.I2C_Timing = 0x20310A0D;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

    I2C_Init(I2C2, &I2C_InitStructure);

    I2C_Cmd(I2C2, ENABLE);
    
    
    
	
	/* A little delay */
	Delay(100);
	
	/* Init LCD */
	SSD1306_WRITECOMMAND(0xAE); //display off
	SSD1306_WRITECOMMAND(0x20); //Set Memory Addressing Mode   
	SSD1306_WRITECOMMAND(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	SSD1306_WRITECOMMAND(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
	SSD1306_WRITECOMMAND(0xC8); //Set COM Output Scan Direction
	SSD1306_WRITECOMMAND(0x00); //---set low column address
	SSD1306_WRITECOMMAND(0x10); //---set high column address
	SSD1306_WRITECOMMAND(0x40); //--set start line address
	SSD1306_WRITECOMMAND(0x81); //--set contrast control register
	SSD1306_WRITECOMMAND(0xFF);
	SSD1306_WRITECOMMAND(0xA1); //--set segment re-map 0 to 127
	SSD1306_WRITECOMMAND(0xA6); //--set normal display
	SSD1306_WRITECOMMAND(0xA8); //--set multiplex ratio(1 to 64)
	SSD1306_WRITECOMMAND(0x3F); //
	SSD1306_WRITECOMMAND(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	SSD1306_WRITECOMMAND(0xD3); //-set display offset
	SSD1306_WRITECOMMAND(0x00); //-not offset
	SSD1306_WRITECOMMAND(0xD5); //--set display clock divide ratio/oscillator frequency
	SSD1306_WRITECOMMAND(0xF0); //--set divide ratio
	SSD1306_WRITECOMMAND(0xD9); //--set pre-charge period
	SSD1306_WRITECOMMAND(0x22); //
	SSD1306_WRITECOMMAND(0xDA); //--set com pins hardware configuration
	SSD1306_WRITECOMMAND(0x12);
	SSD1306_WRITECOMMAND(0xDB); //--set vcomh
	SSD1306_WRITECOMMAND(0x20); //0x20,0.77xVcc
	SSD1306_WRITECOMMAND(0x8D); //--set DC-DC enable
	SSD1306_WRITECOMMAND(0x14); //
	SSD1306_WRITECOMMAND(0xAF); //--turn on SSD1306 panel
	
	/* Clear screen */
	I2C_SSD1306_Fill(SSD1306_COLOR_BLACK);
	
	/* Update screen */
	I2C_SSD1306_UpdateScreen();
	
	/* Set default values */
	SSD1306.CurrentX = 0;
	SSD1306.CurrentY = 0;
	
	/* Initialized OK */
	SSD1306.Initialized = 1;
	
	/* Return OK */
	return 1;
}

void I2C_SSD1306_UpdateScreen(void) {
	uint8_t m;
	
	for (m = 0; m < 8; m++) {
		SSD1306_WRITECOMMAND(0xB0 + m);
		SSD1306_WRITECOMMAND(0x00);
		SSD1306_WRITECOMMAND(0x10);
		
		/* Write multi data */
		
                SSD1306_WRITEMULTI(&SSD1306_Buffer[SSD1306_WIDTH * m], SSD1306_WIDTH);
	}
}

void I2C_SSD1306_ToggleInvert(void) {
	uint16_t i;
	
	/* Toggle invert */
	SSD1306.Inverted = !SSD1306.Inverted;
	
	/* Do memory toggle */
	for (i = 0; i < sizeof(SSD1306_Buffer); i++) {
		SSD1306_Buffer[i] = ~SSD1306_Buffer[i];
	}
}

void I2C_SSD1306_Fill(SSD1306_COLOR_t color) {
	/* Set memory */
	memset(SSD1306_Buffer, (color == SSD1306_COLOR_BLACK) ? 0x00 : 0xFF, sizeof(SSD1306_Buffer));
}

void I2C_SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color) {
	if (
		x >= SSD1306_WIDTH ||
		y >= SSD1306_HEIGHT
	) {
		/* Error */
		return;
	}
	
	/* Check if pixels are inverted */
	if (SSD1306.Inverted) {
		color = (SSD1306_COLOR_t)!color;
	}
	
	/* Set color */
	if (color == SSD1306_COLOR_WHITE) {
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
	} else {
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
	}
}

void I2C_SSD1306_GotoXY(uint16_t x, uint16_t y) {
	/* Set write pointers */
	SSD1306.CurrentX = x;
	SSD1306.CurrentY = y;
}

char I2C_SSD1306_Putc(char ch, TM_FontDef_t* Font, SSD1306_COLOR_t color) {
	uint32_t i, b, j;
	
	/* Check available space in LCD */
	if (
		SSD1306_WIDTH <= (SSD1306.CurrentX + Font->FontWidth) ||
		SSD1306_HEIGHT <= (SSD1306.CurrentY + Font->FontHeight)
	) {
		/* Error */
		return 0;
	}
	
	/* Go through font */
	for (i = 0; i < Font->FontHeight; i++) {
		b = Font->data[(ch - 32) * Font->FontHeight + i];
		for (j = 0; j < Font->FontWidth; j++) {
			if ((b << j) & 0x8000) {
				I2C_SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t) color);
			} else {
				I2C_SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t)!color);
			}
		}
	}
	
	/* Increase pointer */
	SSD1306.CurrentX += Font->FontWidth;
	
	/* Return character written */
	return ch;
}



void I2C_SSD1306_Putd(uint16_t b, uint8_t digit_num, TM_FontDef_t* Font, SSD1306_COLOR_t color)
{
 
    unsigned char datas[5],i,j;
 
    datas[0] = b/10000;
    b = b - datas[0]*10000;
    datas[1] = b/1000;
    b = b - datas[1]*1000;
    datas[2] = b/100;
    b = b - datas[2]*100;
    datas[3] = b/10;
    b = b - datas[3]*10;
    datas[4] = b;
 
    datas[0]+=48;
    datas[1]+=48;
    datas[2]+=48;
    datas[3]+=48;
    datas[4]+=48;
 
    /* find the first non zero digit to display - display at least one digit */
    for (i = 0; i < 5; i++) {
        if (datas[i] != 48) {
            break;
        }
    }
 
    if (digit_num > 0 && digit_num < 6){    // valid range
        i = 5-digit_num;
    }
 
    for (j = i; j < 5; j++)
    {
      I2C_SSD1306_Putc(datas[j], Font, color);
      //LCD5110_write_char(datas[j]);
    }
}





char I2C_SSD1306_Puts(char* str, TM_FontDef_t* Font, SSD1306_COLOR_t color) {
	/* Write characters */
	while (*str) {
		/* Write character by character */
		if (I2C_SSD1306_Putc(*str, Font, color) != *str) {
			/* Return error */
			return *str;
		}
		
		/* Increase string pointer */
		str++;
	}
	
	/* Everything OK, zero should be returned */
	return *str;
}
 

void I2C_SSD1306_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SSD1306_COLOR_t c) {
	int16_t dx, dy, sx, sy, err, e2, i, tmp; 
	
	/* Check for overflow */
	if (x0 >= SSD1306_WIDTH) {
		x0 = SSD1306_WIDTH - 1;
	}
	if (x1 >= SSD1306_WIDTH) {
		x1 = SSD1306_WIDTH - 1;
	}
	if (y0 >= SSD1306_HEIGHT) {
		y0 = SSD1306_HEIGHT - 1;
	}
	if (y1 >= SSD1306_HEIGHT) {
		y1 = SSD1306_HEIGHT - 1;
	}
	
	dx = (x0 < x1) ? (x1 - x0) : (x0 - x1); 
	dy = (y0 < y1) ? (y1 - y0) : (y0 - y1); 
	sx = (x0 < x1) ? 1 : -1; 
	sy = (y0 < y1) ? 1 : -1; 
	err = ((dx > dy) ? dx : -dy) / 2; 

	if (dx == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}
		
		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}
		
		/* Vertical line */
		for (i = y0; i <= y1; i++) {
			I2C_SSD1306_DrawPixel(x0, i, c);
		}
		
		/* Return from function */
		return;
	}
	
	if (dy == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}
		
		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}
		
		/* Horizontal line */
		for (i = x0; i <= x1; i++) {
			I2C_SSD1306_DrawPixel(i, y0, c);
		}
		
		/* Return from function */
		return;
	}
	
	while (1) {
		I2C_SSD1306_DrawPixel(x0, y0, c); 
		if (x0 == x1 && y0 == y1) {
			break;
		}
		e2 = err; 
		if (e2 > -dx) {
			err -= dy;
			x0 += sx;
		} 
		if (e2 < dy) {
			err += dx;
			y0 += sy;
		} 
	}
}

void I2C_SSD1306_DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t c) {
	/* Check input parameters */
	if (
		x >= SSD1306_WIDTH ||
		y >= SSD1306_HEIGHT
	) {
		/* Return error */
		return;
	}
	
	/* Check width and height */
	if ((x + w) >= SSD1306_WIDTH) {
		w = SSD1306_WIDTH - x;
	}
	if ((y + h) >= SSD1306_HEIGHT) {
		h = SSD1306_HEIGHT - y;
	}
	
	/* Draw 4 lines */
	I2C_SSD1306_DrawLine(x, y, x + w, y, c);         /* Top line */
	I2C_SSD1306_DrawLine(x, y + h, x + w, y + h, c); /* Bottom line */
	I2C_SSD1306_DrawLine(x, y, x, y + h, c);         /* Left line */
	I2C_SSD1306_DrawLine(x + w, y, x + w, y + h, c); /* Right line */
}

void I2C_SSD1306_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t c) {
	uint8_t i;
	
	/* Check input parameters */
	if (
		x >= SSD1306_WIDTH ||
		y >= SSD1306_HEIGHT
	) {
		/* Return error */
		return;
	}
	
	/* Check width and height */
	if ((x + w) >= SSD1306_WIDTH) {
		w = SSD1306_WIDTH - x;
	}
	if ((y + h) >= SSD1306_HEIGHT) {
		h = SSD1306_HEIGHT - y;
	}
	
	/* Draw lines */
	for (i = 0; i <= h; i++) {
		/* Draw lines */
		I2C_SSD1306_DrawLine(x, y + i, x + w, y + i, c);
	}
}

void I2C_SSD1306_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1306_COLOR_t color) {
	/* Draw lines */
	I2C_SSD1306_DrawLine(x1, y1, x2, y2, color);
	I2C_SSD1306_DrawLine(x2, y2, x3, y3, color);
	I2C_SSD1306_DrawLine(x3, y3, x1, y1, color);
}


void I2C_SSD1306_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1306_COLOR_t color) {
	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
	yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
	curpixel = 0;
	
	deltax = ABS(x2 - x1);
	deltay = ABS(y2 - y1);
	x = x1;
	y = y1;

	if (x2 >= x1) {
		xinc1 = 1;
		xinc2 = 1;
	} else {
		xinc1 = -1;
		xinc2 = -1;
	}

	if (y2 >= y1) {
		yinc1 = 1;
		yinc2 = 1;
	} else {
		yinc1 = -1;
		yinc2 = -1;
	}

	if (deltax >= deltay){
		xinc1 = 0;
		yinc2 = 0;
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;
	} else {
		xinc2 = 0;
		yinc1 = 0;
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;
	}

	for (curpixel = 0; curpixel <= numpixels; curpixel++) {
		I2C_SSD1306_DrawLine(x, y, x3, y3, color);

		num += numadd;
		if (num >= den) {
			num -= den;
			x += xinc1;
			y += yinc1;
		}
		x += xinc2;
		y += yinc2;
	}
}

void I2C_SSD1306_DrawCircle(int16_t x0, int16_t y0, int16_t r, SSD1306_COLOR_t c) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    I2C_SSD1306_DrawPixel(x0, y0 + r, c);
    I2C_SSD1306_DrawPixel(x0, y0 - r, c);
    I2C_SSD1306_DrawPixel(x0 + r, y0, c);
    I2C_SSD1306_DrawPixel(x0 - r, y0, c);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        I2C_SSD1306_DrawPixel(x0 + x, y0 + y, c);
        I2C_SSD1306_DrawPixel(x0 - x, y0 + y, c);
        I2C_SSD1306_DrawPixel(x0 + x, y0 - y, c);
        I2C_SSD1306_DrawPixel(x0 - x, y0 - y, c);

        I2C_SSD1306_DrawPixel(x0 + y, y0 + x, c);
        I2C_SSD1306_DrawPixel(x0 - y, y0 + x, c);
        I2C_SSD1306_DrawPixel(x0 + y, y0 - x, c);
        I2C_SSD1306_DrawPixel(x0 - y, y0 - x, c);
    }
}

void I2C_SSD1306_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, SSD1306_COLOR_t c) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    I2C_SSD1306_DrawPixel(x0, y0 + r, c);
    I2C_SSD1306_DrawPixel(x0, y0 - r, c);
    I2C_SSD1306_DrawPixel(x0 + r, y0, c);
    I2C_SSD1306_DrawPixel(x0 - r, y0, c);
    I2C_SSD1306_DrawLine(x0 - r, y0, x0 + r, y0, c);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        I2C_SSD1306_DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, c);
        I2C_SSD1306_DrawLine(x0 + x, y0 - y, x0 - x, y0 - y, c);

        I2C_SSD1306_DrawLine(x0 + y, y0 + x, x0 - y, y0 + x, c);
        I2C_SSD1306_DrawLine(x0 + y, y0 - x, x0 - y, y0 - x, c);
    }
}
 
void SSD1306_ON(void) {
	SSD1306_WRITECOMMAND(0x8D);  
	SSD1306_WRITECOMMAND(0x14);  
	SSD1306_WRITECOMMAND(0xAF);  
}
void SSD1306_OFF(void) {
	SSD1306_WRITECOMMAND(0x8D);  
	SSD1306_WRITECOMMAND(0x10);
	SSD1306_WRITECOMMAND(0xAE);  
}
