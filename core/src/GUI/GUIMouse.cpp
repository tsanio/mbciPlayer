#include <allegro5/allegro.h>
#include "../../include/GUI/GUIMouse.h"
#include "../../include/PlayerCommon.h"

using namespace plrCommon;

GUIMouse::GUIMouse()
{
    setLeftButtonState(BUTTON_UP);
    setRightButtonState(BUTTON_UP);
    mouseSpeedY=0;
    dragging=false;
    dragWindow=false;
    doubleClickOccured=false;
    windowDragBarWidth=1000; // Testing this
    windowDragBarHeight=30;
}

GUIMouse::~GUIMouse()
{
    //dtor
}


void GUIMouse::mouseDown(ALLEGRO_MOUSE_EVENT *mouse) {

}

void GUIMouse::mouseUp(ALLEGRO_MOUSE_EVENT *mouse) {

}

void GUIMouse::updatePos(ALLEGRO_MOUSE_EVENT *mouse) {
    mouse_x = mouse->x;
    mouse_y = mouse->y;
    if (middleWheelState<mouse->z)
        mouseWheelRolledUpV = true;
    if (middleWheelState>mouse->z)
        mouseWheelRolledDownV = true;
    middleWheelState = mouse->z;
}

void GUIMouse::mouseUpdate(ALLEGRO_EVENT *event) {


    mouse_last_x = mouse_x;
    mouse_last_y = mouse_y;

    leftButtonStatePrevious  = leftButtonState;
    rightButtonStatePrevious = rightButtonState;

    if (event->type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        if (event->mouse.button == 1) leftButtonState  = BUTTON_DOWN;
        if (event->mouse.button == 2) rightButtonState = BUTTON_DOWN;
    }
    if (event->type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
        if (event->mouse.button == 1) leftButtonState  = BUTTON_UP;
        if (event->mouse.button == 2) rightButtonState = BUTTON_UP;
    }

    mouseWheelRolledUpV   = false;
    mouseWheelRolledDownV = false;
    if (event->type == ALLEGRO_EVENT_MOUSE_AXES) {
        updatePos(&event->mouse);
    }

    mouse_dx = mouse_x - mouse_last_x;
    mouse_dy = mouse_y - mouse_last_y;

    // For android-style dragging the lists..
    mouseSpeedY = (mouseSpeedY+(mouse_dy))/2.0d;
    mouseSpeedX = (mouseSpeedX+(mouse_dx))/2.0d;

    // Mouse dragging. This is one way also to use this mouse's class dragging information.
    if (dragging) {
        moveOffset(mouse_dx,mouse_dy);
    }
    // States for draggin..
    if (!leftButtonStatePrevious && leftButtonState) {
        if (isInsideWindowDragPortion())
            dragWindow=true;
        startDrag();
    }
    if (leftButtonStatePrevious && !leftButtonState) {
        endDrag();
    }

    if (clicked()) {
        // Clicked inside of 200ms twice? It's a double click then..
        if (al_get_time()-lastClickTime<0.2f) {
            doubleClickOccured=true;
        }
        lastClickTime=al_get_time();
    } else {
        doubleClickOccured=false;
    }

}


bool GUIMouse::isInsideWindowDragPortion() {
    if (mouse_x>0&&mouse_x<windowDragBarWidth&&mouse_y>0&&mouse_y<windowDragBarHeight)
        return true;
    else
        return false;
}

void GUIMouse::startDrag()
{
    drag_start_x = mouse_x;
    drag_start_y = mouse_y;
    drag_offset_x=0;
    drag_offset_y=0;
    dragging = true;
}

void GUIMouse::moveOffset(int x, int y)
{
    drag_offset_x += x;
    drag_offset_y += y;
}

void GUIMouse::endDrag()
{
    dragging = false;
    drag_start_x = 0;
    drag_start_y = 0;
    dragWindow=false;
}


