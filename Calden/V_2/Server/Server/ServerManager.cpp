#include "StdAfx.h"
#include "ServerManager.h"
#include "Server.h"
#include "ServerDlg.h"
#include <Windows.h>
#include<winsock2.h>
#include<WS2tcpip.h>
#include<fstream>
#include <iostream>

/*std::string getFileContents(std::ifstream& File)
{
	std::string Lines = "";        //All lines

	if (File)                      //Check if everything is good
	{
		while (File.good())
		{
			std::string TempLine;                  //Temp line
			std::getline(File, TempLine);        //Get temp line
			TempLine += "\0";                      //Add newline character

			Lines += TempLine;                     //Add newline
		}
		return Lines;
	}
	else                           //Return error
	{
		return "ERROR File does not exist.";
	}
}*/

static SOCKET sArray[100];
static int iCount;
ServerManager::ServerManager(CServerDlg* dialog)
{
	m_pDialog = dialog;
}


ServerManager::~ServerManager()
{
	
	closesocket(s);
    WSACleanup();
}

void ServerManager::ClearServer()
{
	closesocket(s);
    WSACleanup();
	/*
	for(int i=1;i<=iCount;++i)
	{
		DWORD dwCode;  
        GetExitCodeThread(cpTh[i]->m_hThread, &dwCode);  
        delete cpTh[i];
		//CloseHandle(m_Thread_handle[i]);
	}*/
}

void ServerManager::StartListening(int iPort)
{
	iCount=0;
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
		m_pDialog->ShowServerInfo("Could not create socket");
    }
 
    printf("Socket created.\n");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( iPort );
     
    //Bind
    if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d" , WSAGetLastError());
		m_pDialog->ShowServerInfo("Bind failed with error code");
        exit(EXIT_FAILURE);
    }
     
    puts("Bind done");
 
    //Listen to incoming connections
    listen(s , 100);
	 char *message;
	 puts("Waiting for incoming connections...");
     m_pDialog->ShowServerInfo("Waiting for incoming connections...\n");
     c = sizeof(struct sockaddr_in);
     
    while( (new_socket = accept(s , (struct sockaddr *)&client, &c)) != INVALID_SOCKET )
    {
        puts("Connection accepted");
       // m_pDialog->ShowServerInfo("Connection accepted\n");
        //Reply to the client
		socklen_t len;
		struct sockaddr_storage addr;
		char ipstr[INET6_ADDRSTRLEN];
		int port;

		len = sizeof addr;
		getpeername(new_socket, (struct sockaddr*)&addr, &len);

		if (addr.ss_family == AF_INET) {
			struct sockaddr_in *s = (struct sockaddr_in *)&addr;
			port = ntohs(s->sin_port);
			inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
		}
		printf("Peer IP address: %s\n", ipstr);
		m_pDialog->ShowServerInfo("Connected Peer IP address: "+string(ipstr)+"\n");
		CWinThread *cTh = AfxBeginThread(
		DataThreadFunc,
		(LPVOID)new_socket);
		++iCount;
		//m_Thread_handle[++iCount] = cTh->m_hThread;
		//cpTh[iCount] = cTh;
		sArray[iCount] = new_socket;
		//message = "Hello Client , I have received your connection.\n";
        //send(new_socket , message , strlen(message) , 0);
		//SetStaticVariable(iTempCount, new_socket);
    }
     
    if (new_socket == INVALID_SOCKET)
    {
        printf("accept failed with error code : %d" , WSAGetLastError());
        return;
    }
}

UINT __cdecl ServerManager::DataThreadFunc(LPVOID pParam)
{
	SOCKET pYourSocket = reinterpret_cast<SOCKET>(pParam);
    //UINT retCode = pYourClass->ThreadFunc();
	//SendReceiveData(pYourClass);
	//std::string TempLine;
	//std::string all;
	//std::string Lines = "";
	//std::ifstream Reader("MOTD.txt");
	char const *message;
	//while (Reader.good()){
	//	std::string TempLine;                  //Temp line
	//	std::getline(Reader, TempLine);        //Get temp line
	//	TempLine += "\0";                      //Add newline character
	//	Lines += TempLine;
	//	std::cout << Lines << " : " << TempLine << std::endl;
	//}
	//message = Lines.c_str();
	message = "Welcome to the Calden server. \n";//handshake reply
	send(pYourSocket, message, strlen(message), 0); //Sending handshake
	//message = TempLine.c_str();
	//send(pYourSocket, message, strlen(message), 0);

    //send(pYourSocket , message , strlen(message) , 0);
	char server_reply[2000]; // Max buffer for server reply
    int recv_size;

	while((recv_size = recv(pYourSocket , server_reply , 2000 , 0)) != SOCKET_ERROR) //While loop the will send back info to
																					 //the other users
	{
		server_reply[recv_size] = '\0'; //Can't be to safe with new lines
		cout <<"Message Received: " << string(server_reply);
		fstream log("log.bin", ios::out | ios::app | ios::binary);
		if (log.is_open()) {
			std::time_t result = std::time(nullptr);
			std::string test = " " + string(server_reply);
			log << std::asctime(std::localtime(&result)) + test;
		}
		log.close();

		for(int i = 1;i<=iCount; i++)
		{
			if( send(sArray[i] , server_reply, recv_size , 0) < 0)
			{
				puts("Send failed");
				//return -1;
			}
		}

	}
    return 0;
}

UINT ServerManager::SendReceiveData(SOCKET cSocket)
{

	return 0;
}

void ServerManager::SetStaticVariable(int iC, SOCKET cS)
{
	iCount = iC;
	sArray[iCount] = cS;
}