#include <SFML/Graphics.hpp>
#include <math.h>
#include <string>
#include <iostream>
#include "Body.h"
#include "Physics.h"

		Body::Body(float mass, float density, sf::Vector2f pos, sf::Vector2f velocity, bool stationary) {
			this->mass = mass;
			this->density = density;
			//let the body be a sphere:
			// radius = ((3*volume) / 4*PI) ^ 1/3 ( 1/3 is approximated with 0.33333f )
			// volume = mass / density
			//radius * 2 scaling for better visuals
			this->radius = pow((((3*(mass / density))) / 4*(3.14159)), 0.33333f)*2;
			this->body.setRadius(this->radius);
			this->body.setPosition(pos.x-radius,pos.y-radius);
			this->pos = pos;
			this->velocity = velocity;
			this->stationary = stationary;
			this->acceleration = sf::Vector2f(0.0f, 0.0f);

			vertices.setPrimitiveType(sf::PrimitiveType::Points);
		}

		float Body::getMass() {
			return this->mass;
		}

		float Body::getDensity() {
			return this->density;
		}

		float Body::getRadius() {
			return this->radius;
		}

		sf::Vector2f Body::getPos() {
			return this->pos;
		}

		sf::CircleShape Body::getBody() {
			return this->body;
		}

		sf::VertexArray Body::getTrail() {
			return this->vertices;
		}

		void Body::move() {
			if(!stationary){
			pos.x += velocity.x;
			pos.y += velocity.y;
			velocity.x += acceleration.x;
			velocity.y += acceleration.y;

			body.setPosition(pos.x - radius, pos.y - radius);

				if(vertices.getVertexCount()==50){
					vertices[vertexCount].position = sf::Vector2f(pos.x, pos.y);
					vertexCount++;
						if(vertexCount>=50){
							vertexCount=0;
						}
				}else{
					vertices.append(sf::Vertex(sf::Vector2f(pos.x, pos.y)));
				}
			}
		}
