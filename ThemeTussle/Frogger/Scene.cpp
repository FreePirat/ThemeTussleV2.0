#include "Scene.h"


Scene::Scene(GameEngine* gameEngine) : m_game(gameEngine)
{}

Scene::~Scene()
{}

void Scene::setPaused(bool paused)
{
    m_isPaused = paused;
}


void Scene::simulate(int)
{}

void Scene::doAction(Command command)
{
	this->sDoAction(command);
}


const CommandMap Scene::getActionMap() const
{
	return m_commands;
}

void Scene::registerAction(int inputKey, std::string command)
{
	m_commands[inputKey] = command;
}
