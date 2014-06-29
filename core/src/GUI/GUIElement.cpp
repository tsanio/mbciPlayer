#include "../../include/GUI/GUIElement.h"
#include "../../include/PlayerCommon.h"

using namespace plrCommon;

GUIElement::GUIElement()
{
    //ctor
    name = "unnamed";
    id   = 0;
    visible = true;
    active = true;
    z = DEFAULT_Z_INDEX;
    toolTipX = 0;
    toolTipY = 0;
    toolTipWidth = 0;
    toolTipHeight = 0;
    toolTipText = "";
    storable = false;
    onToolTip = 0;
    elementList.addElement(this); // Add to elements also :)
}

GUIElement::~GUIElement()
{
    //dtor
}

void GUIElement::unLoad() {
    // Unload something If neccessary.. Called in the end of program
}

void GUIElement::refresh() {
    // Base class refresh..
}

void GUIElement::draw() {
    // The base class's draw.. Propably just empty to be placeholder for new classes.
    if (buttonFont) {
        if (onToolTip>0.0f&&toolTipText!="") {
            float alpha = onToolTip;
            float tipBoxStartX = guiMouse.getMouseX()-2;
            float tipBoxStartY = guiMouse.getMouseY()+18;
            float fontWidth = 6.8;
            al_draw_filled_rectangle(tipBoxStartX, tipBoxStartY, tipBoxStartX+toolTipText.length()*fontWidth, tipBoxStartY+12, al_map_rgba_f(0,0,0,alpha));
            ALLEGRO_COLOR toolTipFontColor = al_map_rgba_f(0.9*alpha, 0.9*alpha, 1.0*alpha, alpha); // <-- correct
            //al_draw_text(buttonFont, toolTipFontColor, guiMouse.getMouseX(), guiMouse.getMouseY()+15, ALLEGRO_ALIGN_CENTRE , toolTipText.c_str());
            al_draw_text(buttonFont, toolTipFontColor, tipBoxStartX+1, tipBoxStartY+1, ALLEGRO_ALIGN_LEFT , toolTipText.c_str());
        }

        if (guiMouse.getMouseX()>toolTipX&&guiMouse.getMouseX()<toolTipX+toolTipWidth&&guiMouse.getMouseY()>toolTipY&&guiMouse.getMouseY()<toolTipY+toolTipHeight&&guiMouse.mouseIdle()) {
            onToolTip+=deltaTime;
            onToolTip=limitValueUp(onToolTip,1.0f);
        } else {
            onToolTip-=deltaTime*0.5f;
            onToolTip=limitValueDown(onToolTip,0.0f);
        }
    }
}

void GUIElement::setToolTipArea(int xt, int yt, int width, int height, string text) {
    setToolTipAreaOnly(xt,yt,width,height);
    toolTipText = text;
}

void GUIElement::setToolTipAreaOnly(int xt, int yt, int width, int height) {
    toolTipX = xt;
    toolTipY = yt;
    toolTipWidth = width;
    toolTipHeight = height;
}

void GUIElement::setToolTipTextOnly(string text) {
    toolTipText = text;
}

void GUIElement::init() {
    // This is called after allegro initialization. So here is easy to load forexample fonts and similar..
}

void GUIElement::show() {
    visible = true;
}

void GUIElement::hide() {
    visible = false;
}

void GUIElement::passiveStateRefresh() {
    // Base for passive function that is called when object is not active.
}

void GUIElement::showAndActivate() {
    visible = true;
    active = true;
}

void GUIElement::hideAndUnactivate() {
    visible = false;
    active = false;
}

void GUIElement::setActivity(bool activity) {
    active = activity;
}

