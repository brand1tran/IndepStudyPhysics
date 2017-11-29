//
//  Particle.cpp
//
//issues: speed and components of velocity not related to each other
#include "Particle.hpp"

//Constructs with position vector and color
Particle::Particle(const sf::Vector2f& position, sf::Color color) {
    display_rect.setPosition(position);
    display_rect.setFillColor(color);
}
void Particle::collide(Particle* other) {
    if(display_rect.getFillColor()==other->display_rect.getFillColor()){
        //create attraction
        //find center of mass of system
        //center vector remains the same
        //create attraction
    }
    float pi = 3.14159265359;
    float diag = sqrtf(10);
    float x = display_rect.getPosition().x;
    float y = display_rect.getPosition().y;
    float otherx = other->display_rect.getPosition().x;
    float othery = other->display_rect.getPosition().y;
    //trig to find theta between the two particles
    float d = pow((x-otherx),2) + pow((y-othery),2);
    float r = pow(d,0.5);
    float theta = atan((y-othery)/(x-otherx));
    float stheta = asin((y-othery)/r);
    float ctheta = acos((x-otherx)/r);
    
    //Augment dx/dy
    float interx = dx*(x-otherx) + dy*(y-othery);
    float intery = dx*(-1*(y-othery)) + dy*(x-otherx);
    
    float interotherx = other->dx*(x-otherx) + other->dy*(y-othery);
    float interothery = other->dx*(-1*(y-othery)) + other->dy*(x-otherx);
    
    float vpx = interotherx*(x-otherx) + intery*(-1*(y-othery));
    float vpy = interotherx*((y-othery)) + intery*(x-otherx);
    
    float vpotherx = interx*(x-otherx) + interothery*(-1*(y-othery));
    float vpothery = interx*((y-othery)) + interothery*(x-otherx);
    
    vpx = vpx/d;
    vpy = vpy/d;
    vpotherx = vpotherx/d;
    vpothery = vpothery/d;
    
    dx = vpx;
    dy = vpy;
    other->dx = vpotherx;
    other->dy = vpothery;
    
    //unstuck them from e/o not working
    /*
    if(ctheta < pi/4){
        other->display_rect.setPosition(x-Coltree::size-1, y+(diag*stheta));
    }
    else if(stheta > pi/4){
        other->display_rect.setPosition(x+(diag*ctheta), y-Coltree::size-1);
    }
    else if(ctheta > 3*pi/4){
        other->display_rect.setPosition(x+Coltree::size+1, y+(diag*stheta));
        
    }
    else if(stheta < -1*pi/4){
        other->display_rect.setPosition(x+(diag*ctheta), y+Coltree::size+1);
    }
    */
}
void Particle::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    target.draw(display_rect, states);
}
Coltree::Coltree(int depth,float t,float b,float l,float r) : top(t),bottom(b),left(l),right(r) {
    if (depth>0) {
        float midx = (left+right)/2.0;
        float midy = (top+bottom)/2.0;
        ul = new Coltree(depth-1,t,midy,l,midx);    //up left
        ll = new Coltree(depth-1,midy,b,l,midx);    //lower left
        ur = new Coltree(depth-1,t,midy,midx,r);    //up right
        lr = new Coltree(depth-1,midy,b,midx,r);    //lower right
    }
}
Coltree::~Coltree() {
    if (ul) delete ul;
    if (ll) delete ll;
    if (ur) delete ur;
    if (lr) delete lr;
}
void Coltree::insert(Particle* dennis) {
    float x = dennis->display_rect.getPosition().x;
    float y = dennis->display_rect.getPosition().y;
    float midx = (left+right)/2.0;
    float midy = (top+bottom)/2.0;
    if ((x<midx and x+dennis->display_rect.getSize().x>midx) or (y<midy and y+dennis->display_rect.getSize().y>midy) or ul==0) {
        int u;
        for (u=0;u<list.size();u++) {
            if (list[u]->display_rect.getPosition().x>dennis->display_rect.getPosition().x) {
                break;
            }
        }
        list.insert(list.begin()+u,dennis);
    } else {
        if (x<midx) {
            if (y<midy) {
                ul->insert(dennis);
            } else {
                ll->insert(dennis);
            }
        } else {
            if (y<midy) {
                ur->insert(dennis);
            } else {
                lr->insert(dennis);
            }
        }
    }
}
void Coltree::calculate(std::vector<Particle*>& alternate) {
    float furthest;
    if (ul) ul->calculate(alternate);
    if (ll) ll->calculate(alternate);
    if (ur) ur->calculate(alternate);
    if (lr) lr->calculate(alternate);
    int i2=0;
    for (int i=0;i<list.size();i++) {
        for (int w=i2;w<alternate.size();w++) {
            float x1=list[i]->display_rect.getPosition().x;
            float x2=alternate[w]->display_rect.getPosition().x;
            if (x2+alternate[w]->display_rect.getSize().x<x1) {
                i2=w;
            } else if (x2>x1+list[i]->display_rect.getSize().x) {
                break;
            } else {
                float y1=list[i]->display_rect.getPosition().y;
                float y2=alternate[w]->display_rect.getPosition().y;
                if ((y1>y2 and y1<y2+alternate[w]->display_rect.getSize().y) or (y2>y1 and y2<y1+list[i]->display_rect.getSize().y)) {
                    list[i]->collide(alternate[w]);
                }
            }
        }
    }
}
/*
void CollideBucket::crossCollide(std::vector<Particle *> & other) {
    int i2=0;
    for (int i=0;i<list.size();i++) {
        for (int w=i2;w<other.size();w++) {
            float x1=list[i]->display_rect.getPosition().x;
            float x2=other[w]->display_rect.getPosition().x;
            if (x2+list[i2]->display_rect.getSize().x<x1) {
                i2=w;
            } else if (x2>x1+list[i]->display_rect.getSize().x) {
                break;
            } else {
                float y1=list[i]->display_rect.getPosition().y;
                float y2=other[w]->display_rect.getPosition().y;
                if ((y1>y2 and y1<y2+list[i2]->display_rect.getSize().y) or (y2>y1 and y2<y1+list[i]->display_rect.getSize().y)) {
                    list[i]->collide(other[w]);
                }
            }
        }
    }
}
void CollideBucket::internalCollide() {
    for (int i=0;i<list.size();i++) {
        for (int i2=i+1;i2<list.size();i2++) {
            if (list[i2]->display_rect.getPosition().x>list[i]->display_rect.getPosition().x+list[i]->display_rect.getSize().x) {
                break;
            }
            float y1=list[i]->display_rect.getPosition().y;
            float y2=list[i2]->display_rect.getPosition().y;
            if ((y1>y2 and y1<y2+list[i2]->display_rect.getSize().y) or (y2>y1 and y2<y1+list[i]->display_rect.getSize().y)) {
                list[i]->collide(list[i2]);
                //list[i2]->collide(list[i]);
            }
        }
    }
}
*/
void Coltree::calculate() {
    for (int i=0;i<list.size();i++) {
        for (int i2=i+1;i2<list.size();i2++) {
            if (list[i2]->display_rect.getPosition().x>list[i]->display_rect.getPosition().x+list[i]->display_rect.getSize().x) {
                break;
            }
            float y1=list[i]->display_rect.getPosition().y;
            float y2=list[i2]->display_rect.getPosition().y;
            if ((y1>y2 and y1<y2+list[i2]->display_rect.getSize().y) or (y2>y1 and y2<y1+list[i]->display_rect.getSize().y)) {
                list[i]->collide(list[i2]);
                //list[i2]->collide(list[i]);
            }
        }
    }
    if (ul) ul->calculate(list);
    if (ll) ll->calculate(list);
    if (ur) ur->calculate(list);
    if (lr) lr->calculate(list);
    if (ul) ul->calculate();
    if (ll) ll->calculate();
    if (ur) ur->calculate();
    if (lr) lr->calculate();
}



















