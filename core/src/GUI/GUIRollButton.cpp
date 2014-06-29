#include "../../include/GUI/GUIRollButton.h"

GUIRollButton::GUIRollButton()
{
    //ctor
    GUIButton();
    setType(TYPE_ROLL_BUTTON);
}

GUIRollButton::GUIRollButton(float px, float py, float radius, string toolTipTxt, bool storableP) {
    //GUIButton(px, py, dx);
    reset();
    createButton(px,py,radius);
    setType(TYPE_ROLL_BUTTON);
    setStorable(storableP);
    GUIElement::setToolTipArea(px-radius,py-radius,radius*2,radius*2,toolTipTxt);
}

GUIRollButton::GUIRollButton(float px, float py, float radius, string txt, string toolTipTxt, bool storableP) {
    //GUIButton(px, py, dx);
    reset();
    createButton(px,py,radius);
    setType(TYPE_ROLL_BUTTON);
    setText(txt);
    setStorable(storableP);
    GUIElement::setToolTipArea(px-radius,py-radius,radius*2,radius*2,toolTipTxt);
}

// WHen this constructor is used with integers, it means this will be rounded to integers.. Not float. Ie port selection 1-6 rather than 1.22-1. :)
GUIRollButton::GUIRollButton(float px, float py, float radius, string txt, int minV, int maxV, string unitTxt, int defValue, string toolTipTxt, bool storableP) {
    reset();
    createButton(px,py,radius);
    setType(TYPE_ROLL_BUTTON);
    setText(txt);
    setMin(minV);
    setMax(maxV);
    setUnite(unitTxt);
    setValue(defValue);
    setStorable(storableP);
    setUnitType(INT_NUM_TYPE);
    GUIElement::setToolTipArea(px-radius,py-radius,radius*2,radius*2,toolTipTxt);
}

GUIRollButton::GUIRollButton(float px, float py, float radius, string txt, float minV, float maxV, string unitTxt, float defValue, string toolTipTxt, bool storableP) {
    //GUIButton(px, py, dx);
    reset();
    createButton(px,py,radius);
    setType(TYPE_ROLL_BUTTON);
    setText(txt);
    setMin(minV);
    setMax(maxV);
    setUnite(unitTxt);
    setValue(defValue);
    setStorable(storableP);
    GUIElement::setToolTipArea(px-radius,py-radius,radius*2,radius*2,toolTipTxt);
}


GUIRollButton::~GUIRollButton()
{
    //dtor
}
