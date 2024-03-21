#include <fstream>
#include <iostream>

#include "Scene_Tussle.h"
#include "Components.h"
#include "Physics.h"
#include "Utilities.h"
#include "MusicPlayer.h"
#include "Assets.h"
#include "SoundPlayer.h"


Scene_Tussle::Scene_Tussle(GameEngine* gameEngine, const std::string& characterPath, const std::string& enemyPath, const std::string& levelPath, const std::string& modePath)
    : Scene(gameEngine)
      , m_worldView(gameEngine->window().getDefaultView()) {
    loadLevel(levelPath);
    loadCharacter(characterPath);
    loadEnemy(enemyPath);
    loadMode(modePath);
    registerActions();

    MusicPlayer::getInstance().play("WesternTheme");

    init();
}


void Scene_Tussle::init() {
    spawnPlayer();
}

void Scene_Tussle::sMovement(sf::Time dt) {
    playerMovement();

    //Move all objects
    for (auto e: m_entityManager.getEntities()) {
        if (e->hasComponent<CTransform>()) {
            auto&tfm = e->getComponent<CTransform>();

            tfm.pos += tfm.vel * dt.asSeconds();
        }
    }

    auto& playerPos = m_player->getComponent<CTransform>().pos;
    auto& playerVel = m_player->getComponent<CTransform>().vel;

    auto& enemyPos = m_enemy->getComponent<CTransform>().pos;
    auto& enemyVel = m_enemy->getComponent<CTransform>().vel;

    auto worldPos = m_worldView.getSize();

    if (playerPos.y >= worldPos.y / 1.4) {
        playerVel.y = 0;
        m_playerCanJump = true;
        //On landing, give the player the idle stance
        if (m_playerHasJumped) {
            m_player->addComponent<CState>("5");
            m_playerHasJumped = false;
        }
    }
    else {
        playerVel.y += m_gravity;
        m_playerHasJumped = true;
    }

    if (enemyPos.y >= worldPos.y / 1.4) {
        enemyVel.y = 0;
        m_enemyCanJump = true;
        //On landing, give the player the idle stance
        if (m_enemyHasJumped) {
            m_enemy->addComponent<CState>("5");
            m_enemyHasJumped = false;
        }
    }
    else {
        enemyVel.y += m_gravity;
        m_enemyHasJumped = true;
    }
}

void Scene_Tussle::registerActions() {
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::Q, "QUIT");
    registerAction(sf::Keyboard::BackSpace, "PAUSE");
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");

    registerAction(sf::Keyboard::A, "LEFT");
    registerAction(sf::Keyboard::D, "RIGHT");
    registerAction(sf::Keyboard::W, "UP");
    registerAction(sf::Keyboard::S, "DOWN");

    registerAction(sf::Keyboard::T, "LIGHT");
    registerAction(sf::Keyboard::Y, "MEDIUM");
    registerAction(sf::Keyboard::U, "HEAVY");
    if (m_versus) {
        registerAction(sf::Keyboard::Left, "LEFT2");
        registerAction(sf::Keyboard::Right, "RIGHT2");
        registerAction(sf::Keyboard::Up, "UP2");
        registerAction(sf::Keyboard::Down, "DOWN2");

        registerAction(sf::Keyboard::I, "LIGHT2");
        registerAction(sf::Keyboard::O, "MEDIUM2");
        registerAction(sf::Keyboard::P, "HEAVY2");
    }
}


void Scene_Tussle::onEnd() {
    SoundPlayer::getInstance().play("Select");
    MusicPlayer::getInstance().play("MainTheme");
    m_game->changeScene("MENU", nullptr, false);
}

void Scene_Tussle::playerMovement() {

    auto& playerPos = m_player->getComponent<CTransform>().pos;
    auto& playerVel = m_player->getComponent<CTransform>().vel;

    auto& enemyPos = m_enemy->getComponent<CTransform>().pos;
    auto& enemyVel = m_enemy->getComponent<CTransform>().vel;

    auto& playerInput = m_player->getComponent<CInput>();
    auto& enemyInput = m_enemy->getComponent<CInput>();

    auto& playerSide = m_player->getComponent<CSide>().side;
    auto& enemySide = m_enemy->getComponent<CSide>().side;

    if (playerSide == "left") {
        m_player->getComponent<CTransform>().scale = { 1.f, 1.f };
    }
    else {
        m_player->getComponent<CTransform>().scale = { -1.f, 1.f };
    }

    auto overlap = Physics::getOverlap(m_player, m_enemy);

    playerVel.x = 0.f;
    enemyVel.x = 0.f;
    
    if (playerInput.LEFT)
        playerVel.x -= 40;

    if (enemyInput.LEFT2)
        enemyVel.x -= 40;
    

    if (playerInput.RIGHT)
        playerVel.x += 55;

    if (enemyInput.RIGHT2)
        enemyVel.x += 55;

    if (playerInput.UP) {
        playerInput.UP = false;
        playerVel.y = -m_playerJumpHeight;
    }

    if (enemyInput.UP2) {
        enemyInput.UP2 = false;
        enemyVel.y = -m_enemyJumpHeight;
    }

    playerVel.x = playerVel.x * m_speed;
    enemyVel.x = enemyVel.x * m_speed;

    if (overlap.x > 0 && overlap.y > 0) {
        if (playerVel.x > 0 && playerSide == "left") {

            if (enemyVel.x >= 0) {
                playerVel.x -= 70;
                enemyVel.x = playerVel.x;
            }
            else { playerVel.x = 0;     enemyVel.x = 0; }
        }
        else if (enemyVel.x > 0 && enemySide == "left") {

            if (playerVel.x >= 0) {
                enemyVel.x -= 70;
                playerVel.x = enemyVel.x;
            }
            else { playerVel.x = 0;     enemyVel.x = 0; }
        }
        else if (playerVel.x < 0 && playerSide == "right") {

            if (enemyVel.x <= 0) {
                playerVel.x += 70;
                enemyVel.x = playerVel.x;
            }
            else { playerVel.x = 0;     enemyVel.x = 0; }
        }
        else if (enemyVel.x < 0 && enemySide == "right") {

            if (playerVel.x <= 0) {
                enemyVel.x += 70;
                playerVel.x = enemyVel.x;
            }
            else { playerVel.x = 0;     enemyVel.x = 0; }
        }
    }

    m_player->getComponent<CBoundingBox>().pos = playerPos + m_playerCurrentPushPos;
    m_enemy->getComponent<CBoundingBox>().pos = enemyPos + m_enemyCurrentPushPos;

    auto& playerState = m_player->getComponent<CState>().state;
    auto& enemyState = m_enemy->getComponent<CState>().state;

    if(playerVel.x == 0 && playerVel.y == 0 && (playerState == "6" || playerState == "4")) { m_player->addComponent<CState>("5"); }
    if (enemyVel.x == 0 && enemyVel.y == 0 && (enemyState == "6" || enemyState == "4")) { m_enemy->addComponent<CState>("5"); }
}


void Scene_Tussle::sRender() {
    m_game->window().setView(m_worldView);

    sf::Text playerLives("Player 1 Health: " + std::to_string(m_playerHP), Assets::getInstance().getFont("main"));
    playerLives.setPosition(5, 20);
    sf::Text enemyLives("Enemy Health: " + std::to_string(m_enemyHP), Assets::getInstance().getFont("main"));
    enemyLives.setPosition(1605, 20);
    int currentTime = m_time.asSeconds();
    sf::Text time("Time: " + std::to_string(currentTime), Assets::getInstance().getFont("main"));
    time.setPosition(805, 20);

    // draw bkg first
    for (auto e: m_entityManager.getEntities("bkg")) {
        if (e->getComponent<CSprite>().has) {
            auto&sprite = e->getComponent<CSprite>().sprite;
            if(m_isPaused){ sprite.setColor(sf::Color(125, 125, 125)); }
            else { sprite.setColor(sf::Color(255, 255, 255)); }
            m_game->window().draw(sprite);
        }
        m_game->window().draw(playerLives);
        m_game->window().draw(enemyLives);       
        m_game->window().draw(time);
    }

    for (auto&e: m_entityManager.getEntities()) {
        if (!e->hasComponent<CAnimation>())
            continue;

        // Draw Sprite
        auto&anim = e->getComponent<CAnimation>().animation;
        auto&tfm = e->getComponent<CTransform>();
        anim.getSprite().setPosition(tfm.pos);
        anim.getSprite().setRotation(tfm.angle);
        m_game->window().draw(anim.getSprite());

        if (m_drawAABB) {
            if (e->hasComponent<CBoundingBox>()) {
                auto push = e->getComponent<CBoundingBox>();
                sf::RectangleShape pushRect;
                pushRect.setSize(sf::Vector2f{ push.size.x, push.size.y});
                centerOrigin(pushRect);
                pushRect.setPosition(e->getComponent<CBoundingBox>().pos);
                pushRect.setFillColor(sf::Color(0, 0, 0, 0));
                pushRect.setOutlineColor(sf::Color{0, 255, 0});
                pushRect.setOutlineThickness(2.f);
                m_game->window().draw(pushRect);

                auto hurt = e->getComponent<CHurtBox>();
                sf::RectangleShape hurtRect;
                hurtRect.setSize(sf::Vector2f{ hurt.size.x, hurt.size.y });
                centerOrigin(hurtRect);
                hurtRect.setPosition(e->getComponent<CHurtBox>().pos);
                hurtRect.setFillColor(sf::Color(0, 0, 0, 0));
                hurtRect.setOutlineColor(sf::Color{ 0, 0, 255 });
                hurtRect.setOutlineThickness(2.f);
                m_game->window().draw(hurtRect);
            }
            if (e->hasComponent<CHitBox>()) {
                auto hit = e->getComponent<CHitBox>();
                    sf::RectangleShape hitRect;
                    hitRect.setSize(sf::Vector2f{ hit.size.x, hit.size.y });
                    centerOrigin(hitRect);
                    hitRect.setPosition(e->getComponent<CHitBox>().pos);
                    hitRect.setFillColor(sf::Color(0, 0, 0, 0));
                    hitRect.setOutlineColor(sf::Color{ 255, 0, 0 });
                    hitRect.setOutlineThickness(2.f);
                    m_game->window().draw(hitRect);
            }
        }
    }

    if (m_isPaused) {
        sf::Text paused("PAUSED", Assets::getInstance().getFont("main"));
        paused.setPosition(600, 350);
        paused.setCharacterSize(200);
        m_game->window().draw(paused);
    }
}


void Scene_Tussle::update(sf::Time dt) {
    sUpdate(dt);
}

void Scene_Tussle::sDoAction(const Command&action) {

    auto playerState = m_player->getComponent<CState>().state;
    auto enemyState = m_enemy->getComponent<CState>().state;

    if (action.type() == "START") {
        if (action.name() == "PAUSE") { setPaused(!m_isPaused); SoundPlayer::getInstance().play("Select"); }
        else if (action.name() == "QUIT") { onEnd(); }

        else if (action.name() == "TOGGLE_TEXTURE") { m_drawTextures = !m_drawTextures; }
        else if (action.name() == "TOGGLE_COLLISION") { m_drawAABB = !m_drawAABB; }
        else if (action.name() == "TOGGLE_GRID") { m_drawGrid = !m_drawGrid; }

        if (m_playerCanMove) {
            //You can cancel anything into a jump
            if (action.name() == "UP") {
                if (m_playerCanJump) {
                    m_player->getComponent<CInput>().UP = true;
                    m_playerCanJump = false;
                    m_player->addComponent<CState>("8");
                }
            }

            //This denies being able to crouch while jumping.
            else if (action.name() == "DOWN" && m_playerCanJump) {
                m_player->getComponent<CInput>().DOWN = true;
                m_player->addComponent<CState>("2");
                m_player->getComponent<CInput>().LEFT = false;
                m_player->getComponent<CInput>().RIGHT = false;
            }

            //This denies being able to crouch while walking.
            else if (action.name() == "LEFT" && !m_player->getComponent<CInput>().DOWN) {
                m_player->getComponent<CInput>().LEFT = true;
                if (m_playerCanJump) { m_player->addComponent<CState>("4"); }
            }

            else if (action.name() == "RIGHT" && !m_player->getComponent<CInput>().DOWN) {
                m_player->getComponent<CInput>().RIGHT = true;
                if (m_playerCanJump) { m_player->addComponent<CState>("6"); }
            }

            //LIGHT ATTACKS
            else if (action.name() == "LIGHT") {
                if (playerState == "2") { m_player->addComponent<CState>("2L"); }
                else if (playerState == "8") { m_player->addComponent<CState>("8L"); }
                else if (playerState == "6") {
                    if (m_playerCanJump) { m_player->addComponent<CState>("6L"); }
                    else { m_player->addComponent<CState>("8L"); }
                }
                else if (playerState == "5" || playerState == "4") {
                    if (m_playerCanJump) { m_player->addComponent<CState>("5L"); }
                    else { m_player->addComponent<CState>("8L"); }
                }
            }

            //MEDIUM ATTACKS
            else if (action.name() == "MEDIUM") {
                if (playerState == "2") { m_player->addComponent<CState>("2M"); }
                else if (playerState == "8") { m_player->addComponent<CState>("8M"); }
                else if (playerState == "6") {
                    if (m_playerCanJump) { m_player->addComponent<CState>("6M"); }
                    else { m_player->addComponent<CState>("8M"); }
                }
                else if (playerState == "5" || playerState == "4") {
                    if (m_playerCanJump) { m_player->addComponent<CState>("5M"); }
                    else { m_player->addComponent<CState>("8M"); }
                }
            }

            //HEAVY ATTACKS
            else if (action.name() == "HEAVY") {
                if (playerState == "2") { m_player->addComponent<CState>("2H"); }
                else if (playerState == "8") { m_player->addComponent<CState>("8H"); }
                else if (playerState == "6") {
                    if (m_playerCanJump) { m_player->addComponent<CState>("6H"); }
                    else { m_player->addComponent<CState>("8H"); }
                }
                else if (playerState == "5" || playerState == "4") {
                    if (m_playerCanJump) { m_player->addComponent<CState>("5H"); }
                    else { m_player->addComponent<CState>("8H"); }
                }
            }
        }

        if (m_enemyCanMove) {
            if (action.name() == "UP2") {
                if (m_enemyCanJump) {
                    m_enemy->getComponent<CInput>().UP2 = true;
                    m_enemyCanJump = false;
                    m_enemy->addComponent<CState>("8");
                }
            }

            else if (action.name() == "DOWN2" && m_enemyCanJump) {
                m_enemy->getComponent<CInput>().DOWN2 = true;
                m_enemy->addComponent<CState>("2");
                m_enemy->getComponent<CInput>().LEFT2 = false;
                m_enemy->getComponent<CInput>().RIGHT2 = false;
            }

            else if (action.name() == "LEFT2" && !m_enemy->getComponent<CInput>().DOWN2) {
                m_enemy->getComponent<CInput>().LEFT2 = true;
                if (m_enemyCanJump) { m_enemy->addComponent<CState>("4"); }
            }

            else if (action.name() == "RIGHT2" && !m_enemy->getComponent<CInput>().DOWN2) {
                m_enemy->getComponent<CInput>().RIGHT2 = true;
                if (m_enemyCanJump) { m_enemy->addComponent<CState>("6"); }
            }

            else if (action.name() == "LIGHT2") {
                if (enemyState == "2") { m_enemy->addComponent<CState>("2L"); }
                else if (enemyState == "8") { m_enemy->addComponent<CState>("8L"); }
                else if (enemyState == "6") {
                    if (m_enemyCanJump) { m_enemy->addComponent<CState>("6L"); }
                    else { m_enemy->addComponent<CState>("8L"); }
                }
                else if (enemyState == "5" || enemyState == "4") {
                    if (m_enemyCanJump) { m_enemy->addComponent<CState>("5L"); }
                    else { m_enemy->addComponent<CState>("8L"); }
                }
            }

            else if (action.name() == "MEDIUM2") {
                if (enemyState == "2") { m_enemy->addComponent<CState>("2M"); }
                else if (enemyState == "8") { m_enemy->addComponent<CState>("8M"); }
                else if (enemyState == "6") {
                    if (m_enemyCanJump) { m_enemy->addComponent<CState>("6M"); }
                    else { m_enemy->addComponent<CState>("8M"); }
                }
                else if (enemyState == "5" || enemyState == "4") {
                    if (m_enemyCanJump) { m_enemy->addComponent<CState>("5M"); }
                    else { m_enemy->addComponent<CState>("8M"); }
                }
            }

            else if (action.name() == "HEAVY2") {
                if (enemyState == "2") { m_enemy->addComponent<CState>("2H"); }
                else if (enemyState == "8") { m_enemy->addComponent<CState>("8H"); }
                else if (enemyState == "6") {
                    if (m_enemyCanJump) { m_enemy->addComponent<CState>("6H"); }
                    else { m_enemy->addComponent<CState>("8H"); }
                }
                else if (enemyState == "5" || enemyState == "4") {
                    if (m_enemyCanJump) { m_enemy->addComponent<CState>("5H"); }
                    else { m_enemy->addComponent<CState>("8H"); }
                }
            }
        }
    }
    else if (action.type() == "END") {
        //PLAYER
        if (action.name() == "LEFT") { m_player->getComponent<CInput>().LEFT = false; }
        else if (action.name() == "RIGHT") { m_player->getComponent<CInput>().RIGHT = false; }
        else if (action.name() == "UP") { m_player->getComponent<CInput>().UP = false; }
        else if (action.name() == "DOWN") { m_player->getComponent<CInput>().DOWN = false; m_player->addComponent<CState>("5"); }

        //ENEMY
        if (action.name() == "LEFT2") { m_enemy->getComponent<CInput>().LEFT2 = false; }
        else if (action.name() == "RIGHT2") { m_enemy->getComponent<CInput>().RIGHT2 = false; }
        else if (action.name() == "UP2") { m_enemy->getComponent<CInput>().UP2 = false; }
        else if (action.name() == "DOWN2") { m_enemy->getComponent<CInput>().DOWN2 = false; m_enemy->addComponent<CState>("5"); }
    }
}


void Scene_Tussle::spawnPlayer() {

    auto playerPos = m_worldView.getSize();
    auto enemyPos = m_worldView.getSize();

    playerPos.x = playerPos.x / 5;
    playerPos.y = playerPos.y / 1.5;

    enemyPos.x = enemyPos.x - 300;
    enemyPos.y = enemyPos.y / 1.5;

    auto pushPlayerPos = sf::Vector2f(playerPos.x - 40, playerPos.y);
    auto pushEnemyPos = sf::Vector2f(enemyPos.x - 400, enemyPos.y);

    m_player = m_entityManager.addEntity("player");
    m_player->addComponent<CTransform>(playerPos);
    m_player->addComponent<CBoundingBox>(sf::Vector2f(m_playerPushbox), sf::Vector2f(pushPlayerPos));
    m_player->addComponent<CInput>();
    m_player->addComponent<CState>("5");

    m_enemy = m_entityManager.addEntity("enemy");
    m_enemy->addComponent<CTransform>(enemyPos);
    m_enemy->addComponent<CBoundingBox>(sf::Vector2f(m_enemyPushbox), sf::Vector2f(pushEnemyPos));
    m_enemy->addComponent<CInput>();
    m_enemy->addComponent<CState>("5");

}

sf::FloatRect Scene_Tussle::getViewBounds() {
    return sf::FloatRect();
}

void Scene_Tussle::sCollisions() {
    adjustPlayerPosition();

    auto playerHitsEnemy = Physics::getHitOverlap(m_player, m_enemy);
    auto enemyHitsPlayer = Physics::getHitOverlap(m_enemy, m_player);

    
    if (playerHitsEnemy.x > 0 && playerHitsEnemy.y > 0 && m_enemy->getComponent<CState>().state != "STUN") {
        m_enemy->addComponent<CState>("HIT");
    }

    if (enemyHitsPlayer.x > 0 && enemyHitsPlayer.y > 0 && m_player->getComponent<CState>().state != "STUN") {
        m_player->addComponent<CState>("HIT");
    }
}


void Scene_Tussle::sUpdate(sf::Time dt) {

    SoundPlayer::getInstance().removeStoppedSounds();
    m_entityManager.update();
    
    auto playerPos = m_player->getComponent<CTransform>().pos;
    auto playerVel = m_player->getComponent<CTransform>().vel;

    auto enemyPos = m_enemy->getComponent<CTransform>().pos;
    auto enemyVel = m_enemy->getComponent<CTransform>().vel;
    
    if (m_isPaused)
        return;

    auto sideCheck = playerPos.x - enemyPos.x;

    if (sideCheck >= 0) {
        m_player->addComponent<CSide>("right");
        m_enemy->addComponent<CSide>("left");
    }
    else {
        m_player->addComponent<CSide>("left");
        m_enemy->addComponent<CSide>("right");
    }

    int playerIdleTimer = m_playerIdleTime.asSeconds();
    if (playerIdleTimer != 0) {
        m_playerIdleTime -= dt;
    }
    else { m_player->addComponent<CState>("idleTimeout"); }

    int enemyIdleTimer = m_enemyIdleTime.asSeconds();
    if (enemyIdleTimer != 0) {
        m_enemyIdleTime -= dt;
    }
    else { m_enemy->addComponent<CState>("idleTimeout"); }

    int playerHurtTimer = m_playerHurtTimer.asSeconds();
    if (playerHurtTimer != 0) {
        m_playerHurtTimer -= dt;
    }
    else { m_playerCanMove = true; }

    int enemyHurtTimer = m_enemyHurtTimer.asSeconds();
    if (enemyHurtTimer != 0) {
        m_enemyHurtTimer -= dt;
    }
    else { m_enemyCanMove = true; }

    m_time -= dt;

    int timer = m_time.asSeconds();
    //if (timer == 0) {
        //FINISH THE GAME
    //}

    checkPlayerState();
    checkEnemyState();
    sAnimation(dt);
    sMovement(dt);
    sCollisions();

}


void Scene_Tussle::sAnimation(sf::Time dt) {

    for (auto e: m_entityManager.getEntities()) {
    
        if (e->hasComponent<CAnimation>()) {

            for (auto dead : m_entityManager.getEntities("dead")) {
                if (dead->getComponent<CAnimation>().animation.hasEnded()) {
                    dead->destroy();
                }
            }

            auto& playerAnim = m_player->getComponent<CAnimation>();
            playerAnim.animation.update(dt);

            for (auto player : m_entityManager.getEntities("player")) {
                if (playerAnim.animation.hasEnded()) {
                    if (m_player->getComponent<CInput>().DOWN && m_playerCanJump) { m_player->addComponent<CState>("2"); }
                    else if (!m_playerCanJump) { m_player->addComponent<CState>("8"); }
                    else {
                        if (m_player->getComponent<CState>().state != "STUN") { m_player->addComponent<CState>("5"); }
                        else {
                            m_player->addComponent<CState>("STUN");
                        }
                    }
                }
            }

            auto& enemyAnim = m_enemy->getComponent<CAnimation>();
            enemyAnim.animation.update(dt);

            for (auto enemy : m_entityManager.getEntities("enemy")) {
                if (enemyAnim.animation.hasEnded()) {
                    if (m_enemy->getComponent<CInput>().DOWN2 && m_enemyCanJump) { m_enemy->addComponent<CState>("2"); }
                    else if (!m_enemyCanJump) { m_enemy->addComponent<CState>("8"); }
                    else {
                        if (m_enemy->getComponent<CState>().state != "STUN") { m_enemy->addComponent<CState>("5"); }
                        else {
                            m_enemy->addComponent<CState>("STUN");
                        }
                    }
                }
            }
        }
    }
}


void Scene_Tussle::adjustPlayerPosition() {

    auto center = m_worldView.getCenter();
    sf::Vector2f viewHalfSize = m_worldView.getSize() / 2.f;

    auto left = center.x - viewHalfSize.x;
    auto right = center.x + viewHalfSize.x;
    auto top = center.y - viewHalfSize.y;
    auto bot = center.y + viewHalfSize.y;

    auto&player_pos = m_player->getComponent<CTransform>().pos;
    auto&enemy_pos = m_enemy->getComponent<CTransform>().pos;
    auto halfSize = sf::Vector2f{20, 20};

    // keep player in bounds
    player_pos.x = std::max(player_pos.x, left + halfSize.x);
    player_pos.x = std::min(player_pos.x, right - halfSize.x);
    player_pos.y = std::max(player_pos.y, top + halfSize.y);
    player_pos.y = std::min(player_pos.y, bot - halfSize.y);

    //keep enemy in bounds
    enemy_pos.x = std::max(enemy_pos.x, left + halfSize.x);
    enemy_pos.x = std::min(enemy_pos.x, right - halfSize.x);
    enemy_pos.y = std::max(enemy_pos.y, top + halfSize.y);
    enemy_pos.y = std::min(enemy_pos.y, bot - halfSize.y);
}

void Scene_Tussle::checkEnemyState() {

    auto enemyPos = m_enemy->getComponent<CTransform>().pos;
    auto enemyState = m_enemy->getComponent<CState>().state;
    auto enemyAnimName = m_enemy->getComponent<CAnimation>().animation.getName();

    stateCheckNohitBox(m_enemy, "5", m_Default2, m_enemyHurtboxSize[12], m_enemyHurtboxPos[12], enemyPos);
    stateCheckNohitBox(m_enemy, "6", m_WalkForward2, m_enemyHurtboxSize[12], m_enemyHurtboxPos[12], enemyPos);
    stateCheckNohitBox(m_enemy, "4", m_WalkBackwards2, m_enemyHurtboxSize[12], m_enemyHurtboxPos[12], enemyPos);
    stateCheckNohitBox(m_enemy, "8", m_Jump2, m_enemyHurtboxSize[14], m_enemyHurtboxPos[14], enemyPos);
    

    if (m_enemyCanJump && m_enemy->getComponent<CInput>().DOWN2 == true) {
        stateCheckNohitBox(m_enemy, "2", m_Crouch2, m_enemyHurtboxSize[13], m_enemyHurtboxPos[13], enemyPos);
    }

    stateCheck(m_enemy, "2L", m_2L2, m_enemyHurtboxSize[3], m_enemyHurtboxPos[3], m_enemyHitboxSize[3], m_enemyHitboxPos[3], enemyPos);
    stateCheck(m_enemy, "2M", m_2M2, m_enemyHurtboxSize[4], m_enemyHurtboxPos[4], m_enemyHitboxSize[4], m_enemyHitboxPos[4], enemyPos);
    stateCheck(m_enemy, "2H", m_2H2, m_enemyHurtboxSize[5], m_enemyHurtboxPos[5], m_enemyHitboxSize[5], m_enemyHitboxPos[5], enemyPos);

    stateCheck(m_enemy, "8L", m_8L2, m_enemyHurtboxSize[9], m_enemyHurtboxPos[9], m_enemyHitboxSize[9], m_enemyHitboxPos[9], enemyPos);
    stateCheck(m_enemy, "8M", m_8M2, m_enemyHurtboxSize[10], m_enemyHurtboxPos[10], m_enemyHitboxSize[10], m_enemyHitboxPos[10], enemyPos);
    stateCheck(m_enemy, "8H", m_8H2, m_enemyHurtboxSize[11], m_enemyHurtboxPos[11], m_enemyHitboxSize[11], m_enemyHitboxPos[11], enemyPos);

    stateCheck(m_enemy, "6L", m_6L2, m_enemyHurtboxSize[6], m_enemyHurtboxPos[6], m_enemyHitboxSize[6], m_enemyHitboxPos[6], enemyPos);
    stateCheck(m_enemy, "6M", m_6M2, m_enemyHurtboxSize[7], m_enemyHurtboxPos[7], m_enemyHitboxSize[7], m_enemyHitboxPos[7], enemyPos);
    stateCheck(m_enemy, "6H", m_6H2, m_enemyHurtboxSize[8], m_enemyHurtboxPos[8], m_enemyHitboxSize[8], m_enemyHitboxPos[8], enemyPos);

    stateCheck(m_enemy, "5L", m_5L2, m_enemyHurtboxSize[0], m_enemyHurtboxPos[0], m_enemyHitboxSize[0], m_enemyHitboxPos[0], enemyPos);
    stateCheck(m_enemy, "5M", m_5M2, m_enemyHurtboxSize[1], m_enemyHurtboxPos[1], m_enemyHitboxSize[1], m_enemyHitboxPos[1], enemyPos);
    stateCheck(m_enemy, "5H", m_5H2, m_enemyHurtboxSize[2], m_enemyHurtboxPos[2], m_enemyHitboxSize[2], m_enemyHitboxPos[2], enemyPos);

    if (enemyState == "HIT") {
        m_enemyCanMove = false;
        m_enemyHurtTimer = sf::seconds(2);
        m_enemyHP -= 5;
        m_enemy->addComponent<CState>("STUN");
    }

    if (enemyState == "idleTimeout" && enemyAnimName != m_Idle2) {
        m_enemy->addComponent<CAnimation>(Assets::getInstance().getAnimation(m_Idle2));
    }

    if (enemyState != "5") {
        m_enemyIdleTime = sf::seconds(10);
    }
}

void Scene_Tussle::checkPlayerState() {

    auto playerPos = m_player->getComponent<CTransform>().pos;
    auto playerState = m_player->getComponent<CState>().state;
    auto playerAnimName = m_player->getComponent<CAnimation>().animation.getName();

    stateCheckNohitBox(m_player, "5", m_Default, m_playerHurtboxSize[12], m_playerHurtboxPos[12], playerPos);
    stateCheckNohitBox(m_player, "6", m_WalkForward, m_playerHurtboxSize[12], m_playerHurtboxPos[12], playerPos);
    stateCheckNohitBox(m_player, "4", m_WalkBackwards, m_playerHurtboxSize[12], m_playerHurtboxPos[12], playerPos);
    stateCheckNohitBox(m_player, "8", m_Jump, m_playerHurtboxSize[14], m_playerHurtboxPos[14], playerPos);


    if (m_playerCanJump && m_player->getComponent<CInput>().DOWN == true) {
        stateCheckNohitBox(m_player, "2", m_Crouch2, m_playerHurtboxSize[13], m_playerHurtboxPos[13], playerPos);
    }

    stateCheck(m_player, "2L", m_2L, m_playerHurtboxSize[3], m_playerHurtboxPos[3], m_playerHitboxSize[3], m_playerHitboxPos[3], playerPos);
    stateCheck(m_player, "2M", m_2M, m_playerHurtboxSize[4], m_playerHurtboxPos[4], m_playerHitboxSize[4], m_playerHitboxPos[4], playerPos);
    stateCheck(m_player, "2H", m_2H, m_playerHurtboxSize[5], m_playerHurtboxPos[5], m_playerHitboxSize[5], m_playerHitboxPos[5], playerPos);

    stateCheck(m_player, "8L", m_8L, m_playerHurtboxSize[9], m_playerHurtboxPos[9], m_playerHitboxSize[9], m_playerHitboxPos[9], playerPos);
    stateCheck(m_player, "8M", m_8M, m_playerHurtboxSize[10], m_playerHurtboxPos[10], m_playerHitboxSize[10], m_playerHitboxPos[10], playerPos);
    stateCheck(m_player, "8H", m_8H, m_playerHurtboxSize[11], m_playerHurtboxPos[11], m_playerHitboxSize[11], m_playerHitboxPos[11], playerPos);

    stateCheck(m_player, "6L", m_6L, m_playerHurtboxSize[6], m_playerHurtboxPos[6], m_playerHitboxSize[6], m_playerHitboxPos[6], playerPos);
    stateCheck(m_player, "6M", m_6M, m_playerHurtboxSize[7], m_playerHurtboxPos[7], m_playerHitboxSize[7], m_playerHitboxPos[7], playerPos);
    stateCheck(m_player, "6H", m_6H, m_playerHurtboxSize[8], m_playerHurtboxPos[8], m_playerHitboxSize[8], m_playerHitboxPos[8], playerPos);

    stateCheck(m_player, "5L", m_5L, m_playerHurtboxSize[0], m_playerHurtboxPos[0], m_playerHitboxSize[0], m_playerHitboxPos[0], playerPos);
    stateCheck(m_player, "5M", m_5M, m_playerHurtboxSize[1], m_playerHurtboxPos[1], m_playerHitboxSize[1], m_playerHitboxPos[1], playerPos);
    stateCheck(m_player, "5H", m_5H, m_playerHurtboxSize[2], m_playerHurtboxPos[2], m_playerHitboxSize[2], m_playerHitboxPos[2], playerPos);

    if (playerState == "HIT") {
        m_playerCanMove = false;
        m_playerHurtTimer = sf::seconds(2);
        m_playerHP -= 5;
        m_player->addComponent<CState>("STUN");
    }

    if (playerState == "idleTimeout" && playerAnimName != m_Idle) {
        m_player->addComponent<CAnimation>(Assets::getInstance().getAnimation(m_Idle));
    }

    if (playerState != "5") {
        m_playerIdleTime = sf::seconds(10);
    }
}

void Scene_Tussle::stateCheck(sPtrEntt character, std::string state, std::string animation, 
    sf::Vector2f hurtboxSize, sf::Vector2f hurtboxPos, sf::Vector2f hitboxSize, sf::Vector2f hitboxPos, sf::Vector2f playerPos) {

    if (character->getComponent<CState>().state == state) {
        if (character->getComponent<CAnimation>().animation.getName() != animation) { character->addComponent<CAnimation>(Assets::getInstance().getAnimation(animation)); }

        character->addComponent<CHurtBox>(sf::Vector2f(hurtboxSize),
            sf::Vector2f(hurtboxPos.x + playerPos.x, hurtboxPos.y + playerPos.y));

        character->addComponent<CHitBox>(sf::Vector2f(hitboxSize),
            sf::Vector2f(hitboxPos.x + playerPos.x, hitboxPos.y + playerPos.y));
    }
}

void Scene_Tussle::stateCheckNohitBox(sPtrEntt character, std::string state, std::string animation,
    sf::Vector2f hurtboxSize, sf::Vector2f hurtboxPos, sf::Vector2f playerPos) {

    if (character->getComponent<CState>().state == state) {
        if (character->getComponent<CAnimation>().animation.getName() != animation) { character->addComponent<CAnimation>(Assets::getInstance().getAnimation(animation)); }

        character->addComponent<CHurtBox>(sf::Vector2f(hurtboxSize),
            sf::Vector2f(hurtboxPos.x + playerPos.x, hurtboxPos.y + playerPos.y));

        character->removeComponent<CHitBox>();
    }
}

void Scene_Tussle::loadLevel(const std::string&path) {
    std::ifstream config(path);
    if (config.fail()) {
        std::cerr << "Open file " << path << " failed\n";
        config.close();
        exit(1);
    }

    std::string token{""};
    config >> token;
    while (!config.eof()) {
        if (token == "Bkg") {
            std::string name;
            sf::Vector2f pos;
            config >> name >> pos.x >> pos.y;
            auto e = m_entityManager.addEntity("bkg");

            auto&sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
            sprite.setOrigin(0.f, 0.f);
            sprite.setPosition(pos);
        }
        else if (token[0] == '#') {
            std::cout << token;
        }

        config >> token;
    }

    config.close();
}

void Scene_Tussle::loadCharacter(const std::string& path) {
    std::ifstream config(path);
    if (config.fail()) {
        std::cerr << "Open file " << path << " failed\n";
        config.close();
        exit(1);
    }

    std::string token{ "" };
    config >> token;
    while (!config.eof()) {
        if (token == "DefaultAnimations") {
            config >> m_Default >> m_Crouch >> m_Jump >> m_Idle >> m_WalkForward >> m_WalkBackwards >> m_Hit;
        }
        else if (token == "StandingHits") {
            config >> m_5L >> m_5M >> m_5H;
        }
        else if (token == "CrouchingHits") {
            config >> m_2L >> m_2M >> m_2H;
        }
        else if (token == "JumpingHits") {
            config >> m_8L >> m_8M >> m_8H;
        }
        else if (token == "ForwardHits") {
            config >> m_6L >> m_6M >> m_6H;
        }
        else if (token == "HitboxPos") {
            sf::Vector2f pos;
            config >> pos.x >> pos.y;
            m_playerHitboxPos.push_back(pos);
        }
        else if (token == "HitboxSize") {
            sf::Vector2f size;
            config >> size.x >> size.y;
            m_playerHitboxSize.push_back(size);
        }
        else if (token == "HurtboxPos") {
            sf::Vector2f pos;
            config >> pos.x >> pos.y;
            m_playerHurtboxPos.push_back(pos);
        }
        else if (token == "HurtboxSize") {
            sf::Vector2f size;
            config >> size.x >> size.y;
            m_playerHurtboxSize.push_back(size);
        }
        else if (token == "Stats") {
            config >> m_playerHP >> m_playerJumpHeight >> m_playerPushbox.x >> m_playerPushbox.y;
        }
        else if (token[0] == '#') {
            std::cout << token << "\n";
        }

        config >> token;
    }

    config.close();
}

void Scene_Tussle::loadEnemy(const std::string& path) {
    std::ifstream config(path);
    if (config.fail()) {
        std::cerr << "Open file " << path << " failed\n";
        config.close();
        exit(1);
    }

    std::string token{ "" };
    config >> token;
    while (!config.eof()) {
        if (token == "DefaultAnimations") {
            config >> m_Default2 >> m_Crouch2 >> m_Jump2 >> m_Idle2 >> m_WalkForward2 >> m_WalkBackwards2 >> m_Hit2;
        }
        else if (token == "StandingHits") {
            config >> m_5L2 >> m_5M2 >> m_5H2;
        }
        else if (token == "CrouchingHits") {
            config >> m_2L2 >> m_2M2 >> m_2H2;
        }
        else if (token == "JumpingHits") {
            config >> m_8L2 >> m_8M2 >> m_8H2;
        }
        else if (token == "ForwardHits") {
            config >> m_6L2 >> m_6M2 >> m_6H2;
        }
        else if (token == "HitboxPos") {
            sf::Vector2f pos;
            config >> pos.x >> pos.y;
            m_enemyHitboxPos.push_back(pos);
        }
        else if (token == "HitboxSize") {
            sf::Vector2f size;
            config >> size.x >> size.y;
            m_enemyHitboxSize.push_back(size);
        }
        else if (token == "HurtboxPos") {
            sf::Vector2f pos;
            config >> pos.x >> pos.y;
            m_enemyHurtboxPos.push_back(pos);
        }
        else if (token == "HurtboxSize") {
            sf::Vector2f size;
            config >> size.x >> size.y;
            m_enemyHurtboxSize.push_back(size);
        }
        else if (token == "Stats") {
            config >> m_enemyHP >> m_enemyJumpHeight >> m_enemyPushbox.x >> m_enemyPushbox.y;
        }
        else if (token[0] == '#') {
            std::cout << token << "\n";
        }

        config >> token;
    }

    config.close();
}

void Scene_Tussle::loadMode(const std::string& path) {
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