#include "Obstacle.h"


#include "SoundManager.h"
#include "TextureManager.h"

Obstacle::Obstacle()
{
	TextureManager::Instance().load("../Assets/textures/Stormtroopers.png", "stormtroopers");

	auto size = TextureManager::Instance().getTextureSize("stormtroopers");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(300.0f, 300.0f);

	setType(OBSTACLE);
	getRigidBody()->isColliding = false;

	SoundManager::Instance().load("../Assets/audio/yay.ogg", "yay", SOUND_SFX);
}

Obstacle::~Obstacle()
= default;

void Obstacle::draw()
{
	TextureManager::Instance().draw("stormtroopers",
		getTransform()->position.x, getTransform()->position.y, 0, 255, true);
}

void Obstacle::update()
{
}

void Obstacle::clean()
{
}
