#pragma once

#include "Scene.h"

class Scene_WinScreen : public Scene
{
private:
	void init();
	void onEnd() override;

	std::string m_winner;
	std::string m_loser;
	int m_winnerNumber;
public:

	Scene_WinScreen(GameEngine* gameEngine, const std::string& winner, const std::string& loser, int winnerNumber);

	void update(sf::Time dt) override;

	void sRender() override;
	void sDoAction(const Command& action) override;

};

