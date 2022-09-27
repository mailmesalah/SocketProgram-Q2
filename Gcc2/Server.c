#include<stdio.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define MY_PORT		8888
#define MAXBUF		256

int main(int argc, char *argv[])
{
	WSADATA wsa;
	SOCKET s, new_socket;
	struct sockaddr_in server, client;
	int c, incomingMsgLen,i;
	char message[MAXBUF],reverseMsg[MAXBUF];

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("Initialised.\n");

	//Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
		WSACleanup();
		exit(errno);
	}

	printf("Socket created.\n");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(MY_PORT);

	//Bind
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		WSACleanup();
		exit(errno);
	}

	puts("Bind done");


	//Listen to incoming connections
	listen(s, 3);

	//Accept and incoming connection
	puts("Waiting for incoming connections...");

	c = sizeof(struct sockaddr_in);
	while ((new_socket = accept(s, (struct sockaddr *)&client, &c)) != INVALID_SOCKET)
	{
		puts("Connection accepted");

		//Reply to the client
		//message = "Hello Client , I have received your connection. But I have to go now, bye\n";
		//strlen(message)
		incomingMsgLen = recv(new_socket, message, MAXBUF, 0);
		message[incomingMsgLen] = '\0';
		
		for (i = 0; i<incomingMsgLen; i++)
		{
			if (isalpha(message[i])) 
			{
				if (IsCharLower(message[i])) {
					message[i] = _toupper(message[i]);
				}
			}
		}

		send(new_socket, _strrev(message), incomingMsgLen, 0);
	}

	if (new_socket == INVALID_SOCKET)
	{
		printf("accept failed with error code : %d", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	closesocket(s);
	WSACleanup();

	return 0;
}
