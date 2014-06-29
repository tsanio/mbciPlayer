#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"
#include "../../include/GUI/GUIScroller.h"
#include "../../include/PlayerCommon.h"
#include "../../include/GUI/GUIMouse.h"

using namespace plrCommon;
using namespace std;

GUIScroller::GUIScroller()
{
    //ctor
}

GUIScroller::~GUIScroller()
{
    //dtor
}

void GUIScroller::setStartUpValues(float xP,float yP,float axisP,float lengthP,float thicknessP, float barLengthP, int zIndexP, string toolTipText) {
    setZ(zIndexP);
    x = xP;
    y = yP;
    axis = axisP;
    length = lengthP;
    thickness = thicknessP;
    position = 0;
    buttonGlow = 0;
    barLength = barLengthP;
    draggingTheScrollFromCursor=false;
    draggingTheScrollFromScrollSlider=false;
    scrollSliderGlobalOrigoX=0;
    scrollSliderGlobalOrigoY=0;
    scrollSliderRelativeCurrentX=0;
    scrollSliderRelativeCurrentY=0;
    scrollSliderGlobalOrigoMouseX=0;
    scrollSliderGlobalOrigoMouseY=0;
    changedPosition=false;
    dragging=false;
    lastDraggingValue=false;
    //elementList.addElement(this); // Add to elements also :)
     if (axis==HORISONTAL_AXIS) {
        GUIElement::setToolTipArea(x,y,length,thickness, toolTipText);
    }
    if (axis==VERTICAL_AXIS) {
        if (guiMouse.getMouseX()>x && guiMouse.getMouseX()<x+thickness && guiMouse.getMouseY()>y && guiMouse.getMouseY()<y+length)
        GUIElement::setToolTipArea(x,y,thickness,length, toolTipText);
    }
}

GUIScroller::GUIScroller(float xP,float yP,float axisP,float lengthP,float thicknessP, float barLengthP, string toolTipText) {
    setStartUpValues(xP,yP,axisP,lengthP,thicknessP, barLengthP, DEFAULT_Z_INDEX, toolTipText);
}

GUIScroller::GUIScroller(float xP,float yP,float axisP,float lengthP,float thicknessP, float barLengthP, int zIndexP, string toolTipText) {
    setStartUpValues(xP,yP,axisP,lengthP,thicknessP, barLengthP, zIndexP, toolTipText);
}

void GUIScroller::draw() {
    if (axis==HORISONTAL_AXIS) {
        // Draw the scrollBar
        al_draw_rectangle(x, y, x+length, y+thickness, al_map_rgba_f(0.2*0.5f, 0.0*0.5f, 0.8*0.5f, 1.0f), 1);
        // Position of the selection part.. The limits are the boundaries ..
        if (position<0) position = 0;
        if (position>length) position = length;
        // Show position is a bit different this case.. it tries to show the bar in center..
        float showPosition = position - (barLength/2.0f);
        if (showPosition<0) showPosition=0;
        if (showPosition+barLength>length) showPosition=length-barLength;
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE); // Nice tint
        //al_draw_filled_rectangle(x+showPosition, y, x+showPosition+barLength, y+thickness, al_map_rgba_f(0.2*0.5f, 0.0*0.5f, 0.8*0.5f, 1.0f));
        float bAlpha = 0.7f;
        al_draw_tinted_bitmap(imgScrollBarSelectionPartScaled, al_map_rgba_f(1.0f*bAlpha, 1.0f*bAlpha, 1.0f*bAlpha, bAlpha), x+showPosition, y, 0);
        al_draw_tinted_bitmap(imgScrollBarSelectionPartHoverScaled, al_map_rgba_f(1.0f*buttonGlow, 1.0f*buttonGlow, 1.0f*buttonGlow, buttonGlow), x+showPosition, y, 0);
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA); // No tint for other drawing..

        // Draw the exact position, if neccessary (ie song position more accurately.)
        al_draw_rectangle(x+position, y, x+position+1, y+thickness, al_map_rgba_f(0.2*0.5f, 0.0*0.5f, 0.8*0.5f, 1.0f), 1);

    } else if (axis==VERTICAL_AXIS) {
        // ...
    }
    GUIElement::draw();
}

void GUIScroller::init() {
        playerLog("Initializing scroller..\n");
        imgScrollBarSelectionPart       = imageLibrary.loadImage("media/gfx/scroll_bar_selection_part.png");
        imgScrollBarSelectionPartHover  = imageLibrary.loadImage("media/gfx/scroll_bar_selection_part_hover.png");
        updateScaledImages();
}

void GUIScroller::updateScaledImages() {
    if (axis==HORISONTAL_AXIS) {
        imgScrollBarSelectionPartScaled = getScaledImage(imgScrollBarSelectionPart,barLength/al_get_bitmap_width(imgScrollBarSelectionPart),thickness/al_get_bitmap_height(imgScrollBarSelectionPart) );
        imgScrollBarSelectionPartHoverScaled = getScaledImage(imgScrollBarSelectionPartHover,barLength/al_get_bitmap_width(imgScrollBarSelectionPart),thickness/al_get_bitmap_height(imgScrollBarSelectionPart) );
    }
    if (axis==VERTICAL_AXIS) {
        //.. fullfill just.. fullfill..
    }
}

bool GUIScroller::mouseIsInsideScroller() {
    if (axis==HORISONTAL_AXIS) {
        if (guiMouse.getMouseX()>x && guiMouse.getMouseX()<x+length && guiMouse.getMouseY()>y && guiMouse.getMouseY()<y+thickness)
            return true;
        else
            return false;
    }
    if (axis==VERTICAL_AXIS) {
        if (guiMouse.getMouseX()>x && guiMouse.getMouseX()<x+thickness && guiMouse.getMouseY()>y && guiMouse.getMouseY()<y+length)
            return true;
        else
            return false;
    }
}

bool GUIScroller::mouseIsInsideScrollerSliderButton() {
    if (axis==HORISONTAL_AXIS) {
        if (guiMouse.getMouseX()>x+position-barLength/2 && guiMouse.getMouseX()<x+position+barLength/2 && guiMouse.getMouseY()>y && guiMouse.getMouseY()<y+thickness)
            return true;
        else
            return false;
    }
    if (axis==VERTICAL_AXIS) {
        if (guiMouse.getMouseX()>x && guiMouse.getMouseX()<x+thickness-barLength/2 && guiMouse.getMouseY()>y+position && guiMouse.getMouseY()<y+position+barLength/2)
            return true;
        else
            return false;
    }
}

void GUIScroller::setPosition(float setPos) {
    if (!dragging) {
        position = setPos*length;
        changedPosition = true;
    }
}

void GUIScroller::refresh() {

    lastDraggingValue = dragging;

    // Glowing
    if (!mouseIsInsideScrollerSliderButton()) {
        buttonGlow-=2.5f*deltaTime;
    if (buttonGlow<0.0f) buttonGlow=0.0f;
    } else {
        buttonGlow+=2.5*deltaTime;
        if (buttonGlow>1.0f) buttonGlow=1.0f;
    }


    // Movin by draggin!
    if (( mouseIsInsideScroller() && guiMouse.getLeftButtonState()) || draggingTheScrollFromScrollSlider || draggingTheScrollFromCursor) {

            // This is the case If we press first the scrollbar button slider to get hold onto it..
            if  ((mouseIsInsideScrollerSliderButton() && !draggingTheScrollFromCursor ) || draggingTheScrollFromScrollSlider)  {
                // Take the relative coordinate in first time..
                if (!draggingTheScrollFromScrollSlider) {
                    scrollSliderGlobalOrigoX    = x + position;
                    scrollSliderGlobalOrigoY    = y + position;
                    scrollSliderGlobalOrigoMouseX = guiMouse.getMouseX();
                    scrollSliderGlobalOrigoMouseY = guiMouse.getMouseY();
                }
                scrollSliderRelativeCurrentX = scrollSliderGlobalOrigoX - (scrollSliderGlobalOrigoMouseX-guiMouse.getMouseX()) - x;
                scrollSliderRelativeCurrentY = scrollSliderGlobalOrigoY - (scrollSliderGlobalOrigoMouseY-guiMouse.getMouseY()) - y;
                draggingTheScrollFromScrollSlider = true;
                if (axis==HORISONTAL_AXIS)
                    position = scrollSliderRelativeCurrentX;
                else
                    position = scrollSliderRelativeCurrentY;
            }

            // If we'r not dragging from the slider button, we are propably just pressing somewhere on the srollbar
            if (!draggingTheScrollFromScrollSlider) {
                draggingTheScrollFromCursor = true;
                if (axis==HORISONTAL_AXIS)
                    position = guiMouse.getMouseX()-x;
                else
                    position = guiMouse.getMouseY()-y;
            }

        dragging = true;
    }

    // Stop dragging the scroll If we'r not pressing anything..
    if (!guiMouse.getLeftButtonState()) {
        draggingTheScrollFromCursor=false;
        draggingTheScrollFromScrollSlider=false;
        dragging=false;
    }

    // The scroll can be moved by rotating mouse wheel..
    float scrollPercentByWheel=10;
    if (mouseIsInsideScroller()) {
        if (guiMouse.mouseWheelRolledUp()) {
            position += (length/100.0f)*scrollPercentByWheel;
            dragging = true;
        }
        if (guiMouse.mouseWheelRolledDown()) {
            position -= (length/100.0f)*scrollPercentByWheel;
            dragging = true;
        }
    }

    // Recognize dragging ending as happening to change the song's position of playing
    if (!dragging && lastDraggingValue)
        changedPosition=true;
    else
        changedPosition=false;


}
