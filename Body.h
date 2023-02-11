#ifndef BODY_H
#define BODY_H

class Body {
private:
  float mass;
  float density;
  float radius;
  bool stationary;

  int vertexCount=0;
  sf::CircleShape body;
  sf::Vector2f pos;
  sf::VertexArray vertices;

public:
  sf::Vector2f velocity;
  sf::Vector2f acceleration;

		Body(float mass, float density, sf::Vector2f pos, sf::Vector2f velocity, bool stationary);

		float getMass();

    float getDensity();

		float getRadius();

		sf::Vector2f getPos();

		sf::CircleShape getBody();

    sf::VertexArray getTrail();

		void move();
};
#endif
