#include <cmath>        // std::abs
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"
#include "../../include/GUI/GUIButton.h"
#include "../../include/PlayerCommon.h"
#include "../../include/GUI/GUICheckBox.h"

using namespace plrCommon;
using namespace std;

GUICheckBox::GUICheckBox()
{
    //ctor
}

GUICheckBox::~GUICheckBox()
{
    //dtor
}

void GUICheckBox::setUp(float px, float py, string txt) {
    reset();
    setPosition(px,py);
    setText(txt);
    setDimension(16,16);
    setType(TYPE_RECTANGULAR);
    checked = false;
}

// Without coordinates. Mostly used for radiobuttons..
GUICheckBox::GUICheckBox(string txt, string toolTipText) {
    setUp(0, 0, txt);
    GUIElement::setToolTipArea(0,0,0,0,toolTipText); // This will be later changed to proper position.
}

GUICheckBox::GUICheckBox(float px, float py, string txt, string toolTipText) {
    setUp(px, py, txt);
    GUIElement::setToolTipArea(px,py,10,10,toolTipText);
}
GUICheckBox::GUICheckBox(float px, float py, string txt, string toolTipText, bool storableP) {
    setUp(px, py, txt);
    GUIElement::setToolTipArea(px,py,10,10,toolTipText);
    setStorable(storableP);
}
GUICheckBox::GUICheckBox(float px, float py, string txt, bool val, string toolTipText) {
    setUp(px, py, txt);
    checked = val;
    GUIElement::setToolTipArea(px,py,10,10,toolTipText);
}
GUICheckBox::GUICheckBox(float px, float py, string txt, bool val, string toolTipText, bool storableP) {
    setUp(px, py, txt);
    checked = val;
    GUIElement::setToolTipArea(px,py,10,10,toolTipText);
    setStorable(storableP);
}

void GUICheckBox::init() {
    GUIButton::init();


    if (getType()==TYPE_CIRCULAR) {
                    //..
    } else {
        imgCheckedIcon   = imageLibrary.loadImage("media/gfx/checked.png");
    }

    imgCheckedIconScaled = getScaledImage(imgCheckedIcon,getScaleFactorX(),getScaleFactorY());
}

void GUICheckBox::changeCheckedValue() {
    if (checked)
        checked=false;
    else
        checked=true;
}

void GUICheckBox::checkMe() {
    checked=true;
}

void GUICheckBox::refresh() {
        GUIButton::refresh();

        // Check If this position is updated from the intial stage(0) so we can change tooltip position as well.
        if (getX()!=0) {
            GUIElement::setToolTipAreaOnly(getX(),getY(),getWidth(),getHeight());
        }

        if (wasClicked())
            changeCheckedValue();
}

void GUICheckBox::draw() {
    GUIButton::draw();
    if (checked) drawTintedButtonIconImage(imgCheckedIconScaled,0.7f);
}
