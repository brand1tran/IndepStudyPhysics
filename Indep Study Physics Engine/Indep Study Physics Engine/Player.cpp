//
//  Player.cpp
//  Indep Study Physics Engine
//
//  Created by Brandon Tran on 10/20/17.
//  Copyright © 2017 BT Softworks. All rights reserved.
//
#include "Player.hpp"
Player::Player(std::string name, const sf::Vector2f& position, sf::Color color) : Particle(position, color){
    this->name = name;
    checkname(this->name);
}

void Player::checkname(std::string name){
    if(name.size() == 0){
        name = "Bill";
    }
}

float Player::Theta(sf::Vector2f position){
    dx = display_rect.getPosition().x - position.x;
    dy = display_rect.getPosition().y - position.y;
    return 1.1;
}
void Player::right(){
    dx += 5;
}
void Player::left(){
    dx -= 5;
}
void Player::up(){
    dy -= 5;
}
void Player::down(){
    dy += 5;
}
