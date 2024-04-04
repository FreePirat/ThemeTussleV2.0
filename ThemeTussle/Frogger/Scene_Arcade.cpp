#include "Scene_Arcade.h"
#include "SoundPlayer.h"
#include "Scene_Tussle.h"
#include <memory>
#include "MusicPlayer.h"
#include "Components.h"

void Scene_Arcade::init()
{
	registerAction(sf::Keyboard::Q, "QUIT");
	registerAction(sf::Keyboard::Escape, "QUIT");
}

void Scene_Arcade::onEnd()
{
	SoundPlayer::getInstance().play("Select");
	m_game->changeScene("MENU", nullptr, false);
}

Scene_Arcade::Scene_Arcade(GameEngine* gameEngine) : Scene(gameEngine)
{
	init();
}

void Scene_Arcade::update(sf::Time dt)
{
	m_entityManager.update();
}

void Scene_Arcade::sRender()
{
	auto e = m_entityManager.addEntity("stage");
	e->addComponent<CSprite>(Assets::getInstance().getTexture("ArcadeStage")).sprite;
	
	auto& sprite = e->getComponent<CSprite>().sprite;
	sprite.setOrigin(0.f, 0.f);
	sprite.setPosition(0, 0);

	m_game->window().draw(sprite);
}

void Scene_Arcade::sDoAction(const Command& action)
{
	if (action.type() == "START") {
		if (action.name() == "QUIT") { onEnd(); }
	}
}
