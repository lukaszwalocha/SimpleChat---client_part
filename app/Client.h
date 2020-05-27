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
	Client() = default;
	~Client() = default;
	Client(boost::asio::ip::tcp::endpoint endpoint, std::shared_ptr<boost::asio::ip::tcp::socket> socket);
	
public:
	void writeToServer();
	void readDataFromServer();
	void sendInfo(std::string joinOrLeave);
	void connectSocket();
	void setNickname();
	void printMessage(std::string& msg);
	void printErr();
	void sendTokenMessage(std::string tokenMsg);
	bool checkLeaveReadiness();
	bool validateLeavingMsg(std::string& msg);
	std::string getNickname();
	int getRandomNumber();
	std::string chooseRoom();

public:
	bool isNicknameDisplayed;
	bool isReadyToLeave;

private:
	boost::asio::ip::tcp::endpoint _endpoint;
	boost::asio::ip::tcp::endpoint endpoint__2;
	boost::asio::ip::tcp::endpoint endpoint__3;

	std::shared_ptr<boost::asio::ip::tcp::socket> _socket;

	std::shared_ptr<boost::asio::io_context> ioContext;

	boost::system::error_code ec;
	boost::mutex blocker;
	std::string _nickname;
	std::string roomChoice;
	int colourHandler;
	int startingColourNumber;
	HANDLE hConsole;
};