#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"

#include "../../include/GUI/GUISpectrumPanel.h"
#include "../../include/PlayerCommon.h"

using namespace plrCommon;
using namespace std;

// Define the whole scroll panel..
GUISpectrumPanel::GUISpectrumPanel()
{
    //Constructor
    setStartUpValues(30,50,24,350,20,15);
}

// Note: The height isn't in pixels. It's the multiply of the panelItemSizeY
void GUISpectrumPanel::setStartUpValues(int xP, int yP, int panelItemSizeYP, int widthP, int heightP, int scrollBarWidthP) {
    x=xP;
    y=yP;
    width=widthP;
    panelItemSizeY=panelItemSizeYP;
    height=panelItemSizeYP*heightP;
    scrollBarWidth=scrollBarWidthP;

    originalToolTipTxt="Choose something..";
    scrollPositionSpeed=0;
    scrollPosition=0;
    selectionIndex=0;
    highlightIndex=-1;
    lockedHighLightItem=-1;
    maxScrollSpeed=100000000000;
    scrollAcc=5.0f;
    draggingTheScrollFromScrollSlider=false;
    draggingTheScrollFromCursor=false;
    scrollSliderRelativeOrigoY=0;
    scrollSliderRelativeOrigoMouseY=0;
    scrollSliderRelativeCurrentY=0;
    visibilityShade=1.0f;

    // Add the items to panel.. Which are all graphs that are known.
    for (int i=0; i<AMOUNT_OF_CHANNELS; i++) {
        if (getGraphTheme(i).showInBarPanel)
            addItem(getGraphTheme(i).text,intToStr(i),getGraphThemeColorOf(i));
    }

}

// Note: The height isn't in pixels. It's the multiply of the panelItemSizeY
GUISpectrumPanel::GUISpectrumPanel(int xP, int yP, int panelItemSizeYP, int widthP, int heightP, int scrollBarWidthP, int zIndexP, string toolTipText) {
    setZ(zIndexP);
    //GUISpectrumPanel(xP, yP, panelItemSizeYP, widthP, heightP, scrollBarWidthP);
    setStartUpValues(xP, yP, panelItemSizeYP, widthP, heightP, scrollBarWidthP);
    GUIElement::setToolTipArea(xP,yP,width,height,toolTipText);
    originalToolTipTxt=toolTipText;
}

// Note: The height isn't in pixels. It's the multiply of the panelItemSizeY
GUISpectrumPanel::GUISpectrumPanel(int xP, int yP, int panelItemSizeYP, int widthP, int heightP, int scrollBarWidthP, string toolTipText)
{
    setStartUpValues(xP, yP, panelItemSizeYP, widthP, heightP, scrollBarWidthP);
    GUIElement::setToolTipArea(xP,yP,width,height,toolTipText);
    originalToolTipTxt=toolTipText;
}

// This is called after allegro initialization so it's possibl to load own fonts here etc.
void GUISpectrumPanel::init() {
    playerLog("Initializing scrollpanel..\n");
}


GUISpectrumPanel::~GUISpectrumPanel()
{
    //Destructor
}

string GUISpectrumPanel::getSelectionItemId() {
     return pItems[selectionIndex].getItemId();
}

void GUISpectrumPanel::addItem(string itemText, string itemId, ALLEGRO_COLOR itemColor) {
    // Graph all double mepty spaces and further empty spaces away to make it look nice.. I hope someone doesn't use double empty spaces in songs for purpose :)
    bool stripDoubleEmptySpaces=true;
    if (stripDoubleEmptySpaces) {
        string itemTextStripped = forceRemoveAllDoubleEmptySpaces(itemText);
        pItems.push_back(PanelItem(itemTextStripped,itemId, itemColor));
    } else
        pItems.push_back(PanelItem(itemText,itemId, itemColor));

}

bool GUISpectrumPanel::mouseIsInsideScrollPanel() {
    if (guiMouse.getMouseX()>x && guiMouse.getMouseX()<x+width && guiMouse.getMouseY()>y && guiMouse.getMouseY()<y+height)
        return true;
    else
        return false;
}

bool GUISpectrumPanel::mouseIsInsideScrollPanelTextPart() {
    if (guiMouse.getMouseX()>x && guiMouse.getMouseX()<x+width-scrollBarWidth && guiMouse.getMouseY()>y && guiMouse.getMouseY()<y+height)
        return true;
    else
        return false;
}

bool GUISpectrumPanel::mouseIsInsideScrollPanelSlider() {
    if (guiMouse.getMouseX()>x+width-scrollBarWidth && guiMouse.getMouseX()<x+width && guiMouse.getMouseY()>y && guiMouse.getMouseY()<y+height)
        return true;
    else
        return false;
}

bool GUISpectrumPanel::mouseIsInsideScrollPanelSliderMiddleButton() {
    if (guiMouse.getMouseX()>x+width-scrollBarWidth && guiMouse.getMouseX()<x+width && guiMouse.getMouseY()>y && guiMouse.getMouseY()<y+height) {
        if (guiMouse.getMouseY()>scrollBarSliderSelectionPartPositionY+y && guiMouse.getMouseY() < scrollBarSliderSelectionPartPositionY+scrollBarSliderSelectionPartHeight+y)
            return true;
        else
            return false;
    } else
        return false;
}

void GUISpectrumPanel::adjustVisibility(int direction) {
    visibilityShade += direction*deltaTime*0.3f;
    if (visibilityShade>1.0f) visibilityShade=1.0f;
    if (visibilityShade<0.0f) visibilityShade=0.0f;
}

void GUISpectrumPanel::refresh() {

    // Handle mouse actions over the scroll panel
    if (mouseIsInsideScrollPanelTextPart()) {
        // Change color when mouse over? :)
        if (guiMouse.getLeftButtonState()) {
            scrollPosition += guiMouse.getMouseDeltaY();
            scrollPositionSpeed = float(guiMouse.getMouseSpeedY())*100.0f;
        }
    }

    // Button up so wee aree flyying and slowing down.
    if (!guiMouse.getLeftButtonState()) {
        scrollPosition += scrollPositionSpeed*deltaTime;
        if (scrollPositionSpeed>0) {
            scrollPositionSpeed -= scrollPositionSpeed*scrollAcc*deltaTime; // 5 is very nice breaking speed but not so good looking.
            if (scrollPositionSpeed<0)
                scrollPositionSpeed=0;
        }
        if (scrollPositionSpeed<0) {
            scrollPositionSpeed += -scrollPositionSpeed*scrollAcc*deltaTime;
            if (scrollPositionSpeed>0)
                scrollPositionSpeed=0;
        }
    }

    // If button was down and up without moving too much, it means we clicked an item and wanted to select it
    // Act only If we have items in the panel or we'r gonna craaaash! :)
    if (pItems.size() > 0) {

        if (mouseIsInsideScrollPanelTextPart()) {
            float selectionSensitivity=0.05d; // How fast the mouse can move max to regocnize click
            if (!guiMouse.getLeftButtonState() && guiMouse.getLeftButtonStatePrevious() && guiMouse.getMouseSpeedY() > -selectionSensitivity && guiMouse.getMouseSpeedY() < selectionSensitivity ) {
                selectionIndex = int((guiMouse.getMouseY()+(-y-scrollPosition))/panelItemSizeY);
                limitSelectionIndexBoundaries();
                // Put here something If you want to show information about the item that was just clicked!
                // Double click! Not implemented!
            }
        }

        // Check if we'r also on something.. Just to highlight on mouse hoovering..
        if (mouseIsInsideScrollPanelTextPart()) {
            highlightIndex = int((guiMouse.getMouseY()+(-y-scrollPosition))/panelItemSizeY);
        } else { // Otherwise we shouldn't highlight anything..
            highlightIndex = -1;
        }

        // Try to scroll the view automaticly back ie. in case If the user pulls it out of range :)
        // Do we have lack of items? So it's under the full view..? Then change the slidepoint abit towards center..
        int tooMuch = 0;
        if (pItems.size()<int(height/panelItemSizeY))
            tooMuch = int(height/panelItemSizeY)-pItems.size();

        if (scrollPosition-tooMuch*panelItemSizeY>panelItemSizeY*0.5f) { // Was 0..
            scrollPositionSpeed -= scrollPosition*(scrollAcc*2.0f)*deltaTime; // But this os more fun. Like a string :)
        }

        float scrollBoarderDownerY=(scrollPosition+(pItems.size()+(6.5+tooMuch))*panelItemSizeY); // was +5
        if (scrollBoarderDownerY<y+height) {
            scrollPositionSpeed += (y+height-scrollBoarderDownerY)*(scrollAcc*2.0f)*deltaTime; // But this os more fun. Like a string :)
        }

        // The scroll can be moved by rotating mouse wheel..
        if (mouseIsInsideScrollPanel()) {
            if (guiMouse.mouseWheelRolledUp())
                scrollPositionSpeed += 300.0f;
            if (guiMouse.mouseWheelRolledDown())
                scrollPositionSpeed -= 300.0f;
        }

        // Change also the tool tip text to match the song text. This will help readability.
        if (highlightIndex>-1&&highlightIndex<pItems.size())
            setToolTipTextOnly(pItems[highlightIndex].getItemText());
        else
            setToolTipTextOnly(originalToolTipTxt);

    }

    // Scrolling of the scrollbar
    if (( mouseIsInsideScrollPanelSlider() && guiMouse.getLeftButtonState()) || draggingTheScrollFromScrollSlider || draggingTheScrollFromCursor) {

            // This is the case If we press first the scrollbar button slider to get hold onto it..
            if  ((mouseIsInsideScrollPanelSliderMiddleButton() && !draggingTheScrollFromCursor ) || draggingTheScrollFromScrollSlider)  {
                // Take the relative coordinate in first time..
                if (!draggingTheScrollFromScrollSlider) {
                    scrollSliderRelativeOrigoY      = scrollBarSliderSelectionPartPositionY;//+scrollBarSliderSelectionPartHeight/2;
                    scrollSliderRelativeOrigoMouseY = guiMouse.getMouseY();
                }
                scrollSliderRelativeCurrentY = scrollSliderRelativeOrigoY - (scrollSliderRelativeOrigoMouseY-guiMouse.getMouseY());
                dragScrollBarFromPointX = guiMouse.getMouseX();
                draggingTheScrollFromScrollSlider = true;

                scrollPosition = -((panelItemSizeY*pItems.size())/height)*(scrollSliderRelativeCurrentY);
            }

            // If we'r not dragging from the slider button, we are propably just pressing somewhere on the srollbar
            if (!draggingTheScrollFromScrollSlider) {
                dragScrollBarFromPointX = guiMouse.getMouseX();
                dragScrollBarFromPointY = guiMouse.getMouseY();
                draggingTheScrollFromCursor = true;
                scrollPosition = -((panelItemSizeY*pItems.size())/height)*(dragScrollBarFromPointY-y-scrollBarSliderSelectionPartHeight/2);
            }


    }

    // Stop dragging the scroll If we'r not pressing anything..
    if (!guiMouse.getLeftButtonState()) {
        draggingTheScrollFromCursor=false;
        draggingTheScrollFromScrollSlider=false;
    }

}

void GUISpectrumPanel::lockHighlightItem() {
    lockedHighLightItem = selectionIndex;
}

void GUISpectrumPanel::clearItems() {
    pItems.clear();
}

void GUISpectrumPanel::limitSelectionIndexBoundaries() {
    if (selectionIndex<0)
        selectionIndex=0;
    if (selectionIndex>pItems.size()-1)
        selectionIndex=pItems.size()-1;
}

void GUISpectrumPanel::loopSelectionIndexInBoundaries() {
    selectionIndex++;
    if (selectionIndex<0)
        selectionIndex=pItems.size()-1;
    if (selectionIndex>pItems.size()-1)
        selectionIndex=0;
}

void GUISpectrumPanel::loopLockedHighlightIndexInBoundaries() {
    lockedHighLightItem++;
    if (lockedHighLightItem<0)
        lockedHighLightItem=pItems.size()-1;
    if (lockedHighLightItem>pItems.size()-1)
        lockedHighLightItem=0;
}

void GUISpectrumPanel::highlightNextItem() {
    loopSelectionIndexInBoundaries();
}

void GUISpectrumPanel::GUISpectrumPanel::selectNextItem() {
    // Selection = selection that user wants to play next
    // Lockedhighlighted = is additional highlighting ie. showing the song that is playe
    // If we'r not choosing again the same. Then choose the highlighted
    if (lockedHighLightItem!=selectionIndex) {
        //selectionIndex = lockedHighLightItem;
        // DO nothing THEN!!
    } else {
        loopSelectionIndexInBoundaries(); // Oh my poor mind..
    }

}

void GUISpectrumPanel::selectItemById(string searchId) {
    for(size_t i = 0; i < pItems.size(); ++i)
    {
        if (pItems.at(i).getItemId() == searchId)
            selectionIndex = i;
    }
}

void GUISpectrumPanel::draw() {

    GUIElement::draw();

    // THE IDEA:
    // Set clipping so that overboard going font won't show up!
    // Draw the stuff
    // Turn the clipping back to normal.. and all is good :)

    al_set_clipping_rectangle(x, y, width-scrollBarWidth, height); // Notice that this is made different later for that scrollbar

        int panelIndex = 0;
        float alpha    = 0.8f;
        typedef vector<PanelItem> vec;

        // Is the scroll negative. If so let's not start the loop unneccessary low but from where it should be drawn(index of list item)
        int startingIndex   = 0;
        int endingIndex     = 0;
        int startingPositionIncreaseY = 0;

        if (scrollPosition<0) {
            startingIndex = int(abs(scrollPosition)/panelItemSizeY);
        }
        startingPositionIncreaseY = startingIndex * panelItemSizeY;

        endingIndex = int(height/panelItemSizeY)+startingIndex+1; // or + 2

        if (scrollPosition>0) {
            endingIndex -= int(abs(scrollPosition)/panelItemSizeY)+1;
        }

        if (endingIndex>pItems.size()) endingIndex = pItems.size();

        for(size_t i = startingIndex; i < endingIndex; ++i)
        {

           float posX = x;
           float posY = y+panelIndex*panelItemSizeY+scrollPosition+startingPositionIncreaseY;

           //if(posY-panelItemSizeY>y+height+panelItemSizeY)break; // leave while-loop If drawing would go out of bounds so it's optimized :)

           if (libraryFont) {
                // Nice mindfuck but it works..
                alpha = 0.8f;
                float fadingThreshold = panelItemSizeY/2;
                float fadingThresholdDown = fadingThreshold*3.0f;
                float fadingScale     = fadingThreshold*1.5f;
                if (posY<y+fadingThreshold) {
                    alpha = alpha-(((y-posY+fadingThreshold)/(fadingScale))*alpha);
                    if (alpha<0.0f) alpha = 0.0f;
                }
                // Nice mindfuck but it works.. Thjis is the another boarder
                if (posY>y+height-fadingThresholdDown) {
                    alpha = alpha-((posY - (y+height-fadingThresholdDown))/fadingScale)*alpha;
                    debugV = alpha-((posY - (y+height-fadingThresholdDown))/fadingScale)*alpha;
                    //alpha = 0.2f;
                    if (alpha<0.0f) alpha = 0.0f;
                }

                alpha *= visibilityShade;

              //  al_draw_text(libraryFont, color, posX, posY, NULL, it->getItemText().c_str());
              // Which of these is the played song?? :DD
              //al_draw_rectangle(posX, posY, posX+width-scrollBarWidth, posY+panelItemSizeY, al_map_rgba_f(0.0f*(alpha/2.0f), 0.7f*(alpha/2.0f), 0.0*(alpha/2.0f), (alpha/2.0f)),1);
              if (i == lockedHighLightItem) {
                al_draw_filled_rectangle(posX, posY, posX+width-scrollBarWidth, posY+panelItemSizeY, al_map_rgba_f(0.4f*(alpha/2.0f), 0.9f*(alpha/2.0f), 0.0*(alpha/2.0f), (alpha/2.0f)));
              } else if (i == selectionIndex) { // Selection box.. Wtf? Is this the song that is currently played?
                al_draw_filled_rectangle(posX, posY, posX+width-scrollBarWidth, posY+panelItemSizeY, al_map_rgba_f(0.0f*(alpha/2.0f), 0.7f*(alpha/2.0f), 0.0*(alpha/2.0f), (alpha/2.0f)));
              } else if (i == highlightIndex) {
                al_draw_filled_rectangle(posX, posY, posX+width-scrollBarWidth, posY+panelItemSizeY, al_map_rgba_f(0.0f*(alpha/3.0f), 0.4f*(alpha/3.0f), 0.0*(alpha/3.0f), (alpha/3.0f)));
              }


              // Draw only colored texts If it's chosen so.. Otherwise bluuues.. I mean blue text :)
              ALLEGRO_COLOR color =
                      al_map_rgba_f(pItems[i].getTextColor().r*alpha,
                                    pItems[i].getTextColor().g*alpha,
                                    pItems[i].getTextColor().b*alpha,
                                                               alpha);


              al_draw_text(libraryFont, color , posX, posY+panelItemSizeY/4, NULL, pItems[i].getItemText().c_str());

            //double amplitudeV = bwMeter.getCurrentEEGChannelSlowBarValue(strToInt(pItems[i].getItemId()));
            double amplitudeV = bwMeter.getCurrentEEGChannelSlowBarValue(strToInt(pItems[i].getItemId()))/100.0d;

            // Draw minimum and maximum of profile channels.
            if (i == bwMeter.PROFILE_CHANNEL_1 || i == bwMeter.PROFILE_CHANNEL_2) {
                double minP = musicLibrary.getMinimumValueInAllSongsOfChannel(strToInt(pItems[i].getItemId()))/100.0d;
                double maxP = musicLibrary.getMaximumValueInAllSongsOfChannel(strToInt(pItems[i].getItemId()))/100.0d;
                float barPointMin = ((width-scrollBarWidth)*(2.0f/3.0f))*minP;
                float barPointMax = ((width-scrollBarWidth)*(2.0f/3.0f))*maxP;
                ALLEGRO_COLOR marginColor =  al_map_rgba_f(1.0f,1.0f,1.0f,1.0f);
                al_draw_filled_rectangle(posX+width/3+(barPointMin)+2, posY-1, posX+width/3+(barPointMin), posY+panelItemSizeY+1, marginColor);
                al_draw_filled_rectangle(posX+width/3+(barPointMax)+2, posY-1, posX+width/3+(barPointMax), posY+panelItemSizeY+1, marginColor);
            }


            ALLEGRO_COLOR colorBar = al_map_rgba(0,0,0,0);
            if (i!=CHANNEL_AVG) {
                colorBar =
                      al_map_rgba_f(pItems[i].getTextColor().r*0.42f,
                                    pItems[i].getTextColor().g*0.42f,
                                    pItems[i].getTextColor().b*0.42f,
                                                               0.42f);
           } else {
                colorBar = getGlobalColorOfFrequency(amplitudeV*100.0d);
           }


              // Draw the graph for amplitude of this channel.
              float barWidth = ((width-scrollBarWidth)*(2.0f/3.0f))*amplitudeV;
              al_draw_filled_rectangle(posX+(width-scrollBarWidth)/3, posY, posX+width/3+(barWidth), posY+panelItemSizeY, colorBar);


           }

            panelIndex++;
        }

        // The boarder..
        al_draw_rectangle(x+1, y+1, x+width-scrollBarWidth, y+height, al_map_rgba_f(0.1*visibilityShade, 0.3*visibilityShade, 0.0*visibilityShade, visibilityShade), 1);
        // Draw the scrollBar
        al_set_clipping_rectangle(x+width-scrollBarWidth, y, scrollBarWidth, height);
        al_draw_rectangle(x+width-scrollBarWidth, y+1, x+width, y+height, al_map_rgba_f(0.0*visibilityShade, 0.5*visibilityShade, 0.0*visibilityShade, visibilityShade), 1);
        // Size of the selection part
        scrollBarSliderSelectionPartHeight = (height/((pItems.size()*panelItemSizeY)/height));
        // Position of the selection part
        scrollBarSliderSelectionPartPositionY = ((-scrollPosition/(pItems.size()*panelItemSizeY))*height);
        if (scrollBarSliderSelectionPartPositionY<0) scrollBarSliderSelectionPartPositionY = 0;
        if (scrollBarSliderSelectionPartPositionY>height-scrollBarSliderSelectionPartHeight) scrollBarSliderSelectionPartPositionY = height-scrollBarSliderSelectionPartHeight;
        al_draw_filled_rectangle(x+width-scrollBarWidth, y+scrollBarSliderSelectionPartPositionY, x+width, y+scrollBarSliderSelectionPartHeight+scrollBarSliderSelectionPartPositionY, al_map_rgba_f(0.0*visibilityShade, 0.8*visibilityShade, 0.0*visibilityShade, visibilityShade));


        al_set_clipping_rectangle(0, 0, al_get_display_width(display), al_get_display_height(display));

        GUIElement::draw();
}
