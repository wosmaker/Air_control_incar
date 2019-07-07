#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_ST77xx.h>

#include <SPI.h>

#include "control_value.h"
#include "pin.h"

Adafruit_ST7735 tft = Adafruit_ST7735(tft_cs, tft_dc, tft_sda, tft_sck, tft_reset);

// color
#define clear_temp ST77XX_BLACK

class display
{
private:

public:
	static void setup();
	static void run();

};

//fan icon
 uint8_t fan[]{ 0, 31, 0, 0, 0, 63, 192, 0, 0, 127, 192, 0, 0, 127, 192, 0, 0, 127, 192, 0, 0, 127, 192,
 0, 0, 127, 0, 0, 56, 127, 0, 0, 124, 62, 0, 0, 254, 62, 127, 0, 255, 35, 255, 128, 255, 221, 255, 128,
 255, 222, 255, 128, 255, 221, 255, 128, 127, 233, 255, 128, 127, 183, 63, 128, 30, 30, 31, 128, 0, 63,
 15, 0, 0, 63, 0, 0, 0, 255, 128, 0, 1, 255, 128, 0, 1, 255, 128, 0, 1, 255, 128, 0, 1, 255, 0, 0, 0,
 254, 0, 0};

//ice icon
 uint8_t ice[]{ 0, 0, 0, 0, 3, 128, 224, 0, 3, 128, 224, 0, 3, 148, 224, 0, 0, 148, 128, 0, 0, 182, 128, 0,
 0, 99, 0, 0, 3, 227, 224, 0, 0, 99, 0, 0, 12, 54, 24, 0, 6, 28, 48, 0, 99, 54, 99, 0, 255, 227, 255, 128,
 97, 34, 71, 0, 98, 62, 35, 0, 4, 62, 16, 0, 8, 34, 8, 0, 1, 227, 192, 0, 7, 99, 112, 0, 0, 231, 128, 0, 0,
 182, 128, 0, 3, 148, 224, 0, 3, 128, 224, 0, 3, 128, 224, 0, 3, 0, 96, 0};

float buffer_front_tempset = 0;
float buffer_front_tempread = 0;

uint8_t buffer_front_speed = 0;
uint8_t buffer_mode = 99;


void tempset()
{
	if(buffer_front_tempset != conv::front_temp_set)
	{
		tft.setTextSize(2);
		// clear
		tft.setTextColor(clear_temp);
    tft.setCursor(103,56);
    tft.print(buffer_front_tempset,1);

		// set
		tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(103,56);
    tft.print(conv::front_temp_set,1);
		buffer_front_tempset = conv::front_temp_set;
	}
}

void tempread()
{
	if(buffer_front_tempread != conv::front_temp_read)
	{
		tft.setTextSize(2);
		//clear
    tft.setTextColor(clear_temp);
    tft.setCursor(12,56);
    tft.print(buffer_front_tempread,1);
		//set
    tft.setTextColor(ST7735_WHITE);
    tft.setCursor(12,56);
    tft.print(conv::front_temp_read,1);

		buffer_front_tempread = conv::front_temp_read;
	}

}

void fanspeed()
{
	if(buffer_front_speed != conv::front_speed)
	{
		//clear
		tft.fillRect( 35, 12, map(buffer_front_speed,min_fan_speed,max_fan_speed,0,105),5, ST7735_BLACK );
		//set
		tft.fillRect( 35, 12, map( conv::front_speed,min_fan_speed,max_fan_speed,0,105),5, ST7735_WHITE );
		buffer_front_speed = conv::front_speed;
	}
}

void compressor()
{
	if(conv::mode == 0 || conv::mode ==3)
	{
		if(conv::compressor == true)
			tft.drawBitmap( 68, 48,ice,25, 25, ST77XX_WHITE ); 
		else
			tft.drawBitmap( 68, 48,ice,25, 25, ST77XX_BLACK ); 
	}
	else if(conv::mode == 1 || conv::mode == 2)
	{
		tft.drawBitmap( 68, 48,ice,25, 25, ST77XX_RED ); 
	}
}

void mode()
{
	if(buffer_mode != conv::mode)
	{
		tft.setTextSize(2);
		tft.setCursor(102,103);
		//clear
		tft.setTextColor(ST7735_BLACK);
		if(buffer_mode == 0)
		  tft.print("COOL");
		else if(buffer_mode == 1)
			tft.print("FAN");
		else if(buffer_mode == 2)
		  tft.print("OFF");
		else if(buffer_mode == 3)
			tft.print("AUTO");

		//set
		tft.setCursor(102,103);
		tft.setTextColor(ST77XX_WHITE);
		if(conv::mode == 0)
		  tft.print("COOL");
		else if(conv::mode == 1)
			tft.print("FAN");
		else if(conv::mode == 2)
		  tft.print("OFF");
		else if(conv::mode == 3)
			tft.print("AUTO");
		buffer_mode = conv::mode;
	}
}

void display::run()
{
	tempset();
	fanspeed();
	compressor();
	mode();
	// tempread();
}

void display::setup()
{
	tft.initR(INITR_BLACKTAB);
	analogWrite(tft_blacklight,conv::blacklight);
	tft.setRotation( 1 );  
	tft.fillScreen(ST77XX_BLACK);
	tft.setTextWrap(false);

	tft.fillRoundRect( 0,32,65, 60, 0, ST77XX_BLACK);
	tft.fillRoundRect( 95,32,65, 60, 0, ST77XX_BLACK);

	tft.setTextSize(1);
	tft.setTextColor(ST77XX_WHITE);
	tft.setCursor(110,39);
	tft.print("setup");
	
	tft.setTextColor(ST77XX_WHITE);
	tft.setCursor(16,39);
	tft.print("room");
	
	tft.setTextSize(1);
	tft.setTextColor(ST77XX_WHITE);
	tft.setCursor(10,78);
	tft.print("Celsuis");
	
	tft.setTextColor(ST77XX_WHITE); 
	tft.setCursor(108,78);
	tft.print("Celsuis");

  tft.drawBitmap( 8, 3,fan,25, 25, ST7735_WHITE );
}



