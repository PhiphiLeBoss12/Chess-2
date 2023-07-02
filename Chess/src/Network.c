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
	FILE* netFile = fopen("network.setting", "r");
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