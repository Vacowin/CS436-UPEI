//------------------------------------------------------------------------
// ExampleGame
//
// Created:	2012/12/06
// Author:	Carel Boers
//	
// Main game class. Derives from Common::Game.
//------------------------------------------------------------------------


#include "ExampleGame.h"

#include "W_Model.h"

#include "week2/ExampleGame/Particle.h"
#include "week2\ExampleGame\Scene.h"
#include <ctime>
#include "week2/ExampleGame/Effect.h"

using namespace week2;


ExampleGame::ExampleGame()
{
}


ExampleGame::~ExampleGame()
{
}


bool ExampleGame::Init()
{
	srand(static_cast<unsigned int>(time(0)));
	Scene::CreateInstance();
	Camera *pCamera1 = new Camera(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f, glm::vec3(-50.0f, 0.0f, 10.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
	Scene::Instance()->SetActiveCamera(pCamera1);

	isFireEffect = true;
	Scene::Instance()->AddTopNode(new Effect("data/effect/effect.fire",glm::vec3(10.0f,0.0f,0.0f)));

	return true;
}


bool ExampleGame::Update(float p_fDelta)
{
	if (glfwGetKey('X') == GLFW_PRESS && !m_bToggleKey)
    {
		isFireEffect = !isFireEffect;
		Scene::Instance()->RemoveNode(0);
		if (isFireEffect)
		{
			Scene::Instance()->AddTopNode(new Effect("data/effect/effect.fire",glm::vec3(10.0f,0.0f,0.0f)));
		}
		else
		{
			Scene::Instance()->AddTopNode(new Effect("data/effect/effect.explosion",glm::vec3(10.0f,0.0f,0.0f)));
		}
        m_bToggleKey = true;
    }
    else if (glfwGetKey('X') == GLFW_RELEASE)
        m_bToggleKey = false;


	Scene::Instance()->Update(p_fDelta);
	return true;
}


void ExampleGame::Render()
{
	Scene::Instance()->Render();
}


void ExampleGame::Shutdown()
{
}