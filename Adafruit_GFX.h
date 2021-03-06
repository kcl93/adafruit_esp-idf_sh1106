#ifndef _ADAFRUIT_GFX_H
#define _ADAFRUIT_GFX_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "gfxfont.h"

/// A generic graphics superclass that can handle all sorts of drawing. At a minimum you can subclass and provide drawPixel(). At a maximum you can do a ton of overriding to optimize. Used for any/all Adafruit displays!
class Adafruit_GFX {
	public:
		Adafruit_GFX(int16_t w, int16_t h); // Constructor
		virtual ~Adafruit_GFX(void){} // Destructor

		// This MUST be defined by the subclass:
		virtual void drawPixel(int16_t x, int16_t y, uint16_t color) = 0;    ///< Virtual drawPixel() function to draw to the screen/framebuffer/etc, must be overridden in subclass. @param x X coordinate.  @param y Y coordinate. @param color 16-bit pixel color.

		// TRANSACTION API / CORE DRAW API
		// These MAY be overridden by the subclass to provide device-specific
		// optimized code.  Otherwise 'generic' versions are used.
		virtual void startWrite(void);
		virtual void writePixel(int16_t x, int16_t y, uint16_t color);
		virtual void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
		virtual void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
		virtual void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
		virtual void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
		virtual void endWrite(void);

		// CONTROL API
		// These MAY be overridden by the subclass to provide device-specific
		// optimized code.  Otherwise 'generic' versions are used.
		virtual void setRotation(uint8_t r);
		virtual void invertDisplay(bool i);

		// BASIC DRAW API
		// These MAY be overridden by the subclass to provide device-specific
		// optimized code.  Otherwise 'generic' versions are used.
		// It's good to implement those, even if using transaction API
		virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
		virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
		virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
		virtual void fillScreen(uint16_t color);
		// Optional and probably not necessary to change
		virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
		virtual void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

		// These exist only with Adafruit_GFX (no subclass overrides)
		void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
		void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
		void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
		void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
		void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
		void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
		void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
		void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
		void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);
		void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color, uint16_t bg);
		void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
		void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg);
		void drawXBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);
		void drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h);
		void drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h);
		void drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[], const uint8_t mask[], int16_t w, int16_t h);
		void drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, uint8_t *mask, int16_t w, int16_t h);
		void drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h);
		void drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h);
		void drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], const uint8_t mask[], int16_t w, int16_t h);
		void drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, uint8_t *mask, int16_t w, int16_t h);
		void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
		void setCursor(int16_t x, int16_t y);
		void setTextColor(uint16_t c);
		void setTextColor(uint16_t c, uint16_t bg);
		void setTextSize(uint8_t s);
		void setTextWrap(bool w);
		void cp437(bool x=true);
		void setFont(const GFXfont *f = NULL);
		void getTextBounds(const char *string, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);

		virtual size_t write(char c);
		virtual void print(const char *s);
		virtual void print(char *s);

		int16_t height(void) const;
		int16_t width(void) const;

		uint8_t getRotation(void) const;

		// get current cursor position (get rotation safe maximum values, using: width() for x, height() for y)
		int16_t getCursorX(void) const;
		int16_t getCursorY(void) const;

	protected:
		void charBounds(char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy);
		const int16_t WIDTH;	///< This is the 'raw' display width - never changes
		const int16_t HEIGHT;	///< This is the 'raw' display height - never changes
		int16_t _width;			///< Display width as modified by current rotation
		int16_t _height;		///< Display height as modified by current rotation
		int16_t cursor_x;		///< x location to start print()ing text
		int16_t cursor_y;		///< y location to start print()ing text
		uint16_t textcolor;		///< 16-bit background color for print()
		uint16_t textbgcolor;	///< 16-bit text color for print()
		uint8_t textsize;		///< Desired magnification of text to print()
		uint8_t rotation;		///< Display rotation (0 thru 3)

		bool wrap;           ///< If set, 'wrap' text at right edge of display
		bool _cp437;         ///< If set, use correct CP437 charset (default is off)
		GFXfont *gfxFont;       ///< Pointer to special font
};


/// A simple drawn button UI element
class Adafruit_GFX_Button {
	public:
		Adafruit_GFX_Button(void);
		// "Classic" initButton() uses center & size
		void initButton(Adafruit_GFX *gfx, int16_t x, int16_t y,
		uint16_t w, uint16_t h, uint16_t outline, uint16_t fill,
		uint16_t textcolor, char *label, uint8_t textsize);
		// New/alt initButton() uses upper-left corner & size
		void initButtonUL(Adafruit_GFX *gfx, int16_t x1, int16_t y1,
		uint16_t w, uint16_t h, uint16_t outline, uint16_t fill,
		uint16_t textcolor, char *label, uint8_t textsize);
		void drawButton(bool inverted = false);
		bool contains(int16_t x, int16_t y);

		void press(bool p);
		bool isPressed();
		bool justPressed();
		bool justReleased();

	private:
		Adafruit_GFX *_gfx;
		int16_t       _x1, _y1; // Coordinates of top-left corner
		uint16_t      _w, _h;
		uint8_t       _textsize;
		uint16_t      _outlinecolor, _fillcolor, _textcolor;
		char          _label[10];

		bool currstate, laststate;
};


/// A GFX 1-bit canvas context for graphics
class GFXcanvas1 : public Adafruit_GFX {
	public:
		GFXcanvas1(uint16_t w, uint16_t h);
		~GFXcanvas1(void);
		void drawPixel(int16_t x, int16_t y, uint16_t color);
		void fillScreen(uint16_t color);
		uint8_t *getBuffer(void);

	private:
		uint8_t *buffer;
};


/// A GFX 8-bit canvas context for graphics
class GFXcanvas8 : public Adafruit_GFX {
	public:
		GFXcanvas8(uint16_t w, uint16_t h);
		~GFXcanvas8(void);
		void drawPixel(int16_t x, int16_t y, uint16_t color);
		void fillScreen(uint16_t color);
		void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
		uint8_t *getBuffer(void);

	private:
		uint8_t *buffer;
};


///  A GFX 16-bit canvas context for graphics
class GFXcanvas16 : public Adafruit_GFX {
	public:
		GFXcanvas16(uint16_t w, uint16_t h);
		~GFXcanvas16(void);
		void drawPixel(int16_t x, int16_t y, uint16_t color);
		void fillScreen(uint16_t color);
		uint16_t *getBuffer(void);

	private:
		uint16_t *buffer;
};

#endif // _ADAFRUIT_GFX_H
