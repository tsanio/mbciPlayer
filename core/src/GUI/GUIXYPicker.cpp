#include "../../include/GUI/GUIXYPicker.h"

using namespace std;
using namespace plrCommon;

GUIXYPicker::GUIXYPicker()
{
    //ctor
}

GUIXYPicker::~GUIXYPicker()
{
    //dtor
}

GUIXYPicker::GUIXYPicker(int xP, int yP, int widthP, int heightP, string toolTipText) {
    //setZ(zIndexP);
    setStartUpValues(xP, yP, widthP, heightP);
    GUIElement::setToolTipArea(xP, yP, widthP, heightP, toolTipText);
    setZ(0);
}

bool GUIXYPicker::mouseIsInsidePanel() {
    if (guiMouse.getMouseX()>x && guiMouse.getMouseX()<x+width && guiMouse.getMouseY()>y && guiMouse.getMouseY()<y+height) {
        return true;
    }
    return false;
}

void GUIXYPicker::refresh() {

    // Handle mouse actions.

    if (mouseIsInsidePanel() && isVisible()) {

        if (guiMouse.getLeftButtonState()) {

            valXraw = (guiMouse.getMouseX() - x)/(width);
            valYraw = 1.0f-(guiMouse.getMouseY() - y)/(height); // Y-axis is upside down in screen :P

        }

    }

    // Disable the disabled axises to zero:
    if (!djModeUseXAxis.isChecked())
        valXraw = 0;
    if (!djModeUseYAxis.isChecked())
        valYraw = 0;

    // Claculate the real value. It's inverted or something a bit from Y-axis..
    valX = valXraw;
    valY = valYraw;

    // Let's add some more precise tooltip stuff..
    if (guiMouse.getLeftButtonState()) {
        if (valX<0.5f && valY<0.5f)
            setToolTipTextOnly("This area will play more passive music.");
        if (valX<0.5f && valY>0.5f)
            setToolTipTextOnly("This area will play more nice music.");
        if (valX>0.5f && valY<0.5f)
            setToolTipTextOnly("This area will play more energetic music.");
        if (valX>0.5f && valY>0.5f)
            setToolTipTextOnly("This area will play energetic and happy music.");
    } else {
        setToolTipTextOnly("Desired song rofile. Click for description in area.");
    }

}

void GUIXYPicker::draw() {

    // Draw background for plaa..
    drawScaledPicture(mapBG,x-boarder,y-boarder,width+boarder,height+boarder);

    // Draw dot.. The selected position. Or show where the brain is approximately now.
    if (djModeRadioButton.getSelectionTxt()=="BCI State List")
        drawRotatedTintedPicture(circleImg,getMapColor(bwMeter.getCurrentProfileMindState(bwMeter.PROFILE_CHANNEL_1),bwMeter.getCurrentProfileMindState(bwMeter.PROFILE_CHANNEL_2)),x+bwMeter.getCurrentProfileMindState(bwMeter.PROFILE_CHANNEL_1)*width,y+(1.0f-bwMeter.getCurrentProfileMindState(bwMeter.PROFILE_CHANNEL_2))*height,totalFrameTime);
    else
        drawRotatedTintedPicture(crossImg,getMapColor(valX,valY),x+valXraw*width,y+(1.0f-valYraw)*height,totalFrameTime);


    GUIElement::draw();
}

void GUIXYPicker::init() {

    mapBG     = imageLibrary.loadImage("media/gfx/DJMap.png");
    crossImg  = imageLibrary.loadImage("media/gfx/cross.png");
    circleImg = imageLibrary.loadImage("media/gfx/circle.png");

}

void GUIXYPicker::setStartUpValues(int xP, int yP, int widthP, int heightP) {

    boarder=5;
    x=xP+boarder;
    y=yP+boarder;
    width=widthP-boarder;
    height=heightP-boarder;
    valX=0.5f;
    valY=0.5f;
    valXraw=0.5f;
    valYraw=0.5f;
    // Prevent division by zero.
    if (width==0)
        width = 1;
    if (height==0)
        height = 1;
}

