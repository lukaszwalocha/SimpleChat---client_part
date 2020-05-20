#pragma once
#include <functional>

struct ThreadingHandler {

	static void write(Client& clientObject);
	static void read(Client& clientObject);
	static void runNewThread(std::function<void(Client&)> funct, Client& clientObj, bool& isRunning);

};
