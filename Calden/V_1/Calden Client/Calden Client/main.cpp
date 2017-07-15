#include "Client.h"
void savePlayerStats(std::string name, int str, int def, int armor, int skill, int wins) {

	std::fstream file;
	std::string filename = name + ".txt";

	file.open(filename, std::ios::out);


	file << str << std::endl;
	file << def << std::endl;
	file << armor << std::endl;
	file << skill << std::endl;
	file << wins << std::endl;

	file.close();

}
void setupPlayerStats(std::string &name) {
	std::random_device rd;
	std::uniform_int_distribution<int> d6(1, 6);
	int str;
	int def;
	int armor;
	int skill;
	int wins;
	std::cout << "Setup Player" << std::endl;

	// TODO: Load the character's file or create a new character.
	std::fstream file;

	std::cout << "Name: ";
	getline(std::cin, name);

	// If they enter no name, use a default
	if (name == "") {
		name = "John Doe";
	}
	std::string filename = name + ".txt";

	file.open(filename, std::ios::in);
	if (file.is_open()) {
		// File exists, read data from file
		std::cout << "File found, loading from file." << std::endl;

		file >> str;
		file >> def;
		file >> armor;
		file >> skill;
		file >> wins;
	}
	else {
		bool accepted = false;
		while (!accepted) {
			int points = 20;

			std::cout << points << " points left." << std::endl;
			std::cout << "Player Str: ";
			std::cin >> str;
			points -= str;

			std::cout << points << " points left." << std::endl;
			std::cout << "Player Def: ";
			std::cin >> def;
			points -= def;

			std::cout << points << " points left." << std::endl;
			std::cout << "Player Armor: ";
			std::cin >> armor;
			points -= armor;

			std::cout << points << " points left." << std::endl;
			std::cout << "Player Skill: ";
			std::cin >> skill;
			points -= skill;

			wins = 0;

			if (points >= 0) {
				accepted = true;
			}
			else {
				std::cout << "You used too many points!" << std::endl;
			}

		}
	}
	savePlayerStats(name, str, def, armor, skill, '0');
}
int main()
{
	std::string name;
	//setupPlayerStats(name); When needed add
	std::cout << "Enter a username" << std::endl;
	std::cin >> name;
	system("cls");
	std::string ip;
	int port;
	std::cout << "enter and ip: ";
	std::cin >> ip;
	std::cout << "enter the port: ";
	std::cin >> port;
	std::cout << "Connection to " << ip << " : " << port << std::endl;

	Client myClient(ip, port); //Create client using ip and port from the user
	system("cls");
	if (!myClient.Connect()) //If client fails to connect...
	{
		std::cout << "Failed to connect to server..." << std::endl;
		system("pause");
		return 1;
	}
	//add me to the send
	std::string userinput;
	//std::string userinput1; 
	while (true)
	{
		std::getline(std::cin, userinput); //Get line if user presses enter and fill the buffer
		if (userinput[0] == '!') {
			userinput.erase(0,1);
			if (userinput == "help") { //\n pause: Request a pause.
				std::cout << RED << "Commands are as follows \n clear: Clears the screen \n exit: Exits the game" << BOLDWHITE << std::endl;
			}else if (userinput == "clear") {
				std::cout << CLEAR;
			}
			else if (userinput == "exit") {
				return 0;
			}
			else if (userinput == "pause") {

			}
			else {
				std::cout << RED << "No command found try !help for the commands" << BOLDWHITE << std::endl;
			}
			userinput.clear();
		}
		else {
			if (userinput == "") {
				userinput = "Has Joined the Chat";
				if (!myClient.SendString(userinput, name));
			}
			else {
				if (!myClient.SendString(userinput, name)) //Send string: userinput, If string fails to send... (Connection issue)
					break; //If send string failed (if connection was closed), leave this loop since we have lost connection to the server
				std::cout << RESET << "Message sent" << BOLDWHITE << std::endl;
				Sleep(10);
			}
		}
	}
	system("pause");
	return 0;
}