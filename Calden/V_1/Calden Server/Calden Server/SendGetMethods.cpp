#include "Server.h"

bool Server::recvall(int ID, char * data, int totalbytes)
{
	int bytesreceived = 0; //Holds the total bytes received
	while (bytesreceived < totalbytes) //While we still have more bytes to recv
	{
		int RetnCheck = recv(Connections[ID], data, totalbytes - bytesreceived, NULL); 
		if (RetnCheck == SOCKET_ERROR) 
			return false;
		bytesreceived += RetnCheck; 
	}
	return true;
}

bool Server::sendall(int ID, char * data, int totalbytes)
{
	int bytessent = 0; 
	while (bytessent < totalbytes) 
	{
		int RetnCheck = send(Connections[ID], data + bytessent, totalbytes - bytessent, NULL);
		if (RetnCheck == SOCKET_ERROR) 
			return false; 
		bytessent += RetnCheck; 
	}
	return true; 
}

bool Server::SendInt(int ID, int _int)
{
	if (!sendall(ID, (char*)&_int, sizeof(int))) 
		return false; 
	return true; 
}

bool Server::GetInt(int ID, int & _int)
{
	if (!recvall(ID, (char*)&_int, sizeof(int))) //Try to receive int... If int fails to be recv'd
		return false; //Return false: Int not successfully received
	return true;//Return true if we were successful in retrieving the int
}

bool Server::SendPacketType(int ID, Packet _packettype)
{
	if (!sendall(ID, (char*)&_packettype, sizeof(Packet))) //Try to send packet type... If packet type fails to send
		return false; //Return false: packet type not successfully sent
	return true; //Return true: packet type successfully sent
}

bool Server::GetPacketType(int ID, Packet & _packettype)
{
	if (!recvall(ID, (char*)&_packettype, sizeof(Packet))) 
		return false; 
	return true;
}

bool Server::SendString(int ID, std::string & _string)
{
	if (!SendPacketType(ID, P_ChatMessage)) 
		return false; 
	int bufferlength = _string.size(); 
	if (!SendInt(ID, bufferlength)) 
		return false; 
	if (!sendall(ID, (char*)_string.c_str(), bufferlength)) 
		return false; 
	return true;
}

bool Server::GetString(int ID, std::string & _string)
{
	int bufferlength; //Holds length of the message
	if (!GetInt(ID, bufferlength)) //Get length of buffer and store it in variable: bufferlength
		return false; //If get int fails, return false
	char * buffer = new char[bufferlength + 1]; //Allocate buffer
	buffer[bufferlength] = '\0'; //Set last character of buffer to be a null terminator so we aren't printing memory that we shouldn't be looking at
	if (!recvall(ID, buffer, bufferlength)) //receive message and store the message in buffer array. If buffer fails to be received...
	{
		delete[] buffer; //delete buffer to prevent memory leak
		return false; //return false: Fails to receive string buffer
	}
	_string = buffer; //set string to received buffer message
	delete[] buffer; //Deallocate buffer memory (cleanup to prevent memory leak)
	return true;//Return true if we were successful in retrieving the string
}
