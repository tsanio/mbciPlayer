#include <allegro5/allegro.h>
#include "Vec2.h"

#ifndef PARTICLE_H
#define PARTICLE_H

/**
 * \brief Particle.h ...::: Particle - A part of BG Effect :::...
*
* Just one tiny particle of background effect.
*
* author: tonis @ 2013
*
*/
class Particle
{
public:
    Particle();
    Particle(Vec2f posTo, Vec2f speedTo, Vec2f screenSizeTo, float screenCornerDistance, int partTypeTo, ALLEGRO_COLOR colorTo);
    virtual ~Particle();
    float speedAngleInRad;
    Vec2f pos;
    Vec2f speed;
    Vec2f screenSize;
    float acc;
    float posZ;
    int partType;
    bool refresh(float directionPower);
    void reset();
    float screenCornerDistance;
    ALLEGRO_COLOR color;
protected:
private:
};

#endif // PARTICLE_H
