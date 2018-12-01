#ifndef __linux__
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#else

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif
#include <allegro5/allegro.h>

#define SERVER "172.16.104.163"		//ip address of udp server
#define BUFLEN	1500	// Buffer length in bytes
#define PORT	21236
#define DATA_SIZE 7
#define DATA_LENGHT (BUFLEN/DATA_SIZE)

typedef struct {
	char type;
	int x;
	int y;
	char img_i;
	char dir;
}Data;

typedef struct {
	unsigned char gameState;
	unsigned char score;
	unsigned char life;
	unsigned char dna;
}GameVar;

void server_initialise(void);

void safe_send(char seqnum, char* msg, int msg_len);
void safe_receive(char seqnum, char* msg, int msg_len);
void ack(bool ack);

void r_receive(bool keys[]);
void r_send(bool keys[]);
void d_receive(Data *,GameVar *);
void d_send(Data *,GameVar);
void set_server(void);
void set_client(char ip[]);
void data_serialize(Data *, unsigned char *, GameVar);
void data_deserialize(Data *, unsigned char *, GameVar *);
