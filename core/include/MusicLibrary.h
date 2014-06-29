#include "Song.h"
#include "BWManager.h"

#include <vector>
#include <string>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#ifndef MUSICLIBRARY_H
#define MUSICLIBRARY_H


//#include "..\include\BWManager.h"
//#include "..\include\Song.h"
//#include "Song.h"

#define BROWSE_MULTIPLE_FILES 0
#define BROWSE_FOLDER         1

enum LIST_MODES {  MOST_ATTENTIVE_LIST,
                   MOST_MEDITATIVE_LIST,
                   MOST_FLOW_LIST,
                   USER_CHOSEN_STATE_LIST,
                   BCI_STATE_LIST,
                   AMOUNT_OF_LIST_MODES
                }; // Notice that the last one tells the amount of these things..

using namespace std;

/**
 * \brief MusicLibrary.h ...::: Music Library :::...
*
* This is the actual music library of the project. This library holds all the songs of current session.
* This whole library can be saved and stored. Saving will store the songs and their preinformation of the graphs.
*
* This class has simple import tools.
*
* This class also handles calculation of the relative values of different graph-stages(uses song's brainwave information for that).
*
* This class has also the DJ Properties which means this is used to request different featured songs according
* different brainwaves.
*
* author: tonis @ 2013
*
*/
class MusicLibrary
{
public:
    MusicLibrary();
    MusicLibrary(string initialFilePath);
    void importFiles(const char* filePath);
    void openSongFilesImportDialog(int flag);
    virtual ~MusicLibrary();
    void reset();
    void removeSong(string id);
    void refresh();
    void createSongBWInfoToLibrary(string sId);
    int getRndPeakDistanceMapPos(string songId,int mode);
    string getRndSongAccordingBrainWaveInfo(int mode);
    bool isEmpty();
    bool isInDJModeLastPlayedList(string searchSongId);
    void addSongToDJModeLastPlayedList(string addSongId);

    float getRelativeValueInAllSongsOfChannel(int chanIndx, float value);
    float getMinimumValueInAllSongsOfChannel(int chanIndx);
    float getMaximumValueInAllSongsOfChannel(int chanIndx);


    vector<Song>song;
    Song getSong(string id);
    Song *getSongAddr(string id);
protected:
private:
    // Boost serialization. This way it's easier to save and load everything without parsing problems.
    // I doubt this won't work as the song stuff is in vector.. hmm..
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        //ar & initialPath;
        ar & song;
    }

    boost::filesystem::path initialPath;
    vector<string> lastDJPlayedItems;
    double lastUpdatedGraphCalculatedInformationTime;
};

#endif // MUSICLIBRARY_H
