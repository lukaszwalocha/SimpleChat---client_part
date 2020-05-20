#pragma once
#include "pch.h"
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <Windows.h>
#include <string>
#include <random>

class Client {
public:
	Client();
	~Client() = default;
	Client(boost::asio::ip::tcp::endpoint endpoint, std::shared_ptr<boost::asio::ip::tcp::socket> socket)
		: _endpoint(endpoint), _socket(socket), isNicknameDisplayed(false), isReadyToLeave(false), startingColourNumber(15) {
		this->colourHandler = getRandomNumber();
	};

public:
	void writeToServer();
	void readDataFromServer();
	void sendInfo(std::string joinOrLeave);
	void connectSocket();
	void setNickname();
	void printMessage(std::string& msg);
	void printErr();
	bool checkLeaveReadiness();
	bool validateLeavingMsg(std::string& msg);
	std::string getNickname();
	int getRandomNumber();

public:
	bool isNicknameDisplayed;
	bool isReadyToLeave;
private:
	boost::asio::ip::tcp::endpoint _endpoint;
	std::shared_ptr<boost::asio::ip::tcp::socket> _socket;
	boost::system::error_code ec;
	boost::mutex blocker;
	std::string _nickname;
	int colourHandler;
	int startingColourNumber;
	HANDLE hConsole;
};