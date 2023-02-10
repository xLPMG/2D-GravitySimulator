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
			sf::Vector2f vN = (normalize(sf::Vector2f(body2.pos.x - pos.x, body2.pos.y - pos.y)));
			sf::Vector2f hN(-vN.y, vN.x);

			float s1v = velocity.x * vN.x + velocity.y * vN.y;
			float s1h = velocity.x * hN.x + velocity.y * hN.y;
			float s2v = body2.velocity.x * vN.x + body2.velocity.y * vN.y;
			//float s2h = body2.vlc.x * hN.x + body2.vlc.y * hN.y;

			s1v = ((mass - body2.mass)*s1v + 2 * body2.mass*s2v) / (mass + body2.mass);
      //s1h = ((mass - body2.mass)*s2h + 2 * body2.mass*s2h) / (mass + body2.mass);

			sf::Vector2f s1vVector(vN.x * s1v, vN.y * s1v);
			sf::Vector2f s1hVector(hN.x * s1h, hN.y * s1h);
			return sf::Vector2f(s1vVector.x + s1hVector.x, s1vVector.y + s1hVector.y);
		}

		void Body::move() {
			if(!stationary){
			pos.x += velocity.x;
			pos.y += velocity.y;
			body.setPosition(pos.x - radius, pos.y - radius);
			velocity.x += acceleration.x;
			velocity.y += acceleration.y;

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
