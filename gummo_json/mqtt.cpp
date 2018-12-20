#include "pch.h"
#include <iostream>
#include "mqtt.h"


mqtt_packet mqtt_parse(char* buff)
{
	char* ctx;
	mqtt_packet packet;
	packet.dir = buff[0];
	packet.src = strtok_s(&buff[2], "/", &ctx);
	packet.dst = strtok_s(NULL, "/", &ctx);
	packet.command = strtok_s(NULL, "/", &ctx);
	packet.parameter = strtok_s(NULL, "/", &ctx);
	return packet;
}
