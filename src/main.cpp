#include <iostream>

#include "config.h"
#include "Game.hpp"

using namespace std;


int main(int argc, char* argv[]) {

	/* Display the app version provided by cmake */
	cout << "Version " << VERSION_MAJOR << "." << VERSION_MINOR << endl;
	try
	{
		Game game;
		game.run();
	}
	catch (std::exception& e)
	{
		std::cout << "\nEXCEPTION: " << e.what() << std::endl;
	}

}
