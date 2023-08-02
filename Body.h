#ifndef BODY_H
#define BODY_H

class Body {
private:
  float mass;
  float radius;
  bool stationary;

  int vertexCount=0;
  sf::CircleShape body;
  sf::Vector2f pos;
  sf::VertexArray vertices;
  sf::Vector2f force;

public:
  sf::Vector2f velocity;
  sf::Vector2f acceleration;

		Body(float mass, float radius, sf::Vector2f pos, sf::Vector2f velocity, bool stationary, sf::Color color);

		float getMass();

		float getRadius();

		sf::Vector2f getPos();

		sf::CircleShape getBody();

    sf::VertexArray getTrail();

    sf::Vector2f getForce();

    void setForce(sf::Vector2f force);

		void move();
};
#endif
