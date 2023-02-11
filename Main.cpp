#include <SFML/Graphics.hpp>
#include <math.h>
#include <string>
#include <iostream>
#include "Body.h"
#include "Physics.h"

using namespace std;

int windowWidth = 1500;
int windowHeight = 1500;

bool gravityEnabled = true;
bool collisionEnabled = true;
bool borderEnabled = true;
bool showTrails = true;
//how far out the "border" extends beyond the visible screen:
int borderOffset = 500;
bool askForInput = false;

void updateBodies(Body bodies[], int totalBodies){
	if (gravityEnabled) {
		computeGravitationalForce(bodies, totalBodies);
	}

	if (collisionEnabled) {
		handleCollisions(bodies, totalBodies, borderEnabled, borderOffset, windowWidth, windowHeight);
	}
}

void inputHook(){
	cout << "C++ Gravity Simulator" << endl;

  string response;

	if(askForInput){
		cout << "Choose a window width (int): ";
		cin >> windowWidth;
		cout << "Choose a window height (int): ";
		cin >> windowHeight;

    cout << "Enable gravity? (y/n): ";
    cin >> response;
    if(response=="y"){
      gravityEnabled=true;
    }else{
      gravityEnabled=false;
    }

    cout << "Enable collision? (y/n): ";
    cin >> response;
    if(response=="y"){
      collisionEnabled=true;
    }else{
      collisionEnabled=false;
    }

    cout << "Enable border? (y/n): ";
    cin >> response;
    if(response=="y"){
      borderEnabled=true;

			cout << "Choose a border offset (int; 0 to have no offset): ";
			cin >> borderOffset;
    }else{
      borderEnabled=false;
    }

		cout << "Draw trails? (y/n): ";
    cin >> response;
    if(response=="y"){
      showTrails=true;
    }else{
      showTrails=false;
    }

	}
}

int main()
{
	inputHook();

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "C++ Gravity Simulator");
	window.setFramerateLimit(144);
	float fps;
	sf::Clock clock;
	sf::Time previousTime = clock.getElapsedTime();
	sf::Time currentTime;

	sf::Text FPS;
	sf::Font font;
  font.loadFromFile("Roboto-Regular.ttf");
	FPS.setFont(font);
	FPS.setCharacterSize(24);
	FPS.setFillColor(sf::Color::White);
	FPS.setStyle(sf::Text::Bold);
/*
	Body b1(10.0f, 1.f, sf::Vector2f(600.f, 10.f), sf::Vector2f(1.f, 1.f), false, sf::Color::Red);
	Body b2(150.0f, 0.5f, sf::Vector2f(750.f, 750.f), sf::Vector2f(0.f, 0.f), false, sf::Color::Blue);
	Body b3(5.0f, 1.0f, sf::Vector2f(850.f, 750.f), sf::Vector2f(0.f, -3.), false, sf::Color::Green);

	Body bodies[3] = {b1,b2,b3};
	*/
	Body b1(100.0f, 1.f, sf::Vector2f(500.f, 750.f), sf::Vector2f(0.f, 0.9f), false, sf::Color::Red);
	Body b2(100.0f, 1.f, sf::Vector2f(900.f, 750.f), sf::Vector2f(0.0f, -0.9f), false, sf::Color::White);

	Body bodies[2] = {b1,b2};


	int totalBodies = sizeof(bodies) / sizeof(bodies[0]);

	while (window.isOpen())
	{
		//default hook
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		updateBodies(bodies, totalBodies);

		window.clear();

		for (int i = 0; i < totalBodies; i++) {
			bodies[i].move();
			window.draw(bodies[i].getBody());
			if(showTrails){
			window.draw(bodies[i].getTrail());
			}
		}
		currentTime = clock.getElapsedTime();
		fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds());
		previousTime = currentTime;
		FPS.setString("FPS: "+to_string(static_cast<int>(fps)));
		window.draw(FPS);

		window.display();
	}

	return 0;
}
