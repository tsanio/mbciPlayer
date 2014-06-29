#include "GUIElement.h"

#ifndef GUISCROLLER_H
#define GUISCROLLER_H


#define HORISONTAL_AXIS 0
#define VERTICAL_AXIS   1
/**
 * \brief GUIScroller.h ...::: GUI Scroller - Sliiidiing we gooo! :::...
*
* This is the sliding bar which is now used mostly for tracking song position when played. Can be used on many other purposes as well.
*
* author: tonis @ 2013
*
*/
class GUIScroller : public GUIElement
{
public:
    GUIScroller();
    GUIScroller(float xP,float yP,float axisP,float lengthP,float thicknessP, float barLengthP, string toolTipText);
    GUIScroller(float xP,float yP,float axisP,float lengthP,float thicknessP, float barLengthP, int zIndexP, string toolTipText);
    virtual ~GUIScroller();
    void refresh();
    void draw();
    void init();
    float getX() { return x; }
    float getY() { return y; }
    void setStartUpValues(float xP,float yP,float axisP,float lengthP,float thicknessP, float barLengthP, int zIndexP, string toolTipText);
    bool mouseIsInsideScroller();
    bool mouseIsInsideScrollerSliderButton();
    float getLength() { return length; }
    bool positionChanged()
    {
        return changedPosition;
    }
    void setPosition(float setPos);
    float getPosition()
    {
        return position/length;
    }
    float getBarPosition()
    {
        return (showPosition/(length-barLength));
    }
    void updateScaledImages();
protected:
private:
    float x,y,axis,length,thickness,position,barLength;
    bool draggingTheScrollFromCursor,draggingTheScrollFromScrollSlider;
    float scrollSliderRelativeCurrentX,scrollSliderRelativeCurrentY,scrollSliderGlobalOrigoMouseX,scrollSliderGlobalOrigoMouseY,scrollSliderGlobalOrigoX,scrollSliderGlobalOrigoY;
    float buttonGlow;
    float showPosition;
    bool changedPosition, dragging, lastDraggingValue;
    ALLEGRO_BITMAP *imgScrollBarSelectionPart;
    ALLEGRO_BITMAP *imgScrollBarSelectionPartHover;
    ALLEGRO_BITMAP *imgScrollBarSelectionPartHoverScaled;
    ALLEGRO_BITMAP *imgScrollBarSelectionPartScaled;

};

#endif // GUISCROLLER_H
