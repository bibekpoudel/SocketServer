//Bibek Poudel
//Just serves one client

#include <iostream>
#include <WS2tcpip.h>

#define PORT 64000
#pragma warning(disable:4996)
#define SIZE 1024



void write_file(SOCKET liSocket) {
	int n;
	FILE* fp;
	char filename[] = "recv.txt";
	char buffer[SIZE];

	fp = fopen(filename, "w");
	while (1) {
		n = recv(liSocket, buffer, SIZE, 0);
		if (n <= 0) {
			break;
			return;
		}
		fprintf(fp, "%s", buffer);
		ZeroMemory(buffer, SIZE);
	}
	return;
}

int main()
{

	int numTemp; //temporary int value for error handling

	//initialize winsock

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout << (stderr, "WSAStartup failed.\n");
		exit(EXIT_FAILURE);
	}

	//create socket
	SOCKET liSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (liSocket == INVALID_SOCKET)
	{
		std::cout << "Socket Creation Failed\n";
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "Socket Created\n";
	}

	//bind ip address and port

	//initializing env for sockaddr
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(PORT);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; //inet_addr("10.12.110.57");

	memset(&(hint.sin_zero), 0, 8);


	numTemp= bind(liSocket, (sockaddr*)&hint, sizeof(hint)); //need to typecast?


	if (numTemp < 0)
	{
		std::cout << "\nFailed to bind to local port\n";
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "\nBound to local port\n";
	}

	//instruct winsock that it is a listening socket
	//listen to queues of request from the client
	numTemp = listen(liSocket, 5); //5 is a queue for backlog. max: SOMAXCONN
	if (numTemp < 0)
	{
		std::cout << "\nFailed to listen to local port\n";
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "\nListening to local port\n";
	}

	//wait for connection
	sockaddr_in clientAddr;
	int clientAddrSize = sizeof(clientAddr);
	
	SOCKET clientSocket = accept(liSocket, (sockaddr*)&clientAddr, &clientAddrSize);
	if (clientSocket == INVALID_SOCKET)
	{
		std::cout << "No Connection\n";
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "Connection Established\n";
	}

	//client's info
	char host[NI_MAXHOST]; //client's remote name
	char service[NI_MAXSERV]; //service or PORT

	ZeroMemory(host, NI_MAXHOST); //can use memset too  / memset(host, 0, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&clientAddr, sizeof(clientAddr), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		std::cout << "Connected with " << host << " at port: " << service << std::endl;
	}
	else
	{
		inet_ntop(AF_INET, &clientAddr.sin_addr, host, NI_MAXHOST);
		std::cout << "Connected with " << host << " at port: " << ntohs(clientAddr.sin_port) << std::endl;
	}
	//close listening socket

	closesocket(liSocket);

	//while loop/ message processing
	//char buff[4096];
	//while (true)
	//{
	//	ZeroMemory(buff, sizeof buff);
	//	int receivedBytes = recv(clientSocket, buff, sizeof buff, 0);
	//	if (receivedBytes < 0)
	//	{
	//		std::cout << "Error receiving\n";
	//		exit(EXIT_FAILURE);
	//	}
	//	else if (receivedBytes == 0)
	//	{
	//		std::cout << "Client Disconnected\n";
	//		break;
	//	}

	//	//echo message back to the client
	//	send(clientSocket, buff, receivedBytes + 1, 0);
	//	std::cout << buff << std::endl;

	//}
	
	write_file(clientSocket);
	printf("[+]Data written in the file successfully.\n");
	
	//close sock
	closesocket(clientSocket);
	
	//clean winsock
	WSACleanup();
}