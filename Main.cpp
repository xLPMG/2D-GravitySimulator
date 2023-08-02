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
bool showTrails = true;
float sensitivity = 3.0f;
bool askForInput = false;
float sensAcc = 1.0f;

void updateBodies(Body bodies[], int totalBodies)
{
	if (gravityEnabled)
	{
		computeGravitationalForce(bodies, totalBodies);
	}

	if (collisionEnabled)
	{
		handleCollisions(bodies, totalBodies);
	}
}

void inputHook()
{
	cout << "C++ Gravity Simulator" << endl;

	string response;

	if (askForInput)
	{
		cout << "Choose a window width (int): ";
		cin >> windowWidth;
		cout << "Choose a window height (int): ";
		cin >> windowHeight;

		cout << "Enable gravity? (y/n): ";
		cin >> response;
		if (response == "y")
		{
			gravityEnabled = true;
		}
		else
		{
			gravityEnabled = false;
		}

		cout << "Enable collision? (y/n): ";
		cin >> response;
		if (response == "y")
		{
			collisionEnabled = true;
		}
		else
		{
			collisionEnabled = false;
		}

		cout << "Draw trails? (y/n): ";
		cin >> response;
		if (response == "y")
		{
			showTrails = true;
		}
		else
		{
			showTrails = false;
		}
	}
}

int main()
{
	inputHook();

	sf::RenderWindow window(sf::VideoMode(1500, 1500), "C++ Gravity Simulator");
	window.setFramerateLimit(144);
	float fps;
	sf::Clock clock;
	sf::Time previousTime = clock.getElapsedTime();
	sf::Time currentTime;

	sf::Text FPS;
	sf::Font font;
	font.loadFromFile("Roboto-Regular.ttf");
	FPS.setFont(font);
	FPS.setFillColor(sf::Color::White);
	FPS.setStyle(sf::Text::Bold);
	/*
		Body b1(10.0f, 1.f, sf::Vector2f(600.f, 10.f), sf::Vector2f(1.f, 1.f), false, sf::Color::Red);
		Body b2(150.0f, 0.5f, sf::Vector2f(750.f, 750.f), sf::Vector2f(0.f, 0.f), false, sf::Color::Blue);
		Body b3(5.0f, 1.0f, sf::Vector2f(850.f, 750.f), sf::Vector2f(0.f, -3.), false, sf::Color::Green);

		Body bodies[3] = {b1,b2,b3};
		*/

	Body b1(100.0f, 10.f, sf::Vector2f(500.f, 750.f), sf::Vector2f(0.f, 0.9f), true, sf::Color::Red);
	Body b2(100.0f, 10.f, sf::Vector2f(900.f, 750.f), sf::Vector2f(0.0f, -0.9f), true, sf::Color::White);
	Body b3(1000.0f, 100.f, sf::Vector2f(1100.f, 750.f), sf::Vector2f(-10.0f, -10.0f), false, sf::Color::Green);

	Body bodies[3] = {b1, b2, b3};

	int totalBodies = sizeof(bodies) / sizeof(bodies[0]);

	sf::View view(sf::Vector2f(750.f, 750.f), sf::Vector2f(1500.f, 1500.f));
	window.setView(view);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::Resized)
      			view.setSize(event.size.width,event.size.height);      
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			view.move(0.0f, -1.0f*sensitivity*sensAcc);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			view.move(0.0f, 1.0f*sensitivity*sensAcc);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			view.move(-1.0*sensitivity*sensAcc, 0.0);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			view.move(1.0*sensitivity*sensAcc, 0.0);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)){
			view.setSize(view.getSize().x+100,view.getSize().y+100);
			sensAcc+=0.1f;
		}else if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)){
			view.setSize(view.getSize().x-100,view.getSize().y-100);
			sensAcc-=0.1f;
			if(sensAcc<1.0f) sensAcc=1.0f;
			if(view.getSize().x<window.getSize().x || view.getSize().y<window.getSize().y) view.setSize(window.getSize().x, window.getSize().y);
		}

		window.setView(view);
		window.clear();

		updateBodies(bodies, totalBodies);
		for (int i = 0; i < totalBodies; i++)
		{
			bodies[i].move();
			window.draw(bodies[i].getBody());
			if (showTrails)
			{
				window.draw(bodies[i].getTrail());
				float x = bodies[i].getPos().x + (bodies[i].getForce().x * 10);
				float y = bodies[i].getPos().y + (bodies[i].getForce().y * 10);
				sf::Vertex line[] =
					{
						sf::Vertex(bodies[i].getPos(), sf::Color::Red),
						sf::Vertex(sf::Vector2f(x, y), sf::Color::Red)};
				window.draw(line, 2, sf::Lines);
			}
		}
		currentTime = clock.getElapsedTime();
		fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds());
		previousTime = currentTime;
		FPS.setCharacterSize(view.getSize().x/100+10);
		FPS.setPosition(view.getCenter().x-view.getSize().x/2,view.getCenter().y-view.getSize().y/2);
		FPS.setString("FPS: " + to_string(static_cast<int>(fps)));
		window.draw(FPS);
		window.display();
	}

	return 0;
}
