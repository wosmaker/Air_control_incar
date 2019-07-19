#pragma once

#include <Arduino.h>

#define off_fan_speed 0
#define min_fan_speed 0
#define max_fan_speed 12

#define min_pwm_speed 2
#define max_pwm_speed 255

#define min_def_temp 0
#define max_def_temp 13

#define min_temp_set 18
#define max_temp_set 32

#define c_temp 0.5 // change temperature
#define e_temp 0.3

#define enL 0
#define enR 1

#define between_time_fan 10 // 1000  = 1 sec
#define between_time_debug 3000
#define between_time_change 60000 // for save to eeprom
#define between_time_temp 3000

#define push_split 500

class conv
{
	public:
		static void reset();

		static bool compressor;
		static uint8_t fan_mode; //  dual: 0, only_front : 1 , only rear : 2
		static uint8_t front_speed; // max 8
		static uint8_t rear_speed; // max 8
		static uint8_t mode; // mode:{0:cool, 1:fan only, 2:off, 3:auto} 
		static uint8_t blacklight; // min-max : 0-255

		static float front_temp_set;
		static float rear_temp_set;

		static uint8_t real_front_speed;
		static uint8_t real_rear_speed;

		static float front_temp_read;
		static float rear_temp_read;

};

	bool conv::compressor = true;

	uint8_t conv::fan_mode = 0;
	uint8_t conv::front_speed = 0;
	uint8_t conv::rear_speed = 0;
	uint8_t conv::mode = 0;
	uint8_t conv::blacklight = 100;

	uint8_t conv::real_front_speed = 0;
	uint8_t conv::real_rear_speed = 0;

	float conv::front_temp_set = 25.00;
	float conv::rear_temp_set = 25.00;

	float conv::front_temp_read = 99.9;
	float conv::rear_temp_read = 99.9;

void conv::reset()
{
	conv::compressor = true;
  conv::fan_mode = 0;
	conv::front_speed = 0;
	conv::rear_speed = 0;
 	conv::mode = 0;
	conv::blacklight = 100;
 	conv::real_front_speed = 0;
	conv::real_rear_speed = 0;

	conv::front_temp_set = 25.00;
	conv::rear_temp_set = 25.00;

	conv::front_temp_read = 99.9;
	conv::rear_temp_read = 99.9;
}

