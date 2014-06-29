#ifndef GUIBUTTON_H
#define GUIBUTTON_H

#include <string>
#include <allegro5/allegro.h>
#include "GUIElement.h"

#define TYPE_CIRCULAR    0
#define TYPE_RECTANGULAR 1
#define TYPE_ROLL_BUTTON 3
#define TYPE_NONE        4

#define FLOAT_NUM_TYPE   0
#define INT_NUM_TYPE     1

/**
 * \brief GUIButton.h ...::: GUI Element - Button :::...
 *
 *
 * This is single button element of the GUI system. Belongs to elements of GUI.
 *
 * author: tonis @ 2013
 *
 */
class GUIButton : public GUIElement
{
public:
    GUIButton();
    GUIButton(float x, float y, float radius, string toolTip);
    GUIButton(float x, float y, float radius, string iconPathP, string toolTip);
    GUIButton(float x, float y, float radius, string iconPathP, string textT, string toolTip);
    GUIButton(float x, float y, float radius, int zIndex, string toolTip);
    GUIButton(float x, float y, float radius, int zIndex, string iconPathP, string toolTip);

    GUIButton(float x, float y, float width, float height, string toolTip);
    GUIButton(float x, float y, float width, float height, string iconPathP, string toolTip);
    GUIButton(float x, float y, float width, float height, int zIndexP, string toolTip);
    GUIButton(float x, float y, float width, float height, int zIndex, string iconPathP, string toolTip);
    GUIButton(float px, float py, float widthP, float heightP, int zIndexP, string iconPathP, string txtT, string toolTip);
    GUIButton(float px, float py, float widthP, float heightP, int zIndexP, string iconPathP, string txtT, bool warp, string toolTip);


    void createButton(float x, float y, float radius); // Circular
    void createButton(float x, float y, float width, float height); // Rectangle
    virtual ~GUIButton();
    bool wasClicked()
    {
        return clicked;
    }
    void clickMe()
    {
        clicked=true;
    }
    bool valueWasChanged()
    {
        return valueChanged;
    }
    void setPreviousValue();
    void setActiveState(bool setState);
    void refresh();
    void draw();
    void reset();
    void init();
    string getTxt() {return text; }
    void passiveStateRefresh();
    void updateScaledPictures();
    void drawTintedButtonEffectImage(ALLEGRO_BITMAP *img, float bAlpha);
    void drawRotatedTintedButtonEffectImage(ALLEGRO_BITMAP *img, float bAlpha, float angle2);
    void drawTintedButtonIconImage(ALLEGRO_BITMAP *img, float bAlpha);
    void setText(string txt)
    {
        text = txt;
    }
    string getText()
    {
        return text;
    }
    void setDimension(float pw, float ph);
    void setPosition(float px, float py);
    void setMin(float minV)
    {
        minimumValue = minV;
    }
    void setMax(float maxV)
    {
        maximumValue = maxV;
    }
    float getMin()
    {
        return minimumValue;
    }
    float getMax()
    {
        return maximumValue;
    }
    string getUnitType() {
        return valueUnit;
    }
    void setUnite(string unitTxt)
    {
        valueUnit = unitTxt;
    }
    void setType(int ptype);
    int getType()
    {
        return type;
    }
    float getY()
    {
        return y;
    }
    float getX()
    {
        return x;
    }
    float getHeight()
    {
        return height;
    }
    float getWidth()
    {
        return width;
    }
    float getValue()
    {
        return value;
    }
    bool getValueChanged() {
        return valueChanged;
    }
    float getConvertedValue()
    {
        return ((value*(maximumValue-minimumValue))+minimumValue);
    }
    void setValue(float putThisValue);
    void limitDowner(float putThisValue);
    void limitUpper(float putThisValue);
    float getScaleFactorX()
    {
        return scaleFactorX;
    }
    float getScaleFactorY()
    {
        return scaleFactorY;
    }
    void setUnitType(int NUM_TYPE_MODE);
protected:
private:
    int type;
    int zIndex;
    float x, y;
    float width, height;
    float radius;
    float buttonGlow;
    float angle;
    float rollStartAngle;
    float rollStartMouseAngle;
    float value;
    float manuallyLastSetValue;
    bool valueChanged;
    float lastValue;
    bool state;
    bool mouseWasOverThisWhenItWasPressedDown;
    bool clicked;
    bool mouseOver;
    bool warp;
    int valueNumType;
    string text;
    string iconPath;
    ALLEGRO_BITMAP *imgButtonBase;
    ALLEGRO_BITMAP *imgButtonBaseScaled;
    ALLEGRO_BITMAP *imgButtonIcon;
    ALLEGRO_BITMAP *imgButtonIconScaled;
    ALLEGRO_BITMAP *imgButtonUp;
    ALLEGRO_BITMAP *imgButtonHover;
    ALLEGRO_BITMAP *imgButtonDown;
    ALLEGRO_BITMAP *imgButtonUpScaled;
    ALLEGRO_BITMAP *imgButtonHoverScaled;
    ALLEGRO_BITMAP *imgButtonDownScaled;
    float imgButtonInsideSize;
    float imgButtonStartGapX;
    float imgButtonStartGapY;
    float scaleFactorX;
    float scaleFactorY;
    float imageSizeX;
    float imageSizeY;
    float minimumValue;
    float maximumValue;
    string valueUnit;
};

#endif // GUIBUTTON_H
