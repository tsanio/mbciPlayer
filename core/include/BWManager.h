#include <ctime>
#include <boost/filesystem.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
/*#include <boost/serialization/vector.hpp>*/
#include <boost/array.hpp>
#include "GUI/GUIElement.h"
//#include "PlayerCommon.h"
#include "BWGraph.h"
#include "BWMeter.h"

#ifndef BWMANAGER_H
#define BWMANAGER_H


using namespace std;
//using namespace plrCommon;

/**
 * \brief BWManager.h ...:::Brainwave Manager Takes Care Of The Graphs:::...
*
* This class holds all the main graphs, updates(Data from BWMeter), draws them and calculates avarageGraph as well.
*
* author: tonis @ 2013
*
*/
class BWManager : public GUIElement
{
public:
    BWManager();
    BWManager(string s, int posX, int posY, int width, int height, string toolTipText);
    virtual ~BWManager();
    BWGraph channel[AMOUNT_OF_CHANNELS];
    void draw();
    void refresh();
    void init();
    void fly();
    void reset();
    void unLoad();
    void newBWTable(string songId,int songLengthInSecs);
    void initClearTable(string songId,int songLengthInSecs);
    void positionChanged();
    void stopRecording();
    void startRecording();
    void drawAllAgain();
    float getLastVirtualCurrentFlowValue();
    float getLastVirtualRelativeStateOfChannel(int chanIndxp);
    float visibilityShade;
    void adjustVisibility(int direction);
    int graphUIX;
    int graphUIY;
    int graphWidth;
    int graphHeight;
    bool mouseInsideManager();
    bool mouseClickedInsideManager();
protected:
private:
    bool recording;
    double lastGraphUpdate;
    double avgV;
    string bwRecordId;
    int bwRecordLengthInSecs;
    int errCode; // This IS UNNECCESSARY DELETE THIS WHEN UPDATING THIS VERSION
    // Boost serialization. This way it's easier to save and load everything without parsing problems.
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & graphUIX;
        ar & graphUIY;
        ar & channel;
        ar & bwRecordId;
        ar & bwRecordLengthInSecs;
        ar & recording;
        ar & lastGraphUpdate;
        ar & avgV;
        ar & graphHeight;
        ar & graphWidth;
        //ar & connectionId;
        ar & errCode;
    }
};

#endif // BWMANAGER_H
