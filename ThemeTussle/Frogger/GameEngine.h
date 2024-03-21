#pragma once


#include "Assets.h"

#include <memory>
#include <map>

class Scene;

using SceneMap = std::map<std::string, std::shared_ptr<Scene>>;

class GameEngine
{

public:
	sf::RenderWindow	        m_window;
	std::string			        m_currentScene;
	SceneMap			        m_sceneMap;
	size_t				        m_simulationSpeed{ 1 };
	bool				        m_running{ true };

	void						loadConfigFromFile(const std::string &path, unsigned int &width, unsigned int &height) const;
	void						init(const std::string& path);
	void						sUserInput();
	std::shared_ptr<Scene>		currentScene();

	// stats
	sf::Text					m_statisticsText;
	sf::Time					m_statisticsUpdateTime{sf::Time::Zero};
	unsigned int				m_statisticsNumFrames{0};

public:

	GameEngine(const std::string& path);

	void changeScene(const std::string& sceneName,
                     std::shared_ptr<Scene> scene,
                     bool endCurrentScene = false);

	void				quit();
	void				run();
	void				quitLevel();
	void				backLevel();

	sf::RenderWindow&	window();

	sf::Vector2f		windowSize() const;
	bool				isRunning();

};

