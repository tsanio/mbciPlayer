#include "../include/SongBwInfo.h"


using namespace plrCommon;



SongBwInfo::SongBwInfo()
{
    //ctor
    reset();
}


SongBwInfo::~SongBwInfo()
{
    //dtor
}


/*
* Reset()
*::::::::
*
* This method just resets all the channgels of this song's brainwave info catalog.
*
*/

void SongBwInfo::reset()
{
    created = false;
    //setZ(50); // Make it drawn on top :)
    for (int i=0; i<AMOUNT_OF_CHANNELS; i++)
        channelInfo[i].reset();
}



/*
* CreateSongBwInfo()
*:::::::::::::::::::
*
* This method creates the all channels' graphs from current situation from the brainwave manager.
* This is usually called to update the peak positions and avarage values of song's mini-info.
*
*/

void SongBwInfo::createSongBwInfo()
{
    // :)
    for (int i=0; i<AMOUNT_OF_CHANNELS; i++)
    {
        channelInfo[i].createFromGraph(bwManager.channel[i],i); // Pass the channel's graph and index to creator
        //playerLog("This is " + intToStr(i+1) + " and " + intToStr(AMOUNT_OF_CHANNELS-i) +  " channels left..");
    }
    created=true;

}



/*
* draw()
*:::::::::::::::::::
*
* This method simply draws all the channels.
*
*/

void SongBwInfo::draw()
{
    if (created)
    {
        for (int i=0; i<AMOUNT_OF_CHANNELS; i++)
            channelInfo[i].draw();
    }
}
