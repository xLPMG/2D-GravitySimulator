#include <SFML/Graphics.hpp>
#include <math.h>
#include <string>
#include <iostream>
#include "Body.h"

using namespace std;

const float G = 6.6743f; //6.6743 × 10^-11 m^3 kg^-1 s^-2

int windowWidth = 1500;
int windowHeight = 1500;

bool gravityEnabled = true;
bool collisionEnabled = true;
bool borderEnabled = true;
bool showTrails = true;
bool askForInput = false;

//how far out the "border" extends beyond the visible screen:
int borderOffset = 1000;

void computeGravitationalForce(Body bodies[], int totalBodies){
	//compute for each body i the sum of all forces from each object j on object i
	for (int i = 0; i < totalBodies; i++) {
		sf::Vector2f force = sf::Vector2f(0.0f, 0.0f);
		for (int j = 0; j < totalBodies; j++) {
			if (i != j) {
			//distance: sqrt( (j.x – i.x)^2 + (j.y – i.y)^2 )
			float distance = sqrt(pow((bodies[i].getPos().x - bodies[j].getPos().x), 2) + pow((bodies[i].getPos().y - bodies[j].getPos().y), 2));

			/*
			- gravitational force: ( ( G * m1 * m2 ) / r^3 ) * r->
			- r-> is the vector pointing from i to j:  (j.x – i.x, j.y – i.y)
			-	note: F1-> from i to j equals -F2-> from j to i
			-	thus instead of calculating F2(j on i) and then -F2, we can just calculate F1 directly
			*/
			force.x += ( G * bodies[i].getMass() * bodies[j].getMass() / pow(distance, 3) ) * (bodies[j].getPos().x - bodies[i].getPos().x);
			force.y +=  ( G * bodies[i].getMass() * bodies[j].getMass() / pow(distance, 3) ) * (bodies[j].getPos().y - bodies[i].getPos().y);
			}
		}

		// F = m * a <=> a = F / m
		bodies[i].acceleration = force / bodies[i].getMass();
	}
}

void handleCollisions(Body bodies[], int totalBodies, bool borderEnabled){
	sf::Vector2f* newVelocity = new sf::Vector2f[totalBodies];
	bool* colission = new bool[totalBodies];

	for (int i = 0; i < totalBodies; i++) {
		newVelocity[i] = sf::Vector2f(0, 0);
		colission[i] = false;
	}

	for (int i = 0; i < totalBodies; i++) {
		for (int j = 0; j < totalBodies; j++) {
			if (i != j){
			//square the distance so we dont have to compute the sqrt
			float distanceSquared = pow((bodies[i].getPos().x - bodies[j].getPos().x), 2) + pow((bodies[i].getPos().y - bodies[j].getPos().y), 2);
			if (distanceSquared <= pow(bodies[i].getRadius() + bodies[j].getRadius(), 2)) {
				colission[i] = true;
				newVelocity[i] += bodies[i].collision(bodies[j]);
			}
		}
		}
	}
	for (int i = 0; i < totalBodies; i++) {
		if (colission[i]) {
			bodies[i].velocity = newVelocity[i];
		}
		if (borderEnabled) {
			if (bodies[i].getPos().x - bodies[i].getRadius() < 0-borderOffset) bodies[i].velocity.x = abs(bodies[i].velocity.x);
			else if (bodies[i].getPos().x + bodies[i].getRadius() > windowWidth+borderOffset) bodies[i].velocity.x = -abs(bodies[i].velocity.x);
			if (bodies[i].getPos().y - bodies[i].getRadius() < 0-borderOffset)	bodies[i].velocity.y = abs(bodies[i].velocity.y);
			else if (bodies[i].getPos().y + bodies[i].getRadius() > windowHeight+borderOffset) bodies[i].velocity.y = -abs(bodies[i].velocity.y);
		}
	}
	delete[] newVelocity;
	delete[] colission;
}

void updateBodies(Body bodies[], int totalBodies){
	if (gravityEnabled) {
		computeGravitationalForce(bodies, totalBodies);
	}

	if (collisionEnabled) {
		handleCollisions(bodies, totalBodies, borderEnabled);
	}
}

int main()
{
  cout << "C++ Gravity Simulator" << endl;

  string response;

if(askForInput){
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

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "C++ Gravity Simulator");
	window.setFramerateLimit(144);

	Body b1(20.0f, 1.0f, sf::Vector2f(100.f, 100.f), sf::Vector2f(0.f, 0.f), false);

	Body b2(50.0f, 1.0f, sf::Vector2f(700.f, 750.f), sf::Vector2f(-0.1f, 0.1f), false);
	Body b3(50.0f, 1.0f, sf::Vector2f(800.f, 750.f), sf::Vector2f(0.1f, -0.1f), false);

	Body bodies[3] = {b1,b2,b3};
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

		//render
		window.clear();
		for (int i = 0; i < totalBodies; i++) {
			bodies[i].move();
			window.draw(bodies[i].getBody());
			if(showTrails){
			window.draw(bodies[i].getTrail());
			}
		}
		window.display();
	}

	return 0;
}
