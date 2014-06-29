#ifndef CHANNELINFO_H
#define CHANNELINFO_H

#include <math.h>
#include <string>
#include "BWGraph.h"

//#include "playerCommon.h"
/**
 * \brief ChannelInfo.h ...:::ChannelInfo (avgMiniGraph, peakPos, avarage):::...
*
* This class is suppsosed to hold mini information of the actual graphs for each song. This is the extracted information.
* Why here is litte graph version of the bigger one? It's because it safes space and smoothens the peak position calculations to more reasonable.
* Here are
* -Avarage value of the graph.
* -Calculations for mini graph.
* -Calculations for peakpositions and gathering them.
*
* author: tonis @ 2013
*
*/
class ChannelInfo
{
public:
    ChannelInfo();
    virtual ~ChannelInfo();
    void calculateSmoothAvarageMiniGraph(BWGraph graphP);
    void findPeakPositions();
    void calculateAvarageValue();
    void createFromGraph(BWGraph graphP, int channelIndexP);
    int getRndPeakPosInSecs();
    int getMotherGraphType() { return motherGraphType; }
    void calculateRecordingPercentage();
    void draw();
    void reset();
    float getAvarageValue() const
    {
        return avarageValue;
    }
    float getRecordedPercentage() const {
        return recordedPercentage;
    }
    float getSmoothValueFromTimePos(int timeP);
protected:
private:
    // Boost serialization. This way it's easier to save and load everything without parsing problems.
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & avarage;
        ar & peakPos;
        ar & peakPosIndex;
        ar & peakPosValue;
        ar & motherGraphType;
        ar & avarageValue;
        ar & divisionInSecs;
        ar & channelIndex;
        ar & recordedPercentage;
        ar & x;
        ar & y;
        ar & h;
        ar & lengthInSecs;
        ar & w;
    }
    vector<int> avarage;
    vector<int> peakPos;
    vector<int> peakPosValue;
    vector<int> peakPosIndex;
    int motherGraphType;
    float avarageValue;
    int thisAddedValue;
    int divisionInSecs;
    int channelIndex;
    float recordedPercentage;
    int x;
    int y;
    int h;
    int lengthInSecs;
    int w;
};

#endif // CHANNELINFO_H
