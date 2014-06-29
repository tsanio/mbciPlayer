#include "GUIButton.h"

#ifndef GUICHECKBOX_H
#define GUICHECKBOX_H

//using namespace plrCommon;
using namespace std;
/**
 * \brief GUICheckBox.h ...::: GUI Element - Checkbox :::...
*
* Checkbox this is a variant of button for making checkbox. Belongs to elements of GUI and Button.
*
* author: tonis @ 2013
*
*/
class GUICheckBox : public GUIButton
{
public:
    GUICheckBox();
    GUICheckBox(float px, float py, string txt, string toolTipText);
    GUICheckBox(float px, float py, string txt, string toolTipText, bool storableP);
    GUICheckBox(float px, float py, string txt, bool val, string toolTipText);
    GUICheckBox(float px, float py, string txt, bool val, string toolTipText, bool storableP);
    GUICheckBox(string txt, string toolTipText);
    virtual ~GUICheckBox();
    void setUp(float px, float py, string txt);
    void refresh();
    void draw();
    void init();
    bool isChecked()
    {
        return checked;
    }
    void changeCheckedValue();
    void setChecked(bool valP) { checked = valP; }
    void checkMe();
protected:
private:
    ALLEGRO_BITMAP *imgCheckedIconScaled;
    ALLEGRO_BITMAP *imgCheckedIcon;
    bool checked;
};

#endif // GUICHECKBOX_H
