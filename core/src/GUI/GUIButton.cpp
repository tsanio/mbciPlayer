#include <cmath>        // std::abs
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"
#include "../../include/GUI/GUIButton.h"
#include "../../include/PlayerCommon.h"
#include "../../include/GUI/GUIMouse.h"
#include "../../include/AngleRoutines.h"

using namespace plrCommon;
using namespace std;

void GUIButton::createButton(float px, float py, float pradius)
{
    //(old ctor 1)
    x = px;
    y = py;
    radius = pradius;
    width = radius*2.0f;
    height = radius*2.0f;
    type   = TYPE_CIRCULAR;
}


void GUIButton::createButton(float px, float py, float widthP, float heightP)
{
    //(old ctor 2)
    x = px;
    y = py;
    width  = widthP;
    height = heightP;
    type   = TYPE_RECTANGULAR;
}

void GUIButton::reset() {
    setUnitType(FLOAT_NUM_TYPE); // Set the ruler value type to show up.. float or int.
    value = 0.5f;
    manuallyLastSetValue=value;
    angle = 180;
    mouseWasOverThisWhenItWasPressedDown=false;
    iconPath="";
    buttonGlow=0.0f;
    state=false;
    clicked=false;
    mouseOver=false;
    text="";
    imgButtonUp    = NULL;
    imgButtonHover = NULL;
    imgButtonDown  = NULL;
    setZ(DEFAULT_Z_INDEX);
    setMin(0.0f);
    setMax(1.0f);
    setUnite("");
    warp=false;
}


GUIButton::GUIButton(float px, float py, float pradius, string toolTip)
{
    //ctor
    reset();
    createButton(px,py,pradius);
    width = radius*2.0f;
    height = radius*2.0f;
   // elementList.addElement(this);
   GUIElement::setToolTipArea(px-radius, py-radius, width, height, toolTip);
}

GUIButton::GUIButton(float px, float py, float pradius, int zIndexP, string toolTip)
{
    //ctor
    reset();
    setZ(zIndexP);
    createButton(px,py,pradius);
    width = radius*2.0f;
    height = radius*2.0f;
   // elementList.addElement(this);
   GUIElement::setToolTipArea(px-radius, py-radius, width, height, toolTip);
}
GUIButton::GUIButton(float px, float py, float pradius, string iconPathP, string toolTip)
{
    //ctor
    reset();
    iconPath = iconPathP;
    createButton(px,py,pradius);
    width = radius*2.0f;
    height = radius*2.0f;
   // elementList.addElement(this);
   GUIElement::setToolTipArea(px-radius, py-radius, width, height, toolTip);
}
GUIButton::GUIButton(float px, float py, float pradius, string iconPathP, string textT, string toolTip)
{
    //ctor
    reset();
    iconPath = iconPathP;
    createButton(px,py,pradius);
    width = radius*2.0f;
    height = radius*2.0f;
    text = textT;
   // elementList.addElement(this);
   GUIElement::setToolTipArea(px-radius, py-radius, width, height, toolTip);
}
GUIButton::GUIButton(float px, float py, float pradius, int zIndexP, string iconPathP, string toolTip)
{
    //ctor
    reset();
    iconPath = iconPathP;
    setZ(zIndexP);
    createButton(px,py,pradius);
    width = radius*2.0f;
    height = radius*2.0f;
    //elementList.addElement(this);
    GUIElement::setToolTipArea(px-radius, py-radius, width, height, toolTip);
}


GUIButton::GUIButton(float px, float py, float widthP, float heightP, string toolTip)
{
    //ctor
    reset();
    createButton(px,py,widthP,heightP);
   // elementList.addElement(this);
   GUIElement::setToolTipArea(px, py, widthP, heightP, toolTip);
}
GUIButton::GUIButton(float px, float py, float widthP, float heightP, int zIndexP, string toolTip)
{
    //ctor
    reset();
    setZ(zIndexP);
    createButton(px,py,widthP,heightP);
   // elementList.addElement(this);
   GUIElement::setToolTipArea(px, py, widthP, heightP, toolTip);
}

GUIButton::GUIButton(float px, float py, float widthP, float heightP, string iconPathP, string toolTip)
{
    //elementList.addElement(this);
    //ctor
    reset();
    iconPath = iconPathP;
    createButton(px,py,widthP,heightP);
   // elementList.addElement(this);
   GUIElement::setToolTipArea(px, py, widthP, heightP, toolTip);
}
GUIButton::GUIButton(float px, float py, float widthP, float heightP, int zIndexP, string iconPathP, string toolTip)
{
    //ctor
    reset();
    iconPath = iconPathP;
    setZ(zIndexP);
    createButton(px,py,widthP,heightP);
   // elementList.addElement(this);
   GUIElement::setToolTipArea(px, py, widthP, heightP, toolTip);
}
GUIButton::GUIButton(float px, float py, float widthP, float heightP, int zIndexP, string iconPathP, string txtT, string toolTip)
{
    //ctor
    reset();
    iconPath = iconPathP;
    setZ(zIndexP);
    createButton(px,py,widthP,heightP);
    setText(txtT);
   // elementList.addElement(this);
   GUIElement::setToolTipArea(px, py, widthP, heightP, toolTip);
}
GUIButton::GUIButton(float px, float py, float widthP, float heightP, int zIndexP, string iconPathP, string txtT, bool warpT, string toolTip)
{
    //ctor
    reset();
    iconPath = iconPathP;
    warp = warpT;
    setZ(zIndexP);
    createButton(px,py,widthP,heightP);
    setText(txtT);
    GUIElement::setToolTipArea(px, py, widthP, heightP, toolTip);
   // elementList.addElement(this);
}

GUIButton::GUIButton()
{
    //ctor
}

GUIButton::~GUIButton()
{
    //dtor
}

void GUIButton::init() {

    if (type==TYPE_CIRCULAR) {
        imgButtonUp    = imageLibrary.loadImage("media/gfx/button_1_round_up.png");
        imgButtonHover = imageLibrary.loadImage("media/gfx/button_1_round_hover.png");
        imgButtonDown  = imageLibrary.loadImage("media/gfx/button_1_round_down.png");
    }
    if (type==TYPE_ROLL_BUTTON) {
        imgButtonUp    = imageLibrary.loadImage("media/gfx/rollbuttonRoll.png");
        imgButtonHover = imageLibrary.loadImage("media/gfx/rollbuttonRollHover.png");
        imgButtonDown  = imageLibrary.loadImage("media/gfx/rollbuttonRollDown.png");
        imgButtonBase  = imageLibrary.loadImage("media/gfx/rollbuttonBase.png");
    }
    if (type==TYPE_RECTANGULAR) {
        imgButtonUp    = imageLibrary.loadImage("media/gfx/button_1_up.png");
        imgButtonHover = imageLibrary.loadImage("media/gfx/button_1_hover.png");
        imgButtonDown  = imageLibrary.loadImage("media/gfx/button_1_down.png");
    }
    if (type!=TYPE_NONE) {
        if (iconPath!="") imgButtonIcon  = imageLibrary.loadImage(iconPath);
        updateScaledPictures();
    }

}


void GUIButton::updateScaledPictures() {
    imgButtonInsideSize=68;
    imgButtonStartGapX=32;
    imgButtonStartGapY=32;
    scaleFactorX=width/imgButtonInsideSize;
    scaleFactorY=height/imgButtonInsideSize;
    imageSizeX = 128;
    imageSizeY = 128;

    imgButtonUpScaled    = getScaledImage(imgButtonUp,scaleFactorX,scaleFactorY);
    imgButtonHoverScaled = getScaledImage(imgButtonHover,scaleFactorX,scaleFactorY);
    imgButtonDownScaled  = getScaledImage(imgButtonDown,scaleFactorX,scaleFactorY);
    if (type==TYPE_ROLL_BUTTON) imgButtonBaseScaled  = getScaledImage(imgButtonBase,scaleFactorX,scaleFactorY);
    if (iconPath!="") imgButtonIconScaled  = getScaledImage(imgButtonIcon,scaleFactorX,scaleFactorY);
}

void GUIButton::drawTintedButtonEffectImage(ALLEGRO_BITMAP *img, float bAlpha) {
    if (type==TYPE_CIRCULAR) {
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE); // Nice tint
            al_draw_tinted_bitmap(img, al_map_rgba_f(1.0f*bAlpha, 1.0f*bAlpha, 1.0f*bAlpha, bAlpha), x-imgButtonStartGapX*scaleFactorX-imageSizeX/4, y-imgButtonStartGapY*scaleFactorY-imageSizeX/4, 0);
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA); // No tint for other drawing..
    }
    if (type==TYPE_ROLL_BUTTON) {
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE); // Nice tint
            al_draw_tinted_bitmap(img, al_map_rgba_f(1.0f*bAlpha, 1.0f*bAlpha, 1.0f*bAlpha, bAlpha), x-imageSizeX/2, y-imageSizeX/2, 0);
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA); // No tint for other drawing..
    }
    if (type==TYPE_RECTANGULAR) {
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE); // Nice tint
        if (!warp) {
            al_draw_tinted_bitmap(img, al_map_rgba_f(1.0f*bAlpha, 1.0f*bAlpha, 1.0f*bAlpha, bAlpha), x-imgButtonStartGapX*scaleFactorX, y-imgButtonStartGapY*scaleFactorY, 0);
        } else {
            float warpFactorX = (0.9f + cos(totalFrameTime)*0.2f);
            float warpFactorY = (0.9f + cos(totalFrameTime+totalFrameTime*0.1f)*0.2f);
            al_draw_tinted_bitmap(img, al_map_rgba_f(1.0f*bAlpha, 1.0f*bAlpha, 1.0f*bAlpha, bAlpha), x-imgButtonStartGapX*scaleFactorX*warpFactorX, y-imgButtonStartGapY*scaleFactorY*warpFactorY, 0);

        }
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA); // No tint for other drawing..
    }
}

// This is meant to work only on roll button now?
void GUIButton::drawRotatedTintedButtonEffectImage(ALLEGRO_BITMAP *img, float bAlpha, float angle2) {
    if (type==TYPE_ROLL_BUTTON) {
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE); // Nice tint
            al_draw_tinted_rotated_bitmap(img,al_map_rgba_f(1.0f*bAlpha, 1.0f*bAlpha, 1.0f*bAlpha, bAlpha),
                                           al_get_bitmap_width(img)/2, al_get_bitmap_height(img)/2,  x, y,
                                           DegToRad(angle2), 0);
            //al_draw_tinted_bitmap(img, al_map_rgba_f(1.0f*bAlpha, 1.0f*bAlpha, 1.0f*bAlpha, bAlpha), x-imgButtonStartGapX*scaleFactorX-imageSizeX/4, y-imgButtonStartGapY*scaleFactorY-imageSizeX/4, 0);
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA); // No tint for other drawing..
    }
}
void GUIButton::drawTintedButtonIconImage(ALLEGRO_BITMAP *img, float bAlpha) {
    if (type==TYPE_CIRCULAR) {
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE); // Nice tint
            al_draw_tinted_bitmap(img, al_map_rgba_f(1.0f*bAlpha, 1.0f*bAlpha, 1.0f*bAlpha, bAlpha), x-imageSizeX/4, y-imageSizeY/4, 0);
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA); // No tint for other drawing..
    }
    if (type==TYPE_ROLL_BUTTON) {
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE); // Nice tint
            al_draw_tinted_bitmap(img, al_map_rgba_f(1.0f*bAlpha, 1.0f*bAlpha, 1.0f*bAlpha, bAlpha), x-imageSizeX/4, y-imageSizeY/4, 0);
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA); // No tint for other drawing..
    }
    if (type==TYPE_RECTANGULAR) {
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE); // Nice tint
        if (!warp) {
            al_draw_tinted_bitmap(img, al_map_rgba_f(1.0f*bAlpha, 1.0f*bAlpha, 1.0f*bAlpha, bAlpha), x, y, 0);
        } else {
            al_draw_tinted_bitmap(img, al_map_rgba_f(1.0f*bAlpha, 1.0f*bAlpha, 1.0f*bAlpha, bAlpha), x, y-buttonGlow*5.0f, 0);

        }
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA); // No tint for other drawing..
    }
}

// This is called ifbutton is not active.. to unvaluate the clicked and mouseover..
void GUIButton::passiveStateRefresh() {
    clicked = false;
    mouseOver = false;
}

// Set activeState
void GUIButton::setActiveState(bool setState) {
    state = setState;
}

void GUIButton::refresh() {

    if (isVisible()) {

        // Simple buttooon logics for clicking :)
        clicked = false;
        if (!guiMouse.getLeftButtonState() && state==true && mouseOver && mouseWasOverThisWhenItWasPressedDown) // Was down.. and now up.. so clicked? And when first pressed down was it on the button or elsewehere and then slided? that's not good..
            clicked = true;

        state = false;
        if (guiMouse.getLeftButtonState() && mouseOver) // Mouse down and over.. and dragging started inside the button?
            state = true;

        // Mouse down and over the button? Then we maybe clicking on this perhaps?
        if (guiMouse.getLeftButtonState() && !guiMouse.getLeftButtonStatePrevious() && mouseOver)
            mouseWasOverThisWhenItWasPressedDown = true;
        if (!guiMouse.getLeftButtonState())
            mouseWasOverThisWhenItWasPressedDown = false;


        // How about mouse over hoovering? and mouse was clicked on button when started dragging?
        if (type==TYPE_CIRCULAR || type==TYPE_ROLL_BUTTON) {
            if (getDistance(x,y,guiMouse.getMouseX(),guiMouse.getMouseY()) < radius && getDistance(x,y,guiMouse.getMouseClickX(),guiMouse.getMouseClickY()) < radius)
                mouseOver = true;
            else {
                // Circular.. Then out if out..
                if (type==TYPE_CIRCULAR) {
                    mouseOver = false;
                } else {
                    // If rollable then it can be rolled until mouse is released.. Even outside of the rollbutton..
                    if (!guiMouse.getLeftButtonState())
                        mouseOver = false;
                }
            }
        } else { // Rectangle?
            if (guiMouse.getMouseX() > x && guiMouse.getMouseX() < x + width && guiMouse.getMouseY() > y && guiMouse.getMouseY() < y + height &&
                guiMouse.getMouseClickX() > x && guiMouse.getMouseClickX() < x + width && guiMouse.getMouseClickY() > y && guiMouse.getMouseClickY() < y + height
                )
                mouseOver = true;
            else
                mouseOver = false;
        }

        // How about refreshing the angle of the picture if it's roll button?
        // ..
        valueChanged = false;
        if (mouseOver && type==TYPE_ROLL_BUTTON && guiMouse.getLeftButtonState()) {
            /*if (!guiMouse.getLeftButtonStatePrevious()) { // If starting to press.. Take the initial angle and add it later to make more comfortable rolling
                rollStartAngle = angle;
                rollStartMouseAngle = angleBetweenInDegrees(x,y,guiMouse.getMouseX(),guiMouse.getMouseY());
            }
            angle = rollStartAngle+(rollStartMouseAngle-angleBetweenInDegrees(x,y,guiMouse.getMouseX(),guiMouse.getMouseY()));*/
            //^Still under construction :D
            angle = angleBetweenInDegrees(x,y,guiMouse.getMouseX(),guiMouse.getMouseY());
            value = ((angle+180)/3.6)/100.0f; // Rullan arvo on aina (0-100)/100 = 0-1.0
            //playerLog("Angle set to " + desToStr(angle));
        }
        if (value!=lastValue)
            valueChanged=true;
        lastValue=value;

    } else {
        clicked  = false; // It's invisible. No one is pushing the button really.
    }
}

void GUIButton::draw() {

        // Don't draw or hassle anything If we don't know yet the answer for the question.. Wether this is already rectangle or circle or IF this is not anything.. then don't act please as
        // we have empty pictures and it would cause some trouuuubles!
        if (type!=TYPE_NONE) {
            // Rectanglee drawing here.?
            if (!mouseOver) {
                //al_draw_rectangle(x, y, x+width, y+height, al_map_rgba_f(0.2*0.5f, 0.0*0.5f, 0.8*0.5f, 1.0f), 1);
                buttonGlow-=2.5f*deltaTime;
                if (buttonGlow<0.0f) buttonGlow=0.0f;
            } else {
                buttonGlow+=2.5*deltaTime;
                if (buttonGlow>1.0f) buttonGlow=1.0f;
                //al_draw_filled_rectangle(x, y, x+width, y+height, al_map_rgba_f(0.2*1.0f, 0.7*1.0f, 0.8*1.0f, 1.0f));
            }

            // Roll button? Draw base :)
            if (type==TYPE_ROLL_BUTTON)
                drawRotatedTintedButtonEffectImage(imgButtonBaseScaled,0.5f,0.0f);

            // Pressed or up button? Choose which to draw..
            if (mouseOver) {
                if (!guiMouse.getLeftButtonState()) {
                    if (type!=TYPE_ROLL_BUTTON) drawTintedButtonEffectImage(imgButtonUpScaled,0.2f);
                    else drawRotatedTintedButtonEffectImage(imgButtonUpScaled,0.2f,angle);
                } else {
                    if (type!=TYPE_ROLL_BUTTON) drawTintedButtonEffectImage(imgButtonDownScaled,1.0f);
                    else drawRotatedTintedButtonEffectImage(imgButtonDownScaled,value,angle);
                }
            } else {
                if (type!=TYPE_ROLL_BUTTON) drawTintedButtonEffectImage(imgButtonUpScaled,0.2f);
                    else drawRotatedTintedButtonEffectImage(imgButtonUpScaled,0.2f,angle);
            }
            // Glow always.. depending how much the cursor was on top of the button..
            // And If we are not pressing already the button.. This avoids the background button image when the button is down and it's smaller so ther's no double button showing up..:)
            // And the glopw is transfered to the button down image now as well
            if (!state && type==TYPE_CIRCULAR)
                drawTintedButtonEffectImage(imgButtonHoverScaled,buttonGlow*0.6f);

            if (!state && type==TYPE_ROLL_BUTTON)  // Draw the hovering
                drawRotatedTintedButtonEffectImage(imgButtonHoverScaled,buttonGlow*0.6f,0.0f);

            if (type==TYPE_ROLL_BUTTON) { // Draw the glow dot.. And Power Meter
                drawGlowDot(x+cos(DegToRad(angle))*12.0f,y+sin(DegToRad(angle))*12.0f,value);
                drawPowerMeter(x+width/2,y+height/2,width,height,value);
            }

            if (type==TYPE_RECTANGULAR) // Except let's leave rectangle glow all the time now..
                drawTintedButtonEffectImage(imgButtonHoverScaled,buttonGlow*0.6f);

            // Draw the icon always..
            if (iconPath!="") drawTintedButtonIconImage(imgButtonIconScaled,0.7f);
        }

        // Draw text also If that's wanted.
        if (text!="") {
            float alpha = 0.9f;
            if (type==TYPE_ROLL_BUTTON) {
                ALLEGRO_COLOR bFontColor  = al_map_rgba_f(0.9*alpha, 0.9*alpha, 1.0*alpha, alpha); // <-- correct
                al_draw_text(buttonFont, bFontColor, x-width*0.2f, y+height*0.55f, NULL, text.c_str());
                // Do we have also special value to show with this?
                if (valueUnit!="") {
                    float showValue = getValue()*(maximumValue-minimumValue)+minimumValue;
                    if (valueNumType==INT_NUM_TYPE)
                        al_draw_text(buttonFont, bFontColor, x+width*1.5f, y, NULL, (intToStr(int(showValue)) + valueUnit).c_str() );
                    else
                        al_draw_text(buttonFont, bFontColor, x+width*1.5f, y, NULL, (desToStr(showValue) + valueUnit).c_str() );
                }

            } else {
                ALLEGRO_COLOR bFontColor  = al_map_rgba_f(0.9*alpha, 0.9*alpha, 1.0*alpha, alpha); // <-- correct
                al_draw_text(buttonFont, bFontColor, x+width*1.1f, y+height/2-buttonFontSize/2, NULL, text.c_str());
            }
        }

        // Tooltip text
        GUIElement::draw();
}

void GUIButton::setPreviousValue() {
    setValue(manuallyLastSetValue);
}

// For rollbutton. Value is between min and max
void GUIButton::setValue(float valueToPut) {
    manuallyLastSetValue = value;
    value = (valueToPut-minimumValue)/(maximumValue-minimumValue);
    if (value<0.0f)
        value=0.0f;
    if (value>1.0f)
        value=1.0f;
    angle = -180.0f+(360.0f*value);
}
// For rollbutton. Value is between min and max. The case when upper roller of range is left under minim roll
void GUIButton::limitUpper(float valueToPut) {
    float factorInc = 0.05f;
    setValue(valueToPut+(maximumValue-minimumValue)*factorInc);
}
// For rollbutton. Value is between min and max. When maximum is rolled below minimum.. This is the minimum that is adjusted before maximum then..
void GUIButton::limitDowner(float valueToPut) {
    float factorInc = 0.05f;
    setValue(valueToPut-(maximumValue-minimumValue)*factorInc);
}
void GUIButton::setDimension(float wP, float hP) {
    width = wP;
    height = hP;
}
void GUIButton::setPosition(float xP, float yP) {
    x = xP;
    y = yP;
}
void GUIButton::setType(int typeP) {
    type = typeP;
}

void GUIButton::setUnitType(int NUM_TYPE_MODE) {
    valueNumType = NUM_TYPE_MODE;
}


