#include "../include/Particle.h"
#include "../include/PlayerCommon.h"
#include "../include/AngleRoutines.h"

using namespace plrCommon;
using namespace std;

void Particle::reset()
{
    acc  = 0.03f;
    posZ = 0;
}

Particle::Particle()
{
    //ctor
}

Particle::Particle(Vec2f posTo, Vec2f speedTo, Vec2f screenSizeTo, float screenCornerDistanceTo, int partTypeTo, ALLEGRO_COLOR colorTo)
{
    reset();
    color = colorTo;
    //color = al_map_rgba_f(1.0f,1.0f,1.0f,1.0f);
    pos   = posTo;
    speed = speedTo;
    screenSize = screenSizeTo;
    speedAngleInRad = speed.radians();
    screenCornerDistance = screenCornerDistanceTo;
    partType = partTypeTo;
}

Particle::~Particle()
{
    //dtor
}

bool Particle::refresh(float directionPower)
{

    if (pos.x()<-64 || pos.x()>screenSize.x()+64 || pos.y()<-64 || pos.y()>screenSize.y()+64)
    {
        return false;
    }
    else
    {
        //speed = speed*(1.0f+(acc*deltaTime));
        //pos  += (speed*deltaTime);
        //float accFactorFromBrainWaves = ((bwMeter.getCurrentRAWFLowValue()+1.0f)/2.0f)*30.0f;
        float accFactorFromBrainWaves = ((getEffectiveValueOfEEG()+1.0f)/2.0f)*5.0f;

        accFactorFromBrainWaves *= accFactorFromBrainWaves;
        speed = speed*(1.0f+(acc*0.1f*accFactorFromBrainWaves));

        /* AIVAN SKITSO :DDDDD AAAHAHA
        float attentiveV  = 0.0f;
        if (bwMeter.getCurrentRAWFLowValue()>0.0f) {
            attentiveV = bwMeter.getCurrentRAWFLowValue();
            attentiveV = attentiveV*100.0f;
            attentiveV *= attentiveV;
            attentiveV /= 100.0f;
        }*/
        float attentiveV  = 0.0f;
        if (getEffectiveValueOfEEG()>0.0f)
        {
            attentiveV = getEffectiveValueOfEEG();
            attentiveV = attentiveV*17.5f;
            attentiveV *= attentiveV;
            attentiveV /= 17.5f;
        }

        Vec2f speedRotate;
        attentiveV *= directionPower;
        if (attentiveV>0.0f)
            speedRotate.setPolar( DegToRad(angleBetweenInDegrees(0,0,speed.x(),speed.y())+90.0f), attentiveV );
        else
        {
            attentiveV = -attentiveV;
            speedRotate.setPolar( DegToRad(angleBetweenInDegrees(0,0,speed.x(),speed.y())-90.0f), attentiveV );
        }
        //speedRotate.setPolar( speed.radians()+(2.0f*pi/4.0f)*attentiveV,speed.magnitude() );
        //speedRotate.setPolar( speed.radians(), getPositiveOfThis(speed.magnitude()) );
        speed  += speedRotate;

        pos  += (speed*0.1f);

        //posZ  = (getPositiveOfThis(screenSize.x()/2.0f - pos.x()) / (screenSize.x()/2.0f)); // pisteen suhdanne keskeltä reunalle.. 0 - 1. sqrt olisi hidas, optimoidaan x-akselin etäisyydel.. se mene kuitenki pois ruudusta päin
        posZ  = distanceBetweenPoints(pos.x(),pos.y(),screenSize.x()/2.0f,screenSize.y()/2.0f) / screenCornerDistance;
        if (posZ>1.0f)posZ=1.0f;
        if (posZ<0.0f)posZ=0.0f;
        return true;
    }
}
