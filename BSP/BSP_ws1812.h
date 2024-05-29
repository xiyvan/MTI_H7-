
#ifndef _BSP_WS2812_H_
#define _BSP_WS2812_H_

#include "spi.h"

#define WS2812_SPI_UNIT     hspi6
extern SPI_HandleTypeDef WS2812_SPI_UNIT;




void WS2812_Ctrl(uint8_t r, uint8_t g, uint8_t b);



#endif
