#include <SFML/Graphics.hpp>
#include <math.h>
#include <string>
#include <iostream>
#include "Body.h"
#include "Physics.h"

using namespace std;

int windowWidth = 1500;
int windowHeight = 1500;
bool showTrails = true;
float sensitivity = 3.0f;
float sensAcc = 1.0f;

void inputHook()
{
	cout << "C++ Gravity Simulator" << endl;

	string response;

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

int main()
{
	inputHook();
	Body b1(100.0f, 10.f, sf::Vector2f(500.f, 750.f), sf::Vector2f(0.f, 0.1f), false, sf::Color::Red);
	Body b2(100.0f, 10.f, sf::Vector2f(900.f, 750.f), sf::Vector2f(0.0f, -0.9f), false, sf::Color::White);
	Body bodies[2] = {b1, b2};
	int totalBodies = sizeof(bodies) / sizeof(bodies[0]);

	sf::RenderWindow window(sf::VideoMode({1500, 1500}), "C++ Gravity Simulator");
	window.setFramerateLimit(144);
	float fps;
	sf::Clock clock;
	sf::Time previousTime = clock.getElapsedTime();
	sf::Time currentTime;
	sf::Font font;
	font.openFromFile("Roboto-Regular.ttf");
	sf::Text FPS(font);
	FPS.setFont(font);
	FPS.setFillColor(sf::Color::White);
	FPS.setStyle(sf::Text::Bold);

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
				view.setSize(event.size.width, event.size.height);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			view.move(0.0f, -1.0f * sensitivity * sensAcc);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			view.move(0.0f, 1.0f * sensitivity * sensAcc);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			view.move(-1.0 * sensitivity * sensAcc, 0.0);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			view.move(1.0 * sensitivity * sensAcc, 0.0);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
		{
			view.setSize(view.getSize().x + 100, view.getSize().y + 100);
			sensAcc += 0.1f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
		{
			view.setSize(view.getSize().x - 100, view.getSize().y - 100);
			sensAcc -= 0.1f;
			if (sensAcc < 1.0f)
				sensAcc = 1.0f;
			if (view.getSize().x < window.getSize().x || view.getSize().y < window.getSize().y)
				view.setSize(window.getSize().x, window.getSize().y);
		}

		window.setView(view);
		window.clear();

		computeGravitationalForce(bodies, totalBodies);
		handleCollisions(bodies, totalBodies);
		for (int i = 0; i < totalBodies; i++)
		{
			bodies[i].move();
			window.draw(bodies[i].getBody());
			if (showTrails)
			{
				window.draw(bodies[i].getTrail());
			}
		}
		currentTime = clock.getElapsedTime();
		fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds());
		previousTime = currentTime;
		FPS.setCharacterSize(view.getSize().x / 100 + 10);
		FPS.setPosition(view.getCenter().x - view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2);
		FPS.setString("FPS: " + to_string(static_cast<int>(fps)));
		window.draw(FPS);
		window.display();
	}

	return 0;
}
