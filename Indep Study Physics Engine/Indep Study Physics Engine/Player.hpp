//
//  Player.hpp
//  Indep Study Physics Engine
//
//  Created by Brandon Tran on 10/20/17.
//  Copyright © 2017 BT Softworks. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp
#include "Particle.hpp"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <string.h>
class Player : public Particle{
public:
    Player(std::string name, const sf::Vector2f& position, sf::Color color);
    void checkname(std::string name);
    std::string name;
    int hp = 1000;
    //angle of Particle to cursor
    float Theta(sf::Vector2f position);
    void right();
    void left();
    void up();
    void down();
};

#endif /* Player_hpp */
