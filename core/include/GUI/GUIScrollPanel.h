#include <sstream>
#include <stdio.h>
#include <math.h>
#include <limits>
#include <string>
#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include "GUIElement.h"

#ifndef GUISCROLLPANEL_H
#define GUISCROLLPANEL_H

using namespace std;
/**
 * \brief GUIScrollPanel.h
*
* ...::: GUI Scroll Panel - Classical Scroll Panel :::...
*
* This holds text with given ids and colors that can be used for example showing the songs of playlist or choosing some options.
*
* author: tonis @ 2013
*
*/
class PanelItem
{
public:
    PanelItem(string itemTexta, string itemIda, ALLEGRO_COLOR itemColora);
    string getItemText() const;
    string getItemId() const;
    ALLEGRO_COLOR getTextColor()
    {
        return itemColor;
    }
private:
    string itemText, itemId;
    ALLEGRO_COLOR itemColor;
};



class GUIScrollPanel : public GUIElement
{
public:
    GUIScrollPanel();
    GUIScrollPanel(int xP, int yP, int panelItemSizeYP, int widthP, int heightP, int scrollBarWidthP, string toolTipText);
    GUIScrollPanel(int xP, int yP, int panelItemSizeYP, int widthP, int heightP, int scrollBarWidthP, int zIndexP, string toolTipText);
    virtual ~GUIScrollPanel();

    void setStartUpValues(int xP, int yP, int panelItemSizeYP, int widthP, int heightP, int scrollBarWidthP);
    void addItem(string itemText, string itemId, ALLEGRO_COLOR textColora);
    void mouseDown(float x, float y);
    void mouseUp(float x, float y);
    void refresh();
    void draw();
    void init();
    void clearItems();
    void selectItemById(string id);
    void limitSelectionIndexBoundaries();
    void loopSelectionIndexInBoundaries();
    void loopLockedHighlightIndexInBoundaries();
    void highlightNextItem();
    void selectNextItem();
    void jumpToShowSelection();
    void lockHighlightItem();
    bool mouseIsInsideScrollPanel();
    bool mouseIsInsideScrollPanelTextPart();
    bool mouseIsInsideScrollPanelSlider();
    bool mouseIsInsideScrollPanelSliderMiddleButton();
    string getSelectionItemId();
    string getSelectionItemText();
    void adjustVisibility(int dir);
    int getSelectionIndex()
    {
        return selectionIndex;
    }
    bool newSelectionWasMade() {
        if (lastSelectionIndex!=selectionIndex)
            return true;
        else
            return false;
    }
    vector<PanelItem> pItems;

protected:
private:
    float x,y,width,height;
    float scrollBarWidth;
    float scrollPosition;
    float scrollPositionSpeed;
    float scrollBarSliderSelectionPartHeight;
    float scrollBarSliderSelectionPartPositionY;
    float dragScrollBarFromPointX;
    float dragScrollBarFromPointY;
    float scrollSliderRelativeOrigoY;
    float scrollSliderRelativeOrigoMouseY;
    float scrollSliderRelativeCurrentY;
    float maxScrollSpeed;
    float scrollAcc;
    bool  draggingTheScrollFromScrollSlider;
    bool  draggingTheScrollFromCursor;
    int   selectionIndex;
    int   lastSelectionIndex;
    int   highlightIndex;
    string originalToolTipTxt;
    int   lockedHighLightItem;
    float panelItemSizeY;
    float visibilityShade;
    //ALLEGRO_FONT *libraryFont;
    //ALLEGRO_BITMAP *scrollPanelDrawBuffer;
    float debugV;
};

#endif // GUISCROLLPANEL_H
