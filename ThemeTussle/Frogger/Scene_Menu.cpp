#include "Scene_Menu.h"
#include"Scene_Settings.h"
#include "Scene_Tussle.h"
#include "Scene_Character_Select.h"
#include <memory>
#include "SoundPlayer.h"
#include "MusicPlayer.h"

void Scene_Menu::onEnd()
{
	m_game->window().close();
}

Scene_Menu::Scene_Menu(GameEngine* gameEngine)
	: Scene(gameEngine)
{
	MusicPlayer::getInstance().play("MainTheme");

	init();
}

void Scene_Menu:: init()
{
    registerAction(sf::Keyboard::W,			"UP");
    registerAction(sf::Keyboard::Up,		"UP");
    registerAction(sf::Keyboard::S,			"DOWN");
    registerAction(sf::Keyboard::Down,	 	"DOWN");
	registerAction(sf::Keyboard::Enter,		"PLAY");
	registerAction(sf::Keyboard::P,			"PLAY");
	m_title = "THEME TUSSLE";

	m_menuStrings.push_back("ARCADE MODE");
	m_menuStrings.push_back("VERSUS MODE");
	m_menuStrings.push_back("TUTORIAL MODE");
	m_menuStrings.push_back("SETTINGS");
	m_menuStrings.push_back("QUIT");

	m_levelPaths.push_back("../assets/arcade.txt");
	m_levelPaths.push_back("../assets/versus.txt");
	m_levelPaths.push_back("../assets/tutorial.txt");

	m_menuText.setFont(Assets::getInstance().getFont("main"));

	const size_t CHAR_SIZE{ 64 };
	m_menuText.setCharacterSize(CHAR_SIZE);

}

void Scene_Menu::update(sf::Time dt)
{
	m_entityManager.update();
}


void Scene_Menu::sRender()
{
	 
	sf::View view = m_game->window().getView();
	view.setCenter(m_game->window().getSize().x / 2.f, m_game->window().getSize().y / 2.f);
	m_game->window().setView(view);

	static const sf::Color selectedColor(255, 255, 255);
	static const sf::Color normalColor(0, 0, 0);

	m_game->window().clear();

	auto e = m_entityManager.addEntity("stage");
	e->addComponent<CSprite>(Assets::getInstance().getTexture("Stage")).sprite;
	auto& sprite = e->getComponent<CSprite>().sprite;
	sprite.setOrigin(0.f, 0.f);
	sprite.setPosition(0, 0);

	m_game->window().draw(sprite);

	m_menuText.setFillColor(normalColor);
	m_menuText.setString(m_title);
	m_menuText.setPosition(m_game->window().getSize().x / 2.75, 10);
	m_game->window().draw(m_menuText);

	for (size_t i{ 0 }; i < m_menuStrings.size(); ++i)
	{
		m_menuText.setFillColor((i == m_menuIndex ? selectedColor : normalColor));
		m_menuText.setPosition(32, 32 + (i+1) * 96);
		m_menuText.setString(m_menuStrings.at(i));
		m_game->window().draw(m_menuText);
	} 
	//m_game->window().display();

}


void Scene_Menu::sDoAction(const Command& action)
{
	if (action.type() == "START")
	{
		if (action.name() == "UP")
		{
			m_menuIndex = (m_menuIndex + m_menuStrings.size() - 1) % m_menuStrings.size();
		}
		else if (action.name() == "DOWN")
		{
			m_menuIndex = (m_menuIndex + 1) % m_menuStrings.size();
		}
		else if (action.name() == "PLAY")
		{
			if (m_menuIndex == 0) {
				SoundPlayer::getInstance().play("Select");
				m_game->changeScene("ARCADE", std::make_shared<Scene_Character_Select>(m_game, m_levelPaths[m_menuIndex]));
			}
			if (m_menuIndex == 1) {
				SoundPlayer::getInstance().play("Select");
				m_game->changeScene("VERSUS", std::make_shared<Scene_Character_Select>(m_game, m_levelPaths[m_menuIndex]));
			}
			if (m_menuIndex == 2) {
				SoundPlayer::getInstance().play("Select");
				m_game->changeScene("TUTORIAL", std::make_shared<Scene_Character_Select>(m_game, m_levelPaths[m_menuIndex]));
			}
			if (m_menuIndex == 3) {
				SoundPlayer::getInstance().play("Select");
				m_game->changeScene("SETTINGS", std::make_shared<Scene_Settings>(m_game));
			}
			if (m_menuIndex == 4) {
				onEnd();
			}
		}
	}

}
