#pragma once


typedef struct
{
	char dir;			// direction
	char* src;			// name pointer
	char* dst;			// group or name pointer
	char* command;		// command pointer
	char* parameter;	// parameter pointer[JSON]
} mqtt_packet;


extern mqtt_packet mqtt_parse(char* buff);
