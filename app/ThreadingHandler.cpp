#include "pch.h"
#include "Client.h"
#include "ThreadingHandler.h"



 void ThreadingHandler::write(Client& clientObject) {

	while (true) {
		clientObject.writeToServer();
		if (clientObject.checkLeaveReadiness()) {
			boost::this_thread::sleep(5);
			break;
		}
	}
}


 void ThreadingHandler::read(Client& clientObject) {
	while (true) {
		clientObject.readDataFromServer();
		if (clientObject.checkLeaveReadiness()) {
			std::cout << "Disconnecting from server.." << std::endl;
			boost::this_thread::sleep(5);
			break;
		}
	}
}


 void ThreadingHandler::runNewThread(std::function<void(Client&)> funct, Client& clientObj, bool& isRunning) {

	if (!isRunning) {
		boost::thread newThread{ boost::bind(funct, std::ref(clientObj)) };

		if (newThread.joinable()) {
			newThread.detach();
		}
		isRunning = true;
	}
}
