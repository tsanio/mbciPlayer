#ifndef PLRGUI_H
#define PLRGUI_H

/**
 * \brief PlrGUI.h ...::: Player GUI - Drawing draawing! :::...
*
* Here is most of the drawing calls of each elements. Also other important thing here is the hiding and showing
* of different elements according chosen panel.
*
* author: tonis @ 2013
*
*/
class PlrGUI
{
public:
    PlrGUI();
    virtual ~PlrGUI();
protected:
private:
};

// NOTICE THAT YOU HAVE TO PRESENT THE GLOBAL VARIABLES HERE AND ALSO IN THE CPP!! and the extern
namespace plrCommon
{


    void hideAllPanels();
    void showPanel(int panelIndex);
    void updateMainGUI();// Main update GUI


};

#endif // PLRGUI_H
