#pragma once

#include "Scene.h"
#include "GameEngine.h"
class Scene_Arcade : public Scene
{

private:

	void init();
	void onEnd() override;

public:

	Scene_Arcade(GameEngine* gameEngine);

	void update(sf::Time dt) override;

	void sRender() override;
	void sDoAction(const Command& action) override;


};


