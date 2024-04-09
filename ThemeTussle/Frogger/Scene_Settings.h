#pragma once

#include "Scene.h"

class Scene_Settings : public Scene
{

private:
	bool						m_volumeSet{ false };
	std::vector<std::string>	m_menuStrings;
	sf::Text					m_menuText;
	std::vector<std::string>	m_levelPaths;
	int							m_menuIndex{ 0 };
	std::string					m_title;
	int							m_volumeNumber{ 100 };
	int							m_oldVolume{ 100 };


	void init();
	void onEnd() override;
public:

	Scene_Settings(GameEngine* gameEngine);

	void update(sf::Time dt) override;

	void sRender() override;
	void sDoAction(const Command& action) override;

};

