#include <algorithm>
#include <vector>
#include <string>
#include "../../include/GUI/GUIElementList.h"
#include "../../include/PlayerCommon.h"
#include "../../include/GUI/GUIElement.h"

using namespace plrCommon;
using namespace std;



GUIElementList::GUIElementList()
{
    //ctor
    elementIdCounter=0;
}

GUIElementList::~GUIElementList()
{
    //dtor
}

// Hey it's also possible to make it so that we create the items here IF neccessary. We'd need ie. boost::ptr_vector or  something like this typedef std::vector<std::unique_ptr<base>> container;

// Add element to the element list..
GUIElement* GUIElementList::addElement(GUIElement *elem) {
    addElement(elem, elem->getZ());
}

// Add element to the element list.. This time also z-index is given.
// This is deprecated to use outside from GUIElement, but it could be handled also that way if needed for some reason..
GUIElement* GUIElementList::addElement(GUIElement *elem, int zIndex) {
    element.push_back(elem);
    elem->setZ(zIndex);
    elem->setId(elementIdCounter);
    updateZIndexes(); // Update Z-indexes everytime we add something in the element list.
    elementIdCounter++;
    return elem;
}

void GUIElementList::refresh() {
    for(size_t i = 0; i < element.size(); ++i)
    {
        if (element[i]->isActive()) // Update only active elements..
            element[i]->refresh();
        else
            element[i]->passiveStateRefresh();
    }
}

void GUIElementList::initElements() {
    playerLog("Start initializing gui elements..\n");
    for(size_t i = 0; i < element.size(); ++i)
    {
            element[i]->init();
    }
}

void GUIElementList::unLoadElements() {
    playerLog("Unloading lots of stuff..\n");
    for(size_t i = 0; i < element.size(); ++i)
    {
            element[i]->unLoad();
    }
}

/*bool GUIElementList::compareZ(GUIElement *a, GUIElement *b) {
    if (a->getZ() < b->getZ())
        return true;
    else
        return false;
}*/

struct compareElementZIndexes {
  bool operator ()(GUIElement *a, GUIElement *b) { return a->getZ() < b->getZ(); }
};

void GUIElementList::updateZIndexes() {
    // Z tells the z-index. The ordering is the ordering of the vector.
    // Use the compare function to sort
    sort (element.begin(), element.end(), compareElementZIndexes());
}

// This is mostly to check that the Z-indexes went correct.
void GUIElementList::showElementsInfo() {
    for(size_t i = 0; i < element.size(); ++i)
    {
         //playerLog("Element's id: " + intToStr(element[i]->getId()) + " and z-index " + intToStr(element[i]->getZ()) + "\n");
    }
}

void GUIElementList::draw() {
    for(size_t i = 0; i < element.size(); ++i)
    {
         if (element[i]->isVisible())
                element[i]->draw();
    }
}


