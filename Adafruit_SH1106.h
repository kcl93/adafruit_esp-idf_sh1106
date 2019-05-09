/*********************************************************************
This is a library for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

These displays use SPI to communicate, 4 or 5 pins are required to  
interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

/*********************************************************************
I change the adafruit SSD1306 to SH1106

SH1106 driver similar to SSD1306 so, just change the display() method.

However, SH1106 driver don't provide several functions such as scroll commands.


*********************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "Adafruit_GFX.h"

extern "C" {
	#include "driver/gpio.h"
	#include "driver/spi_master.h"
}


typedef volatile uint8_t PortReg;
typedef uint8_t PortMask;



#define BLACK 0
#define WHITE 1
#define INVERSE 2


/*=========================================================================
    SH1106 Displays
    -----------------------------------------------------------------------
    The driver is used in multiple displays (128x64, 128x32, etc.).
    Select the appropriate display below to create an appropriately
    sized framebuffer, etc.

    SH1106_128_64  128x64 pixel display

    SH1106_128_32  128x32 pixel display

    SH1106_96_16

    -----------------------------------------------------------------------*/
   #define SH1106_128_64
//   #define SH1106_128_32
//   #define SH1106_96_16
/*=========================================================================*/

#if defined SH1106_128_64 && defined SH1106_128_32
  #error "Only one SH1106 display can be specified at once in SH1106.h"
#endif
#if !defined SH1106_128_64 && !defined SH1106_128_32 && !defined SH1106_96_16
  #error "At least one SH1106 display must be specified in SH1106.h"
#endif

#if defined SH1106_128_64
  #define SH1106_LCDWIDTH				128
  #define SH1106_LCDHEIGHT				64
#endif
#if defined SH1106_128_32
  #define SH1106_LCDWIDTH				128
  #define SH1106_LCDHEIGHT				32
#endif
#if defined SH1106_96_16
  #define SH1106_LCDWIDTH				96
  #define SH1106_LCDHEIGHT				16
#endif

#define SH1106_SETCONTRAST				0x81
#define SH1106_DISPLAYALLON_RESUME		0xA4
#define SH1106_DISPLAYALLON				0xA5
#define SH1106_NORMALDISPLAY			0xA6
#define SH1106_INVERTDISPLAY			0xA7
#define SH1106_DISPLAYOFF				0xAE
#define SH1106_DISPLAYON				0xAF

#define SH1106_SETDISPLAYOFFSET			0xD3
#define SH1106_SETCOMPINS				0xDA

#define SH1106_SETVCOMDETECT			0xDB

#define SH1106_SETDISPLAYCLOCKDIV		0xD5
#define SH1106_SETPRECHARGE				0xD9

#define SH1106_SETMULTIPLEX				0xA8

#define SH1106_SETLOWCOLUMN				0x00
#define SH1106_SETHIGHCOLUMN			0x10

#define SH1106_SETSTARTLINE				0x40

#define SH1106_SETPAGEADDRESS			0xB0

#define SH1106_MEMORYMODE				0x20
#define SH1106_COLUMNADDR				0x21
#define SH1106_PAGEADDR					0x22

#define SH1106_COMSCANINC				0xC0
#define SH1106_COMSCANDEC				0xC8

#define SH1106_SEGREMAP					0xA0

#define SH1106_CHARGEPUMP				0x8D

#define SH1106_EXTERNALVCC				0x1
#define SH1106_SWITCHCAPVCC				0x2

// Scrolling #defines
/*
#define SH1106_ACTIVATE_SCROLL 0x2F
#define SH1106_DEACTIVATE_SCROLL 0x2E
#define SH1106_SET_VERTICAL_SCROLL_AREA 0xA3
#define SH1106_RIGHT_HORIZONTAL_SCROLL 0x26
#define SH1106_LEFT_HORIZONTAL_SCROLL 0x27
#define SH1106_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SH1106_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A
*/





class Adafruit_SH1106 : public Adafruit_GFX {
	public:
		Adafruit_SH1106(spi_host_device_t SPI, gpio_num_t DC, gpio_num_t RST);

		void begin(uint8_t switchvcc = SH1106_SWITCHCAPVCC, bool reset=true);
		inline void SH1106_command(uint8_t c)
			{
				SPI_snd(&c, 1, 0);
			}
		inline void SH1106_data(uint8_t c)
			{
				SPI_snd(&c, 1, 1);
			}

		void clearDisplay(void);
		void invertDisplay(uint8_t i);
		void display(void);

		void dim(uint8_t contrast);

		void drawPixel(int16_t x, int16_t y, uint16_t color);

		virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
		virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

	private:
		int8_t _vccstate;
		gpio_num_t _mosi, _sclk, _dc, _rst, _cs;
		spi_host_device_t _spi;
		spi_device_handle_t _spi_device;

		void SPI_snd(uint8_t *data, uint8_t len, uint8_t dc);
		static void spi_pre_transfer_callback(spi_transaction_t *t);

		inline void drawFastVLineInternal(int16_t x, int16_t y, int16_t h, uint16_t color) __attribute__((always_inline));
		inline void drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color) __attribute__((always_inline));
};


