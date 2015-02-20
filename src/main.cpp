#include <iostream>

#include "config.h"
#include "Game.h"

using namespace std;


int main(int argc, char* argv[]) {

	/* Display the app version provided by cmake */
	cout << "Version " << VERSION_MAJOR << "." << VERSION_MINOR << endl;

	Game game;
	game.run();

}
