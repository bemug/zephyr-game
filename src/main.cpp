#include <iostream>
#include <SFML/Graphics.hpp>
#include "config.h"

using namespace std;


int main(int argc, char* argv[]) {

	/* Display the app version provided by cmake */
	cout << "Version " << VERSION_MAJOR << "." << VERSION_MINOR << endl;

	/* Initializing the game */

	sf::RenderWindow app(sf::VideoMode(800, 600), "Zephyr");
	app.setFramerateLimit(60);

	while (app.isOpen()) {

		/* Process events */
		sf::Event event;
		while (app.pollEvent(event)) {
			switch (event.type) {
				/* Window close */
				case sf::Event::Closed:
					app.close();
					break;
				default:
					break;	
			}		
		}

		/* Compute */

		/* Render */
		app.clear();
		app.display();
	}
}		
