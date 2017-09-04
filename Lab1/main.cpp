/*We certify that this work is entirely our own.The assessor of this project may reproduce this project
and provide copies to other academic staff, and/or communicate a copy of
his project to a plagiarism - checking service, which may retain a copy of the project on its database.”
Jon Trusheim*/
#include <stdio.h>
#include <string.h>
#include "RakNet/RakPeerInterface.h"
#include "RakNet/MessageIdentifiers.h"
#include <iostream>
#include "RakNet/BitStream.h"
#include "RakNet/RakNetTypes.h"  

using namespace RakNet;

enum GameMessages
{
	ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1,

};

//made my own struct to replace the bit streams
#pragma pack(push, 1)
struct myMessage
{
	unsigned char typeId;
	char* message;
};
#pragma pack(pop)
int main(void)
{
	//added variables 
	void* mainWindow = GetForegroundWindow();
	char str[512];
	char junk[512];
	unsigned int maxClients;
	unsigned int serverPort;
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	bool isServer;
	RakNet::Packet *packet;

	printf("(C) or (S)erver?\n");
	fgets(str, 512, stdin);

	if ((str[0] == 'c') || (str[0] == 'C'))
	{
		//made it so the server port took user input
		printf("Server port number?\n");
		std::cin >> serverPort;
		fgets(junk, 512, stdin);
		RakNet::SocketDescriptor sd;
		peer->Startup(1, &sd, 1);
		isServer = false;
	}
	else {
		//made it so the server port took user input
		printf("Server port number?\n");
		std::cin >> serverPort;
		fgets(junk, 512, stdin);
		//made it so the number of maximum clients took user input
		printf("What is the maximum number of Clients?\n");
		std::cin >> maxClients;
		fgets(junk, 512, stdin);
		RakNet::SocketDescriptor sd(serverPort, 0);
		peer->Startup(maxClients, &sd, 1);
		isServer = true;
	}


	if (isServer)
	{
		printf("Starting the server.\n");
		// We need to let the server accept incoming connections from the clients
		peer->SetMaximumIncomingConnections(maxClients);
	}
	else {
		printf("Enter server IP or hit enter for 127.0.0.1\n");
		fgets(str, 512, stdin);
		if (str[0] == '\n') {
			strcpy(str, "127.0.0.1");
		}
		printf("Starting the client.\n");
		peer->Connect(str, serverPort, 0, 0);
	}

	while (1)
	{
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			switch (packet->data[0])
			{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("Another client has disconnected.\n");
				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf("Another client has lost the connection.\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("Another client has connected.\n");
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				printf("Our connection request has been accepted.\n");
			//changed from bit streams to structs
				myMessage structOut;
				structOut.message = "Welcome user to Skynet";
				structOut.typeId = (unsigned char)ID_GAME_MESSAGE_1;
				peer->Send((char*)(&structOut), sizeof(structOut), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
			}
			case ID_NEW_INCOMING_CONNECTION:
				printf("A connection is incoming.\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				if (isServer) {
					printf("May the Force be with you.\n");
				}
				else {
					printf("May the Force be with you.\n");
				}
				break;
				break;
			case ID_CONNECTION_LOST:
				if (isServer) {
					printf("May the Force be with you.\n not good");
				}
				else {
					printf("May the Force be with you.\n not good");
				}
				break;
			case ID_GAME_MESSAGE_1:
			{
				//changed the message resieved to take my struct instead of bit streams
				myMessage *message = (myMessage*)(packet->data);
				printf("%s\n", message->message);
			}
			break;
			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}
		}

		//added an if statement to leave the loop
		if (GetAsyncKeyState(VK_ESCAPE) && mainWindow == GetForegroundWindow())
		{
			// exit game loop
			break;
		}
	}


	RakNet::RakPeerInterface::DestroyInstance(peer);

	return 0;
}
