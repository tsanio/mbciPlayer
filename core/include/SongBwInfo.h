#include <string>
#include <audiere.h>
#include <boost/filesystem.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "PlayerCommon.h"
#include "ChannelInfo.h"

#ifndef SONGBWINFO_H
#define SONGBWINFO_H

using namespace std;
using namespace audiere;

/**
 * \brief SongBwInfo.h/cpp ...:::Song's brainwave information holder:::...
*
* This class is like a keeper of all channels' information of specific song. It holds and updates the channels infos of song. Also draws something..
*
* Author: tonis @ 2013
*
*/
class SongBwInfo
{
public:
    SongBwInfo();
    virtual ~SongBwInfo();
    void createSongBwInfo();
    void draw(); // Overwrite :)
    void reset();

    bool isReady() const
    {
        return created;
    }

    ChannelInfo channelInfo[AMOUNT_OF_CHANNELS];

protected:
private:
    // Boost serialization. This way it's easier to save and load everything without parsing problems.
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & channelInfo;
        ar & created;
    }
    bool created;

};

#endif // SONGBWINFO_H



