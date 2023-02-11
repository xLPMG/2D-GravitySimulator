#include <SFML/Graphics.hpp>
#include <math.h>
#include "Body.h"
#include "Physics.h"

const float G = 6.6743f; //6.6743 × 10^-11 m^3 kg^-1 s^-2

sf::Vector2f normalize(sf::Vector2f v) {
	sf::Vector2f result;
	float length = sqrt(v.x * v.x + v.y * v.y);
	result.x = v.x / length;
	result.y = v.y / length;
	return result;
}

sf::Vector2f collide(Body body1, Body body2){
	//find the norm of the vector from the point of collision of this body and the point of collision of body2
	sf::Vector2f vN = (normalize(sf::Vector2f(body2.getPos().x - body1.getPos().x, body2.getPos().y - body1.getPos().y)));
	//calculate the p-value that takes into account the velocities of both bodies
	float p = 2 * (body1.velocity.x * vN.x + body1.velocity.y * vN.y - body2.velocity.x * vN.x - body2.velocity.y * vN.y) /
		(body1.getMass() + body2.getMass());

		sf::Vector2f newVelocity;
		newVelocity.x = body1.velocity.x - p * body1.getMass() * vN.x;
		newVelocity.y = body1.velocity.y - p * body1.getMass() * vN.y;
		return newVelocity;
}

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
			*/
			force.x += ( G * bodies[i].getMass() * bodies[j].getMass() / pow(distance, 3) ) * (bodies[j].getPos().x - bodies[i].getPos().x);
			force.y +=  ( G * bodies[i].getMass() * bodies[j].getMass() / pow(distance, 3) ) * (bodies[j].getPos().y - bodies[i].getPos().y);
			}
		}
		// F = m * a <=> a = F / m
		bodies[i].acceleration = force / bodies[i].getMass();
	}
}

void handleCollisions(Body bodies[], int totalBodies, bool borderEnabled, int borderOffset, int windowWidth, int windowHeight){
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
			if (distanceSquared <= (bodies[i].getRadius() + bodies[j].getRadius()) * (bodies[i].getRadius() + bodies[j].getRadius())) {
				colission[i] = true;
				newVelocity[i] += collide(bodies[i], bodies[j]);
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
