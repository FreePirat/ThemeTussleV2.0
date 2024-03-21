#pragma once

#include "Scene.h"
#include "GameEngine.h"

class Scene_Character_Select : public Scene
{

private:

	sPtrEntt					m_stage{ nullptr };
	std::vector<std::string>	m_menuNames;
	std::vector<std::string>	m_menuLevels;
	sf::Text					m_menuText;
	std::vector<std::string>	m_characterPath;
	std::vector<std::string>	m_stagePath;
	std::vector<std::string>	m_modePath;
	int							m_menuIndex{ 0 };
	int							m_levelIndex{ 0 };
	std::string					m_title;
	std::string					m_modeFilePath;
	bool						m_arcade{ false };
	bool						m_versus{ false };
	bool						m_tutorial{ false };
	bool						m_stageToggle{ false };
	std::string					m_character;
	std::string					m_enemy;
	bool						m_player1Picked{ false };
	

	void						loadStage();

	void init();
	void onEnd() override;
public:

	Scene_Character_Select(GameEngine* gameEngine, const std::string& levelPath);

	void update(sf::Time dt) override;

	void sRender() override;
	void sDoAction(const Command& action) override;
	void loadLevel(const std::string& path);


};

