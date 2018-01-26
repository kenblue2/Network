#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define _CRT_SECURE_NO_WARNINGS
#pragma comment (lib , "ws2_32.lib")

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
		printf("\rclient >> %s\n>> ",buf);
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

	SOCKET serv_sock;
	serv_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(serv_sock == SOCKET_ERROR)
	{
		printf("socket() Error\n");
		return 0;
	}

	SOCKADDR_IN serv_addr = {0};
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(4000);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	retval = bind(serv_sock,(SOCKADDR*)&serv_addr,sizeof(SOCKADDR));
	if(retval == SOCKET_ERROR)
	{
		printf("bind() Error\n");
		return 0;
	}

	listen(serv_sock,5);

	SOCKADDR_IN clnt_addr = {0};
	int size = sizeof(SOCKADDR_IN);
	SOCKET clnt_sock = accept(serv_sock,(SOCKADDR*)&clnt_addr,&size);
	if(clnt_sock == SOCKET_ERROR)
	{
		printf("accept() Error\n");
		return 0;
	}
	printf("클라이언트 접속\n");
	printf("IP : %s, Port : %d\n",inet_ntoa(clnt_addr.sin_addr),clnt_addr.sin_port);
	_beginthread(RecvThread,NULL,(void*)clnt_sock);

	while(1)
	{
		char buf[255] = {0};
		printf(">> ");
		gets_s(buf);
		int sendsize = send(clnt_sock,buf,strlen(buf),0);
		if(sendsize == 0)
			break;
	}

	closesocket(clnt_sock);
	closesocket(serv_sock);
	WSACleanup();
}