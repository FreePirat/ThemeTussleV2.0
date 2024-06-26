#include <fstream>
#include <iostream>

#include "Scene_Character_Select.h"
#include "Scene_Menu.h"
#include "Scene_Tussle.h"
#include <memory>
#include "Components.h"
#include "Physics.h"
#include "Utilities.h"
#include "MusicPlayer.h"
#include "Assets.h"
#include "SoundPlayer.h"
#include <random>

void Scene_Character_Select::onEnd()
{
	m_game->window().close();
}

Scene_Character_Select::Scene_Character_Select(GameEngine* gameEngine, const std::string& levelPath)
	: Scene(gameEngine)
{
	m_modeFilePath = levelPath;
	loadLevel(levelPath); //THIS WILL CHECK IF THE GAME IS IN ARCADE, TUTORIAL OR VERSUS MODE
	init();
}

void Scene_Character_Select::init()
{
	loadStage();

	registerAction(sf::Keyboard::Enter, "PLAY");
	registerAction(sf::Keyboard::P, "PLAY");
	registerAction(sf::Keyboard::D, "RIGHT");
	registerAction(sf::Keyboard::Right, "RIGHT");
	registerAction(sf::Keyboard::A, "LEFT");
	registerAction(sf::Keyboard::Left, "LEFT");
	registerAction(sf::Keyboard::Q, "BACK");
	registerAction(sf::Keyboard::Escape, "BACK");
	//You can only change stage on VERSUS
	if (m_versus) {
		registerAction(sf::Keyboard::W, "UP");
		registerAction(sf::Keyboard::Up, "UP");
		registerAction(sf::Keyboard::S, "DOWN");
		registerAction(sf::Keyboard::Down, "DOWN");
		registerAction(sf::Keyboard::BackSpace, "TOGGLE");
		m_title = "SELECT A STAGE";

		m_menuLevels.push_back("AN ODD PLACE");
		m_menuLevels.push_back("THE KINGDOM OF WEBS");
		m_menuLevels.push_back("WESTERN FIELDS");
	}

	m_menuNames.push_back("NINA");
	m_menuNames.push_back("SUZIE");
	m_menuNames.push_back("RANDOM");
	m_menuNames.push_back("LEO");
	m_menuNames.push_back("CHARLIE");

	std::random_device rd;
	std::mt19937 rand(rd());

	std::uniform_int_distribution<> dis(1, 4);

	int characterPicker = dis(rand);

	std::string randomPath;
	if (characterPicker == 1) { randomPath = "../assets/nina.txt"; }
	else if (characterPicker == 2) { randomPath = "../assets/nina.txt"; }
	else if (characterPicker == 3) { randomPath = "../assets/charlie.txt"; }
	else { randomPath = "../assets/charlie.txt"; }

	m_characterPath.push_back("../assets/nina.txt");
	m_characterPath.push_back("../assets/nina.txt");
	m_characterPath.push_back(randomPath);
	m_characterPath.push_back("../assets/charlie.txt");
	m_characterPath.push_back("../assets/charlie.txt");

	m_stagePath.push_back("../assets/danger.txt");
	m_stagePath.push_back("../assets/kingdom.txt");
	m_stagePath.push_back("../assets/western.txt");

	m_modePath.push_back("../assets/arcade.txt");
	m_modePath.push_back("../assets/versus.txt");
	m_modePath.push_back("../assets/tutorial.txt");

	m_menuText.setFont(Assets::getInstance().getFont("main"));

	const size_t CHAR_SIZE{ 100 };
	m_menuText.setCharacterSize(CHAR_SIZE);

	m_player1 = m_entityManager.addEntity("Player1");
	m_player2 = m_entityManager.addEntity("Player2");

	m_ninaIcon = m_entityManager.addEntity("CSNina");
	m_suzieIcon = m_entityManager.addEntity("CSSuzie");
	m_randomIcon = m_entityManager.addEntity("CSRandom");
	m_leoIcon = m_entityManager.addEntity("CSLeo");
	m_charlieIcon = m_entityManager.addEntity("CSCharlie");
}

void Scene_Character_Select::update(sf::Time dt)
{
	m_entityManager.update();
}


void Scene_Character_Select::sRender()
{

	sf::View view = m_game->window().getView();
	view.setCenter(m_game->window().getSize().x / 2.f, m_game->window().getSize().y / 2.f);
	m_game->window().setView(view);

	static const sf::Color selectedColor(255, 255, 0);
	static const sf::Color footerColor(255, 255, 255);
	static const sf::Color nonSelectedColor(0, 0, 0, 0);

	m_game->window().clear();

	auto e = m_entityManager.addEntity("characterSelect");
	if (m_versus && !m_stageToggle) {
		e->addComponent<CSprite>(Assets::getInstance().getTexture("Select")).sprite;
	}
	else if (!m_versus && !m_stageToggle) {
		e->addComponent<CSprite>(Assets::getInstance().getTexture("Select1P")).sprite;
	}

	if (!m_stageToggle) {
		e->addComponent<CState>("background");
		auto& sprite = e->getComponent<CSprite>().sprite;
		sprite.setOrigin(0.f, 0.f);
		sprite.setPosition(0, 0);
		m_game->window().draw(sprite);
	}


	if (!m_stageToggle) {

		if (m_menuIndex == 0) {
			m_ninaIcon->addComponent<CSprite>(Assets::getInstance().getTexture("CSNinaSelected"));
			//Player 1
			if (!m_player1Picked) {
				m_player1->addComponent<CSprite>(Assets::getInstance().getTexture("NWin"));
			}
			//Player 2
			else {
				m_player2->addComponent<CSprite>(Assets::getInstance().getTexture("NWin"));
			}
			m_notPlayable = false;
		}
		else {
			m_ninaIcon->addComponent<CSprite>(Assets::getInstance().getTexture("CSNina"));
		}

		if (m_menuIndex == 1) {
			m_suzieIcon->addComponent<CSprite>(Assets::getInstance().getTexture("CSSuzieSelected"));
			if (!m_player1Picked) {
				m_player1->addComponent<CSprite>(Assets::getInstance().getTexture("Suzie"));
			}
			else {
				m_player2->addComponent<CSprite>(Assets::getInstance().getTexture("Suzie"));
			}
			m_notPlayable = true;
		}
		else { m_suzieIcon->addComponent<CSprite>(Assets::getInstance().getTexture("CSSuzie")); }

		if (m_menuIndex == 2) {
			m_randomIcon->addComponent<CSprite>(Assets::getInstance().getTexture("CSRandomSelected"));
			if (!m_player1Picked) {
				m_player1->addComponent<CSprite>(Assets::getInstance().getTexture("Random"));
			}
			else {
				m_player2->addComponent<CSprite>(Assets::getInstance().getTexture("Random"));
			}
			m_notPlayable = false;
		}
		else { m_randomIcon->addComponent<CSprite>(Assets::getInstance().getTexture("CSRandom")); }

		if (m_menuIndex == 3) {
			m_leoIcon->addComponent<CSprite>(Assets::getInstance().getTexture("CSLeoSelected"));
			if (!m_player1Picked) {
				m_player1->addComponent<CSprite>(Assets::getInstance().getTexture("Leo"));
			}
			else {
				m_player2->addComponent<CSprite>(Assets::getInstance().getTexture("Leo"));
			}
			m_notPlayable = true;
		}
		else { m_leoIcon->addComponent<CSprite>(Assets::getInstance().getTexture("CSLeo")); }

		if (m_menuIndex == 4) {
			m_charlieIcon->addComponent<CSprite>(Assets::getInstance().getTexture("CSCharlieSelected"));
			if (!m_player1Picked) {
				m_player1->addComponent<CSprite>(Assets::getInstance().getTexture("Charlie"));
			}
			else {
				m_player2->addComponent<CSprite>(Assets::getInstance().getTexture("Charlie"));
			}
			m_notPlayable = false;
		}
		else { m_charlieIcon->addComponent<CSprite>(Assets::getInstance().getTexture("CSCharlie")); }

		auto& player1Sprite = m_player1->getComponent<CSprite>().sprite;
		player1Sprite.setOrigin(0.f, 0.f);
		player1Sprite.setPosition(50, 100);
		m_player1->addComponent<CState>("player1");

		auto& player2Sprite = m_player2->getComponent<CSprite>().sprite;
		player2Sprite.setOrigin(0.f, 0.f);
		player2Sprite.setPosition(1650, 100);

		auto& ninaSprite = m_ninaIcon->getComponent<CSprite>().sprite;
		ninaSprite.setOrigin(0.f, 0.f);
		ninaSprite.setPosition(282, 605);

		auto& suzieSprite = m_suzieIcon->getComponent<CSprite>().sprite;
		suzieSprite.setOrigin(0.f, 0.f);
		suzieSprite.setPosition(540, 605);

		auto& randomSprite = m_randomIcon->getComponent<CSprite>().sprite;
		randomSprite.setOrigin(0.f, 0.f);
		randomSprite.setPosition(794, 605);

		auto& leoSprite = m_leoIcon->getComponent<CSprite>().sprite;
		leoSprite.setOrigin(0.f, 0.f);
		leoSprite.setPosition(1046, 605);

		auto& charlieSprite = m_charlieIcon->getComponent<CSprite>().sprite;
		charlieSprite.setOrigin(0.f, 0.f);
		charlieSprite.setPosition(1298, 605);
	}
	else {
		for (auto& e : m_entityManager.getEntities()) {
			if (!e->hasComponent<CSprite>() || e->getComponent<CState>().state == "background")
				continue;
			if (m_player1Picked && e->getComponent<CState>().state == "player1") {
				continue;
			}
			e->removeComponent<CSprite>();
		}
	}

	if (m_levelIndex == 0) { m_stage->addComponent<CSprite>(Assets::getInstance().getTexture("danger2")); }
	else if (m_levelIndex == 1) { m_stage->addComponent<CSprite>(Assets::getInstance().getTexture("Kingdom2")); }
	else if (m_levelIndex == 2) { m_stage->addComponent<CSprite>(Assets::getInstance().getTexture("Western2")); }

	auto& stageSprite = m_stage->getComponent<CSprite>().sprite;
	stageSprite.setOrigin(0.f, 0.f);
	if(!m_stageToggle){ stageSprite.setPosition(330, 8); }
	else { stageSprite.setPosition(30, 100); }

	if (m_stageToggle) {
		auto characterSelect = m_entityManager.addEntity("characterSelect");
		characterSelect->addComponent<CSprite>(Assets::getInstance().getTexture("StageSelect")).sprite;
		characterSelect->addComponent<CState>("background");
		auto& sprite = characterSelect->getComponent<CSprite>().sprite;
		sprite.setOrigin(0.f, 0.f);
		sprite.setPosition(0, 0);
		m_game->window().draw(sprite);
	}

	for (auto& e : m_entityManager.getEntities()) {
		if (!e->hasComponent<CSprite>() || e->getComponent<CState>().state == "background")
			continue;

		auto& sprite = e->getComponent<CSprite>().sprite;
		m_game->window().draw(sprite);
	}

	m_game->window().draw(stageSprite);

	if (m_notPlayable && !m_stageToggle) {
		sf::Text playable("Character cannot be played", Assets::getInstance().getFont("main"), 100);
		playable.setFillColor(sf::Color(255, 10, 0));
		playable.setOutlineColor(sf::Color(255, 255, 255));
		playable.setOutlineThickness(2);
		playable.setPosition(500, 400);
		m_game->window().draw(playable);
	}

	m_menuText.setFillColor(selectedColor);
	if (m_stageToggle) {
		m_menuText.setString(m_title);
	}
	else {
		m_menuText.setString("");
	}
	m_menuText.setPosition(m_game->window().getSize().x / 2.75, 10);
	m_game->window().draw(m_menuText);

	if (!m_stageToggle) {
		for (size_t i{ 0 }; i < m_menuNames.size(); ++i)
		{
			m_menuText.setFillColor((i == m_menuIndex ? selectedColor : nonSelectedColor));
			m_menuText.setOrigin(0, 0);
			m_menuText.setPosition(32 + (i + 1) * 250, 800);
			m_menuText.setString(m_menuNames.at(i));
			m_game->window().draw(m_menuText);
		}
	}

	else {
		for (size_t i{ 0 }; i < m_menuLevels.size(); ++i)
		{
			m_menuText.setFillColor((i == m_levelIndex ? selectedColor : nonSelectedColor));
			m_menuText.setOrigin(0, 0);
			m_menuText.setPosition(1000, 32 + (i + 1) * 150);
			m_menuText.setString(m_menuLevels.at(i));
			m_game->window().draw(m_menuText);
		}
	}
	sf::Text footer;
	//VERSUS MENU
	if (m_versus && !m_stageToggle) {
		sf::Text footer("Go Back: Q/ ESC	Change Stage: BackSpace	 Select Character: P/ Enter",
			Assets::getInstance().getFont("main"), 30);
		footer.setFillColor(footerColor);
		footer.setPosition(m_game->window().getSize().x - 750, 950);
		m_game->window().draw(footer);
	}
	//STAGE SELECT
	else if(m_stageToggle) {
		sf::Text footer("Go Back: Q/ ESC	Previous Stage: UP	Next Stage: DOWN	Select Stage: Spacebar",
			Assets::getInstance().getFont("main"), 30);
		footer.setFillColor(footerColor);
		footer.setPosition(m_game->window().getSize().x - 875, 950);
		m_game->window().draw(footer);
	}
	//TUTORIAL AND ARCADE
	else {
		sf::Text footer("Go Back: Q/ ESC	Select Character: P/ Enter",
			Assets::getInstance().getFont("main"), 30);
		footer.setFillColor(footerColor);
		footer.setPosition(m_game->window().getSize().x - 475, 950);
		m_game->window().draw(footer);
	}
}

void Scene_Character_Select::loadStage() {

	m_stage = m_entityManager.addEntity("Stage");
}


void Scene_Character_Select::sDoAction(const Command& action)
{
	if (action.type() == "START")
	{
		if (action.name() == "LEFT" && !m_stageToggle)
		{
			m_menuIndex = (m_menuIndex + m_menuNames.size() - 1) % m_menuNames.size();
		}
		else if (action.name() == "RIGHT" && !m_stageToggle)
		{
			m_menuIndex = (m_menuIndex + 1) % m_menuNames.size();
		}
		else if (action.name() == "UP" && m_stageToggle)
		{
			m_levelIndex = (m_levelIndex + m_menuLevels.size() - 1) % m_menuLevels.size();
		}
		else if (action.name() == "DOWN" && m_stageToggle)
		{
			m_levelIndex = (m_levelIndex + 1) % m_menuLevels.size();
		}
		else if (action.name() == "TOGGLE" && !m_stageToggle) {
			SoundPlayer::getInstance().play("Select");
			m_stageToggle = true;
		}
		else if (action.name() == "PLAY")
		{
			if (!m_stageToggle && !m_versus) {
					//Picked a Character
					SoundPlayer::getInstance().play("Select");
					if (m_menuIndex == 0) { m_enemy = "../assets/nina.txt"; }
					else if (m_menuIndex == 1) { m_enemy = "../assets/nina.txt"; }
					else if (m_menuIndex == 2) { m_enemy = "../assets/nina.txt"; }
					else if (m_menuIndex == 3) { m_enemy = "../assets/nina.txt"; }
					//														              PLAYER CHARACTER FILE      ENEMY           STAGE FILE              MODE FILE        
					m_game->changeScene("GAME" + m_number++, std::make_shared<Scene_Tussle>(m_game, m_characterPath[m_menuIndex], m_enemy,  m_stagePath[m_levelIndex], m_modeFilePath));
			} 
			else if (!m_stageToggle && m_versus) {
				if (!m_player1Picked) { m_character = m_characterPath[m_menuIndex]; m_player1Picked = true; }
				else {
					//Picked Two Characters
					SoundPlayer::getInstance().play("Select");
					//																   PLAYER CHARACTER	  SECOND PLAYER CHARACTER             STAGE FILE             MODE FILE
					m_game->changeScene("GAME" + m_number++, std::make_shared<Scene_Tussle>(m_game, m_character, m_characterPath[m_menuIndex], m_stagePath[m_levelIndex], m_modeFilePath));
				}
			}
			else {
				//Picked a stage, the index will be saved so no need for if statements
				SoundPlayer::getInstance().play("Select");
				m_stageToggle = false;
			}
		}
		else if (action.name() == "BACK") {
			if (!m_stageToggle) {  m_game->changeScene("MENU" + m_number, nullptr, false); }
			else { m_stageToggle = false; }
		}
	}


}

void Scene_Character_Select::loadLevel(const std::string& path) {
	std::ifstream config(path);
	if (config.fail()) {
		std::cerr << "Open file " << path << " failed\n";
		config.close();
		exit(1);
	}

	std::string token{ "" };
	config >> token;
	while (!config.eof()) {
		if (token == "Arcade") {
			m_arcade = true;
		}
		else if (token == "Versus") {
			m_versus = true;
		}
		else if (token == "Tutorial") {
			m_tutorial = true;
		}
		else if (token[0] == '#') {
			std::cout << token;
		}

		config >> token;
	}

	config.close();
}

