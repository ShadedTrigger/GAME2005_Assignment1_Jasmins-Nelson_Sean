#pragma once
#ifndef __PROJECTILE__
#define __PROJECTILE__

#include "TextureManager.h"
#include <glm/vec4.hpp>

#include "Agent.h"

class Projectile final : public Agent
{
public:
	Projectile();
	~Projectile();

	// Inherited via GameObject
	void draw() override;
	void update() override;
	void clean() override;

	void turnRight();
	void turnLeft();
	void moveForward();
	void moveBack();

	void move();

	// getters
	float getMaxSpeed() const;

	// setters
	void setMaxSpeed(float newSpeed);

private:
	void m_checkBounds();
	void m_reset();

	float m_maxSpeed;
	float m_turnRate;

	float m_startingX = 100,
		m_startingY = 300,
		m_launchElevationAngle = 200,
		m_launchSpeed = 230,
		m_accelerationGravity = 98;
	int m_t = 0;
	float vx, vy, av;

};


#endif /* defined (__PROJECTILE__) */

