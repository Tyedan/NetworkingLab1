#include <stdio.h>
#include "RakNet/RakPeerInterface.h"
#include <iostream>

#define MAX_CLIENTS 10
#define SERVER_PORT 60000

using namespace RakNet;

int main(void)
{
	char str[512];
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	bool isServer;
	char junk[512];
	unsigned int maxClients;
	unsigned int serverPort;

	printf("(C) or (S)erver?\n");
	fgets(str, 512, stdin);
	if ((str[0] == 'c') || (str[0] == 'C'))
	{
		printf("Server port number?\n");
		std::cin >> serverPort;
		fgets(junk, 512, stdin);
		SocketDescriptor sd;
		peer->Startup(1, &sd, 1);
		isServer = false;
	}
	else {
		printf("Server Port?\n");
		std::cin >> serverPort;
		fgets(junk, 512, stdin);
		printf("What is the maximum number of Clients?\n");
		std::cin >> maxClients;
		fgets(junk, 512, stdin);
		SocketDescriptor sd(SERVER_PORT, 0);
		peer->Startup(MAX_CLIENTS, &sd, 1);
		isServer = true;
	}


	// TODO - Add code body here

	RakNet::RakPeerInterface::DestroyInstance(peer);

	return 0;
}