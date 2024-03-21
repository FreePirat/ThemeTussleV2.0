#include <fstream>
#include <iostream>

#include "Scene_Settings.h"
#include "Scene_Menu.h"
#include "Scene_Tussle.h"
#include <memory>
#include "Components.h"
#include "Physics.h"
#include "Utilities.h"
#include "MusicPlayer.h"
#include "Assets.h"
#include "SoundPlayer.h"

void Scene_Settings::onEnd()
{
	m_game->window().close();
}

Scene_Settings::Scene_Settings(GameEngine* gameEngine)
	: Scene(gameEngine)
{
	init();
}

void Scene_Settings::init()
{
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::Up, "UP");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::Down, "DOWN");
	registerAction(sf::Keyboard::Enter, "PLAY");
	registerAction(sf::Keyboard::P, "PLAY");
	registerAction(sf::Keyboard::D, "RIGHT");
	registerAction(sf::Keyboard::Right, "RIGHT");
	registerAction(sf::Keyboard::A, "LEFT");
	registerAction(sf::Keyboard::Left, "LEFT");
	registerAction(sf::Keyboard::Space, "SAVE");
	registerAction(sf::Keyboard::Q, "QUIT");
	registerAction(sf::Keyboard::Escape, "QUIT");
	m_title = "THEME TUSSLE";

	m_menuStrings.push_back("ADJUST VOLUME");
	m_menuStrings.push_back("CHANGE CONTROLS");
	m_menuStrings.push_back("CLOSE");

	m_menuText.setFont(Assets::getInstance().getFont("main"));

	const size_t CHAR_SIZE{ 64 };
	m_menuText.setCharacterSize(CHAR_SIZE);

}

void Scene_Settings::update(sf::Time dt)
{
	m_entityManager.update();
}


void Scene_Settings::sRender()
{

	sf::View view = m_game->window().getView();
	view.setCenter(m_game->window().getSize().x / 2.f, m_game->window().getSize().y / 2.f);
	m_game->window().setView(view);

	static const sf::Color selectedColor(255, 255, 255);
	static const sf::Color normalColor(0, 0, 0);

	m_game->window().clear();

	if (m_volumeSet == false) {

		auto e = m_entityManager.addEntity("settings");
		e->addComponent<CSprite>(Assets::getInstance().getTexture("Settings")).sprite;
		auto& sprite = e->getComponent<CSprite>().sprite;
		sprite.setOrigin(0.f, 0.f);
		sprite.setPosition(0, 0);

		m_game->window().draw(sprite);
	}

	m_menuText.setFillColor(normalColor);
	m_menuText.setString(m_title);
	m_menuText.setPosition(m_game->window().getSize().x / 2.75, 10);
	m_game->window().draw(m_menuText);

	for (size_t i{ 0 }; i < m_menuStrings.size(); ++i)
	{
		if (m_volumeSet == false) {

			m_menuText.setFillColor((i == m_menuIndex ? selectedColor : normalColor));
			m_menuText.setPosition(32, 32 + (i + 1) * 96);
			m_menuText.setString(m_menuStrings.at(i));
			m_game->window().draw(m_menuText);
		}
		else {

			auto e = m_entityManager.addEntity("audio");
			e->addComponent<CSprite>(Assets::getInstance().getTexture("Audio")).sprite;
			auto& sprite = e->getComponent<CSprite>().sprite;
			sprite.setOrigin(0.f, 0.f);
			sprite.setPosition(0, 0);

			m_game->window().draw(sprite);

			sf::Texture texVolume;
			sf::Sprite volumeSetter;
			texVolume.loadFromFile("../assets/Textures/Volume.png");
			volumeSetter.setTexture(texVolume);
			volumeSetter.setScale(5, 5);
			volumeSetter.setOrigin(0, 0);
			volumeSetter.setPosition(m_game->window().getSize().x / 10.f, m_game->window().getSize().y / 10.f);

			sf::Text footer("QUIETER: A/ <-    LOUDER: D/ ->   SAVE CHANGES: Spacebar	DISCARD CHANGES: Q/ ESC",
				Assets::getInstance().getFont("main"), 20);
			footer.setFillColor(selectedColor);
			footer.setPosition(m_game->window().getSize().x - 1000, 950);

			sf::Text number(std::to_string(m_volumeNumber),
				Assets::getInstance().getFont("main"), 200);
			number.setFillColor(selectedColor);
			number.setPosition(m_game->window().getSize().x / 2.4, m_game->window().getSize().y / 2.75);

			m_game->window().draw(volumeSetter);
			m_game->window().draw(footer);
			m_game->window().draw(number);
		}
	}

	//m_game->window().display();

}


void Scene_Settings::sDoAction(const Command& action)
{
	if (action.type() == "START")
	{
		if (action.name() == "UP" && m_volumeSet == false)
		{
			m_menuIndex = (m_menuIndex + m_menuStrings.size() - 1) % m_menuStrings.size();
		}
		else if (action.name() == "DOWN" && m_volumeSet == false)
		{
			m_menuIndex = (m_menuIndex + 1) % m_menuStrings.size();
		}
		else if (action.name() == "PLAY" && m_volumeSet == false)
		{
			if (m_menuIndex == 0) {
				SoundPlayer::getInstance().play("Select");
				m_volumeSet = true;
			}
			if (m_menuIndex == 1) {
				//This should send the player to the menu for Controls TODO
				SoundPlayer::getInstance().play("Select");
			}
			if (m_menuIndex == 2) {
				SoundPlayer::getInstance().play("Select");
				m_game->changeScene("MENU", nullptr, false);
			}
		}
		if (m_volumeSet == true) {	
			if (action.name() == "RIGHT" && m_volumeNumber <= 98) {
				SoundPlayer::getInstance().removeStoppedSounds();
				if (SoundPlayer::getInstance().isEmpty()) { SoundPlayer::getInstance().play("Select"); };
				m_volumeNumber += 2;
				MusicPlayer::getInstance().setVolume(m_volumeNumber);
			}
			else if (action.name() == "LEFT" && m_volumeNumber >= 2) {
				SoundPlayer::getInstance().removeStoppedSounds();
				if (SoundPlayer::getInstance().isEmpty()) { SoundPlayer::getInstance().play("Select"); };
				m_volumeNumber -= 2;
				MusicPlayer::getInstance().setVolume(m_volumeNumber);
			}
			else if (action.name() == "SAVE") {
				SoundPlayer::getInstance().play("Select"); 
				MusicPlayer::getInstance().setVolume(m_volumeNumber);
				m_oldVolume = m_volumeNumber;
				m_volumeSet = false;
			}
			else if (action.name() == "QUIT") {
				SoundPlayer::getInstance().play("Select");
				MusicPlayer::getInstance().setVolume(m_oldVolume);
				m_volumeNumber = m_oldVolume;
				m_volumeSet = false;
			}
		}
	}

}

