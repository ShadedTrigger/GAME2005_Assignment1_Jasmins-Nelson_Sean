#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "background.h"
#include "Scene.h"
#include "Button.h"
#include "Label.h"
#include "projectile.h"
#include "Ship.h"
#include "Obstacle.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;

	Projectile* m_pProjectile;

	Obstacle* m_pStormtroopers;
	Background* m_pBG;
	
	float m_startingX = 50.0f,
		m_startingY = 425.0f,
		m_launchElevationAngle = 315.0f,
		m_launchSpeed = 200.0f,
		m_accelerationGravity = 40.0f,
		m_stormtrooperX = 485.0f;
	bool m_running = false;
	float vx, vy;

	// UI Items
	Button* m_pBackButton;
	Button* m_pNextButton;
	Label* m_pInstructionsLabel;
	Label* m_pInstructionsLabel2;

	Label* m_pNameLabel{};
	Label* m_pStudentNumberLabel{};
};

#endif /* defined (__PLAY_SCENE__) */