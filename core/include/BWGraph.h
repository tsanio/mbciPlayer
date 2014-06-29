#ifndef BWCHANNEL_H
#define BWCHANNEL_H

#include <math.h>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/array.hpp>
#include <audiere.h>
#include "GUI/GUIMouse.h"
#include "GUI/GUIElementList.h"
#include "ImageLibrary.h"

using namespace std;


#define GRAPH_TABLE_LOG_SIZE 3600

#define GRAPH_SIZE_X 499
#define GRAPH_SIZE_Y 30
#define GRAPH_SPACING 2

// The empty index to in graph. This means this is not recorded point.
#define GRAPH_EMPTY_INDEX -1

#define deltaHzMax   4.0f
#define thetaHzMax   7.0f
#define alphaHzMax   11.0f
#define betaHzMax    20.0f

typedef struct
{
    float r;
    float g;
    float b;
    string text;
    float bottomFreq;
    float topFreq;
    float centerFreq;
    bool useForAvarageCalculation;
    bool showInBarPanel;
    bool useInRelativeCalculation;
} CHANNEL_THEME;

string        getGraphDescriptionText(int type);
CHANNEL_THEME   getGraphTheme(int type);
ALLEGRO_COLOR getGraphThemeColorOf(int type);
ALLEGRO_COLOR getGlobalColorOfFrequency(double value);
bool useThisTypeForAvarageCalculation(int type);
bool useThisTypeForOpenEEG(int type);
bool useThisTypeInRelativeCalculations(int type);
int  getThisTypeCenterFreq(int type);

/**
 * \brief BWGraph.h ...::: Brainwave Graph - Dataholder for brainwave channel logging. :::...
*
* This class holds the information of one single channel of brainwave information. It receives the updates from BWManager(which receives it from BWMeter).
* Here are functions to draw the graphs and put them information. Also here exists the themes of each different graphs as they have different colors and center frequencies etc.
*
* Sidenode(This is captured originally from my old project for recording brainwaves also.):
*  Toni Sanio 2011
*  #18.10.2011 class created..
*  #7.7.2013 Class captured and modified for music player with bci.. One graph represents the channel of information from bci device.
*
* author: tonis @ 2013
*
*/
class BWGraph
{
public:
    BWGraph();
    virtual ~BWGraph();
    void create(int x2, int y2, int type2, int secAmount);
    void draw(float vShade);
    void refresh();
    void refresh(double newValue);
    void insertTo(int gIndex,double newValue);
    double getCurrentValue();
    double getCurrentRawValue();
    double getHzCenter();
    double getValueOfCurrentSongsPlayedPos();
    ALLEGRO_COLOR getCurrentDrawColor(double value);
    ALLEGRO_COLOR getColorOfFrequency(double value);
    void clear();
    void makeZeroLine();
    double getCurrentWeightedValue();
    bool useThisForAvarageCalculation();
    //void makeSineLine();
    double valueOf(int gIndex);
    void dontUseLastPosition();
    double getLatestUpdatedValue();
    void drawWholeGraph();
    void drawZeroLine();

    int x,y,w,h;
    int freq;
    int lastTime;
    int type;
    int ySize;
    int lengthInSecs;
    int recordToTimePoint;
    float alpha;
    int recordToTimePointLast;
    bool dontUseLastPositionThisTime;

    // Raw values array that holds actually the most precious data :)
    vector<double> rawValues;
    double  value;
    string text;
    float p2;
    double theLatestScaledValue;

    //vector <int>rawValues;
    ALLEGRO_BITMAP *graphPic;
    ALLEGRO_COLOR chosenColor;

protected:
private:
    // Boost serialization. This way it's easier to save and load everything without parsing problems.
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & rawValues;
        ar & text;
        ar & alpha;
        ar & value;
        ar & text;
        ar & p2;
        ar & x;
        ar & y;
        ar & w;
        ar & h;
        ar & freq;
        ar & lastTime;
        ar & type;
        ar & ySize;
        ar & lengthInSecs;
        ar & recordToTimePoint;
        ar & alpha;
        ar & recordToTimePointLast;
        ar & dontUseLastPositionThisTime;
        ar & theLatestScaledValue;
    }


};

#endif // BWCHANNEL_H

