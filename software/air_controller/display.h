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

uint8_t buffer_front_speed = 0;
uint8_t buffer_rear_speed = 0;


void tempset()
{
	tft.setTextSize(2);
	tft.setTextColor(ST7735_WHITE,clear_temp);
	tft.setCursor(103,56);
	tft.print(conv::front_temp_set,1);
}

void tempread()
{
	tft.setTextSize(2);
	tft.setTextColor(ST7735_WHITE,clear_temp);
	tft.setCursor(12,56);
	tft.print(conv::front_temp_read,1);
}

void fanspeed()
{
	if(conv::mode == 3)
	{
		 // front 
		buffer_front_speed =  map( conv::real_front_speed,min_pwm_speed,max_pwm_speed,5,105);
		tft.fillRect( 35,12, buffer_front_speed,4, ST7735_WHITE);
		tft.fillRect(35 + buffer_front_speed, 12, 105-buffer_front_speed,4,ST7735_BLACK);

		// rear
		buffer_rear_speed =  map( conv::real_rear_speed,min_pwm_speed,max_pwm_speed,5,105);
		tft.fillRect( 35, 17, buffer_front_speed,4, ST7735_WHITE);
		tft.fillRect(35 + buffer_front_speed, 17, 105-buffer_front_speed,4,ST7735_BLACK);
	}
	else 
	{
		buffer_front_speed = map( conv::front_speed,min_fan_speed,max_fan_speed,5,105);
		tft.fillRect( 35, 12,buffer_front_speed,4, ST7735_WHITE );
		tft.fillRect( 35 + buffer_front_speed, 12, 105 - buffer_front_speed, 4, ST7735_BLACK );

		// rear
		buffer_rear_speed =  map( conv::rear_speed,min_fan_speed,max_fan_speed,5,105);
		tft.fillRect( 35, 17, buffer_front_speed,4, ST7735_WHITE);
		tft.fillRect(35 + buffer_front_speed, 17, 105-buffer_front_speed,4,ST7735_BLACK);
	}
	
}

void compressor()
{
	if(conv::mode == 0 || conv::mode ==3)
	{
		if(conv::compressor == true)
			tft.drawBitmap( 68, 48,ice,25, 25, ST77XX_WHITE ); 
		else
			tft.fillRect( 70, 46,29,29, ST77XX_BLACK );
	}
	else if(conv::mode == 1 || conv::mode == 2)
	{
		tft.drawBitmap( 68, 48,ice,25, 25, ST77XX_RED ); 
	}
}

void mode()
{
	tft.setTextSize(2);
	tft.setCursor(102,103);
	tft.setTextColor(ST77XX_WHITE,ST77XX_BLACK);
	if(conv::mode == 0)
		tft.print("COOL");
	else if(conv::mode == 1)
		tft.print(" FAN");
	else if(conv::mode == 2)
		tft.print(" OFF");
	else if(conv::mode == 3)
		tft.print("AUTO");
}

void display::run()
{
	tempset();
	fanspeed();
	compressor();
	mode();
	tempread();
}

void display::setup()
{
	// tft.begin(115200);
	tft.initR(INITR_BLACKTAB);
	analogWrite(tft_blacklight,conv::blacklight);
	tft.setRotation( 1 );  
	tft.fillScreen(ST77XX_BLACK);
	tft.setTextWrap(false);

	// tft.fillRoundRect( 0,32,65, 60, 0, ST77XX_BLACK);
	// tft.fillRoundRect( 95,32,65, 60, 0, ST77XX_BLACK);

	tft.setTextSize(1);
	tft.setTextColor(ST77XX_WHITE);
	tft.setCursor(110,39);
	tft.print("setup");
	
	tft.setTextColor(ST77XX_WHITE);
	tft.setCursor(16,39);
	tft.print("room");

	tft.setTextSize(1);
	tft.setTextColor(ST77XX_WHITE);
	tft.setCursor(15,78);
	tft.print("o");
	tft.setTextSize(2);
	tft.print("C");

	tft.setTextSize(1);
	tft.setTextColor(ST77XX_WHITE);
	tft.setCursor(114,78);
	tft.print("o");
	tft.setTextSize(2);
	tft.print("C");

  tft.drawBitmap( 8, 3,fan,25, 25, ST7735_WHITE );
}



