#include <sstream>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>
#include <allegro5/allegro_native_dialog.h>
#include "../include/MusicLibrary.h"
#include "../include/PlayerCommon.h"

using namespace std;
using namespace boost::filesystem;
using namespace plrCommon;

MusicLibrary::MusicLibrary()
{
    //constructor
}

MusicLibrary::MusicLibrary(string initialFilePath)
{
    //constructor
    initialPath = path(initialFilePath);
}
MusicLibrary::~MusicLibrary()
{
    //dtor
}

void MusicLibrary::reset()
{
    // Initialize everyhthing again. This is for clearing the list again totally..
    song.clear();
}

void MusicLibrary::removeSong(string id)
{
    for(size_t i = 0; i < song.size(); ++i)
    {
        if (song[i].getId() == id) // Remove songs with corresponding id's
            song.erase (song.begin()+i);
    }
}

Song MusicLibrary::getSong(string id)
{
    for(size_t i = 0; i < song.size(); ++i)
    {
        if (song[i].getId() == id) // Return song if id matched!
            return song[i];
    }
    // Otherwise null
    playerLog("\n ERROR: Didn't find song in MusicLibrary(getSong)! \n");
    return Song(NULL);
}

Song *MusicLibrary::getSongAddr(string id)
{
    for(size_t i = 0; i < song.size(); ++i)
    {
        if (song[i].getId() == id) // Return song if id matched!
            return &song[i];
    }
    // Otherwise null.. This should actually crash or something..
    playerLog("Couldn't find song!!!..AaaaRGH!");
    return NULL;
}

// This could be called more often..
void MusicLibrary::createSongBWInfoToLibrary(string sId)
{
    // ..
    if (!isEmpty()) {
        Song *updateSong = getSongAddr(sId);
        if (updateSong!=NULL)
            updateSong->updateSongBrainWaveInformation();
        else
            playerLog("Asked song was removed already?..");
    }
    else
        playerLog("Nothing to update..");
}

// This acts similarly as query.. in first and push out from back
void MusicLibrary::addSongToDJModeLastPlayedList(string addSongId)
{
    // Get the good size of last played list .. usually percentage ie 25% of the whole list size. This helps not to play the same songs over again.
    int sizeOfLastPlayedList = DJModeLastPlayedItemsListSize*song.size();
    if (sizeOfLastPlayedList==0)
        sizeOfLastPlayedList=1;

    lastDJPlayedItems.insert ( lastDJPlayedItems.begin() , addSongId );
    if (lastDJPlayedItems.size()>sizeOfLastPlayedList)
        lastDJPlayedItems.pop_back(); // Keep the size in limits..
}

// Carefull.. Check the dj list.. not anything else
bool MusicLibrary::isInDJModeLastPlayedList(string searchSongId)
{
    for(size_t i = 0; i < lastDJPlayedItems.size(); i++)
    {
        if (lastDJPlayedItems.at(i) == searchSongId) // Return song if id matched!
            return true;
    }
    return false;
}


float MusicLibrary::getRelativeValueInAllSongsOfChannel(int chanIndx, float value)
{
    float minV = getMinimumValueInAllSongsOfChannel(chanIndx);
    float maxV = getMaximumValueInAllSongsOfChannel(chanIndx);
    float divider = (maxV - minV);
    if (divider == 0) divider = 1.0f; // Prevent division by zero.
    float relativeV = (value - minV)/divider;
    if (relativeV<0.0f) // It sometimes goes over board If it's taken from EEG, because it can have different values.
        relativeV=0.0f;
    if (relativeV>1.0f)
        relativeV=1.0f;
    return relativeV;
}

float MusicLibrary::getMinimumValueInAllSongsOfChannel(int chanIndx)
{

    float minV = 9999999999;
    for(size_t i = 0; i < song.size(); ++i)
    {
        // Count only those which have bw info..!
        if (song[i].isBwInfoUsable())
        {
            if (song[i].getChannelAvg(chanIndx) < minV)
            {
                minV = song[i].getChannelAvg(chanIndx);
            }
        }
    }

    return minV;
}

float MusicLibrary::getMaximumValueInAllSongsOfChannel(int chanIndx)
{

    float maxV = 0;
    for(size_t i = 0; i < song.size(); ++i)
    {
        // Count only those which have bw info..!
        if (song[i].isBwInfoUsable())
        {
            if (song[i].getChannelAvg(chanIndx) > maxV)
            {
                maxV = song[i].getChannelAvg(chanIndx);
            }
        }
    }

    return maxV;
}


// Map point class. Used to map songs and sort them to find the right songs to play in DJ Mode.
class MapPoint
{
public:
  MapPoint(int ip, double dp);
  int indexP;
  double distP;
  bool operator<(const MapPoint &rhs) const { return distP < rhs.distP; }
};
MapPoint::MapPoint(int ip, double dp) {
    indexP = ip;
    distP = dp;
}


// Search the 2dimensional closest point or some of the closest point and return it.
int MusicLibrary::getRndPeakDistanceMapPos(string songIdp, int mode)
{
    // Loop through simplified information (smoothened) to find the closest position first and return it.
    vector<MapPoint> foundPosListOfCandidates;
    foundPosListOfCandidates.clear();
    Song thisSong = getSong(songIdp);

    // 1. List all peak position distances into list .. Like high scores. Use smooth graphs so it's same just to jump in periods of 5 secs or so.
    double thisDistance;
    for (int i=0; i<thisSong.getLengthInSecs(); i+=SMOOTH_GRAPH_DIVISION_IN_SECS)
    {
        if (i<thisSong.getLengthInSecs())
        {
            if (mode == USER_CHOSEN_STATE_LIST)
                thisDistance = thisSong.getMapDistanceFromTimeLinePointAutoAxis(XYDJPicker.getValX(),XYDJPicker.getValY(), i);
            if (mode == BCI_STATE_LIST)
                thisDistance = thisSong.getMapDistanceFromTimeLinePointAutoAxis(bwMeter.getCurrentProfileMindState(bwMeter.PROFILE_CHANNEL_1),bwMeter.getCurrentProfileMindState(bwMeter.PROFILE_CHANNEL_2), i);

            MapPoint mp(i,thisDistance);
            foundPosListOfCandidates.insert ( foundPosListOfCandidates.begin() , mp );
        }
    }

    // 2. Sort table like high scores should be sorted.
    sort(foundPosListOfCandidates.begin(), foundPosListOfCandidates.end());

    // Pick random peak from "high scores" :)
    if (foundPosListOfCandidates.size()>0)
    {
        srand (time(NULL));
        int picked = rand() % int(foundPosListOfCandidates.size()*DJModeTopRankingRate);
        playerLog("picked index " + intToStr(picked) + " with timep "  + intToStr( foundPosListOfCandidates[picked].indexP) + " and dista " + desToStr( foundPosListOfCandidates[picked].distP));
        return foundPosListOfCandidates[picked].indexP;
    }

    // Or the standard.
    return 1; // Return time in seconds.
}

string MusicLibrary::getRndSongAccordingBrainWaveInfo(int mode)   // Perhaps another parameter for accurate flow or it would be multiplied x 100 to make it more accurate and then divided? :)
{

    vector<MapPoint> candidates;
    candidates.clear();

    srand (time(NULL));
    double thisDistance;
    // 1. List the songs with their distance to asked profile for sorting
    for (int i=0; i<song.size(); i++)
    {
         //if (song[rndSongIndex].isBwInfoCreated())
        if (song[i].isBwInfoUsable())
        {
            if (mode == USER_CHOSEN_STATE_LIST)
                thisDistance = song[i].getProfileMapDistanceToAutoAxis(XYDJPicker.getValX(),XYDJPicker.getValY());
            if (mode == BCI_STATE_LIST)
                thisDistance = song[i].getProfileMapDistanceToAutoAxis(bwMeter.getCurrentProfileMindState(bwMeter.PROFILE_CHANNEL_1),bwMeter.getCurrentProfileMindState(bwMeter.PROFILE_CHANNEL_2));
                // Check also that this song is not already played ie. just last round or few rounds ago it would be strange to play it over and over again.
                if (!isInDJModeLastPlayedList(song[i].getId()))
                {
                    // Merge candidate for later usage..
                    MapPoint mp(i,thisDistance);
                    candidates.insert ( candidates.begin() , mp );
                }
        }
    }

    // 2. Sort it like highscores!
    sort(candidates.begin(), candidates.end());

    // 3. Pick some of the yammiest :p
    if (candidates.size()>0)
    {
        int picked = rand() % int(candidates.size()*DJModeTopRankingRate);
        addSongToDJModeLastPlayedList(song[candidates[picked].indexP].getId());
        return song[candidates[picked].indexP].getId();
    }

    // If something fails.. Let's give just random song..
    string rndSongId = song[rand() % song.size()].getId();
    addSongToDJModeLastPlayedList(rndSongId);
    return rndSongId;

}


bool MusicLibrary::isEmpty()
{
    if (song.size()>0)
        return false;
    else
        return true;
}

// Refreshh!!
void MusicLibrary::refresh()
{

    // Update in 1 second interval.. I gues it's enough for now..
    if (al_get_time()-lastUpdatedGraphCalculatedInformationTime>1.0d)
    {
        lastUpdatedGraphCalculatedInformationTime = al_get_time();
        createSongBWInfoToLibrary(playThis.getId());
    }

}

// flag ALLEGRO_FILECHOOSER_FOLDER or ALLEGRO_FILECHOOSER_FOLDER for example.. To choose folder or just file choosing dialog.
void MusicLibrary::openSongFilesImportDialog(int flag)
{
    ALLEGRO_FILECHOOSER *fchooser;
    if (flag == BROWSE_MULTIPLE_FILES)
        fchooser = al_create_native_file_dialog(initialPath.string().c_str(),"Choose song files.","*.mp3;*.ogg;*.wav;*.flac;*.aiff;*.mod;*.s3m;*.xm;*.it;*.*",ALLEGRO_FILECHOOSER_MULTIPLE);
    else if (flag == BROWSE_FOLDER)
        fchooser = al_create_native_file_dialog(initialPath.string().c_str(),"Choose song folder.","*.mp3;*.ogg;*.wav;*.flac;*.aiff;*.mod;*.s3m;*.xm;*.it;*.*",ALLEGRO_FILECHOOSER_FOLDER);
    else
        fchooser = al_create_native_file_dialog(initialPath.string().c_str(),"Choose song files.","*.mp3;*.ogg;*.wav;*.flac;*.aiff;*.mod;*.s3m;*.xm;*.it;*.*",ALLEGRO_FILECHOOSER_MULTIPLE);

    al_show_native_file_dialog(display,fchooser);

    int amountOfChosenPaths = al_get_native_file_dialog_count(fchooser);

    //const ALLEGRO_PATH = al_get_native_file_dialog_path(fchooser, amountOfChosenPaths);
    playerLog("Amount of chosen files: " + intToStr(amountOfChosenPaths) + "\n");

    if (amountOfChosenPaths>0)
    {
        for (int i=0; i<amountOfChosenPaths; i++)
        {
            const char *chosenPath = al_get_native_file_dialog_path(fchooser, i);
            if (chosenPath != NULL)
            {
                //fprintf(stderr, ("Chosen paths: " + string(chosenPaths) + "\n").c_str());
                playerLog(("Looking for chosen path " + intToStr(i) + ": " + string(chosenPath) + "\n").c_str());
                importFiles(string(chosenPath).c_str());
            }
            else
                playerLog("Null was given from the filechoser..");
        }
    }
    else
    {
        playerLog("No files chosen..");
    }
    al_destroy_native_file_dialog(fchooser);
}

void MusicLibrary::importFiles(const char *importPath)
{

    path p (importPath);

    try
    {
        if (exists(p))    // does p actually exist?
        {
            if (is_regular_file(p))         // is p a regular file?
            {

                if (isValidMusicPlayerFile(p))
                {
                    Song newSong(p);
                    if (newSong.isSeekable() && !newSong.hasFailedToLoad())  // Only seekables seems to work in our program nowadays..
                        song.push_back(newSong);
                }
                else
                {
                    // Not correct music file..
                }
            }

            else if (is_directory(p))      // is p a directory?
            {
                typedef vector<path> vec;             // store paths,
                vec v;                                // so we can sort them later

                copy(directory_iterator(p), directory_iterator(), back_inserter(v));

                sort(v.begin(), v.end());             // sort, since directory iteration
                // is not ordered on some file systems

                for (vec::const_iterator it (v.begin()); it != v.end(); ++it)
                {
                    importFiles((*it).string().c_str()); // Recursion
                }
            }

            else
            {
                //playerLog(p.string() + " exists, but is neither a regular file nor a directory\n");
            }
        }
        else
        {
            //playerLog(p.string() + " does not exist\n");
        }
    }

    catch (const filesystem_error& ex)
    {
        //playerLog(ex.what() + '\n');
    }
}
