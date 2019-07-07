#pragma once

#include <arduino.h>
#include <SimpleRotary.h>
#include <Encoder_Polling_V2.h>


#include "pin.h"
#include "control_value.h"

class func
{
	private:
		static uint8_t fmap(uint8_t i);
		static uint8_t afmap(float temp_read,float temp_set);
		static void encoder_left();
		static void encoder_right();
	public:
		static void begin();

		static void encoder_run();
		static void compressor_run();
		static void fan_run();
		static void mode_run();
		static void temp_read_run();

};

	byte L,R,BR,BL;
	uint8_t real_front_speed = 0, real_rear_speed = 0;
	uint64_t time_fan = 0;

// Pin A, Pin B, Button Pin
SimpleRotary enL(_leften_outa,_leften_outb,_leften_btn);
SimpleRotary enR(_righten_outa,_righten_outb,_righten_btn);

void func::begin()
{
	pinMode(_compressor,OUTPUT);
}

uint8_t func::fmap(uint8_t i)
{
	uint8_t speed;
	if(i == off_fan_speed) speed = off_fan_speed;
	else 
	{
		speed = map(i,min_fan_speed,max_fan_speed,min_pwm_speed,max_pwm_speed);
	}
	return speed;
}

uint8_t func::afmap(float temp_read,float temp_set)
{
	uint8_t speed;
	float def_temp = temp_read - temp_set;
	if(def_temp < 0) def_temp = 0; 
	speed = map(def_temp,min_def_temp,max_def_temp,min_pwm_speed,max_pwm_speed);
	return speed;
}

void func::temp_read_run()
{
	int fVo , rVo;
	float R1 = 100000;
	float fLogR2, fR2, fT;
	float rLogR2, rR2, rT;
	float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

	fVo = analogRead(_front_temp);
	fR2 = R1 * (1023.0/(float)fVo - 1.0);
	fLogR2 = log(fR2);
	fT = (1.0 / (c1 + c2*fLogR2 + c3*fLogR2*fLogR2*fLogR2));
	conv::front_temp_read = fT - 273.15;

	rVo = analogRead(_rear_temp);
	rR2 = R1 * (1023.0/(float)rVo - 1.0);
	rLogR2 = log(rR2);
	rT = (1.0 / (c1 + c2*rLogR2 + c3*rLogR2*rLogR2*rLogR2));
	conv::rear_temp_read = rT - 273.15;
																																																																																																																																																																													
}

void func::encoder_run()
{
	func::encoder_left();
	func::encoder_right();

	if(L == 1 || L == 2)
	{
		conv::mode = 0;
	}
}

void func::compressor_run()
{
	if(conv::mode == 0 || conv::mode == 3) 
	{
		if(conv::front_temp_read <= conv::front_temp_set && conv::rear_temp_read <= conv::rear_temp_set)
		{
			conv::compressor = false;
		}
		else if(conv::front_temp_read > conv::front_temp_set + e_temp || conv::rear_temp_read > conv::rear_temp_set + e_temp)
		{
			conv::compressor = true;
		}
	}
	else if(conv::mode == 1 || conv::mode == 2)
	{
		conv::compressor = false;
	}
}
 
void func::fan_run()
{
	if(millis() - time_fan > between_time_fan)
	{
		time_fan = millis();
		if(conv::mode == 0 || conv::mode == 1)
		{	
			if(conv::fan_mode == 0) 
			{
				if(real_front_speed < func::fmap(conv::front_speed))
				{
					real_front_speed++;
					real_rear_speed = real_front_speed;
				}
				else if(real_front_speed > func::fmap(conv::front_speed))
				{
					real_front_speed--;
					real_rear_speed = real_front_speed;
				}
			}
			else if(conv::fan_mode == 1)
			{
				if(real_front_speed < func::fmap(conv::front_speed)) real_front_speed++;
				else if(real_front_speed > func::fmap(conv::front_speed)) real_front_speed--;
			}
			else if(conv::fan_mode == 2)
			{
				if(real_rear_speed < func::fmap(conv::rear_speed)) real_rear_speed++;
				else if(real_rear_speed > func::fmap(conv::rear_speed)) real_rear_speed--;
			}
		}
		else if(conv::mode == 2)
		{
			if(conv::fan_mode == 0 && real_front_speed > 0)
			{
				real_front_speed--;
				real_rear_speed = real_front_speed;    
			}
			else if(conv::fan_mode == 1 && real_front_speed > 0)real_front_speed--;
			else if(conv::fan_mode == 2 && real_rear_speed > 0)real_rear_speed--;
		}
		else if(conv::mode == 3)
		{
			if(conv::fan_mode == 0) 
			{
				if(real_front_speed < func::afmap(conv::front_temp_read,conv::front_temp_set))
				{
					real_front_speed++;
					real_rear_speed = real_front_speed;
				}
				else if(real_front_speed > func::afmap(conv::front_temp_read,conv::front_temp_set))
				{
					real_front_speed--;
					real_rear_speed = real_front_speed;
				}
			}
			else if(conv::fan_mode == 1)
			{
				if(real_front_speed < func::afmap(conv::front_temp_read,conv::front_temp_set)) real_front_speed++;
				else if(real_front_speed > func::afmap(conv::front_temp_read,conv::front_temp_set)) real_front_speed--;
			}
			else if(conv::fan_mode == 2)
			{
				if(real_rear_speed < func::afmap(conv::front_temp_read,conv::front_temp_set)) real_rear_speed++;
				else if(real_rear_speed > func::afmap(conv::rear_temp_read,conv::rear_temp_set)) real_rear_speed--;
			}
		}
	}
	analogWrite(_front_fan,real_front_speed);
	analogWrite(_rear_fan,real_rear_speed);
}

void func::mode_run()
{
	BR = enR.pushType(push_split);
	if(BR == 1) 
	{
		conv::mode = 3; // pushed auto mode	
		Serial.println("endoer press ");

	}
	else if(BR == 2) conv::mode = 1; // long pushed fan only mode
}


void func::encoder_left()
{
	L = enL.rotate();
	if(L == 1) //CW
	{
		if(conv::fan_mode == 0 && conv::front_speed < max_fan_speed)
		{
				conv::front_speed++;
				conv::rear_speed = conv::front_speed;
		}
		else if(conv::fan_mode == 1 && conv::front_speed < max_fan_speed)
		{
			conv::front_speed++;
		}
		else if(conv::fan_mode == 2 && conv::rear_speed < max_fan_speed)
		{
			conv::rear_speed++;
		}
	}
	else if(L == 2) //CCW
	{
		if(conv::fan_mode == 0 && conv::front_speed > min_fan_speed)
		{
				conv::front_speed--;
				conv::rear_speed = conv::front_speed;
		}
		else if(conv::fan_mode == 1 && conv::front_speed > min_fan_speed)
		{
			conv::front_speed--;
		}
		else if(conv::fan_mode == 2 && conv::rear_speed > min_fan_speed)
		{
			conv::rear_speed--;
		}
	}
}

void func::encoder_right()
{
	R = enR.rotate();
	if(R == 1) //CW
	{
		if(conv::fan_mode == 0 && conv::front_temp_set < max_temp_set)
		{
				conv::front_temp_set += c_temp;
				conv::rear_temp_set = conv::front_temp_set;
		}
		else if(conv::fan_mode == 1 && conv::front_temp_set < max_temp_set)
		{
			conv::front_temp_set += c_temp;
		}
		else if(conv::fan_mode == 2 && conv::front_temp_set < max_temp_set)
		{
			conv::rear_temp_set += c_temp;
		}
	}
	else if(R == 2) //CCW
	{
		if(conv::fan_mode == 0 && conv::front_temp_set > min_temp_set)
		{
				conv::front_temp_set -= c_temp;
				conv::rear_temp_set = conv::front_temp_set;
		}
		else if(conv::fan_mode == 1 && conv::front_temp_set > min_temp_set)
		{
			conv::front_temp_set -= c_temp;
		}
		else if(conv::fan_mode == 2 && conv::rear_temp_set > min_temp_set)
		{
			conv::rear_temp_set -= c_temp;
		}
	}
}

