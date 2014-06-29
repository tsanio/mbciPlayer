#ifndef GUIELEMENT_H
#define GUIELEMENT_H

#include <string>

#define STORABLE    true
#define UN_STORABLE false

#define DEFAULT_Z_INDEX 15

using namespace std;
/**
 * \brief GUIElement.h ...::: GUI Element - The element of all these GUI interface toys :::...
*
* This is the very basic definition of GUI Element we have here. All other classes inherits from
* here and they follow the schema of init,refresh,draw,hide,show and set z-position to implement
* the very basic GUI functionality that is required in this project.
*
* NOTE: All GUIElements are listed into GUIElementList by default!! This is how this GUI engine works.
*
* author: tonis @ 2013
*
*/
class GUIElement
{
public:
    GUIElement();
    virtual ~GUIElement();
    virtual void draw();
    virtual void refresh();
    virtual void init();
    virtual void passiveStateRefresh();
    void setToolTipArea(int xt, int yt, int width, int height, string text);
    void setToolTipTextOnly(string text);
    void setToolTipAreaOnly(int xt, int yt, int width, int height);
    int getZ()
    {
        return z;
    }
    void setZ( int zParam )
    {
        z = zParam;
    }
    int getId()
    {
        return id;
    }
    void setId( int idP )
    {
        id = idP;
    }
    string getName()
    {
        return name;
    }
    void setActivity( bool activity );
    void setName( string nameP )
    {
        name = nameP;
    }
    void show();
    void hide();
    void unLoad();
    void showAndActivate();
    void hideAndUnactivate();
    bool isStorable() {
        return storable;
    }
    void setStorable(bool sval) {
        storable = sval;
    }
    bool isVisible()
    {
        return visible;
    }
    bool isActive()
    {
        return active;
    }
protected:
private:
    int id;
    int z;
    string name;
    bool visible;
    bool storable;
    bool active;
    float toolTipX;
    float toolTipY;
    float toolTipWidth;
    float toolTipHeight;
    string toolTipText;
    float onToolTip;
};
#endif // GUIELEMENT_H

