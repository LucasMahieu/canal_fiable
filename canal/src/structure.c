/*
 * ======================================================================
 *         Author:  Lucas MAHIEU (), lucas.mahieu@grenoble-inp.org
 * ======================================================================
 */

#include "structure.h"
#include <arpa/inet.h>
#include <stdio.h>
#include "bug.h"

/**
 * Cette fonction permet de bufferiser/serialiser la structure 'packet'
 */
char *serialize_packet(char *buffer, Packet *p)
{ 
	buffer = serialize_uint32(buffer, p->numPacket);
	buffer = serialize_ack(buffer, p->ack);
	buffer = serialize_message(buffer, p->message);
	return buffer;
}
/**
 * Cette fonction permet remplir la structure 'packet' depuis un buffer
 */
Packet *deserialize_packet(char *buffer, Packet *p)
{ 
	buffer = deserialize_uint32(buffer, &(p->numPacket));
	buffer = deserialize_ack(buffer, &(p->ack));
	buffer = deserialize_message(buffer, p->message);
	return p;
}

char *serialize_uint32(char *buffer, uint32_t value)
{
	uint32_t value_big_endien = htonl(value);
	memcpy(buffer, &value_big_endien, sizeof(uint32_t)); 
	return buffer + sizeof(uint32_t);
}
char *deserialize_uint32(char *buffer, uint32_t *value)
{
	uint32_t value_host_endien;;
	memcpy(&value_host_endien, buffer, sizeof(uint32_t)); 
	*value = ntohl(value_host_endien);
	return buffer + sizeof(uint32_t);
}

char *serialize_ack(char *buffer, uint8_t ack)
{
	memcpy(buffer, &ack, sizeof(uint8_t));
	return buffer + sizeof(uint8_t);
}
char *deserialize_ack(char *buffer, uint8_t *ack)
{
	*ack = buffer[0];
	return buffer + sizeof(uint8_t);
}

char *serialize_message(char *buffer, char *message)
{
	strncpy(buffer, message, MAX_MES_LEN);
	return buffer + strnlen(message, MAX_MES_LEN);
}
char *deserialize_message(char *buffer,char *message)
{
	strncpy(message, buffer, MAX_MES_LEN);
	return buffer + strnlen(message, MAX_MES_LEN);
}

uint8_t send_pkt(Socket s, Packet *p, struct sockaddr *dest, socklen_t slen)
{
	char buf[MAX_PKT_LEN];
	char *ptr = NULL;
	ptr = serialize_packet(buf, p);
	int ret = sendto(s, buf, ptr - buf, 0, dest, slen);
	//fprintf(stderr, "SEND_PKT : ret:%d, ptr:%p - buf:%p = %lu\n" ,ret, ptr,buf,ptr-buf);
	if (ret == ptr - buf)
	   return 1;
	return 0;
}

uint8_t receive_pkt(Socket s, Packet *p, struct sockaddr *dest, socklen_t *slen)
{
	char toRec[MAX_PKT_LEN];
	if (recvfrom(s, toRec, MAX_PKT_LEN, 0, dest, slen) == -1) {
		bug("recvfrom()");
		return 0;
	} 
	if (deserialize_packet(toRec, p) == NULL) {
		bug("Error deserialise_pck");
		return 0;
	} 
	return 1;
}
