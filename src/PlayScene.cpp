#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 0, 0, 0, 255);

	// Ground line
	SDL_RenderDrawLineF(Renderer::Instance().getRenderer(), 0, m_startingY, 800, m_startingY);

	// Draw arc of motion using simulated projectile
	const int numSamples = 200;
	const float simulationDuration = 10.0f;
	float simulationTime = 0;
	const float simulationTimeStep = simulationDuration / (float)numSamples;
	glm::vec2 prevSample = { m_startingX, m_startingY };
	glm::vec2 nextSample;
	for (int i = 0; i < numSamples; i++)
	{
		nextSample.x = m_startingX + vx * simulationTime;
		nextSample.y = m_startingY + vy * simulationTime + 0.5f * m_accelerationGravity * pow(simulationTime, 2);
		SDL_RenderDrawLineF(Renderer::Instance().getRenderer(), prevSample.x, prevSample.y, nextSample.x, nextSample.y);
		
		prevSample = nextSample;
		simulationTime += simulationTimeStep;
	}

	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	updateDisplayList();
	auto dt = Game::Instance().getDeltaTime();
	if (m_running)
	{
		m_pProjectile->getRigidBody()->velocity.y += m_accelerationGravity * dt;

		m_pProjectile->getTransform()->position.x += m_pProjectile->getRigidBody()->velocity.x * dt;
		m_pProjectile->getTransform()->position.y += m_pProjectile->getRigidBody()->velocity.y * dt;
	}
	if (m_pProjectile->getTransform()->position.y > 650)
	{
		m_running = false;
	}
	if (EventManager::Instance().keyPressed(SDL_SCANCODE_SPACE))
	{
		// Set initial position
		m_pProjectile->getTransform()->position.x = m_startingX;
		m_pProjectile->getTransform()->position.y = m_startingY;

		// Set initial velocity
		vx = cos(Util::Deg2Rad * m_launchElevationAngle) * m_launchSpeed;
		vy = sin(Util::Deg2Rad * m_launchElevationAngle) * m_launchSpeed;
		m_pProjectile->getRigidBody()->velocity.x = vx;
		m_pProjectile->getRigidBody()->velocity.y = vy;

		if (!m_running)
			m_running = true;
		else
		{
			m_pProjectile->getRigidBody()->velocity.x = 0;
			m_pProjectile->getRigidBody()->velocity.y = 0;
			m_running = !m_running;
			vx = 0;
			vy = 0;
		}
	}
	if (!m_running)
	{
		// Set initial position
		m_pProjectile->getTransform()->position.x = m_startingX;
		m_pProjectile->getTransform()->position.y = m_startingY;

		// Set initial velocity
		vx = m_launchSpeed * cos(Util::Deg2Rad * m_launchElevationAngle);
		vy = m_launchSpeed * sin(Util::Deg2Rad * m_launchElevationAngle);
		m_pProjectile->getRigidBody()->acceleration.y = 0;
		m_pProjectile->getRigidBody()->velocity.x = vx;
		m_pProjectile->getRigidBody()->velocity.y = vy;
	}
	m_pStormtroopers->getTransform()->position.x = m_stormtrooperX;
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();
	

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance().quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance().changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance().changeSceneState(END_SCENE);
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	m_pBG = new Background();
	addChild(m_pBG);
	
	m_pProjectile = new Projectile();
	addChild(m_pProjectile);

	m_pStormtroopers = new Obstacle();
	addChild(m_pStormtroopers);

	m_pStormtroopers->getTransform()->position.x = m_stormtrooperX;
	m_pStormtroopers->getTransform()->position.y = m_startingY - 50;

	m_pProjectile->getTransform()->position.x = m_startingX;
	m_pProjectile->getTransform()->position.y = m_startingY;

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 500.0f);

	m_pInstructionsLabel2 = new Label("Press spacebar to launch the thermal detonator", "Consolas");
	m_pInstructionsLabel2->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 525.0f);

	addChild(m_pInstructionsLabel);
	addChild(m_pInstructionsLabel2);
	
	const SDL_Color blue = { 0, 0, 255, 255 };
	m_pNameLabel = new Label("Sean Jasmins-Nelson", "Consolas", 20, blue, glm::vec2(400.0f, 450.0f));
	m_pNameLabel->setParent(this);
	addChild(m_pNameLabel);

	m_pStudentNumberLabel = new Label("Student #: 101297706", "Consolas", 20, blue, glm::vec2(400.0f, 475.0f));
	m_pStudentNumberLabel->setParent(this);
	addChild(m_pStudentNumberLabel);

	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Your Window Title Goes Here", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	ImGui::SliderFloat("Starting X", &m_startingX, 0.0f, 800.0f, "%.1f");
	ImGui::SliderFloat("Starting Y", &m_startingY, 0.0f, 600.0f, "%.1f");
	ImGui::SliderFloat("Launch Speed", &m_launchSpeed, 0.0f, 400.0f, "%.1f");
	ImGui::SliderFloat("Launch Angle", &m_launchElevationAngle, 0.0f, 360.0f, "%.2f");
	ImGui::SliderFloat("Gravity", &m_accelerationGravity, 0.0f, 200.0f, "%.1f");
	ImGui::SliderFloat("Stormtrooper X", &m_stormtrooperX, 100.0f, 700.0f, "%.1f");

	ImGui::End();
}
