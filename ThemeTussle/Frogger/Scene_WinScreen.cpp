#include "Scene_WinScreen.h"
#include "Components.h"
#include "Scene_Tussle.h"
#include <memory>
#include "SoundPlayer.h"
#include "MusicPlayer.h"

void Scene_WinScreen::init()
{
	registerAction(sf::Keyboard::Q, "QUIT");
	registerAction(sf::Keyboard::Escape, "QUIT");
}

void Scene_WinScreen::onEnd()
{
	m_game->window().close();
}

Scene_WinScreen::Scene_WinScreen(GameEngine* gameEngine, const std::string& winner, const std::string& loser, int winnerNumber) : Scene(gameEngine)
{

	m_winner = winner;
	m_loser = loser;
	m_winnerNumber = winnerNumber;

	init();
}

void Scene_WinScreen::update(sf::Time dt)
{
	m_entityManager.update();
}

void Scene_WinScreen::sRender()
{

	sf::View view = m_game->window().getView();
	view.setCenter(m_game->window().getSize().x / 2.f, m_game->window().getSize().y / 2.f);
	m_game->window().setView(view);

	m_game->window().clear();

	auto e = m_entityManager.addEntity("stage");
	e->addComponent<CSprite>(Assets::getInstance().getTexture("WinnerStage")).sprite;
	auto& sprite = e->getComponent<CSprite>().sprite;
	sprite.setOrigin(0.f, 0.f);
	sprite.setPosition(0, 0);

	sf::Text winnerText("", Assets::getInstance().getFont("main"));

	if (m_winnerNumber == 1) {
		winnerText.setString("THE WINNER IS PLAYER 1");
	}
	else {
		winnerText.setString("THE WINNER IS PLAYER 2");
	}

	sf::Text title("END SCENE", Assets::getInstance().getFont("main"));
	title.setPosition(800, 10);
	title.setCharacterSize(100);

	auto w = m_entityManager.addEntity("winner");
	w->addComponent<CSprite>(Assets::getInstance().getTexture(m_winner));
	auto& winner = w->getComponent<CSprite>().sprite;
	winner.setScale(1.5, 1.5);
	winner.setOrigin(0.f, 0.f);
	winner.setPosition(850, 200);

	auto l = m_entityManager.addEntity("loser");
	l->addComponent<CSprite>(Assets::getInstance().getTexture(m_loser));
	auto& loser = l->getComponent<CSprite>().sprite;
	loser.setScale(-1, 1);
	loser.setOrigin(0.f, 0.f);
	loser.setPosition(1800, 800);

	winnerText.setPosition(650, 850);
	winnerText.setCharacterSize(100);

	m_game->window().draw(sprite);
	m_game->window().draw(winnerText);
	m_game->window().draw(winner);
	m_game->window().draw(loser);
	m_game->window().draw(title);
}

void Scene_WinScreen::sDoAction(const Command& action)
{
	if (action.type() == "START") {
		if (action.name() == "QUIT") { onEnd(); }
	}
}
