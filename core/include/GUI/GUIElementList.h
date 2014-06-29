#ifndef GUIELEMENTLIST_H
#define GUIELEMENTLIST_H

#include <string>
#include <vector>
#include "GUIElement.h"
/**
 * \brief GUIElementList.h ...::: GUI Element List - Main list of all GUI Elements :::...
*
* This is where all GUI Elements are listed and handled in each update. Thanks.
*
* author: tonis @ 2013
*
*/
class GUIElementList
{
public:
    GUIElementList();
    virtual ~GUIElementList();
    GUIElement* addElement(GUIElement* elem);
    GUIElement* addElement(GUIElement* elem, int zIndex);
    vector<GUIElement*> getElements() { return element; }
    void refresh();
    void draw();
    void updateZIndexes();
    void showElementsInfo();
    void initElements();
    void unLoadElements();
    // bool compareZ(GUIElement *a, GUIElement *b);
protected:
private:
    int elementIdCounter;
    vector<GUIElement*>element;
    //Song getElement(string id);
    //Song getElement(string name);
};

#endif // GUIELEMENTLIST_H
