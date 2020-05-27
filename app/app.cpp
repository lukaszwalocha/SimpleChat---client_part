#include "pch.h"
#include "Client.h"
#include "ThreadingHandler.h"




int main() {

	boost::asio::io_context ioContext;
	std::shared_ptr<boost::asio::ip::tcp::socket> socket = std::make_shared<boost::asio::ip::tcp::socket>(ioContext);
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 1233);
	Client clientObject(endpoint, socket);
	
	clientObject.connectSocket();
	clientObject.setNickname();
	ioContext.run();

	bool writeThreadRunning = false;
	bool readThreadRunning = false;


	while (true) {
		try {
			
			if (!clientObject.isNicknameDisplayed) {
				clientObject.sendInfo(" has joined the room.");
			}
			
			ThreadingHandler::runNewThread(&ThreadingHandler::write, clientObject, writeThreadRunning);
			ThreadingHandler::runNewThread(&ThreadingHandler::read, clientObject, readThreadRunning);

			if (clientObject.checkLeaveReadiness()) {

				for (auto i = 5; i > 0; i--) {
					boost::this_thread::sleep(boost::posix_time::seconds(1));
					std::cout << "Exitting..." << i << std::endl;
				}
				break;
			}
		}
		catch (std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
	return 0;
}

