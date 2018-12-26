/*
The MIT License (MIT)
Copyright (c) 2018 Koichi Nishino
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include "stdio.h"
#include "gummo_json.h"
#include "MQTTSNPacket/MQTTSNPacket.h"
#include "transport.h"

int test_mqttsn(void)
{
	int rc = 0;
	int mysock;
	unsigned char buf[200];
	int buflen = sizeof(buf);
	MQTTSN_topicid topic;
	unsigned char* payload = (unsigned char*)"mypayload";
	int payloadlen = strlen((char*)payload);
	int len = 0;
	unsigned char dup = 0;
	int qos = 1;
	unsigned char retained = 0;
	short packetid = 1;
	char *topicname = "a long topic name";
	char *host = "127.0.0.1";
	int port = 1883;
	MQTTSNPacket_connectData options = MQTTSNPacket_connectData_initializer;
	unsigned short topicid;

	mysock = transport_open();
	if (mysock < 0)
		return mysock;


	printf("Sending to hostname %s port %d\n", host, port);

	options.clientID.cstring = "pub0sub1 MQTT-SN";
	len = MQTTSNSerialize_connect(buf, buflen, &options);
	rc = transport_sendPacketBuffer(host, port, buf, len);

	/* wait for connack */
	if (MQTTSNPacket_read(buf, buflen, transport_getdata) == MQTTSN_CONNACK)
	{
		int connack_rc = -1;

		if (MQTTSNDeserialize_connack(&connack_rc, buf, buflen) != 1 || connack_rc != 0)
		{
			printf("Unable to connect, return code %d\n", connack_rc);
			goto exit;
		}
		else
			printf("connected rc %d\n", connack_rc);
	}
	else
		goto exit;


	/* subscribe */
	printf("Subscribing\n");
	topic.type = MQTTSN_TOPIC_TYPE_NORMAL;
	topic.data.long_.name = topicname;
	topic.data.long_.len = strlen(topic.data.long_.name);
	len = MQTTSNSerialize_subscribe(buf, buflen, 0, 2, packetid, &topic);
	rc = transport_sendPacketBuffer(host, port, buf, len);

	if (MQTTSNPacket_read(buf, buflen, transport_getdata) == MQTTSN_SUBACK) 	/* wait for suback */
	{
		unsigned short submsgid;
		int granted_qos;
		unsigned char returncode;

		rc = MQTTSNDeserialize_suback(&granted_qos, &topicid, &submsgid, &returncode, buf, buflen);
		if (granted_qos != 2 || returncode != 0)
		{
			printf("granted qos != 2, %d return code %d\n", granted_qos, returncode);
			goto exit;
		}
		else
			printf("suback topic id %d\n", topicid);
	}
	else
		goto exit;

	printf("Publishing\n");
	/* publish with short name */
	topic.type = MQTTSN_TOPIC_TYPE_NORMAL;
	topic.data.id = topicid;
	++packetid;
	len = MQTTSNSerialize_publish(buf, buflen, dup, qos, retained, packetid,
		topic, payload, payloadlen);
	rc = transport_sendPacketBuffer(host, port, buf, len);

	/* wait for puback */
	if (MQTTSNPacket_read(buf, buflen, transport_getdata) == MQTTSN_PUBACK)
	{
		unsigned short packet_id, topic_id;
		unsigned char returncode;

		if (MQTTSNDeserialize_puback(&topic_id, &packet_id, &returncode, buf, buflen) != 1 || returncode != MQTTSN_RC_ACCEPTED)
			printf("Unable to publish, return code %d\n", returncode);
		else
			printf("puback received, msgid %d topic id %d\n", packet_id, topic_id);
	}
	else
		goto exit;

	printf("Receive publish\n");
	if (MQTTSNPacket_read(buf, buflen, transport_getdata) == MQTTSN_PUBLISH)
	{
		unsigned short packet_id;
		int qos, payloadlen;
		unsigned char* payload;
		unsigned char dup, retained;
		MQTTSN_topicid pubtopic;

		if (MQTTSNDeserialize_publish(&dup, &qos, &retained, &packet_id, &pubtopic,
			&payload, &payloadlen, buf, buflen) != 1)
			printf("Error deserializing publish\n");
		else
			printf("publish received, id %d qos %d\n", packet_id, qos);

		if (qos == 1)
		{
			len = MQTTSNSerialize_puback(buf, buflen, pubtopic.data.id, packet_id, MQTTSN_RC_ACCEPTED);
			rc = transport_sendPacketBuffer(host, port, buf, len);
			if (rc == 0)
				printf("puback sent\n");
		}
	}
	else
		goto exit;

	len = MQTTSNSerialize_disconnect(buf, buflen, 0);
	rc = transport_sendPacketBuffer(host, port, buf, len);

exit:
	transport_close();

	return 0;
}

int main()
{
	char json_data[512];
	char msg[] = "{\"a\" : 123 , \"data\" :[{ \"b\": [ 64 , 12 ], \"msg\" : \"foo bar\" },{ \"b\": [ 22 , 99 ], \"msg\" : \"ok\" }],  \"msg\" : \"hello world\"}";	
	test1_data tmp;

	json_deserialize(&tmp, &struct_types[TYPE_ID_TEST1_DATA], msg);
	printf("---deserialize---\n");
	printf("test1_data.a = %d\n", tmp.a);
	printf("test1_data.msg = %s\n", tmp.msg);
	printf("test1_data.data[0].b[0] = %d\n", tmp.data[0].b[0]);
	printf("test1_data.data[0].b[1] = %d\n", tmp.data[0].b[1]);
	printf("test1_data.data[0].msg = %s\n", tmp.data[0].msg);
	printf("test1_data.data[1].b[0] = %d\n", tmp.data[1].b[0]);
	printf("test1_data.data[1].b[1] = %d\n", tmp.data[1].b[1]);
	printf("test1_data.data[1].msg = %s\n", tmp.data[1].msg);

	json_serialize(&tmp, &struct_types[TYPE_ID_TEST1_DATA], json_data, 512);
	printf("---serialize---\n");
	printf("%s\n", json_data);

	test_mqttsn();
}
