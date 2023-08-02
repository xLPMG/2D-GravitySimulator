#include <SFML/Graphics.hpp>
#include <math.h>
#include <string>
#include <iostream>
#include "Body.h"
#include "Physics.h"

		Body::Body(float mass, float radius, sf::Vector2f pos, sf::Vector2f velocity, bool stationary, sf::Color color) {
			this->mass = mass;
			this->radius = radius;
			this->body.setRadius(this->radius);
			this->body.setPosition(pos.x-radius,pos.y-radius);
			this->pos = pos;
			this->velocity = velocity;
			this->stationary = stationary;
			this->acceleration = sf::Vector2f(0.0f, 0.0f);

			this->force = sf::Vector2f(0.0f, 0.0f);

			body.setFillColor(color);
			vertices.setPrimitiveType(sf::PrimitiveType::Points);
		}

		float Body::getMass() {
			return this->mass;
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

		sf::Vector2f Body::getForce(){
			return this->force;
		}

		void Body::setForce(sf::Vector2f force){
			this->force = force;
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
