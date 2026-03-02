#include "Server.hpp"

void Server::capLs(Client &cli)
{
	std::vector<std::string> tokens = split(cli.getBuffer(), ' ');
	if (tokens.size() != 2 || tokens[0] != "CAP" || tokens[1] != "LS")
	{
		std::cerr << "Error: expected CAP LS" << std::endl;
		return ; //!
	}
	std::string msg = "CAP * LS";
	cli.sendMessageOnClientFd(msg);
}

void Server::cmdPass(Client &myClient)
{
	std::cout << myClient << std::endl;
	std::string passWord;
	std::size_t nameStart = (myClient.getBuffer()).find_first_of(' ');
	if (nameStart == myClient.getBuffer().size())
		return;
	passWord = (myClient.getBuffer()).substr(nameStart + 1);
	//?be more specifiques on the spaces rules for password
	std::cout << "passWord is'" << passWord << "'" << std::endl;
	if (passWord == _password)
	{
		myClient.setGrade(2);
		std::cout << myClient << std::endl;
	}
	else
		std::cout << "Error :wrong Password." << std::endl;
}


void Server::cmdTest(Client &myClient)
{
	std::cout << myClient << std::endl;
	std::string passWord;
	std::size_t nameStart = (myClient.getBuffer()).find_first_of(' ');
	if (nameStart == myClient.getBuffer().size())
		return;
	passWord = (myClient.getBuffer()).substr(nameStart + 1);
	//?be more specifiques on the spaces rules for password
	myClient.setGrade(2);
	myClient.setNickName(passWord);
}

void Server::cmdNick(Client &myClient)
{
	std::cout << myClient << std::endl;
	std::string nickname;
	std::size_t nameStart = (myClient.getBuffer()).find_first_of(' ');
	if (nameStart == myClient.getBuffer().size() || nameStart == std::string::npos)
		return;
	nickname = (myClient.getBuffer()).substr(nameStart + 1);
	if (isValidString(nickname))
	{
		if (findFdByNickName(nickname) == -1)
		{
			myClient.setNickName(nickname);
			std::cout << myClient << std::endl;
		}
		else
			std::cout << "Error : Nickame already used." << std::endl;
	}
	else
		std::cout << "Error :Not a valid Nickame." << std::endl;
}
void Server::cmdUser(Client &myClient)
{
	std::string realname;
	std::string cpy = myClient.getBuffer();
	if (cpy.find(":") == std::string::npos)
		std::cout << "Error :could not find ':' to start collect realname" << std::endl;
	realname = cpy.substr(cpy.find(":") + 1);
	cpy.erase(cpy.find(":") - 1);
	std::vector<std::string> tokens;
	tokens = split(cpy, " ");
	if (tokens.size() > 4 || tokens.size() < 2)
	{
		std::cout << "Error :Not a valid User entry." << std::endl;
		return;
	}
	else if (myClient.getAuthStatus() == 1 || 2)
	{
		myClient.setRealName(realname);
		myClient.setUserName(tokens[1]);
		std::cout << myClient.getUserName() << " is now grade 2." << std::endl;
		myClient.setGrade(2);
		std::cout << myClient << std::endl;
	}
	else
		std::cout << "Error :User Cannot get grade 2." << std::endl;
}