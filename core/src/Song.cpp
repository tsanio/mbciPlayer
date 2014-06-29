#include <sstream>
#include <stdio.h>
#include <math.h>
#include <limits>
#include <string>
#include <boost/filesystem.hpp>
#include <audiere.h>
#include "../include/PlayerCommon.h"
#include "../include/Song.h"

using namespace boost::filesystem;
using namespace std;
using namespace plrCommon;
using namespace audiere;

Song::Song()
{
    //ctor
    pathStr = "unnamed";
    relativePathStr = "unnamed";
    id      = "unnamed";
    album   = "unnamed";
    artist  = "unnamed";
    title   = "unnamed";
    length  = 0;
    bwInfoCreated = false;
    thisIsEmptySong = true;
    songBwInfo.reset();
}

Song::Song(int number)
{
    thisIsEmptySong = true;
    bwInfoCreated = false;
    songBwInfo.reset();
}

/*
* Song(path p)
*:::::::::::::
*
* The This method loads this song with the given audio file and it's information for later usage.
*
* param1 : Path to the song.
*
* Returns : none
*/
Song::Song(path p)
{
    //constructor for loading directly from path..
    songBwInfo.reset();
    thisIsEmptySong = false;
    pathStr = "unnamed";
    relativePathStr = "unnamed";
    id      = "unnamed";
    album   = "unnamed";
    artist  = "unnamed";
    title   = "unnamed";
    length  = 0;
    bwInfoCreated = false;
    failedToLoad = false;

    sizeOfFile = intToStr(file_size(p));

    //playerLog("Loading the song information..");

    pathStr = p.string();
    path getcwd = boost::filesystem::current_path();
    relativePathStr = (naive_uncomplete(p, getcwd)).string(); // USe the naive method to find relative path..

    // Hmm this is excactly the same as in main.. So we should perhaps make this into global function? For now like this..
    SampleSourcePtr samplesource;
    string properPath = "";
    if ( boost::filesystem::exists( pathStr.c_str() ) )
    {
        //  playerLog("Found song audio file from absolute path!");
        properPath = pathStr.c_str();
    }
    else if ( boost::filesystem::exists( relativePathStr.c_str() ) )
    {
        //  playerLog("Found song audio file from relative path!.");
        properPath = relativePathStr.c_str();
    }
    // And which is the peroper path to open audio file?
    if (properPath!="")
    {
        //  playerLog("Opened PATH: " + properPath + " (size: " + sizeOfFile + ") \n");
        samplesource = OpenSampleSource (  properPath.c_str(), FF_AUTODETECT);
    }
    else
    {
        playerLog("Couldn't find song from either of these paths: " + pathStr + " OR " + relativePathStr + " ..\n ");
    }
    // else
    //   playerLog("ScrewD! Song is not loading again..");

    //OutputStreamPtr stream       = OpenSound (device, samplesource, true);

    if (!samplesource)
    {
        // failure
        //playerLog("Failed to load song! \n");
        failedToLoad=true;
    }
    else
    {

        seekable=true;

        // Is this seekable source?
        if (samplesource->isSeekable())
        {
            //playerLog("Source is seekable! :) ");

            // Find taginformation..
            for (int i=0; i<samplesource->getTagCount(); i++)
            {
                //  playerLog("Tag nro " + intToStr(i) + ".. key is " + samplesource->getTagKey(i) + " = " + samplesource->getTagValue(i) );
                //      playerLog("Tag nro " + i + " key is " + (samplesource->getTagKey(i)).string() + " and value is " + (samplesource->getTagValue(i)).string() );

                // Album
                if (string(samplesource->getTagKey(i)) == "album")
                    album  = samplesource->getTagValue(i);

                // Artist
                if (string(samplesource->getTagKey(i)) == "artist")
                    artist  = samplesource->getTagValue(i);

                // Title
                if (string(samplesource->getTagKey(i)) == "title")
                    title  = samplesource->getTagValue(i);

            }

            // Graph amount of frames and calculate seconds, minutes, hours etc
            length = samplesource->getLength();
            SampleFormat sampleFormat;
            samplesource->getFormat(channelCount,sampleRate,sampleFormat);

            // Length in seconds
            musicLengthSeconds = samplesource->getLength()/sampleRate;    // Retrieves number in seconds, ex: 175678555

            float musicLengthSecondsActual = (int(musicLengthSeconds)%60);
            float musicLengthMinutesActual = int(musicLengthSeconds/60);
            float musicLengthHoursActual   = int(musicLengthMinutesActual/60);


            // playerLog("Artist " + artist + " in his album " + album + " presenting song titled " + title);
            // playerLog("Songlength mm:ss -> "+intToStr(musicLengthMinutesActual) +":"+intToStr(musicLengthSecondsActual)+"\n");

            //stream->setRepeat(true);
            //stream->setVolume(0.5f); // 50% volume
            //stream->play();

            // Check if something left unnamed we'll give them names according folder and filenames :)
            if (artist=="unnamed" || artist=="") {
                artist = parseInfoFromPath(properPath,ARTIST_PATH);
            }
            if (album=="unnamed" || album=="") {  //NOTICE ! THUS IS JUST TURNED TO GET THE TITLE.. BECAUSE IT MAYBE THAT WE OONLY HAVE THE FILENAME And usually it's the same as title also :)
                album = parseInfoFromPath(properPath,ALBUM_PATH);
            }
            if (title=="unnamed" || title=="")
            {
                title = parseInfoFromPath(properPath,TITLE_PATH);
            }


            //id = "_" + artist + "_" + album + "_" + sizeOfFile;
            id = "_" + artist + "_" + album + "_"  + title + "_"; // And also add the file size after cutting this to the boost filesize limit characters amount.
            // Remove special characters to make sure this doesn't fuck up anything..
            // Remove folder characters If those are left for some reason! Or it will crash..
            //id = removeChars(id, "\\/!&',.-<>*=?+-");
            //playerLog("Now id is " + id);
            id = removeChars(id, "/\\.?");
            //playerLog("Now id is " + id);
            id = removeExcessFromMiddle(id, 90); // Limit the id filenamesize to 90 character for boost's sake. Don't worry about destroying important info when we can assume that there is probably just album info in the middle.. artis tand song is more relevant.. for me :)
            //playerLog("Now id is " + id);
            id += sizeOfFile; // Finally size of file

        }
        else
        {
            // playerLog("Source is not seekable! So this will be removed alltogether :s ");
            // And that means we cannot play it!
            seekable=false;
        }

        if (!isSeekable() || hasFailedToLoad())
        {
            playerLog("The  audio file " + properPath + " was unable to load..\n ");
        }

    } // Failed to load?

}

// Get's the value which tells relationship of attention and meditation value. It can be shifted to promote more reasonable values.
/*float Song::getAvgFlowValueZeroToHundred() const {
    float ATT_FACTOR = 1.0f;
    //float MED_FACTOR = 0.75f; //0.75?
    float MED_FACTOR = 1.0f;
    return songBwInfo.channelInfo[CHANNEL_ATT].getAvarageValue()*ATT_FACTOR - songBwInfo.channelInfo[CHANNEL_MED].getAvarageValue()*MED_FACTOR; // -0.75 - 1. neg is too lazy, 1 is too anxious
}*/
// Flow axis tells the relation ship between attention and meditation while flow value only tells about the distance, the flow..
float Song::getAvgFlowAxisValueMinusHundredToHundred() const
{
    float ATT_FACTOR = 1.0f;
    //float MED_FACTOR = 0.75f; //0.75?
    float MED_FACTOR = 1.0f;
    return songBwInfo.channelInfo[CHANNEL_ATT].getAvarageValue()*ATT_FACTOR - songBwInfo.channelInfo[CHANNEL_MED].getAvarageValue()*MED_FACTOR; // -0.75 - 1. neg is too lazy, 1 is too anxious
}

//Get the abarage flow axis value
float Song::getAvgFlowAxisValue() const
{
    //return getAvgFlowValueZeroToHundred()/100.0f;
    return getAvgFlowAxisValueMinusHundredToHundred()/100.0f;
}

// Get string representation of the channel avarage values for results etc.
string Song::getStrChannelAvgAndDescription(int chanIx) const
{
    return "Avarage value of " + getGraphTheme(chanIx).text + " is " + desToStr(songBwInfo.channelInfo[chanIx].getAvarageValue()) + "\n";
}

// Get what it's asked for. The asked channel's avarage value as string representation.
string Song::getStrChannelAvg(int chanIx) const
{
    return desToStr(getChannelAvg(chanIx));
}

// Get channel avarage of asked index.
float Song::getChannelAvg(int chanIx) const
{
    return songBwInfo.channelInfo[chanIx].getAvarageValue();
}

// Get the text representation of the channel ie. "Delta" or "Theta"
string Song::getStrChannelDescription(int chanIx) const
{
    return getGraphTheme(chanIx).text;
}

// Get all desired song's channels avarage values in string representation.. Used for results usually
string Song::getItemAvgBWDataStr() const
{
    string returnStr = "";
    returnStr += getStrChannelAvgAndDescription(CHANNEL_DELTA);
    returnStr += getStrChannelAvgAndDescription(CHANNEL_THETA);
    returnStr += getStrChannelAvgAndDescription(CHANNEL_ALPHA);
    returnStr += getStrChannelAvgAndDescription(CHANNEL_BETA);
    returnStr += getStrChannelAvgAndDescription(CHANNEL_GAMMA);
    returnStr += getStrChannelAvgAndDescription(CHANNEL_AVG);
    returnStr += getStrChannelAvgAndDescription(CHANNEL_ATT);
    returnStr += getStrChannelAvgAndDescription(CHANNEL_MED);
    return returnStr;
}

bool Song::isBwInfoUsable() const
{
    // If the channels show some sense numbers. It's probably usable. Otherwise it's almost empty record!
    if (songBwInfo.isReady())
    {
        // Take some graph and check much had been recorded. Ie. Theta is something which is usually recorded at least.
        if (songBwInfo.channelInfo[generalEstimateChannel].getRecordedPercentage()>0.25f) // If 25% of song is recorded. It's good to go.
            {
                //playerLog("Found good song");
                return true;
            }
            //playerLog("No bw data recorded for 25% . only " + desToStr(songBwInfo.channelInfo[generalEstimateChannel].getRecordedPercentage()*100.0f));
            return false;
    }
    else
    {
        return false;
    }
}

// Get color of the text color according this song's brainwave information. Ie calm song is different in color than energetic song.
ALLEGRO_COLOR Song::getTextColor() const
{

    ALLEGRO_COLOR theColor = al_map_rgb_f(0.0, 0.0, 0.9); // This is init color If no bw data is available.

    if (songBwInfo.isReady() && isBwInfoUsable())
    {
        theColor = al_map_rgb_f(0.2, 0.2, 0.2);

        float prof1Val = musicLibrary.getRelativeValueInAllSongsOfChannel(bwMeter.PROFILE_CHANNEL_1,getChannelAvg(bwMeter.PROFILE_CHANNEL_1));
        float prof2Val = musicLibrary.getRelativeValueInAllSongsOfChannel(bwMeter.PROFILE_CHANNEL_2,getChannelAvg(bwMeter.PROFILE_CHANNEL_2));

        // # prof1Val = activity usually (beta)
        // # prof2Val = liking of the song usually (theta)
        theColor = getMapColor(prof1Val,prof2Val);

    }

    return theColor;
}

string Song::getSongPositionStr(int frames) {
    string str = "";
    str += intToStr(getPlayedHoursInView(frames)) + ":" + intToStr(getPlayedMinutesInView(frames)) + ":" + intToStr(getPlayedSecondsInView(frames));
    return str;
}

float Song::getMapDistanceFromTimeLinePointAutoAxis(float v1, float v2, int timeP)
{

    float amountOfActivity = 0;
    if (djModeUseXAxis.isChecked())
        amountOfActivity = musicLibrary.getRelativeValueInAllSongsOfChannel(bwMeter.PROFILE_CHANNEL_1,songBwInfo.channelInfo[bwMeter.PROFILE_CHANNEL_1].getSmoothValueFromTimePos(timeP));
    else
        v1 = 0;

    float amountOfLiking  = 0;
    if (djModeUseYAxis.isChecked())
        amountOfLiking   = musicLibrary.getRelativeValueInAllSongsOfChannel(bwMeter.PROFILE_CHANNEL_2,songBwInfo.channelInfo[bwMeter.PROFILE_CHANNEL_2].getSmoothValueFromTimePos(timeP));
    else
        v2 = 0;

    return getDistance(v1,v2,amountOfActivity,amountOfLiking);
}

float Song::getProfileMapDistanceToAutoAxis(float v1, float v2)
{

    float amountOfActivity = 0;
    if (djModeUseXAxis.isChecked())
        amountOfActivity  = musicLibrary.getRelativeValueInAllSongsOfChannel(bwMeter.PROFILE_CHANNEL_1,getChannelAvg(bwMeter.PROFILE_CHANNEL_1));
    else
        v1 = 0;

    float amountOfLiking   = 0;
    if (djModeUseYAxis.isChecked())
        amountOfLiking     = musicLibrary.getRelativeValueInAllSongsOfChannel(bwMeter.PROFILE_CHANNEL_2,getChannelAvg(bwMeter.PROFILE_CHANNEL_2));
    else
        v1 = 2;

    return getDistance(v1,v2,amountOfActivity,amountOfLiking);
}

// Get song position in seoconds.
int Song::getPositionInSecs(int frames)
{
    return int(frames/sampleRate);
}

// Get position in seconds as float
float Song::getPositionInSecs_f(int frames)
{
    return float(float(frames)/float(sampleRate));
}

// No need to comment.. really.. bullshit to comment..
// Allright. This functio returns the asked time position in timeline as frames of the song's audio sample.
int Song::getPositionInFrames(int seconds)
{
    return int(seconds*sampleRate);
}

// Parse information of this path to get album, title etc If for some reason it was hard to find otherways
// NOTICE: Not in use currently!
string Song::parseInfoFromPath(string properPath, int seekingFor)
{
    playerLog("Trying to parse the file info from path " + properPath);
    vector <string>pathBlocks;
    string result = "unnamed";
    if (seekingFor==TITLE_PATH)
    {
        // Split the folder margins so we have filenametitle.xx
        splitstring s(properPath.c_str());
        vector<string> flds = s.split('\\',10); // Remember in c++ compiler double slash is slash
        // Split the filenametitle from filenametitle.xx
        //splitstring s2(flds[flds.size()-1].c_str());
        //vector<string> flds2 = s2.split('.',10); // Remember in c++ compiler double slash is slash

        //unsigned found = str.find_last_of("/\\");
        //std::cout << " path: " << str.substr(0,found) << '\n';
        //std::cout << " file: " << str.substr(found+1) << '\n';

        string thisFilename = flds[flds.size()-1].c_str();
        unsigned found = thisFilename.find_last_of("."); // The filename extension part

        string filenameWithoutExtension = thisFilename.substr(0,found);

        //result = flds2[flds2.size()-2]; // Sometimes names has numbers like 1. My first song.mp3 <- so we need to notice that also.

        if (filenameWithoutExtension!="")
            result = filenameWithoutExtension;
        else
            result = "unnamedz";

        playerLog("Found title: " + result);
    }
    if (seekingFor==ALBUM_PATH)
    {
        // Split the album from artist/album/artist
        splitstring s(properPath.c_str());
        vector<string> flds = s.split('\\',10); // Remember in c++ compiler double slash is slash
        if (flds.size()>1)  {
            result = flds[flds.size()-2];
        } else
            result = "unnamedz";
        playerLog("Found album: " + result);
    }
    if (seekingFor==ARTIST_PATH)
    {
        // Split the artist from artist/album/artist
        splitstring s(properPath.c_str());
        vector<string> flds = s.split('\\',10); // Remember in c++ compiler double slash is slash
        if (flds.size()>2)  {
            result = flds[flds.size()-3];
        } else
            result = "unnamedz";
        playerLog("Found artist: " + result);
    }
    return result;
}

// The extracted data will be calculated from here on..
void Song::updateSongBrainWaveInformation()
{
    // Yeap here after few unneccessary drifts..
    //playerLog("Going to create song's brainwaves extracted information..");
    songBwInfo.createSongBwInfo();
    bwInfoCreated = true;
}

// Draw song's extracted brainwave information..
void Song::drawSongExtractedBwInformation()
{
    songBwInfo.draw();
}

// Soooong
Song::~Song()
{
    //dtor
}
