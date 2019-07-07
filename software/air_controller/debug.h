#pragma once

#include <Arduino.h>

#include "air_controller.h"

class debug
{
private:

public:
	static void begin();
	static void conv();

};

uint16_t debug_count = 0;
uint64_t time_debug = 0;

void debug_conv()
{
	debug_count++;
	Serial.print("DEBUG :: control value count << ");Serial.println(debug_count);
	Serial.print("compressor 			:: ");Serial.println(conv::compressor);
	Serial.print("fan_mode 				:: ");Serial.println(conv::fan_mode);
	Serial.print("front_speed 		:: ");Serial.println(conv::front_speed);
	Serial.print("rear_speed  		:: ");Serial.println(conv::rear_speed);
	Serial.print("mode 						:: ");Serial.println(conv::mode);
	Serial.print("blacklight 			:: ");Serial.println(conv::blacklight);
	Serial.print("front_temp_set 	:: ");Serial.println(conv::front_temp_set);
	Serial.print("rear_temp_set 	:: ");Serial.println(conv::rear_temp_set);
	Serial.print("front_temp_read :: ");Serial.println(conv::front_temp_read);
	Serial.print("rear_temp_read 	:: ");Serial.println(conv::rear_temp_read);
	Serial.println("DEBUG :: end control value");
	Serial.println();
	Serial.println();

}

void debug::begin()																								
{
	Serial.begin(9600);
}

void debug::conv()
{
	if(millis() - time_debug > between_time_debug)
	{
		time_debug = millis();
		debug_conv();
	}                                                                                                                                                                                                                                                                                                                                                               
}

