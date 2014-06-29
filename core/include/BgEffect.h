#include <vector>
#include "Particle.h"
#include "GUI/GUIElement.h"
#include "PlayerCommon.h"
#include "Vec2.h"
#include "AngleRoutines.h"

#ifndef BGEFFECT_H
#define BGEFFECT_H

#define SCALE_DIMENSIONS 30
/**
 * \brief BgEffect.h ...::: Background Effect - Eye candy. :::...
*
* This class is used just to draw nice visualization on the backgroun.
*
* author: tonis @ 2013
*
*/
class BgEffect : public GUIElement
{
public:
    BgEffect();
    BgEffect(float screenSizeX, float screenSizeY);
    void refresh();
    void init();
    void reset();
    void draw();
    void drawVisualization();
    virtual ~BgEffect();
protected:
private:
    double lastGenerateUpdate;
    double nextGenerateUpdate;
    double lastUpdateTime;
    vector<Particle> particles;
    ALLEGRO_BITMAP *starBgImg;
    ALLEGRO_BITMAP *starImgMain;
    ALLEGRO_BITMAP *starImgMain2;
    ALLEGRO_BITMAP *starImgMain3;
    ALLEGRO_BITMAP *starImgScaled[SCALE_DIMENSIONS];
    ALLEGRO_BITMAP *starImgScaled2[SCALE_DIMENSIONS];
    ALLEGRO_BITMAP *starImgScaled3[SCALE_DIMENSIONS];
    Vec2f center;
    Vec2f screenSize;
    float screenCornerDistance;
    float alpha;
    int colorMode;
    double lastUpdateColorTime;
    double nextColorUpdateTime;

    double nextDirectionChangeTime;
    double lastDirectionUpdate;
    float directionPower;
    int direction;

    float rf;
    float gf;
    float bf;

};

#endif // BGEFFECT_H
