#pragma once

#include <arduino.h>
#include <Encoder_Polling_V2.h>

#include <RBD_Timer.h> 
#include <RBD_Button.h> 

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

	int8_t L,R;
	uint64_t time_fan = 0;
	uint64_t time_temp = 0;
	

	RBD::Button enLB(_leften_btn);
	RBD::Button enRB(_righten_btn);
	
	uint8_t smooth_i = 0;
	uint64_t time_smooth_temp = 0;
	float s_temp[20]{2.1f};


void smooth_temp_input(float input)
{
	input = ((int)(input * 10))/10.0;
	s_temp[smooth_i++] = input;
	if(smooth_i == 21) smooth_i = 0;
}

float smooth_temp_output()
{
	float output = 0;
	for(int i = 0 ; i <= 20 ; i++)
	{
		output += s_temp[i]; 
	}
	output = output / 21; 
	output = ((int)(output * 10))/10.0;
	return output;
}

void func::begin()
{
	pinMode(_compressor,OUTPUT);
	encoder_begin();
	attach_encoder(enL, _leften_outa, _leften_outb);
	attach_encoder(enR, _righten_outa, _righten_outb);
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
	if(millis() - time_smooth_temp > 100)
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
		smooth_temp_input(fT - 273.15);

		// rVo = analogRead(_rear_temp);
		// rR2 = R1 * (1023.0/(float)rVo - 1.0);
		// rLogR2 = log(rR2);
		// rT = (1.0 / (c1 + c2*rLogR2 + c3*rLogR2*rLogR2*rLogR2));
	}

	if(millis() - time_temp > between_time_temp)
	{
		conv::front_temp_read = smooth_temp_output();
		conv::rear_temp_read = 	conv::front_temp_read;
		time_temp = millis();
	}																																																																																																																																																																								
}

void func::encoder_run()
{
	func::encoder_left();
	func::encoder_right();

	if(L == 1 || L == -1)
	{
		conv::mode = 0;
	}
}

void func::compressor_run()
{

	if(conv::mode == 1 || conv::mode == 2 || ( conv::front_speed == 0 && conv::rear_speed == 0))
	{
		conv::compressor = false;
	}	

  else if(conv::mode == 0 || conv::mode == 3) 
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

	if(conv::compressor == true)
		digitalWrite(_compressor,HIGH);
	else
		digitalWrite(_compressor,LOW);
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
				if(conv::real_front_speed < func::fmap(conv::front_speed))
				{
					conv::real_front_speed++;
					conv::real_rear_speed = conv::real_front_speed;
				}
				else if(conv::real_front_speed > func::fmap(conv::front_speed))
				{
					conv::real_front_speed--;
					conv::real_rear_speed = conv::real_front_speed;
				}
			}
			else if(conv::fan_mode == 1)
			{
				if(conv::real_front_speed < func::fmap(conv::front_speed)) conv::real_front_speed++;
				else if(conv::real_front_speed > func::fmap(conv::front_speed)) conv::real_front_speed--;
			}
			else if(conv::fan_mode == 2)
			{
				if(conv::real_rear_speed < func::fmap(conv::rear_speed)) conv::real_rear_speed++;
				else if(conv::real_rear_speed > func::fmap(conv::rear_speed)) conv::real_rear_speed--;
			}
		}
		else if(conv::mode == 2)
		{
			if(conv::fan_mode == 0 && conv::real_front_speed > 0)
			{
				conv::real_front_speed--;
				conv::real_rear_speed = conv::real_front_speed;    
			}
			else if(conv::fan_mode == 1 && conv::real_front_speed > 0)conv::real_front_speed--;
			else if(conv::fan_mode == 2 && conv::real_rear_speed > 0)conv::real_rear_speed--;
		}
		else if(conv::mode == 3)
		{
			if(conv::fan_mode == 0) 
			{
				if(conv::real_front_speed < func::afmap(conv::front_temp_read,conv::front_temp_set))
				{
					conv::real_front_speed++;
					conv::real_rear_speed = conv::real_front_speed;
				}
				else if(conv::real_front_speed > func::afmap(conv::front_temp_read,conv::front_temp_set))
				{
					conv::real_front_speed--;
					conv::real_rear_speed = conv::real_front_speed;
				}
			}
			else if(conv::fan_mode == 1)
			{
				if(conv::real_front_speed < func::afmap(conv::front_temp_read,conv::front_temp_set)) conv::real_front_speed++;
				else if(conv::real_front_speed > func::afmap(conv::front_temp_read,conv::front_temp_set)) conv::real_front_speed--;
			}
			else if(conv::fan_mode == 2)
			{
				if(conv::real_rear_speed < func::afmap(conv::front_temp_read,conv::front_temp_set)) conv::real_rear_speed++;
				else if(conv::real_rear_speed > func::afmap(conv::rear_temp_read,conv::rear_temp_set)) conv::real_rear_speed--;
			}
		}
	}
	analogWrite(_front_fan,conv::real_front_speed);
	analogWrite(_rear_fan,conv::real_rear_speed);
}

void func::mode_run()
{
	if(enRB.onPressed()) 
	{
		conv::mode = 3; // pushed auto mode	
	}
	// else if(BR == 2) conv::mode = 1; // long pushed fan only mode
}


void func::encoder_left()
{
	L = encoder_data(enL);
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
	if(L == -1) //CCW
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
	R = encoder_data(enR);
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
	if(R == -1) //CCW
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

