#include "background.h"


#include "SoundManager.h"
#include "TextureManager.h"

Background::Background()
{
	TextureManager::Instance().load("../Assets/textures/background.png", "background");

	auto size = TextureManager::Instance().getTextureSize("background");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.0f, 300.0f);

	setType(OBSTACLE);
	getRigidBody()->isColliding = false;

}

Background::~Background()
= default;

void Background::draw()
{
	TextureManager::Instance().draw("background",
		getTransform()->position.x, getTransform()->position.y, 0, 255, true);
}

void Background::update()
{
}

void Background::clean()
{
}
