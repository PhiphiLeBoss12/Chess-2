#include "Network.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void initNetworkServer(IPaddress* ip, TCPsocket* tcpServer, TCPsocket* tcpClient) {
	// Server creation
	if (SDLNet_ResolveHost(ip, NULL, 6969) != 0) {
		printf("Failed to create server! NET error: %s\n", SDLNet_GetError());
		return;
	}
	*tcpServer = SDLNet_TCP_Open(ip);
	if (!(*tcpServer)) {
		printf("Failed to initialize socket! NET error: %s\n", SDLNet_GetError());
		return;
	}

	printf("Waiting for client...\n");
	*tcpClient = SDLNet_TCP_Accept(*tcpServer);
	while (!(*tcpClient)) {
		SDL_Delay(1000);
		*tcpClient = SDLNet_TCP_Accept(*tcpServer);
	}
	IPaddress* ipAdd = SDLNet_TCP_GetPeerAddress(*tcpClient);
	if (ipAdd)
		printf("Client connected! Client name: %s\n", SDLNet_ResolveIP(ipAdd));
}

void initNetworkClient(IPaddress* ip, TCPsocket* tcpServer) {
	const char distIp[128];
	FILE* netFile = fopen("Network", "r");
	fscanf(netFile, "%s", distIp);

	if (SDLNet_ResolveHost(ip, distIp, 6969) != 0) {
		printf("Failed to resolve host! NET error: %s\n", SDLNet_GetError());
		return;
	}

	// Connect to server
	*tcpServer = SDLNet_TCP_Open(ip);
	if (!(*tcpServer)) {
		printf("Failed to connect to server! NET error: %s\n", SDLNet_GetError());
		return;
	}

	IPaddress* ipAdd = SDLNet_TCP_GetPeerAddress(*tcpServer);
	if (ipAdd)
		printf("Server connected! Server name: %s\n", SDLNet_ResolveIP(ipAdd));
}

int recievePacket(TCPsocket* tcpServer, void* data, unsigned int size) {
	int result = SDLNet_TCP_Recv(*tcpServer, data, size);

	if (result <= 0)
		printf("Connection closed!\n");
}

void netExampleServer() {
	IPaddress ip;

	TCPsocket tcpServer, tcpClient;

	char buf[1024];
	unsigned int result;

	if (SDLNet_Init() != 0) {
		printf("Failed to initialize SDL_net! NET error: %s\n", SDLNet_GetError());
		return;
	}

	initNetworkServer(&ip, &tcpServer, &tcpClient);

	Packet packet;
	packet.Int = rand() % 10;
	packet.Float = 69.69f;
	packet.Sent = 1;
	result = SDLNet_TCP_Send(tcpClient, &packet, sizeof(Packet));

	SDL_Delay(5000);

	// Cleanup
	SDLNet_TCP_Close(tcpClient);
	SDLNet_TCP_Close(tcpServer);
	SDLNet_Quit();	
}

void netExampleClient() {
	IPaddress ip;

	TCPsocket tcpServer;

	char buf[1024];
	unsigned int result;

	if (SDLNet_Init() != 0) {
		printf("Failed to initialize SDL_net! NET error: %s\n", SDLNet_GetError());
		return;
	}

	initNetworkClient(&ip, &tcpServer, "localhost");

	Packet packet;
	packet.Sent = 0;
	while (!packet.Sent)
		recievePacket(&tcpServer, &packet, sizeof(Packet));
	printf("Got a packet! Int: %d, Float: %f, Sent: %d\n", packet.Int, packet.Float, packet.Sent);

	SDL_Delay(5000);

	// Cleanup
	SDLNet_TCP_Close(tcpServer);
	SDLNet_Quit();
}