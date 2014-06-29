#include "../PlayerCommon.h"

#ifndef GUIXYPICKER_H
#define GUIXYPICKER_H

/**
 * \brief GUIXYPicker.h ...::: GUI XY Picker - Pick a 2D value! :::...
*
* This can be used to pick a 2 dimensional value 0-1f,0-1f. It's used to track the user's BCI state of
* watched axises or it's used for user to choose the desired song area.
*
* X - Axis in this project is Beta?
* Y - Axis in this project is Theta?
*
* author: tonis @ 2013
*
*/
class GUIXYPicker : public GUIElement
{
public:
    GUIXYPicker();

    GUIXYPicker(int xP, int yP, int widthP, int heightP, string toolTipText);
    void refresh();
    void draw();
    void init();
    void setStartUpValues(int xP, int yP, int widthP, int heightP);
    bool mouseIsInsidePanel();
    double getValX() const
    {
        return valX;
    }
    double getValY() const
    {
        return valY;
    }
    void setValX(double valXP) {
        valX = valXP;
    }
    void setValY(double valYP) {
        valY = valYP;
    }
    void setValues(double valXP, double valYP) {
        valX = valXP;
        valY = valYP;
    }
    virtual ~GUIXYPicker();

protected:
private:

    double x,y,width,height;
    double valX, valY;
    double valXraw, valYraw;
    ALLEGRO_BITMAP *mapBG;
    ALLEGRO_BITMAP *crossImg;
    ALLEGRO_BITMAP *circleImg;
    double boarder;
};

#endif // GUIXYPICKER_H
