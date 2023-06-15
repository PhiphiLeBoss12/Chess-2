#pragma once
#include <SDL2/SDL_net.h>

void initNetworkServer(IPaddress* ip, TCPsocket* tcpServer, TCPsocket* tcpClient);
void initNetworkClient(IPaddress* ip, TCPsocket* tcpServer, const char* distIp);
void receivePacket(TCPsocket* tcpServer, void* data, unsigned int size, unsigned int* result);

void netExampleServer();
void netExampleClient();