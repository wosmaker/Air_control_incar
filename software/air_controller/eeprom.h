#pragma once

#include <Arduino.h>
#include <EEPROM.h>

#include "control_value.h"

typedef struct 
{
	bool compressor;
	uint8_t fan_mode; // 0: split between front and rear, 1: same setup between front and rear
	uint8_t front_speed; // max 8
	uint8_t rear_speed; // max 8
	uint8_t mode; // mode:{0:cool,1:fan only, 2:off} 
	uint8_t blacklight; // min-max : 0-255
	float front_temp_set;
	float rear_temp_set;
}eeprom_value;

eeprom_value eev;

class eeprom
{
private:
	static int eeAddress;

public:
	static void put();
	static void get();
};

int eeprom::eeAddress = 0;

void eeprom::put()
{	
	 eev = {
		conv::compressor,
		conv::fan_mode,
		conv::front_speed,
		conv::rear_speed,
		conv::mode,
		conv::blacklight,
		conv::front_temp_set,
		conv::rear_temp_set
	};

	EEPROM.put(eeprom::eeAddress,eev);
}

void eeprom::get()
{
		EEPROM.get(eeprom::eeAddress,eev);

		conv::compressor = eev.compressor;
		conv::fan_mode = eev.fan_mode;
		conv::front_speed = eev.front_speed;
		conv::rear_speed = eev.rear_speed;
		conv::mode = eev.mode;
		conv::blacklight = eev.blacklight;
		conv::front_temp_set = eev.front_temp_set;
		conv::rear_temp_set = eev.rear_temp_set;
}
