#include "Animation.h"
#include "Utilities.h"


Animation::Animation(const std::string &name,
                     const sf::Texture &t,
                     std::vector<sf::IntRect> frames,
                     sf::Time tpf,
                     bool repeats)
        : m_name(name)
        , m_frames(frames)
        , m_timePerFrame(tpf)
        , m_isRepeating(repeats)
        , m_countDown(tpf)
        , m_sprite(t, m_frames[0])
{
    centerOrigin(m_sprite);

    std::cout << name << " tpf: " << m_timePerFrame.asMilliseconds() << "ms\n";
}


void Animation::update(sf::Time dt) {

    m_countDown -= dt;
    if (m_countDown < sf::Time::Zero) {
        m_countDown = m_timePerFrame;
        m_currentFrame += 1;

        if ( m_currentFrame == m_frames.size()  && !m_isRepeating )
            return;  // on the last frame of non-repeating animaton, leave it
        else
            m_currentFrame = (m_currentFrame % m_frames.size());

        m_sprite.setTextureRect(m_frames[m_currentFrame]);
        centerOrigin(m_sprite);
    }
}


bool Animation::hasEnded() const {
    return (m_currentFrame >= m_frames.size());
}


const std::string &Animation::getName() const {
    return m_name;
}


sf::Sprite &Animation::getSprite() {
    return m_sprite;
}

sf::Vector2f Animation::getBB() const {
    return static_cast<sf::Vector2f>(m_frames[m_currentFrame].getSize());
}
