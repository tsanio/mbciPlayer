// Visualization of stars for the background.
#include "../include/BgEffect.h"
#include "../include/PlayerCommon.h"

using namespace plrCommon;
using namespace std;

void BgEffect::reset()
{
    center = Vec2f(0,0);
    particles.clear();
    rf = 0.0f;
    gf = 0.0f;
    bf = 0.0f;
    nextColorUpdateTime = 1.0f;
    nextDirectionChangeTime = 10.0f;
    lastDirectionUpdate = 0;
    directionPower = 0.0f;
    direction = 1;
    nextGenerateUpdate = 1.0f;
}

BgEffect::BgEffect()
{
    reset();//ctor
}

BgEffect::BgEffect(float screenSizeX, float screenSizeY)
{
    reset();
    screenSize = Vec2f(screenSizeX,screenSizeY);
    center = Vec2f(screenSizeX/2.0f,screenSizeY/2.0f);
    screenCornerDistance = distanceBetweenPoints(0,0,screenSize.x()/2.0f,screenSize.y()/2.0f);
}

BgEffect::~BgEffect()
{
    //dtor
}

void BgEffect::init()
{
    playerLog("Initializing Background Visualization.");
    starImgMain = imageLibrary.loadImage("media/gfx/starImgMed.png");
    for (int i=0; i<SCALE_DIMENSIONS; i++)
        starImgScaled[i] = getScaledImage(starImgMain,((i*0.1f)+0.03f)/2.0f,((i*0.1f)+0.03f)/2.0f);
    // Init images?
    lastGenerateUpdate = al_get_time();
    lastUpdateTime = al_get_time();
    lastUpdateColorTime = al_get_time();
    srand (time(NULL));
    //starBgImg = imageLibrary.createImage(screenSize.x(), screenSize.y());
    starBgImg = al_create_bitmap(screenSize.x(), screenSize.y());
}

void BgEffect::refresh()
{

    if (showVisualization.isChecked())
    {

        if (al_get_time() - lastUpdateTime > 0.02d)
        {
            lastUpdateTime = al_get_time();

            // Direnction power updates
            if (al_get_time() - lastDirectionUpdate > nextDirectionChangeTime)
            {
                lastDirectionUpdate = al_get_time();
                nextDirectionChangeTime = getRandomZeroToOne()+getRandomZeroToOne()*16.0f+5.0f;
                direction = -direction;
                // Let's make a rule.. If too powerful last time.. We need to slow down totally.!
                if (directionPower==1.0f||directionPower==-1.0f)
                {
                    nextDirectionChangeTime = 12.0f;
                    //playerLog("Was too powerful.");
                }
            }
            directionPower += direction*0.001f;
            if (directionPower>1.0f)
                directionPower=1.0f;
            if (directionPower<-1.0f)
                directionPower=-1.0f;

            vector<Particle>::iterator it = particles.begin();
            while(it != particles.end())   // Thanks stack overflow!
            {
                if(!(*it).refresh(directionPower))   // Is this particle out of range? Then remove it
                {
                    it = particles.erase(it);
                }
                else ++it;
            }
        }

        // Take color information according brainwave state
        float rawFlow = getEffectiveValueOfEEG(); // From -1 to 1

        /*
        float rf = rawFlow*2.0f;
        if (rf<0.0f) rf=0.0f;

        float gf = (getDistanceBetweenValues(0,rawFlow))*2.0f;
        limitValueUp(gf,1.0f);

        float bf = -rawFlow*2.0f;
        if (bf<0.0f) bf=0.0f;*/

        if (al_get_time() - lastUpdateColorTime > nextColorUpdateTime)
        {
            lastUpdateColorTime = al_get_time();
            nextColorUpdateTime = getRandomZeroToOne()+getRandomZeroToOne()*7.0f;
            gf = getRandomZeroToOne();
            bf = getRandomZeroToOne();
            rf = getRandomZeroToOne();
        }


        float minSpeed = 1.0f;

        // Generate the spawned particles to center
        if (al_get_time() - lastGenerateUpdate>nextGenerateUpdate)
        {

            lastGenerateUpdate = al_get_time();
            nextGenerateUpdate = getRandomZeroToOne()*0.030f+0.01f;

            float speedValue = 1.5f*((rawFlow+1.0f)/2.0f); // powerv x 0-1.0f

            float basicAmountOfSpawn = float(rand() % 10)+5;
            /*float increaseForSpawnByMeditation = 1.0f;
            if (rawFlow<0.0f)
                    increaseForSpawnByMeditation = (-bwMeter.getCurrentRAWFLowValue())*6.0f+1.0f;*/
            float increaseForSpawnByAttention = 1.0f;
            if (rawFlow>0.0f)
                increaseForSpawnByAttention = (getEffectiveValueOfEEG())*2.0f+1.0f;

            float decreaseForSpawnMeditation = 1.0f;
            if (getEffectiveValueOfEEG()<0.0f)
                decreaseForSpawnMeditation = 1.0f-(-getEffectiveValueOfEEG()*0.9f);

            //if ((rand() % 5) == 1) {
            for (int i=0; i<int(basicAmountOfSpawn*increaseForSpawnByAttention*decreaseForSpawnMeditation); i++)
            {
                float rndSpeedPower = getRandomZeroToOne()*5.0f+minSpeed+speedValue;
                Vec2f rndSpeed;
                int particleType = int(getRandomZeroToOne()*100.0f);
                if (particleType<90) particleType = 0;
                else particleType = 1;
                rndSpeed.setPolar(float(getRandomRad()),rndSpeedPower);
                particles.push_back(Particle(center, rndSpeed, screenSize, screenCornerDistance, particleType, al_map_rgb_f(rf,gf,bf)));
            }
            //}
        }
    }

}


void BgEffect::drawVisualization()
{

    if (showVisualization.isChecked())
    {

        float increaseLightMeditation = 1.0f;
        if (getEffectiveValueOfEEG()<0.0f)
            increaseLightMeditation = (-getEffectiveValueOfEEG())+1.0f;

        // Clear to color IF we'r meditating
//        if ((increaseLightMeditation*0.1f)>0.0f)
        // WOW SIISTI EFEKTI.. koko kuva smoothautuu :O -> al_clear_to_color(al_map_rgba_f(1.0f*(increaseLightMeditation*0.01f),1.0f*(increaseLightMeditation*0.01f),1.0f*(increaseLightMeditation*0.01f),(increaseLightMeditation*0.01f)));
//            al_clear_to_color(al_map_rgb_f(1.0f*(increaseLightMeditation*0.1f),1.0f*(increaseLightMeditation*0.1f),1.0f*(increaseLightMeditation*0.1f)));

        // Draw particles (Iterate to optimize a bit..)
        for (std::vector<Particle>::iterator it = particles.begin() ; it != particles.end(); ++it)
        {
            alpha = (*it).posZ*0.3f + 0.0f; // Center unvisible, corner yes visible..
            alpha *= increaseLightMeditation;
            if (alpha>1.0f) alpha = 1.0f;
            if ((*it).partType==1)
            {
                drawTintedPicture(starImgScaled[ int((*it).posZ*float(SCALE_DIMENSIONS-1)) ],
                                  al_map_rgba_f((*it).color.r*alpha, (*it).color.g*alpha, (*it).color.b*alpha, alpha),
                                  (*it).pos.x()-al_get_bitmap_width(starImgScaled[ int((*it).posZ*float(SCALE_DIMENSIONS-1)) ])/2.0f,
                                  (*it).pos.y()-al_get_bitmap_height(starImgScaled[ int((*it).posZ*float(SCALE_DIMENSIONS-1)) ])/2.0f,
                                  0);
            }
            else
            {
                alpha = (*it).posZ; // Center unvisible, corner yes visible..
                alpha *= increaseLightMeditation;
                if (alpha<0.1f)
                    alpha = 0.0f;
                else
                {
                    alpha = (alpha - 0.1f)/0.1f;
                    if (alpha>1.0f) alpha = 1.0f;
                }
                al_draw_pixel((*it).pos.x(), (*it).pos.y(), al_map_rgba_f((*it).color.r*alpha, (*it).color.g*alpha, (*it).color.b*alpha, alpha));
            }
        }
    }
}


void BgEffect::draw()
{
    // This is actually empty BECAUSE we want to draw it in background separately, not on the top of bg picture.. Maybe later this maybe changed..
}
