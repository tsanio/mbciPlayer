#ifndef GUIMOUSE_H
#define GUIMOUSE_H

#include <allegro5/allegro.h>

#define BUTTON_DOWN    1
#define BUTTON_UP      0

/*
#define MOUSE_L		0x01
#define MOUSE_L_NEW	0x02
#define MOUSE_LS	(MOUSE_L | MOUSE_L_NEW)
#define MOUSE_R		0x04
#define MOUSE_R_NEW	0x08
#define MOUSE_RS	(MOUSE_R | MOUSE_R_NEW)
#define MOUSE_M		0x10
#define MOUSE_M_NEW	0x20
#define MOUSE_MS	(MOUSE_M | MOUSE_M_NEW)
*/
/**
 * \brief GUIMouse.h ...::: GUI Mouse - Mouse functionality :::...
*
* This is our mouse class. This can be used anywhere from project to request mouse state.
*
* author: tonis @ 2013
*
*/
class GUIMouse
{
public:
    GUIMouse();
    virtual ~GUIMouse();
    int getMouseX()
    {
        return mouse_x;
    }
    //void SetMouseX(int val) { mouse_x = val; }
    int getMouseY()
    {
        return mouse_y;
    }
    bool mouseIdle() {
        float idlelimit=0.00001f;
        if (getMouseSpeedX()>-idlelimit&&getMouseSpeedX()<idlelimit&&getMouseSpeedY()>-idlelimit&&getMouseSpeedY()<idlelimit)
            return true;
        else
            return false;
    }
    double getMouseSpeedY()
    {
        return mouseSpeedY;
    }
    double getMouseSpeedX()
    {
        return mouseSpeedX;
    }
    //void SetMouseY(int val) { mouseY = val; }
    int getMiddleWheelState()
    {
        return middleWheelState;
    }
    int getLeftButtonState()
    {
        return leftButtonState;
    }
    bool mouseWheelRolledUp()
    {
        return mouseWheelRolledUpV;
    }
    bool mouseWheelRolledDown()
    {
        return mouseWheelRolledDownV;
    }
    int getLeftButtonStatePrevious()
    {
        return leftButtonStatePrevious;
    }
    void setLeftButtonState(int val)
    {
        leftButtonState = val;
    }
    int getRightButtonState()
    {
        return rightButtonState;
    }
    int getRightButtonStatePrevious()
    {
        return rightButtonStatePrevious;
    }
    int getDragOffsetX()
    {
        return drag_offset_x;
    }
    int getDragOffsetY()
    {
        return drag_offset_y;
    }
    int getMouseDeltaX()
    {
        return mouse_dx;
    }
    int getMouseDeltaY()
    {
        return mouse_dy;
    }
    int isDraggingWindow()
    {
        return dragWindow;
    }
    bool isDragging()
    {
        return dragging;
    }
    void setRightButtonState(int val)
    {
        rightButtonState = val;
    }
    float getMouseDragStartX() {
        return drag_start_x;
    }
    float getMouseDragStartY() {
        return drag_start_y;
    }
    // Click returns the positions where mouse was clicked if it's dragging.. this makes sure it is not dragged from other element..
    float getMouseClickX() {
        if (isDragging()) {
            return drag_start_x;
        } else
            return getMouseX();
    }
    float getMouseClickY() {
        if (isDragging()) {
            return drag_start_y;
        } else
            return getMouseY();
    }
    bool clicked() {
        if (getLeftButtonStatePrevious() && !getLeftButtonState())
            return true;
        else
            return false;
    }
    bool doubleClicked() {
        return doubleClickOccured;
    }
    void mouseDown(ALLEGRO_MOUSE_EVENT *mouse);
    void mouseUp(ALLEGRO_MOUSE_EVENT *mouse);
    void updatePos(ALLEGRO_MOUSE_EVENT *mouse);
    void mouseUpdate(ALLEGRO_EVENT *event);
    void startDrag();
    void moveOffset(int x, int y);
    bool isInsideWindowDragPortion();
    void endDrag();
protected:
private:
    int mouse_b;
    int mouse_x, mouse_y;
    int mouse_dx, mouse_dy;
    int leftButtonState;
    int rightButtonState;
    int leftButtonStatePrevious;
    int rightButtonStatePrevious;
    int mouse_last_x,mouse_last_y;
    int middleWheelState;
    bool mouseWheelRolledUpV;
    bool mouseWheelRolledDownV;
    double mouseSpeedY;
    double mouseSpeedX;
    bool dragging;
    bool doubleClickOccured;
    double lastClickTime;
    int drag_start_x;
    int drag_start_y;
    int drag_offset_x;
    int drag_offset_y;
    int windowDragBarWidth;
    int windowDragBarHeight;
    bool dragWindow;
};

#endif // GUIMOUSE_H
