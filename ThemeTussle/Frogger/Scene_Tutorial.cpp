#include "Scene_Tutorial.h"
#include "SoundPlayer.h"
#include "Scene_Tussle.h"
#include <memory>
#include "MusicPlayer.h"
#include "Components.h"

void Scene_Tutorial::init()
{
	registerAction(sf::Keyboard::D, "NEXT");
	registerAction(sf::Keyboard::Right, "NEXT");
	registerAction(sf::Keyboard::Left, "PREVIOUS");
	registerAction(sf::Keyboard::A, "PREVIOUS");
	registerAction(sf::Keyboard::Q, "QUIT");
	registerAction(sf::Keyboard::Escape, "QUIT");
}

void Scene_Tutorial::onEnd()
{
	SoundPlayer::getInstance().play("Select");
	m_game->changeScene("MENU", nullptr, false);
}

Scene_Tutorial::Scene_Tutorial(GameEngine* gameEngine) : Scene(gameEngine)
{
	init();
}

void Scene_Tutorial::update(sf::Time dt)
{
	m_entityManager.update();
}

void Scene_Tutorial::sRender()
{
	auto e = m_entityManager.addEntity("stage");
	if (m_currentScreenNumber == 0) {
		e->addComponent<CSprite>(Assets::getInstance().getTexture("TutorialStage")).sprite;
	}
	else if (m_currentScreenNumber == 1) {
		e->addComponent<CSprite>(Assets::getInstance().getTexture("TutorialStage2")).sprite;
	}
	else {
		e->addComponent<CSprite>(Assets::getInstance().getTexture("TutorialStage3")).sprite;
	}
	
	auto& sprite = e->getComponent<CSprite>().sprite;
	sprite.setOrigin(0.f, 0.f);
	sprite.setPosition(0, 0);

	m_game->window().draw(sprite);
}

void Scene_Tutorial::sDoAction(const Command& action)
{
	if (action.type() == "START") {
		if (action.name() == "QUIT") { onEnd(); }
		else if (action.name() == "NEXT" && m_currentScreenNumber != 2) {
			SoundPlayer::getInstance().play("Select");
			m_currentScreenNumber += 1;
		}
		else if (action.name() == "PREVIOUS" && m_currentScreenNumber != 0) {
			SoundPlayer::getInstance().play("Select");
			m_currentScreenNumber -= 1;
		}
	}
}
