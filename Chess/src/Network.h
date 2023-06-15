#pragma once
#include <SDL2/SDL_net.h>

// Temp
typedef struct Packet {
	int Int;
	float Float;
	int Sent;
} Packet;

void initNetworkServer(IPaddress* ip, TCPsocket* tcpServer, TCPsocket* tcpClient);
void initNetworkClient(IPaddress* ip, TCPsocket* tcpServer, const char* distIp);
void recievePacket(TCPsocket* tcpServer, void* data, unsigned int size);

void netExampleServer();
void netExampleClient();