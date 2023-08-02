#ifndef PHYSICS_H
#define PHYSICS_H

sf::Vector2f normalize(sf::Vector2f v);

sf::Vector2f collide(Body body1, Body body2);

void computeGravitationalForce(Body bodies[], int totalBodies);

void handleCollisions(Body bodies[], int totalBodies);

#endif
