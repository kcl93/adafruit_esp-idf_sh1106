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
All text above, and the splash screen below must be included in any redistribution
*********************************************************************/

/*********************************************************************
I change the adafruit SSD1306 to SH1106

SH1106 driver similar to SSD1306 so, just change the display() method.

However, SH1106 driver don't provide several functions such as scroll commands.


*********************************************************************/


#include "Adafruit_SH1106.h"

#include <string.h>
#include <stdio.h>

extern "C" {
	#include "freertos/FreeRTOS.h"
	#include "freertos/task.h"
	#include "sdkconfig.h"
}


#define INVALID_SPI	SPI_HOST



// the memory buffer for the LCD

static uint8_t buffer[SH1106_LCDHEIGHT * SH1106_LCDWIDTH / 8] = { 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
	0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x80, 0x80, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xF8, 0xE0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80,
	0x80, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0xFF,
	#if (SH1106_LCDHEIGHT * SH1106_LCDWIDTH > 96*16)
		0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00,
		0x80, 0xFF, 0xFF, 0x80, 0x80, 0x00, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x80, 0x80,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x8C, 0x8E, 0x84, 0x00, 0x00, 0x80, 0xF8,
		0xF8, 0xF8, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xE0, 0xE0, 0xC0, 0x80,
		0x00, 0xE0, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xC7, 0x01, 0x01,
		0x01, 0x01, 0x83, 0xFF, 0xFF, 0x00, 0x00, 0x7C, 0xFE, 0xC7, 0x01, 0x01, 0x01, 0x01, 0x83, 0xFF,
		0xFF, 0xFF, 0x00, 0x38, 0xFE, 0xC7, 0x83, 0x01, 0x01, 0x01, 0x83, 0xC7, 0xFF, 0xFF, 0x00, 0x00,
		0x01, 0xFF, 0xFF, 0x01, 0x01, 0x00, 0xFF, 0xFF, 0x07, 0x01, 0x01, 0x01, 0x00, 0x00, 0x7F, 0xFF,
		0x80, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0xFF,
		0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x03, 0x0F, 0x3F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xC7, 0xC7, 0x8F,
		0x8F, 0x9F, 0xBF, 0xFF, 0xFF, 0xC3, 0xC0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC, 0xFC,
		0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xF8, 0xF8, 0xF0, 0xF0, 0xE0, 0xC0, 0x00, 0x01, 0x03, 0x03, 0x03,
		0x03, 0x03, 0x01, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01,
		0x03, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01, 0x03, 0x03, 0x00, 0x00,
		0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0x01, 0x00, 0x00, 0x00, 0x03,
		0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		#if (SH1106_LCDHEIGHT == 64)
			0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x1F, 0x0F,
			0x87, 0xC7, 0xF7, 0xFF, 0xFF, 0x1F, 0x1F, 0x3D, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0x7C, 0x7D, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x0F, 0x07, 0x00, 0x30, 0x30, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0xFE, 0xFE, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xC0, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0xC0, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x3F, 0x1F,
			0x0F, 0x07, 0x1F, 0x7F, 0xFF, 0xFF, 0xF8, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xF8, 0xE0,
			0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFE, 0x00, 0x00,
			0x00, 0xFC, 0xFE, 0xFC, 0x0C, 0x06, 0x06, 0x0E, 0xFC, 0xF8, 0x00, 0x00, 0xF0, 0xF8, 0x1C, 0x0E,
			0x06, 0x06, 0x06, 0x0C, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFE, 0xFE, 0x00, 0x00, 0x00, 0x00, 0xFC,
			0xFE, 0xFC, 0x00, 0x18, 0x3C, 0x7E, 0x66, 0xE6, 0xCE, 0x84, 0x00, 0x00, 0x06, 0xFF, 0xFF, 0x06,
			0x06, 0xFC, 0xFE, 0xFC, 0x0C, 0x06, 0x06, 0x06, 0x00, 0x00, 0xFE, 0xFE, 0x00, 0x00, 0xC0, 0xF8,
			0xFC, 0x4E, 0x46, 0x46, 0x46, 0x4E, 0x7C, 0x78, 0x40, 0x18, 0x3C, 0x76, 0xE6, 0xCE, 0xCC, 0x80,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x0F, 0x1F, 0x1F, 0x3F, 0x3F, 0x3F, 0x3F, 0x1F, 0x0F, 0x03,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00,
			0x00, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x03, 0x07, 0x0E, 0x0C,
			0x18, 0x18, 0x0C, 0x06, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x01, 0x0F, 0x0E, 0x0C, 0x18, 0x0C, 0x0F,
			0x07, 0x01, 0x00, 0x04, 0x0E, 0x0C, 0x18, 0x0C, 0x0F, 0x07, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00,
			0x00, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x07,
			0x07, 0x0C, 0x0C, 0x18, 0x1C, 0x0C, 0x06, 0x06, 0x00, 0x04, 0x0E, 0x0C, 0x18, 0x0C, 0x0F, 0x07,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		#endif
	#endif
};

#define sh1106_swap(a, b) { int16_t t = a; a = b; b = t; }






void Adafruit_SH1106::spi_pre_transfer_callback(spi_transaction_t *t)
{
	gpio_set_level((gpio_num_t)((uint8_t*)&t->user)[0], (uint32_t)((uint8_t*)&t->user)[1]);
}



// the most basic function, set a single pixel
void Adafruit_SH1106::drawPixel(int16_t x, int16_t y, uint16_t color)
{
	if ((x < 0) || (x >= width()) || (y < 0) || (y >= height()))
	{
		return;
	}

	// check rotation, move pixel around if necessary
	switch (getRotation())
	{
		case 1:
			sh1106_swap(x, y);
			x = WIDTH - x - 1;
			break;

		case 2:
			x = WIDTH - x - 1;
			y = HEIGHT - y - 1;
			break;

		case 3:
			sh1106_swap(x, y);
			y = HEIGHT - y - 1;
			break;
	}

	// x is which column
	switch (color)
	{
		case WHITE:
			buffer[x+ (y/8)*SH1106_LCDWIDTH] |=  (1 << (y&7));
			break;
		case BLACK:
			buffer[x+ (y/8)*SH1106_LCDWIDTH] &= ~(1 << (y&7));
			break;
		case INVERSE:
			buffer[x+ (y/8)*SH1106_LCDWIDTH] ^=  (1 << (y&7));
			break;
	}
}



Adafruit_SH1106::Adafruit_SH1106(spi_host_device_t SPI, gpio_num_t DC, gpio_num_t RST) : Adafruit_GFX(SH1106_LCDWIDTH, SH1106_LCDHEIGHT)
{
	memset((void*)&_spi_device, 0, sizeof(_spi_device));
	switch(SPI)
	{
		case HSPI_HOST:
			_spi = SPI;
			_cs = GPIO_NUM_15;
			_rst = RST;
			_dc = DC;
			_sclk = GPIO_NUM_14;
			_mosi = GPIO_NUM_13;
			_vccstate = SH1106_SWITCHCAPVCC;
			break;

		case VSPI_HOST:
			_spi = SPI;
			_cs = GPIO_NUM_5;
			_rst = RST;
			_dc = DC;
			_sclk = GPIO_NUM_18;
			_mosi = GPIO_NUM_23;
			_vccstate = SH1106_SWITCHCAPVCC;
			break;

		default:
			_spi = INVALID_SPI;
			_cs = GPIO_NUM_NC;
			_rst = GPIO_NUM_NC;
			_dc = GPIO_NUM_NC;
			_sclk = GPIO_NUM_NC;
			_mosi = GPIO_NUM_NC;
			_vccstate = SH1106_SWITCHCAPVCC;
			break;
	}
}



void Adafruit_SH1106::begin(uint8_t vccstate, bool reset)
{
	gpio_config_t io_conf;
	spi_bus_config_t spi_bus_conf;
	spi_device_interface_config_t spi_dev_conf;

	//Normal output configuration for pins rst, cs and dc
	io_conf.intr_type = GPIO_INTR_DISABLE; //disable interrupt
	io_conf.mode = GPIO_MODE_OUTPUT; //set as output
	io_conf.pin_bit_mask = ((uint64_t)1 << _rst)|((uint64_t)1 << _dc)|((uint64_t)1 << _cs); //bit mask of the pins that you want to set
	io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE; //disable pull-down
	io_conf.pull_up_en = GPIO_PULLUP_DISABLE; //disable pull-up
	gpio_config(&io_conf); //configure GPIO with the given settings
	// Set CS high

	this->_vccstate = vccstate;

	//Initialize the SPI bus
	memset((void*)&spi_bus_conf, 0, sizeof(spi_bus_conf));
	spi_bus_conf.miso_io_num = GPIO_NUM_NC;
	spi_bus_conf.mosi_io_num = _mosi;
	spi_bus_conf.sclk_io_num = _sclk;
	spi_bus_conf.quadwp_io_num = GPIO_NUM_NC;
	spi_bus_conf.quadhd_io_num = GPIO_NUM_NC;
	spi_bus_conf.max_transfer_sz = 128;
	spi_bus_conf.flags = 0;
	spi_bus_conf.intr_flags = 0;
	spi_bus_initialize(HSPI_HOST, &spi_bus_conf, 1);

	//Attach the LCD to the SPI bus
	memset((void*)&spi_dev_conf, 0, sizeof(spi_dev_conf));
	spi_dev_conf.clock_speed_hz = 4000000; //Clock out at 4MHz
	spi_dev_conf.mode = 0; //SPI mode 0
	spi_dev_conf.spics_io_num = _cs; //CS pin
	spi_dev_conf.queue_size = 2; //We want to be able to queue 2 transactions at a times
	spi_dev_conf.pre_cb = spi_pre_transfer_callback; //Specify pre-transfer callback to handle D/C line
	spi_bus_add_device(_spi, &spi_dev_conf, &_spi_device);
	
	if(reset == true)
	{
		gpio_set_level(_rst, 1);
		// VDD (3.3V) goes high at start, lets just chill for a ms
		vTaskDelay(1);
		// bring reset low
		gpio_set_level(_rst, 0);
		// wait 10ms
		vTaskDelay(10 / portTICK_PERIOD_MS);
		// bring out of reset
		gpio_set_level(_rst, 1);
		// turn on VCC (9V?)
	}

	#if defined SH1106_128_32
		// Init sequence for 128x32 OLED module
		SH1106_command(SH1106_DISPLAYOFF);                    // 0xAE
		SH1106_command(SH1106_SETDISPLAYCLOCKDIV);            // 0xD5
		SH1106_command(0x80);                                  // the suggested ratio 0x80
		SH1106_command(SH1106_SETMULTIPLEX);                  // 0xA8
		SH1106_command(0x1F);
		SH1106_command(SH1106_SETDISPLAYOFFSET);              // 0xD3
		SH1106_command(0x0);                                   // no offset
		SH1106_command(SH1106_SETSTARTLINE | 0x00);            // line #0
		SH1106_command(SH1106_CHARGEPUMP);                    // 0x8D
		if(_vccstate == SH1106_EXTERNALVCC)
		{
			SH1106_command(0x10);
		}
		else
		{
			SH1106_command(0x14);
		}
		SH1106_command(SH1106_MEMORYMODE);                    // 0x20
		SH1106_command(0x00);                                  // 0x0 act like ks0108
		SH1106_command(SH1106_SEGREMAP | 0x01);
		SH1106_command(SH1106_COMSCANDEC);
		SH1106_command(SH1106_SETCOMPINS);                    // 0xDA
		SH1106_command(0x02);
		SH1106_command(SH1106_SETCONTRAST);                   // 0x81
		SH1106_command(0x8F);
		SH1106_command(SH1106_SETPRECHARGE);                  // 0xd9
		if(_vccstate == SH1106_EXTERNALVCC)
		{
			SH1106_command(0x22);
		}
		else
		{
			SH1106_command(0xF1);
		}
		SH1106_command(SH1106_SETVCOMDETECT);                 // 0xDB
		SH1106_command(0x40);
		SH1106_command(SH1106_DISPLAYALLON_RESUME);           // 0xA4
		SH1106_command(SH1106_NORMALDISPLAY);                 // 0xA6
	#endif

	#if defined SH1106_128_64
		// Init sequence for 128x64 OLED module
		SH1106_command(SH1106_DISPLAYOFF);                    // 0xAE
		SH1106_command(SH1106_SETDISPLAYCLOCKDIV);            // 0xD5
		SH1106_command(0x80);                                  // the suggested ratio 0x80
		SH1106_command(SH1106_SETMULTIPLEX);                  // 0xA8
		SH1106_command(0x3F);
		SH1106_command(SH1106_SETDISPLAYOFFSET);              // 0xD3
		SH1106_command(0x00);                                   // no offset

		SH1106_command(SH1106_SETSTARTLINE | 0x00);            // line #0 0x40
		SH1106_command(SH1106_CHARGEPUMP);                    // 0x8D
		if(_vccstate == SH1106_EXTERNALVCC)
		{
			SH1106_command(0x10);
		}
		else
		{
			SH1106_command(0x14);
		}
		SH1106_command(SH1106_MEMORYMODE);                    // 0x20
		SH1106_command(0x00);                                  // 0x0 act like ks0108
		SH1106_command(SH1106_SEGREMAP | 0x01);
		SH1106_command(SH1106_COMSCANDEC);
		SH1106_command(SH1106_SETCOMPINS);                    // 0xDA
		SH1106_command(0x12);
		SH1106_command(SH1106_SETCONTRAST);                   // 0x81
		if(_vccstate == SH1106_EXTERNALVCC)
		{
			SH1106_command(0x9F);
		}
		else
		{
			SH1106_command(0xCF);
		}
		SH1106_command(SH1106_SETPRECHARGE);                  // 0xd9
		if(_vccstate == SH1106_EXTERNALVCC)
		{
			SH1106_command(0x22);
		}
		else
		{
			SH1106_command(0xF1);
		}
		SH1106_command(SH1106_SETVCOMDETECT);                 // 0xDB
		SH1106_command(0x40);
		SH1106_command(SH1106_DISPLAYALLON_RESUME);           // 0xA4
		SH1106_command(SH1106_NORMALDISPLAY);                 // 0xA6
	#endif

	#if defined SH1106_96_16
		// Init sequence for 96x16 OLED module
		SH1106_command(SH1106_DISPLAYOFF);                    // 0xAE
		SH1106_command(SH1106_SETDISPLAYCLOCKDIV);            // 0xD5
		SH1106_command(0x80);                                  // the suggested ratio 0x80
		SH1106_command(SH1106_SETMULTIPLEX);                  // 0xA8
		SH1106_command(0x0F);
		SH1106_command(SH1106_SETDISPLAYOFFSET);              // 0xD3
		SH1106_command(0x00);                                   // no offset
		SH1106_command(SH1106_SETSTARTLINE | 0x00);            // line #0
		SH1106_command(SH1106_CHARGEPUMP);                    // 0x8D
		if(_vccstate == SH1106_EXTERNALVCC)
		{
			SH1106_command(0x10);
		}
		else
		{
			SH1106_command(0x14);
		}
		SH1106_command(SH1106_MEMORYMODE);                    // 0x20
		SH1106_command(0x00);                                  // 0x0 act like ks0108
		SH1106_command(SH1106_SEGREMAP | 0x01);
		SH1106_command(SH1106_COMSCANDEC);
		SH1106_command(SH1106_SETCOMPINS);                    // 0xDA
		SH1106_command(0x2);	//ada x12
		SH1106_command(SH1106_SETCONTRAST);                   // 0x81
		if(_vccstate == SH1106_EXTERNALVCC)
		{
			SH1106_command(0x10);
		}
		else
		{
			SH1106_command(0xAF);
		}
		SH1106_command(SH1106_SETPRECHARGE);                  // 0xd9
		if(vccstate == SH1106_EXTERNALVCC)
		{
			SH1106_command(0x22);
		}
		else
		{
			SH1106_command(0xF1);
		}
		SH1106_command(SH1106_SETVCOMDETECT);                 // 0xDB
		SH1106_command(0x40);
		SH1106_command(SH1106_DISPLAYALLON_RESUME);           // 0xA4
		SH1106_command(SH1106_NORMALDISPLAY);                 // 0xA6
	#endif

	SH1106_command(SH1106_DISPLAYON);//--turn on oled panel
}



void Adafruit_SH1106::invertDisplay(uint8_t i)
{
  if(i)
  {
    SH1106_command(SH1106_INVERTDISPLAY);
  }
  else
  {
    SH1106_command(SH1106_NORMALDISPLAY);
  }
}



/*
#define SH1106_SETLOWCOLUMN 0x00
#define SH1106_SETHIGHCOLUMN 0x10
#define SH1106_SETSTARTLINE 0x40
*/



void Adafruit_SH1106::display(void)
{
	uint16_t page;
	
	SH1106_command(SH1106_SETLOWCOLUMN | 0x00);		// low col = 0
	SH1106_command(SH1106_SETHIGHCOLUMN | 0x00);	// hi col = 0
	SH1106_command(SH1106_SETSTARTLINE | 0x00);		// line #0
	
	for (page = 0; page < (SH1106_LCDHEIGHT/8); page++)
	{
		// send a bunch of data in one transmission
		SH1106_command(SH1106_SETPAGEADDRESS | page);//set page address
		SH1106_command(SH1106_SETLOWCOLUMN | 0x02);//set lower column address
		SH1106_command(SH1106_SETHIGHCOLUMN | 0x00);//set higher column address

		SPI_snd(&buffer[page*SH1106_LCDWIDTH], SH1106_LCDWIDTH, 1);
	}
}



// clear everything
void Adafruit_SH1106::clearDisplay(void)
{
	memset(buffer, 0, (SH1106_LCDWIDTH*SH1106_LCDHEIGHT/8));
}



inline void Adafruit_SH1106::SPI_snd(uint8_t *data, uint8_t len, uint8_t dc)
{
	spi_transaction_t t;
	if(len == 0)
	{
		return;             //no need to send anything
	}
	memset((void*)&t, 0, sizeof(t));//Zero out the transaction
	t.length = len*8;                 //Len is in bytes, transaction length is in bits.
	t.tx_buffer = data;               //Data
	((uint8_t*)&t.user)[0] = (uint8_t)_dc;
	((uint8_t*)&t.user)[1] = (uint8_t)dc;
	spi_device_polling_transmit(_spi_device, &t);  //Transmit!
}



void Adafruit_SH1106::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
	bool bSwap = false;

	switch(rotation)
	{
		case 0:
			// 0 degree rotation, do nothing
			break;

		case 1:
			// 90 degree rotation, swap x & y for rotation, then invert x
			bSwap = true;
			sh1106_swap(x, y);
			x = WIDTH - x - 1;
			break;

		case 2:
			// 180 degree rotation, invert x and y - then shift y around for height.
			x = WIDTH - x - 1;
			y = HEIGHT - y - 1;
			x -= (w-1);
			break;

		case 3:
			// 270 degree rotation, swap x & y for rotation, then invert y  and adjust y for w (not to become h)
			bSwap = true;
			sh1106_swap(x, y);
			y = HEIGHT - y - 1;
			y -= (w-1);
			break;
	}

	if(bSwap)
	{
		drawFastVLineInternal(x, y, w, color);
	}
	else
	{
		drawFastHLineInternal(x, y, w, color);
	}
}



void Adafruit_SH1106::drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color)
{
	// Do bounds/limit checks
	if(y < 0 || y >= HEIGHT)
	{
		return;
	}

	// make sure we don't try to draw below 0
	if(x < 0)
	{
		w += x;
		x = 0;
	}

	// make sure we don't go off the edge of the display
	if( (x + w) > WIDTH)
	{
		w = (WIDTH - x);
	}

	// if our width is now negative, punt
	if(w <= 0)
	{
		return;
	}

	// set up the pointer for  movement through the buffer
	register uint8_t *pBuf = buffer;
	// adjust the buffer pointer for the current row
	pBuf += ((y/8) * SH1106_LCDWIDTH);
	// and offset x columns in
	pBuf += x;

	register uint8_t mask = 1 << (y&7);

	switch (color)
	{
		case WHITE:
			while(w--)
			{
				*pBuf++ |= mask;
			}
			break;

		case BLACK:
			mask = ~mask;
			while(w--)
			{
				*pBuf++ &= mask;
			}
			break;

		case INVERSE:
			while(w--)
			{
				*pBuf++ ^= mask;
			}
			break;
	}
}



void Adafruit_SH1106::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
  bool bSwap = false;
  switch(rotation) { 
    case 0:
      break;
    case 1:
      // 90 degree rotation, swap x & y for rotation, then invert x and adjust x for h (now to become w)
      bSwap = true;
      sh1106_swap(x, y);
      x = WIDTH - x - 1;
      x -= (h-1);
      break;
    case 2:
      // 180 degree rotation, invert x and y - then shift y around for height.
      x = WIDTH - x - 1;
      y = HEIGHT - y - 1;
      y -= (h-1);
      break;
    case 3:
      // 270 degree rotation, swap x & y for rotation, then invert y 
      bSwap = true;
      sh1106_swap(x, y);
      y = HEIGHT - y - 1;
      break;
  }

  if(bSwap) { 
    drawFastHLineInternal(x, y, h, color);
  } else {
    drawFastVLineInternal(x, y, h, color);
  }
}



void Adafruit_SH1106::drawFastVLineInternal(int16_t x, int16_t __y, int16_t __h, uint16_t color)
{

  // do nothing if we're off the left or right side of the screen
  if(x < 0 || x >= WIDTH) { return; }

  // make sure we don't try to draw below 0
  if(__y < 0) { 
    // __y is negative, this will subtract enough from __h to account for __y being 0
    __h += __y;
    __y = 0;

  } 

  // make sure we don't go past the height of the display
  if( (__y + __h) > HEIGHT) { 
    __h = (HEIGHT - __y);
  }

  // if our height is now negative, punt 
  if(__h <= 0) { 
    return;
  }

  // this display doesn't need ints for coordinates, use local uint8_t registers for faster juggling
  register uint8_t y = __y;
  register uint8_t h = __h;


  // set up the pointer for fast movement through the buffer
  register uint8_t *pBuf = buffer;
  // adjust the buffer pointer for the current row
  pBuf += ((y/8) * SH1106_LCDWIDTH);
  // and offset x columns in
  pBuf += x;

  // do the first partial uint8_t, if necessary - this requires some masking
  register uint8_t mod = (y&7);
  if(mod) {
    // mask off the high n bits we want to set 
    mod = 8-mod;

    // note - lookup table results in a nearly 10% performance improvement in fill* functions
    // register uint8_t mask = ~(0xFF >> (mod));
    static uint8_t premask[8] = {0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE };
    register uint8_t mask = premask[mod];

    // adjust the mask if we're not going to reach the end of this uint8_t
    if( h < mod) { 
      mask &= (0XFF >> (mod-h));
    }

  switch (color) 
    {
    case WHITE:   *pBuf |=  mask;  break;
    case BLACK:   *pBuf &= ~mask;  break;
    case INVERSE: *pBuf ^=  mask;  break;
    }
  
    // fast exit if we're done here!
    if(h<mod) { return; }

    h -= mod;

    pBuf += SH1106_LCDWIDTH;
  }


  // write solid bytes while we can - effectively doing 8 rows at a time
  if(h >= 8) { 
    if (color == INVERSE)  {          // separate copy of the code so we don't impact performance of the black/white write version with an extra comparison per loop
      do  {
      *pBuf=~(*pBuf);

        // adjust the buffer forward 8 rows worth of data
        pBuf += SH1106_LCDWIDTH;

        // adjust h & y (there's got to be a faster way for me to do this, but this should still help a fair bit for now)
        h -= 8;
      } while(h >= 8);
      }
    else {
      // store a local value to work with 
      register uint8_t val = (color == WHITE) ? 255 : 0;

      do  {
        // write our value in
      *pBuf = val;

        // adjust the buffer forward 8 rows worth of data
        pBuf += SH1106_LCDWIDTH;

        // adjust h & y (there's got to be a faster way for me to do this, but this should still help a fair bit for now)
        h -= 8;
      } while(h >= 8);
      }
    }

  // now do the final partial uint8_t, if necessary
  if(h) {
    mod = h & 7;
    // this time we want to mask the low bits of the bytes, vs the high bits we did above
    // register uint8_t mask = (1 << mod) - 1;
    // note - lookup table results in a nearly 10% performance improvement in fill* functions
    static uint8_t postmask[8] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F };
    register uint8_t mask = postmask[mod];
    switch (color) 
    {
      case WHITE:   *pBuf |=  mask;  break;
      case BLACK:   *pBuf &= ~mask;  break;
      case INVERSE: *pBuf ^=  mask;  break;
    }
  }
}
