#ifndef GUIRADIOBUTTON_H
#define GUIRADIOBUTTON_H

// Radiobutton is like this:
// Choose which is your way to go:
// left [ ]
// right [ ]
// take a teabreak [x]
using namespace std;
/**
 * \brief GUIRadioButton.h ...::: GUI RadioButton - The multi checkbox! :::...
*
* This radio button is a classical radio button: Many checkboxes inside of this where only one can be chosen.
*
* author: tonis @ 2013
*
*/
class GUIRadioButton : public GUIButton
{
    public:
        GUIRadioButton();
        void init();
        void refresh();
        void draw();
        void addOption(GUICheckBox *thisCheckBox);
        void hide();
        void show();
        void unCheckAll();
        void setCheckedIndex(int indexToCheck);
        int getCheckedIndex();
        bool getRadioOptionChanged() {
            return radioOptionChanged;
        }
        float getNextYPos();
        float getPosY() { return getX(); }
        float getPosX() { return getY(); }
        float getItemPosX();
        string getSelectionTxt() { return selectionText; }
        GUIRadioButton(float px, float py, string txtHeader, string toolTipTxt);
        GUIRadioButton(float px, float py, string txtHeader, string toolTipTxt, bool storableP);
        virtual ~GUIRadioButton();
    protected:
    private:
        vector<GUICheckBox*> options;
        string selectionText;
        float nextYPos;
        bool radioOptionChanged;
};

#endif // GUIRADIOBUTTON_H
