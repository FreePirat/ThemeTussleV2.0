#ifndef BREAKOUT_SCENE_BREAKOUT_H
#define BREAKOUT_SCENE_BREAKOUT_H

#include <SFML/System/Time.hpp>
#include <SFML/Graphics.hpp>
#include "EntityManager.h"
#include "Entity.h"
#include "Scene.h"
#include "GameEngine.h"

class Scene_Tussle : public Scene {
private:
    sPtrEntt        m_player{nullptr};
    sPtrEntt        m_enemy{nullptr};
    sf::View        m_worldView;
    sf::FloatRect   m_worldBounds;

    bool			m_drawTextures{true};
    bool			m_drawAABB{false};
    bool			m_drawGrid{false};

    bool            m_arcade{ false };
    bool            m_versus{ false };
    bool            m_tutorial{ false };

    bool            m_playerCanJump{ true };
    bool            m_playerCanMove{ true };
    bool            m_playerCanBeHit{ true };
    bool            m_playerHoldingDown{ false };
    bool			m_playerHasJumped{ false };
    int             m_playerHP{ 400 };
    int             m_playerHPCheck{ 400 };
    int             m_playerJumpHeight{ 600 };
    sf::Vector2f    m_playerPushbox{ 100, 100 };
    sf::Time        m_playerIdleTime = sf::seconds(10);
    sf::Vector2f    m_playerCurrentPushPos{ -30, 0 };
    
    int             m_hitComboNumber = { 0 };
    int             m_comboNumberChecker = { 0 };

    bool            m_enemyCanJump{ true };
    bool            m_enemyCanMove{ true };
    bool            m_enemyCanBeHit{ true };
    bool            m_enemyHoldingDown{ false };
    bool			m_enemyHasJumped{ false };
    int             m_enemyHP{ 400 };
    int             m_enemyHPCheck{ 400 };
    int             m_enemyJumpHeight{ 600 };
    sf::Vector2f    m_enemyPushbox{ 100, 100 };
    sf::Time        m_enemyIdleTime = sf::seconds(10);
    sf::Vector2f    m_enemyCurrentPushPos{ -30, 0 };

    float           m_gravity{ 10 };
    int             m_speed{ 10 };
    sf::Time        m_time = sf::seconds(120);

    #ifndef PlayerAnimations
    //FOR ALL THE ANIMATIONS (THIS LETS THEM BE DYNAMIC AS ALL CHARACTERS HAVE THESE)
    std::string     m_Default;
    std::string     m_Crouch;
    std::string     m_Jump;
    std::string     m_Idle;
    std::string     m_WalkForward;
    std::string     m_WalkBackwards;
    std::string     m_Hit;
    std::string     m_5L;
    std::string     m_5M;
    std::string     m_5H;
    std::string     m_2L;
    std::string     m_2M;
    std::string     m_2H;
    std::string     m_8L;
    std::string     m_8M;
    std::string     m_8H;
    std::string     m_6L;
    std::string     m_6M;
    std::string     m_6H;
    #endif

    #ifndef EnemyAnimations
    std::string     m_Default2;
    std::string     m_Crouch2;
    std::string     m_Jump2;
    std::string     m_Idle2;
    std::string     m_WalkForward2;
    std::string     m_WalkBackwards2;
    std::string     m_Hit2;
    std::string     m_5L2;
    std::string     m_5M2;
    std::string     m_5H2;
    std::string     m_2L2;
    std::string     m_2M2;
    std::string     m_2H2;
    std::string     m_8L2;
    std::string     m_8M2;
    std::string     m_8H2;
    std::string     m_6L2;
    std::string     m_6M2;
    std::string     m_6H2;
    #endif

    std::vector<sf::Vector2f>	m_playerHitboxPos;
    std::vector<sf::Vector2f>	m_playerHitboxSize;

    std::vector<sf::Vector2f>	m_playerHurtboxPos;
    std::vector<sf::Vector2f>	m_playerHurtboxSize;

    std::vector<sf::Vector2f>	m_enemyHitboxPos;
    std::vector<sf::Vector2f>	m_enemyHitboxSize;

    std::vector<sf::Vector2f>	m_enemyHurtboxPos;
    std::vector<sf::Vector2f>	m_enemyHurtboxSize;

    //systems
    void            sMovement(sf::Time dt);
    void            sCollisions();
    void            sUpdate(sf::Time dt);
    void            sAnimation(sf::Time dt);

    void	        onEnd() override;

    // helper functions
    void            playerMovement();
    void            adjustPlayerPosition();
    void            checkPlayerState();
    void            checkEnemyState();
    void	        registerActions();
    void            spawnPlayer();

    void            statePlayerCheck(std::string state, std::string animation,
        sf::Vector2f hurtboxSize, sf::Vector2f hurtboxPos, sf::Vector2f hitboxSize, sf::Vector2f hitboxPos, sf::Vector2f playerPos);

    void            stateEnemyCheck(std::string state, std::string animation,
        sf::Vector2f hurtboxSize, sf::Vector2f hurtboxPos, sf::Vector2f hitboxSize, sf::Vector2f hitboxPos, sf::Vector2f playerPos);

    void            stateCheckNohitBox(sPtrEntt character, std::string state, std::string animation,
        sf::Vector2f hurtboxSize, sf::Vector2f hurtboxPos, sf::Vector2f playerPos);

    void            init();
    void            loadLevel(const std::string &path);
    void            loadCharacter(const std::string& path);
    void            loadEnemy(const std::string& path);
    void            loadMode(const std::string& path);
    sf::FloatRect   getViewBounds();

public:

    Scene_Tussle(GameEngine *gameEngine, const std::string &characterPath, const std::string& enemyPath, const std::string& levelPath, const std::string& modePath);
    void		  update(sf::Time dt) override;
    void		  sDoAction(const Command& command) override;
    void		  sRender() override;

};

#endif //BREAKOUT_SCENE_BREAKOUT_H
