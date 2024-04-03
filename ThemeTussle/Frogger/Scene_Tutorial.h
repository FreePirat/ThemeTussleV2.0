#pragma once

#include "Scene.h"
#include "GameEngine.h"
class Scene_Tutorial : public Scene
{

private:

	int		m_currentScreenNumber{ 0 };

	void init();
	void onEnd() override;

public:

	Scene_Tutorial(GameEngine* gameEngine);

	void update(sf::Time dt) override;

	void sRender() override;
	void sDoAction(const Command& action) override;


};


