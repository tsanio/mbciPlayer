#include <string>
#include <audiere.h>
#include <boost/filesystem.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "SongBwInfo.h"

#define ARTIST_PATH 0
#define ALBUM_PATH 1
#define TITLE_PATH 2

#ifndef SONG_H
#define SONG_H

using namespace std;
using namespace audiere;

/**
 * \brief Song.h ...::: The Song - Item Of Medialibrary :::...
*
* This class represents a song that holds filepath, mini brainwave information and all othe song's related information neccessary.
* The exact brainwave information can be also accessed through here from bwInfo->channelInfo->graph, but this is only loaded during runtime
* whilst preparing the preinformation. In other words it's quite reliable to use during runtime!
*
* author: tonis @ 2013
*
*/
class Song
{
public:
    Song();
    Song(boost::filesystem::path p);
    Song(int number);
    virtual ~Song();
    string getPathStr() const
    {
        return pathStr;
    }
    string getRelativePathStr() const
    {
        return relativePathStr;
    }
    void setPathStr(string val)
    {
        pathStr = val;
    }
    string getId() const
    {
        return id;
    }
    void setId(string val)
    {
        id = val;
    }
    string getAlbum()
    {
        return album;
    }
    void setAlbum(string val)
    {
        album = val;
    }
    string getArtist() const
    {
        return artist;
    }
    void setArtist(string val)
    {
        artist = val;
    }
    string getTitle() const
    {
        return title;
    }
    string parseInfoFromPath(string properPath, int whatYaSeeking);
    ALLEGRO_COLOR getTextColor() const;
    void setTitle(string val)
    {
        title = val;
    }
    bool isNull()
    {
        return thisIsEmptySong;
    }
    bool isBwInfoCreated()
    {
        return bwInfoCreated;
    }
    bool isBwInfoUsable() const;
    float getPositionInSecs_f(int frames);
    int getPositionInSecs(int frames);
    int getPositionInFrames(int seconds);
    int getLength() const
    {
        return length;
    }
    int getLengthInSecs() const
    {
        return int(length/sampleRate);
    }
    int getLengthSeconds() const
    {
        return musicLengthSeconds;
    }

    int getPositionRemainingSecondsInView(int frames)
    {
        return (int((musicLengthSeconds-getPositionInSecs(frames) ) )%60);
    }
    int getPositionRemainingMinutesInView(int frames)
    {
        return int((musicLengthSeconds-getPositionInSecs(frames) ) /60);
    }

    int getLengthSecondsInView() const
    {
        return (int(musicLengthSeconds)%60);
    }
    int getLengthMinutesInView() const
    {
        return int(musicLengthSeconds/60);
    }
    int getLengthHoursInView() const
    {
        return int(musicLengthSeconds/60/60);
    }

    int getPlayedSecondsInView(int frames)
    {
        return (int((getPositionInSecs(frames) ) )%60);
    }
    int getPlayedMinutesInView(int frames)
    {
        return int((getPositionInSecs(frames) ) /60);
    }
    int getPlayedHoursInView(int frames)
    {
        return int((getPositionInSecs(frames) ) /60/60);
    }

    bool isSeekable()
    {
        return seekable;
    }
    bool hasFailedToLoad()
    {
        return failedToLoad;
    }
    void updateSongBrainWaveInformation();
    void drawSongExtractedBwInformation();
    SongBwInfo getSongBwInfo()
    {
        return songBwInfo;
    }
    float getAvgFlowAxisValue() const;
    //float getAvgFlowValueZeroToHundred() const;
    float getAvgFlowAxisValueMinusHundredToHundred() const;
    float getChannelAvg(int chanIx) const;
    float getProfileMapDistanceToAutoAxis(float v1, float v2);
    float getMapDistanceFromTimeLinePointAutoAxis(float v1, float v2, int timeP);
    string getItemAvgBWDataStr() const;
    string getStrChannelAvgAndDescription(int chanIndx) const;
    string getStrChannelAvg(int chanIndx) const;
    string getStrChannelDescription(int chanIx) const;
    string getSongPositionStr(int frames);

protected:
private:
    // Boost serialization. This way it's easier to save and load everything without parsing problems.
    friend class boost::serialization::access;
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & pathStr;
        ar & relativePathStr;
        ar & id;
        ar & album;
        ar & artist;
        ar & title;
        ar & length;
        ar & sizeOfFile;
        ar & musicLengthSeconds;
        ar & thisIsEmptySong;
        ar & channelCount;
        ar & sampleRate;
        ar & seekable;
        ar & bwInfoCreated;
        ar & songBwInfo;
        ar & failedToLoad;
    }

    string pathStr;
    string relativePathStr;
    string id;
    string album;
    string artist;
    string title;
    string sizeOfFile;
    bool   seekable;
    int    length;
    bool   thisIsEmptySong;
    float  musicLengthSeconds;
    int    channelCount;
    int    sampleRate;
    bool   failedToLoad;
    bool   bwInfoCreated;
    SongBwInfo songBwInfo;
};

#endif // SONG_H
