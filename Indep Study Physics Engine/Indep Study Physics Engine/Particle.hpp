//
//  Particle.hpp
//

#ifndef Particle_hpp
#define Particle_hpp

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <stdio.h>
#include <iostream>
#include <cmath>


struct Particle {
    Particle(const sf::Vector2f& position, sf::Color color);
    
    sf::RectangleShape display_rect;
    
    float speed=5;
    float dx=0;
    float dy=0;
    
    void collide(Particle* other);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
//collision tree
struct Coltree {
    constexpr static const float size=10;
    Coltree* ul=0;
    Coltree* ll=0;
    Coltree* ur=0;
    Coltree* lr=0;
    std::vector<Particle*> list;
    float top;
    float bottom;
    float left;
    float right;
    Coltree(int,float,float,float,float);
    ~Coltree();
    void insert(Particle*);
    void calculate();
    void calculate(std::vector<Particle*>&);
};

struct CollideBucket {
    std::vector<Particle*> list;
    void internalCollide();
    void crossCollide(std::vector<Particle*>&);
};
struct AltCollide {
    std::vector<CollideBucket> buckets;
    void insert(Particle*);
    AltCollide(int);
    void calculate();
};


#endif /* Particle_hpp */
