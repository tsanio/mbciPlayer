// RAdio button consists of multible checkboxes :)
#include <cmath>        // std::abs
#include <string>
#include <iostream>
#include <memory>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"
#include "../../include/GUI/GUIButton.h"
#include "../../include/PlayerCommon.h"
#include "../../include/GUI/GUIRadioButton.h"

using namespace plrCommon;
using namespace std;

#define ITEM_SPACING 15

GUIRadioButton::GUIRadioButton()
{
    //ctor
}

GUIRadioButton::~GUIRadioButton()
{
    //dtor
}
GUIRadioButton::GUIRadioButton(float px, float py, string txtHeader, string toolTipTxt, bool storableP) {
    reset();
    setPosition(px,py);
    setText(txtHeader);
    setDimension(16,16);
    setType(TYPE_NONE);
    nextYPos = py+ITEM_SPACING;
    selectionText = "None";
    radioOptionChanged=false;
    setStorable(storableP);
}

GUIRadioButton::GUIRadioButton(float px, float py, string txtHeader, string toolTipTxt) {
    reset();
    setPosition(px,py);
    setText(txtHeader);
    setDimension(16,16);
    setType(TYPE_NONE);
    nextYPos = py+ITEM_SPACING;
    selectionText = "None";
    radioOptionChanged=false;
    GUIElement::setToolTipArea(px,py,txtHeader.length()*8,10,toolTipTxt);
}


void GUIRadioButton::addOption(GUICheckBox *checkBox) {
    options.push_back(checkBox);
    checkBox->setPosition(getItemPosX(),getNextYPos());
    nextYPos += ITEM_SPACING;
    // If this is the first added thing here.. Uncheck others and..
    if (options.size()==1) {
        unCheckAll();
        options.at(0)->checkMe();
        // Choose the selection
        selectionText = checkBox->getText();
    }
}

float GUIRadioButton::getNextYPos() {
    return nextYPos;
}

float GUIRadioButton::getItemPosX() {
    return getX() + ITEM_SPACING;
}

void GUIRadioButton::init() {
    GUIButton::init();
    // No images loaded?
}

// Override the hide-function to hide also all buttons..
void GUIRadioButton::hide() {
    GUIButton::hide();
    for(size_t i = 0; i < options.size(); ++i)    {
        options[i]->hide();
    }
}

// Override the show-function to show also all buttons..
void GUIRadioButton::show() {
    GUIButton::show();
    for(size_t i = 0; i < options.size(); ++i)    {
        options[i]->show();
    }
}

void GUIRadioButton::unCheckAll() {
    for(size_t i = 0; i < options.size(); ++i)
        if (options[i]->isChecked())
            options[i]->changeCheckedValue();
}

void GUIRadioButton::setCheckedIndex(int indexToCheck) {
    unCheckAll();
    options[indexToCheck]->checkMe();
    selectionText = options[indexToCheck]->getText();
}

int GUIRadioButton::getCheckedIndex() {
    for(size_t i = 0; i < options.size(); ++i)    {
        if (options[i]->isChecked())
            return i;
    }
    return 0;
}


void GUIRadioButton::refresh() {
    GUIButton::refresh();
    radioOptionChanged=false;
    // If something was clicked.. Uncheck all other options.. This is radiobuttons! :)
    for(size_t i = 0; i < options.size(); ++i)    {
        if (options[i]->isActive()) {
            if (options[i]->wasClicked()) {// If something was clicked then unclick others ..
                // Choose the selection
                selectionText = options[i]->getText();
                // Then unable all except the one which was just enabled
                for(size_t q = 0; q < options.size(); ++q)    {
                    if (options[q]->isActive()) {
                        if (!options[q]->wasClicked() && options[q]->isChecked() || (options[q]->wasClicked() && !options[q]->isChecked())) { // Uncheck unclicked checkboxes. Crazy logic. That later one makes sure everything cannot be disabled. The value is disabled before in the guicheckbox if you look carefully. It's abit confusing.
                            // If new selection.. we inform that something was chosen..
                            if (options[q]->getText()!=getSelectionTxt())
                                radioOptionChanged=true;
                            // Check anyway..
                            options[q]->changeCheckedValue();
                        }
                    }
                }
            }
        }
    }
}


void GUIRadioButton::draw() {
    GUIButton::draw();
    // We only draw header of this radiobutton.. Otherwise all buttons are drawn automatically from elementlist unless we specially want to draw them here for some reason..
    // Draw text also If that's wanted.
    /*if (text!="") {
        float alpha = 0.9f;
        ALLEGRO_COLOR bFontColor = al_map_rgba_f(0.9*alpha, 0.9*alpha, 1.0*alpha, alpha); // <-- correct
        al_draw_text(buttonFont, bFontColor, x+width*1.5f, y+height/2-buttonFontSize/2, NULL, text.c_str());
    }*/
}
