#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment (lib, "ws2_32.lib")
#include <stdio.h>
#include <WinSock2.h>
#include <process.h>


void __cdecl RecvThread (void * p)
{
	SOCKET sock = (SOCKET)p;
	char buf[256];
	while(1)
	{
		int recvsize = recv(sock,buf,sizeof(buf),0);
		if(recvsize == 0)
		{
			printf("접속종료\n");
			break;
		}
		buf[recvsize] = '\0';
		printf("\rserver >> %s\n>> ",buf);
	}
}


int main()
{
	WSADATA wsaData;
	int retval = WSAStartup(MAKEWORD(2,2),&wsaData);
	if(retval != 0)
	{
		printf("WSAStartup() Error\n");
		return 0;
	}

	SOCKET serv_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

	SOCKADDR_IN serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(4000);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	retval = connect(serv_sock,(SOCKADDR*)&serv_addr,sizeof(SOCKADDR));
	if(retval == SOCKET_ERROR)
	{
		printf("connect() Error\n");
		return 0;
	}
	_beginthread(RecvThread,NULL,(void*)serv_sock);
	while(1)
	{
		char buf[255] = {0};
		printf(">> ");
		gets_s(buf);
		int sendsize = send(serv_sock,buf,strlen(buf),0);
		if(sendsize == 0)
			break;
	}

	closesocket(serv_sock);
	WSACleanup();
}