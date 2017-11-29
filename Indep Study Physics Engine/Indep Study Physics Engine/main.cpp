//
//Created by ##########
//Ideal Gas Branch
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
    float friction = 3;
    int sWidth = 1920;
    int sHeight = 1080;
    int size = 10;
    int pwidth = 50;
    int pheight = 100;
    int meteors = 10;
    int element[3][3] = {
        {255, 0, 0},        //oxygen
        {200,200,200},      //carbon
        {255,255,0}         //sulfur
    };
    //Objects
    std::vector<Particle> bank;
    for(int i = 0; i < 1000; i++){
        int tempx = rand() % sWidth;
        int tempy = rand() % sHeight;
        
        int type = rand() % 3;
        
        int r = element[type][0];
        int g = element[type][1];
        int b = element[type][2];
        
        bank.push_back(Particle(sf::Vector2f(tempx,tempy), sf::Color(r,g,b)));
        bank[i].display_rect.setSize(sf::Vector2f(size,size));
        
        bank[i].dx = rand() % 2 - 1;
        bank[i].dy = rand() % 2 - 1;
    }
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
        }
        
        Coltree tree(2,0,sHeight,0,sWidth);
        for (int u=0;u<bank.size();u++) {
            tree.insert(&bank[u]);
        }
        tree.calculate();

        //Clear screen
        window.clear();

        //Draw
        for(int i = 0; i<bank.size(); i++){
            //move
            if(i != bank.size()-1){
                bank[i].display_rect.move(bank[i].dx, bank[i].dy);
            }
            //if hit ground
            if(bank[i].display_rect.getPosition().y+size > sHeight){
                bank[i].display_rect.setPosition(bank[i].display_rect.getPosition().x, sHeight-size);
                bank[i].dy= -1* bank[i].dy;
            }
            //if y<0
            if(bank[i].display_rect.getPosition().y < 0){
                bank[i].display_rect.setPosition(bank[i].display_rect.getPosition().x, 0);
                bank[i].dy *= -1;
            }
            //if x<0
            if(bank[i].display_rect.getPosition().x < 0){
                bank[i].display_rect.setPosition(0, bank[i].display_rect.getPosition().y);
                bank[i].dx= -1 * bank[i].dx;
            }
            //if x>sWidth
            if(bank[i].display_rect.getPosition().x+size > sWidth){
                bank[i].display_rect.setPosition(sWidth-size, bank[i].display_rect.getPosition().y);
                bank[i].dx= -1 * bank[i].dx;
            }
        }
        //draw
        for(int i = 0; i<bank.size(); i++){
            window.draw(bank[i].display_rect);
            bank[i].draw(window, sf::RenderStates::Default);
        }
        //update window
        window.display();
        
    }

    return EXIT_SUCCESS;
}


