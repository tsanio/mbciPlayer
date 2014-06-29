#include <sstream>
#include <stdio.h>
#include <math.h>
#include <limits>
#include <string>
#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include "GUIElement.h"
#include "GUIScrollPanel.h"


#ifndef SPECTRUMPANEL_H
#define SPECTRUMPANEL_H

using namespace std;
/**
 * \brief GUISpectrumPanel.h
*
* ...::: GUI Scroll Panel - Classical Scroll Panel :::...
*
* This holds text with given ids and colors that can be used for example showing the songs of playlist or choosing some options.
*
* author: tonis @ 2013
*
*/


class GUISpectrumPanel : public GUIElement
{
public:
    GUISpectrumPanel();
    GUISpectrumPanel(int xP, int yP, int panelItemSizeYP, int widthP, int heightP, int scrollBarWidthP, string toolTipText);
    GUISpectrumPanel(int xP, int yP, int panelItemSizeYP, int widthP, int heightP, int scrollBarWidthP, int zIndexP, string toolTipText);
    virtual ~GUISpectrumPanel();

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
    int   highlightIndex;
    string originalToolTipTxt;
    int   lockedHighLightItem;
    float panelItemSizeY;
    float visibilityShade;
    //ALLEGRO_FONT *libraryFont;
    //ALLEGRO_BITMAP *scrollPanelDrawBuffer;
    float debugV;
};

#endif // SPECTRUMPANEL_H
