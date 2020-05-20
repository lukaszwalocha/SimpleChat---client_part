#include "pch.h"
#include "Client.h"


Client::Client() {
	this->hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
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

void Client::connectSocket() {
	this->_socket->connect(this->_endpoint);
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