//
//Created by ##########
//
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Font.hpp>
#include "Particle.hpp"
#include "ResourcePath.hpp"
#include "Player.hpp"

int main(int, char const**){
    //Constants
    float gravity = 0.5;
    float friction = 3;
    int sWidth = 1920;
    int sHeight = 1080;
    int size = 10;
    int pwidth = 50;
    int pheight = 100;
    int meteors = 10;
    std::string name = "Blob";

    //Objects
    std::vector<Particle> bank;
    for(int i = 0; i < 1000; i++){
        int tempx = rand() % sWidth;
        int tempy = rand() % sHeight;
        
        int r = rand() % 255;
        int g = rand() % 255;
        int b = rand() % 255;
        
        bank.push_back(Particle(sf::Vector2f(tempx,tempy), sf::Color(r,g,b)));
        bank[i].display_rect.setSize(sf::Vector2f(size,size));
    }
    //player class
    Player player(name, sf::Vector2f(0,0), sf::Color(255,255,255));
    player.display_rect.setSize(sf::Vector2f(pwidth,pheight));
    player.display_rect.setPosition(0, 0);
    bank.push_back(player);
    //set position of meteors
    for(int i = bank.size()-1-meteors; i < bank.size()-1; i++){
        bank[i].display_rect.setPosition(bank[i].display_rect.getPosition().x, -1000);
        bank[i].dx = rand() % 20 - 10;
    }
    //Create the main window
    sf::RenderWindow window(sf::VideoMode(sWidth, sHeight), "Indep Study Physics Engine");
    //Set the Icon of the App
    sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    window.setKeyRepeatEnabled(false);
    
    //FPS
    window.setFramerateLimit(60);
    
    //Loop
    while (window.isOpen())
    {
        //Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            //Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            //Escape pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
            
            //refresh
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                for(int i = 0; i<bank.size(); i++){
                    int tempx = rand() % sWidth;
                    int tempy = rand() % sHeight;
                    bank[i].display_rect.setPosition(tempx, tempy);
                    bank[i].dx = 0.0;
                    bank[i].dy = 0.0;
                    bank[i].speed = 5;
                }
            }
            //meteors
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::I){
                for(int i = bank.size()-1-meteors; i < bank.size()-1; i++){
                    bank[i].display_rect.setPosition(bank[i].display_rect.getPosition().x, -1000);
                    bank[i].dx = rand() % 20 - 10;
                    bank[i].dy = 50;
                }
            }
        }
        /*
        //refresh player values
        player.display_rect.setPosition(bank[bank.size()-2].display_rect.getPosition());
        player.dx = bank[bank.size()-2].dx;
        player.dy = bank[bank.size()-2].dy;
        //key press
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
            player.up();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            player.down();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            player.left();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            player.right();
        }
        //friction
        if(player.dx > 0){
            player.dx -= 1;
        }else if(player.dx < 0){
            player.dx += 1;
        }
        if(player.dy < 0){
           player.dy += 1;
        }
        //limits
        if(player.dx > 20){
            player.dx = 20;
        }
        if(player.dy > 20){
            player.dy = 20;
        }
        
        bank.pop_back();
        bank.push_back(player);
        bank[bank.size()-1].display_rect.move(player.dx, player.dy);
        
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            //shoot
            projectile += 1;
            sf::Vector2f pos = player.display_rect.getPosition();
            bank.push_back(Particle(pos, sf::Color(255,0,0)));
            bank[bank.size()-1].display_rect.setSize(sf::Vector2f(size,size));
            bank[bank.size()-1].dx = pos.x - event.mouseWheel.x;
            bank[bank.size()-1].dy = pos.y - event.mouseWheel.y;
            std::cout << "click";
        }
        */
        Coltree tree(2,0,sHeight,0,sWidth);
        for (int u=0;u<bank.size();u++) {
            tree.insert(&bank[u]);
        }
        tree.calculate();

        //Clear screen
        window.clear();

        //Draw
        for(int i = 0; i<bank.size(); i++){
            bank[i].dy += gravity;
            if(i != bank.size()-1){
                bank[i].display_rect.move(bank[i].dx, bank[i].dy);
            }
            //if hit y bound
            if(bank[i].display_rect.getPosition().y+size > sHeight){
                bank[i].display_rect.setPosition(bank[i].display_rect.getPosition().x, sHeight-size);
                bank[i].dy= -1* bank[i].dy;
                bank[i].dy += friction;
                if(bank[i].dy > 0){
                    bank[i].dy = 0;
                }
                if(bank[i].dx > 0){
                    bank[i].dx -= friction;
                    if(bank[i].dx < 0){
                        bank[i].dx = 0;
                    }
                }else if(bank[i].dx < 0){
                    bank[i].dx += friction;
                    if(bank[i].dx > 0){
                        bank[i].dx = 0;
                    }
                }
            }
            //if x<0
            if(bank[i].display_rect.getPosition().x < 0){
                bank[i].display_rect.setPosition(0, bank[i].display_rect.getPosition().y);
                bank[i].dx= -1 * bank[i].dx;
                bank[i].dx *= 0.5;
            }
            //if x>sWidth
            if(bank[i].display_rect.getPosition().x+size > sWidth){
                bank[i].display_rect.setPosition(sWidth-size, bank[i].display_rect.getPosition().y);
                bank[i].dx= -1 * bank[i].dx;
                bank[i].dx *= 0.5;
            }
        }
        for(int i = 0; i<bank.size(); i++){
            window.draw(bank[i].display_rect);
            bank[i].draw(window, sf::RenderStates::Default);
        }
        //Update the window
        window.display();
        
    }

    return EXIT_SUCCESS;
}


