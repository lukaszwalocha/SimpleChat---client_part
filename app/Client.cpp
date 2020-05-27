#include "pch.h"
#include "Client.h"


Client::Client(boost::asio::ip::tcp::endpoint endpoint, std::shared_ptr<boost::asio::ip::tcp::socket> socket)
	: _endpoint(endpoint), _socket(socket), isNicknameDisplayed(false), isReadyToLeave(false), startingColourNumber(15)
{
	this->ioContext = std::make_shared<boost::asio::io_context>();

	this->colourHandler = getRandomNumber();
	this->hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	this->endpoint__2.address(boost::asio::ip::address::from_string("127.0.0.1"));
    this->endpoint__2.port ( 1234);

	this->endpoint__3.address ( boost::asio::ip::address::from_string("127.0.0.1"));
	this->endpoint__3.port ( 1235);
	
}

int Client::getRandomNumber() {

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(1, 15);

	int randomNumber = dist(mt);

	return randomNumber;
}

bool Client::validateLeavingMsg(std::string& msg) {

	const std::string leavingMessage = ": end\n";
	std::size_t isMsgFound = msg.find(leavingMessage);
	int leaveMsgLength = leavingMessage.length();
	int nicknameLength = this->_nickname.length();

	if (isMsgFound != std::string::npos && msg.length() == nicknameLength + leaveMsgLength) {
		return true;
	}

	return false;
}

bool Client::checkLeaveReadiness() {
	if (this->isReadyToLeave) {
		return true;
	}
	return false;
}

void Client::sendInfo(std::string joinOrLeave) {

	std::string startingInfo;
	startingInfo = "User " + this->_nickname + joinOrLeave;
	startingInfo += '\n';

	boost::asio::write(*this->_socket, boost::asio::buffer(startingInfo), this->ec);
	this->isNicknameDisplayed = true;
}

void Client::setNickname() {

	this->blocker.lock();
	std::cout << "Type your nickname: ";
	this->blocker.unlock();
	std::getline(std::cin, this->_nickname, '\n');
}

std::string Client::getNickname() {
	return this->_nickname;
}


std::string Client::chooseRoom() {

	std::cout << "Choose room: " << std::endl;
	std::cout << "1. Gamers" << std::endl;
	std::cout << "2. Anglers" << std::endl;
	std::getline(std::cin, roomChoice, '\n');

	if (roomChoice == "1" || roomChoice == "2") {
		return roomChoice;
	}
	else {
		std::cout << "There is no such an option. Please type again.";
		return chooseRoom();
	}

}

void Client::sendTokenMessage(std::string tokenMessage) {

	tokenMessage += '\n';

	boost::asio::write(*this->_socket, boost::asio::buffer(tokenMessage), this->ec);

	std::cout<<this->_socket->local_endpoint()<<std::endl;
}


void Client::connectSocket() {

	std::string choice = chooseRoom();

	this->_socket->connect(this->_endpoint); // connecting to the main room endpoint

	std::cout << "Connection with the main room established!" << std::endl;

	if (choice == "1") {
		
		sendTokenMessage("connect to Gamers");
		this->_socket.reset();
		this->_socket = std::make_shared<boost::asio::ip::tcp::socket>(*this->ioContext);
		this->_socket->connect(this->endpoint__2);
	}
	else if (choice == "2") {

		sendTokenMessage("connect to Anglers");
		this->_socket.reset();
		this->_socket = std::make_shared<boost::asio::ip::tcp::socket>(*this->ioContext);
		this->_socket->connect(this->endpoint__3);
	}
}

void Client::writeToServer() {

	std::string msg;
	std::string originalMsg;

	std::getline(std::cin, msg);
	originalMsg = msg;
	msg = this->_nickname + ": " + msg + '\n';

	if (validateLeavingMsg(msg)) {
		this->sendInfo(" has left the room.");
	}

	if (originalMsg != "end") {
		boost::asio::write(*this->_socket, boost::asio::buffer(msg), this->ec);
	}
	else {
		this->isReadyToLeave = true;
	}
}

void Client::printMessage(std::string& msg) {

	this->blocker.lock();

	SetConsoleTextAttribute(this->hConsole, this->colourHandler);
	std::cout << msg << std::endl;
	SetConsoleTextAttribute(this->hConsole, this->startingColourNumber);

	this->blocker.unlock();

}

void Client::printErr() {
	this->blocker.lock();
	std::cout << "Error occured: " << std::endl;
	std::cout << this->ec.message() << std::endl;
	this->blocker.unlock();
}


void Client::readDataFromServer() {

	try {
		boost::asio::streambuf buff;
		boost::asio::read_until(*this->_socket, buff, '\n');

		std::string receivedMessage = boost::asio::buffer_cast<const char*>(buff.data());
		printMessage(receivedMessage);
	}
	catch (std::exception& e) {
		std::cout << "Unable to read message from server." << std::endl;
	}
}