#pragma comment(lib,"ws2_32.lib") //Winsock Library
#include "StdAfx.h"
#include "ClientCon.h"
#include<stdio.h>
#include <string.h>
#include "ClientDlg.h"


ClientCon::ClientCon(CClientDlg *dlg)
{
	m_pClient = dlg;
}


ClientCon::~ClientCon(void)
{
	closesocket(s);
	WSACleanup();
}

void ClientCon::StartConnect(string sAddress, int iPort, string sUsername)
{
    struct sockaddr_in server;
    char *message , server_reply[2000];
    int recv_size;
	m_pUser = sUsername;

    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return;
    }
     
    printf("Initialised.\n");
     
    //Create a socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
 
    printf("Socket created.\n");
     
     
	server.sin_addr.s_addr = inet_addr(sAddress.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons( iPort );
 
    //Connect to remote server
    if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error");
        return;
    }
     
    puts("Connected");
     
    //Send some data
    //message = "GET / HTTP/1.1\r\n\r\n";
   /*
    puts("Data Send\n");
     */
    //Receive a reply from the server
    while((recv_size = recv(s , server_reply , 2000 , 0)) != SOCKET_ERROR)
    { 
		puts("Reply received\n");
 
		//Add a NULL terminating character to make it a proper string before printing
		server_reply[recv_size] = '\0';
		puts(server_reply);
		std::string sTempMsg =string(server_reply);
		m_pClient->ShowServerInfo(sTempMsg);
    }
    
}

void ClientCon::SendData(string sMessage)
{
	string sTemp = m_pUser +">>"+ sMessage + "\n";

	if( send(s , sTemp.c_str(), sTemp.size() , 0) < 0)
    {
        puts("Send failed");
        return;
    }
}