#ifndef GUIROLLBUTTON_H
#define GUIROLLBUTTON_H

#include "GUIButton.h"
/**
 * \brief GUIRollButton.h ...::: GUI Rollable Button - This is the Button of the user interface :::...
*
* Basic button which can be rolled and this gives the value 0.000-1.000f which can be used nicely for something as volume or port choosing.
*
* author: tonis @ 2013
*
*/
class GUIRollButton : public GUIButton
{
public:
    GUIRollButton();
    GUIRollButton(float px, float py, float radius, string toolTipTxt, bool storable);
    GUIRollButton(float px, float py, float radius, string txt, string toolTipTxt, bool storable);
    GUIRollButton(float px, float py, float radius, string txt, float minV, float maxV, string unitTxt, float defaultValue, string toolTipTxt, bool storable);
    GUIRollButton(float px, float py, float radius, string txt, int   minV, int   maxV, string unitTxt, int   defValue, string     toolTipTxt, bool storable);
    virtual ~GUIRollButton();
protected:
private:

};

#endif // GUIROLLBUTTON_H
