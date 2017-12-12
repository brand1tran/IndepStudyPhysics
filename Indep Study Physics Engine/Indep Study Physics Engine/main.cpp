//
//Created by Brando
//Ideal Gas Branch
//the bonds lists retains information it is given
//bonds lists have problems of getting the input in event of a collision
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Font.hpp>
#include "Particle.hpp"
#include "ResourcePath.hpp"

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
    std::vector<Particle> bonda;
    std::vector<Particle> bondb;
    for(int i = 0; i < 100; i++){
        int tempx = rand() % sWidth;
        int tempy = rand() % sHeight;
        
        int type = rand() % 3;
        //testing value
        type = 2;
        
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
                }
            }
        }
        
        Coltree tree(2,0,sHeight,0,sWidth);
        for (int u=0;u<bank.size();u++) {
            tree.insert(&bank[u]);
        }
        
        //std::cout << bonda.size();
        for(int i=0; i<bonda.size();i++){
            std::cout << "fill tree bonds" << bonda.size() << "\n";
            tree.bond1.push_back(&bonda[i]);
            tree.bond2.push_back(&bondb[i]);
            std::cout << "filled tree" << tree.bond1.size() << "\n";
        }
        bonda.clear();
        bondb.clear();
        //std::cout << "Calculate" << "\n";
        tree.calculate();
        tree.attract();
        //suspected source of error
        std::cout << "Tree" << tree.bond1.size() << "\n";
        for(int i=0; i<tree.bond1.size(); i++){
            std::cout << "fill bonds" << tree.bond1.size() << "\n";
            bonda.push_back(*tree.bond1[i]);
            bondb.push_back(*tree.bond2[i]);
        }
        if(bonda.size() > 0){
            //break;
        }
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
        std::cout << "--------------------------" << "\n";
    }

    return EXIT_SUCCESS;
}


