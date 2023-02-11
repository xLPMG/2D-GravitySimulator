#include <SFML/Graphics.hpp>
#include <math.h>
#include <string>
#include <iostream>
#include "Body.h"

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

		sf::Vector2f Body::collision(Body body2) {
			//find the norm of the vector from the point of collision of this body and the point of collision of body2
			sf::Vector2f vN = (normalize(sf::Vector2f(body2.pos.x - pos.x, body2.pos.y - pos.y)));
			//calculate the p-value that takes into account the velocities of both bodies
			float p = 2 * (velocity.x * vN.x + velocity.y * vN.y - body2.velocity.x * vN.x - body2.velocity.y * vN.y) /
        (mass + body2.getMass());

				sf::Vector2f newVelocity;
				newVelocity.x = velocity.x - p * mass * vN.x;
				newVelocity.y = velocity.y - p * mass * vN.y;
				return newVelocity;
		}

		void Body::move() {
			if(!stationary){
			pos.x += velocity.x;
			pos.y += velocity.y;
			velocity.x += acceleration.x;
			velocity.y += acceleration.y;

			body.setPosition(pos.x - radius, pos.y - radius);

				if(vertices.getVertexCount()==30){
					vertices[vertexCount].position = sf::Vector2f(pos.x, pos.y);
					vertexCount++;
						if(vertexCount>=30){
							vertexCount=0;
						}
				}else{
					vertices.append(sf::Vertex(sf::Vector2f(pos.x, pos.y)));
				}
			}
		}

    sf::Vector2f Body::normalize(sf::Vector2f v) {
      sf::Vector2f result;
      float length = sqrt(v.x * v.x + v.y * v.y);
      result.x = v.x / length;
      result.y = v.y / length;
      return result;
    }
