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

#include "week2/ExampleGame/DebugCube.h"
#include "week2\ExampleGame\Scene.h"
#include <ctime>

using namespace week2;


ExampleGame::ExampleGame()
{
}


ExampleGame::~ExampleGame()
{
}

bool ExampleGame::Init()
{
	Scene::CreateInstance();
	Camera *pCamera = new Camera(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f, glm::vec3(0.0f, 30.0f, 10.0f), glm::vec3(1.0f,1.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
	Scene::Instance()->SetActiveCamera(pCamera);

	srand(static_cast<unsigned int>(time(0)));
	for (int j = 0; j<100; j++)
	{
		DebugCube *cube = new DebugCube(Scene::Instance()->GetCurrentNodeID(), glm::vec3(rand()%100, 0, rand()%100), glm::vec3((rand()%20 + 10)*0.1f, (rand()%20+ 10)*0.1f, (rand()%20+ 10)*0.1f));
		Scene::Instance()->AddTopNode(cube);
	}

	return true;
}


bool ExampleGame::Update(float p_fDelta)
{
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