#ifndef BREAKOUT_UTILITIES_H
#define BREAKOUT_UTILITIES_H


#include <SFML/Graphics.hpp>
#include <iostream>



sf::Vector2f    normalize(sf::Vector2f v);
float           bearing(const sf::Vector2f& v);
sf::Vector2f    uVecBearing(float b);
float           length(const sf::Vector2f& v);
float           dist(const sf::Vector2f& u, const sf::Vector2f& v);


float radToDeg(float r);
float degToRad(float d);

template<typename T>
inline void centerOrigin(T &t) {
    auto bounds = t.getLocalBounds();
    t.setOrigin((bounds.width / 2.f) + bounds.left,
                (bounds.height / 2.f) + bounds.top);
}


template<typename T>
inline void centerOrigin(T *t) {
    auto bounds = t->getLocalBounds();
    t->setOrigin((bounds.width / 2.f) + bounds.left,
                (bounds.height / 2.f) + bounds.top);
}


template <typename T>
inline std::ostream& operator<<(std::ostream& os, const sf::Vector2<T>& v) {
    os << "{" << v.x << ", " << v.y << "}";
    return os;
}


template <typename T>
inline std::ostream& operator<<(std::ostream& os, const sf::Rect<T>& r) {
    os << "{{" << r.left << ", " << r.top << "}, {"<< r.width << ", " << r.height << "}";
    return os;
}


template <typename T>
inline void displayBounds(const T& t) {
    std::cout << "Local bounds : " << t.getLocalBounds() << "\n";
    std::cout << "Global bounds: " << t.getGlobalBounds() << "\n";
}





#endif //BREAKOUT_UTILITIES_H
