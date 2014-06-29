#include "../include/PlrFunctionality.h"
#include "../include/PlrGUI.h"
#include "../include/PlayerCommon.h"
#include "../include/BoostCopyDir.h"

#define UN_ACTIVE -1

PlrFunctionality::PlrFunctionality()
{
    //ctor
}

PlrFunctionality::~PlrFunctionality()
{
    //dtor
}


namespace plrCommon
{


void updateFadingsGeneral()
{
    // If we'r playing current stream and it's facing it's end. We should perhaps stop it and fade it out?
    if (stream)
        if (stream->isPlaying())
            // End of song timeline with fadetime?
            if (playThis.getPositionInSecs(stream->getLength() - stream->getPosition()) < fadingLengthRollButton.getConvertedValue())
            {
                // Oh my god! Save all data Before it's lost :)
                saveBWTable();
                fadeOutToStream(stream);
                // Looping checkbox only affects the normal playing..
                if (playModeClicked == RECORD_PLAY_PANEL)
                {
                    if (loopingCheckBox.isChecked()) // Loop same?
                        playSameTrackAgain();
                    else
                        playNextTrack(); // Or next?!
                }
                // No looping whatsovere in djpanelmode.. just playing random stuff
                if (playModeClicked == DJ_PANEL)
                {
                    playNextTrack();
                }
            }

    updateFadingIn();
    updateFadingOut();

}



void updateFadingOut()
{
    if (fadeOut)
    {
        if (fadeOutStream->isPlaying())
        {
            //float delta = float(fadeOutStream->getLength())-float(fadeOutStream->getPosition());
            float timePassed = al_get_time() - fadeOutStarted;
            if (timePassed==0) timePassed=0.1f;
            float deltaVolValue = 1.0f-(timePassed/fadingLengthRollButton.getConvertedValue());
            if (deltaVolValue>1.0f)deltaVolValue=1.0f;
            if (deltaVolValue<0.0f)
            {
                deltaVolValue=0.0f;
                fadeOut = false;
            }
            fadeOutStream->setVolume(deltaVolValue*fadeOutStartVolume);
        }
        else
        {
            // Stop fadiing stream totally!
            fadeOut = false;
        }
    }
}

void updateFadingIn()
{
    if (fadeIn)
    {
        if (stream->isPlaying())
        {
            float timePassed = al_get_time() - fadeInStarted;
            if (timePassed==0) timePassed=0.1f;
            float deltaVolValue = (timePassed/fadingLengthRollButton.getConvertedValue());
            if (deltaVolValue>1.0f)
            {
                deltaVolValue=1.0f;
                fadeIn = false;
            }
            if (deltaVolValue<0.0f)deltaVolValue=0.0f;
            stream->setVolume(deltaVolValue*guiVolumeRollButton.getValue());
        }
    }
}


void initStreamStartUpValues()
{
    djModeCurrentPieceDuration = 30.0f; // This is randomized later on..
    fadeOut  = false;
    startTimeToPlaySongInDjMode = UN_ACTIVE;
}

void loadBWTable(Song thisSong)
{
    // Load bwgraphs if file exists already..
    //playerLog( ("Loading BWData from: " + (BWFolder+thisSong.getId()+".bw" )).c_str() );

    if ( boost::filesystem::exists( (BWFolder+thisSong.getId()+".bw" ).c_str() ))
        //if ( boost::filesystem::exists( "duble" ))
    {
        std::ifstream ifs( (BWFolder+thisSong.getId()+".bw").c_str()  );
        //std::ifstream ifs( "duble" );
        boost::archive::text_iarchive ia(ifs);
        // read class state from archive
        ia >> bwManager;
        // archive and stream closed when destructors are called
        bwManager.drawAllAgain();
        //playerLog("Loaded fine!");
    } else {
        // It's normal that most of bw are not recorded for user :)
        //playerLog( ("Brainwave data missing from: " + (BWFolder+thisSong.getId()+".bw" )).c_str() );
    }
}

// This is a bit heavy way to reset colors to the list.. Only used when loading the list :) Or some other also? It's a bit heavy going through all list files the songs.
void refreshBWPreInformations()
{
    playerLog("Updating PlayList And It's preinformation");
    for (int i=0; i<musicLibrary.song.size(); i++)
    {
        //Fill the bwmanager with this song info.. Load all songs from list and roll em through manager to update the library with bw data
        Song tmpSong = musicLibrary.song.at(i);
        loadBWTable(tmpSong);
        musicLibrary.createSongBWInfoToLibrary(tmpSong.getId());
    }
    if (!playThis.isNull())
    {
        loadBWTable(playThis);
        musicLibrary.createSongBWInfoToLibrary(playThis.getId());
    }
}

// This just refreshes the elements.. Update the preinformation also and other relevat to make sure everything is up to date as possible.
void refreshPlayList ()
{
    // Some other refresh also here?

    typedef vector<Song> songVector;
    playListPanel.clearItems();
    for (songVector::const_iterator it (musicLibrary.song.begin()); it != musicLibrary.song.end(); ++it)
    {
        if (brainWaveColoredInMenuBox.isChecked())
        {
            playListPanel.addItem(it->getArtist() + " - " + it->getTitle(), it->getId(), it->getTextColor() );
        }
        else
        {
            float alpha = 0.8f;
            ALLEGRO_COLOR playListNormalColor = al_map_rgba_f(0.5f*alpha,0.5f,0.95f*alpha,alpha );
            playListPanel.addItem(it->getArtist() + " - " + it->getTitle(), it->getId(), playListNormalColor );
        }
    }
}


void loadBWTable()
{
    loadBWTable(playThis);
}

void saveBWTable()
{
    if (!playThis.isNull())
    {
        // Save the graph table first..
        playerLog(("Saving BWData into: " + (BWFolder+playThis.getId()+".bw" )).c_str());
        // Save also the brainwave graphs
        std::ofstream ofs( (BWFolder+playThis.getId()+".bw").c_str() );
        //std::ofstream ofs( "duble" );
        // save data to archive
        {
            boost::archive::text_oarchive oa(ofs);
            // write class instance to archive
            oa << bwManager;
            // archive and stream closed when destructors are called
        }

        // And another veery important part is to create the avarage information
        // etc related to song/bwinformation which is saved directly to library
        // This is also calculated later in one second interval in music library refrehs..
        musicLibrary.createSongBWInfoToLibrary(playThis.getId());
        playerLog("Passed!");
        // Also update the correspoding data item in playlist
        refreshPlayList();
    }
}

// I actually don't know why this f.. works but it seems to work fine :D
void fadeOutToStream(OutputStreamPtr streamToFade)
{
    if (streamToFade)
    {
        fadeOutStream = streamToFade;
        fadeOutStream->setRepeat(false);
        fadeOut = true;
        fadeOutStartVolume=streamToFade->getVolume();
        fadeOutStarted = al_get_time();
    }
}

void pauseSelectedSong()
{
    if (songPlaying)
    {
        if (stream && stream->isPlaying())
        {
            pausedAtPoint=stream->getPosition();
            stream->stop();
        }
        songPlaying=false;
        playButton.showAndActivate();
        pauseButton.hideAndUnactivate();
        bwManager.stopRecording();

        pauseStartTime = al_get_time();

        saveBWTable();
    }
}

void removeSelectedSong()
{
    musicLibrary.removeSong(playListPanel.getSelectionItemId());
    refreshPlayList();
}


void initializeBrainWaveTable()
{
    bwManager.newBWTable(playThis.getId(),playThis.getLengthSeconds());
    loadBWTable();
    bwManager.startRecording();
}

void updateDJMode()
{
    // The idea here is mostly to check for time to change song :)
    if (startTimeToPlaySongInDjMode!=UN_ACTIVE && stream->isPlaying())
    {
        if (al_get_time() - startTimeToPlaySongInDjMode > djModeCurrentPieceDuration)
        {
            playDJMode(); // Song chaange :)
        }
    }
}

void findDJModeSongAndPlayIt()
{
    // Create a list of last played songs or songs
    // Function to get attentive song (and check that it hasn't been played for ie. in 5 cycles)
    // Start playing it
    // After this change this to mixing it..
    int mode=0;
    if (djModeRadioButton.getSelectionTxt() == "Chosen State List")
    {
        playerLog("Searching for Chosen State music..");
        mode = USER_CHOSEN_STATE_LIST;
    }
    if (djModeRadioButton.getSelectionTxt() == "BCI State List")
    {
        playerLog("Searching for User State music..");
        mode = BCI_STATE_LIST;
    }

    string rndSongId    = musicLibrary.getRndSongAccordingBrainWaveInfo(mode); // Get some top song for the request...

    if (rndSongId!="false")
    {

        int rndSongPosInSec = 0;
        int rndSongPosInAudiereSongTime = 0; // Start from beinning.. If whole songs in this case..

        // Play whole song!.. or part?
        if (djPlayPartitions.getSelectionTxt() == "Play Full Songs")
        {
            djModeCurrentPieceDuration = musicLibrary.getSong(rndSongId).getLengthInSecs(); // Play whole song
        }
        else
        {
            // Partition .. not whole song.. search for peak!
            rndSongPosInSec = musicLibrary.getRndPeakDistanceMapPos(rndSongId, mode);

            // And move it abit back to fade better correctly to the position of desire..
            rndSongPosInSec    -= delayBeforePeakPosRollButton.getConvertedValue();

            if (rndSongPosInSec<0)
                rndSongPosInSec = 0;

            rndSongPosInAudiereSongTime = musicLibrary.getSong(rndSongId).getPositionInFrames(rndSongPosInSec);

            // Randomize the next part played duration..
            srand (time(NULL));
            djModeCurrentPieceDuration = (rand() % ( int(djModeMaxLengthOfPartRollButton.getConvertedValue()) - int(djModeMinLengthOfPartRollButton.getConvertedValue()) ) ) + int(djModeMinLengthOfPartRollButton.getConvertedValue()); // min - max random

        }
        // Let's try to play song part.. First fade this out
        fadeOutToStream(stream);
        // And fade another one in! :)
        startPlayingNewSongPos(rndSongId, rndSongPosInAudiereSongTime);
        startTimeToPlaySongInDjMode = al_get_time();
        // And what did we got the next playing time?
        playerLog("Dj Mode Part Length: " + intToStr(djModeCurrentPieceDuration) + "s");

    }
    else    // Returned false from finding song?
    {
        playerLog("No songs in playlist to use for DJMode!");
    }


}

void playDJMode()
{

    // Hide playbutton and show pausebutton
    pauseButton.showAndActivate();
    playButton.hideAndUnactivate();

    // Then find song for DJ Mode and play it! B)
    findDJModeSongAndPlayIt();

    // Show the selection also on list..
    if (playModeClicked == DJ_PANEL)
        playListPanel.selectItemById(playThis.getId());
}

bool validateSongToPlay()
{
    if (playThis.isNull() == false && playThis.isSeekable())
        return true;
    else
    {
        playerLog("Couldn't find the song matching in playlist! Weird problem? ! ");
        if (!playThis.isSeekable())
            playerLog("It's not seekable!");
        if (playThis.isNull())
            playerLog("IT has disappeared on playlist..");
        songPlaying = false;
        pauseButton.hideAndUnactivate();
        playButton.showAndActivate();
        return false;
    }
}


// Update stream statuses, djmodes, fading, etc..
void updateStreamUpdaters()
{
    updateFadingsGeneral();
    // The djmode update.. I don't know should I change this later .. the position of this a bit..
    updateDJMode();

}

// Opens audiosource from absolute or relative path from given song.. Returns true on success.
bool openSampleSourceToSong(Song songToLoad)
{

    string properPath = "";
    if ( boost::filesystem::exists( songToLoad.getPathStr().c_str() ) )
    {
        playerLog("Found song audio file from absolute path!");
        properPath = songToLoad.getPathStr().c_str();
    }
    else if ( boost::filesystem::exists( songToLoad.getRelativePathStr().c_str() ) )
    {
        playerLog("Found song audio file from relative path!.");
        properPath = songToLoad.getRelativePathStr().c_str();
    }
    if (properPath!="")
    {
        playerLog("Opened PATH: " + properPath + "\n");
        samplesource = OpenSampleSource ( properPath.c_str(), FF_AUTODETECT);
        return true;
    }
    else
    {
        playerLog("Found song audio file from relative or absolute path!");
        return false;
    }
}

// This is used after fading in to correctly continue playing with normal volume..
void startPlayingNewSongPos(string playSongId, int playSongPos)
{

    // Make sure playlist has targeted the right song.
    playListPanel.selectItemById(playSongId);

    // Throw the playlist panel to view the selection..  But it's not chosen yet so are you sure?
    playListPanel.jumpToShowSelection();

    // Let's refresh everything to update things all the time bright!
    refreshPlayList();

    playerLog("Going to play main stream " + playSongId + "\n");

    playThis    = musicLibrary.getSong(playSongId);
    playThisPtr = musicLibrary.getSongAddr(playSongId);

    fadeIn = true;
    fadeInStarted = al_get_time();

    if (validateSongToPlay())
    {

        // Opens sample source to song from relative or abslt. path
        if (openSampleSourceToSong(playThis))   // and returns wether succeeds or not
        {
            songInfoTextX = 0; // Reset songinfo text position.
            songPlaying = true;
            pausedAtPoint = -1;
            playerLog("Found song.. Let's play! :)");
            playListPanel.lockHighlightItem();

            stream = OpenSound (device, samplesource, true); // streaming vika.--
            //playerLog("Opened PATH: " + playThis.getPathStr() + "\n");
            if (!stream)
            {
                playerLog("Failed to load stream! \n");
            }
            else
            {
                if (stream->isSeekable())
                    playerLog("Source is seekable! :) ");
                else
                    playerLog("Source is not seekable! :s ");
                stream->setRepeat(true);
                stream->setVolume(0); // Volume is set to zero in the start...
                stream->setPosition(playSongPos);
                stream->play();
                // Okay new song starts playing! Initialize brainwave Table!
                initializeBrainWaveTable();
            }
        }
    }
}

// Play button.. Unite for all..
void playSongButtonPressed()
{

    // Oh my god! Save all data Before it's lost :)
    if (songPlaying) {
        saveBWTable();
        fadeOutToStream(stream);
    }

    // And play!
    playSelectedSong();
}

// This is actually for both-> DJ And recordplay
void playSelectedSong()
{

    playListPanel.jumpToShowSelection();

    // Let's refresh everything to update things all the time bright!
    refreshPlayList();

    songInfoTextX = 0; // Reset songinfo text position.

    // Did we pause at some point? We have to checck which panel is open in case user want's to change playMode :)
    if ( (playMode == RECORD_PLAY_PANEL && pausedAtPoint>-1 && playThis.getId() == playListPanel.getSelectionItemId() && playThis.isSeekable()) ||
            (playMode == DJ_PANEL && pausedAtPoint>0 && playThis.isSeekable()) )
    {

        // If not selecting new item to play.. Then play the current one.
        stream->play();
        // If we came from pause.. We should choose the new position of scrollbar as new position If it is different
        stream->setPosition(int(songScrollBar.getPosition()*float(stream->getLength())));
        pausedAtPoint=-1;
        songPlaying=true;

        playButton.hideAndUnactivate();
        pauseButton.showAndActivate();

        initializeBrainWaveTable();

        // If we were in dj-mode, make sure the paused time will be added!
        if (startTimeToPlaySongInDjMode!=UN_ACTIVE)
            djModeCurrentPieceDuration += (al_get_time() - pauseStartTime);

        // Should we also consider the fade in time.. So that it doesn't pump up but goes nicely even after pause.. The fade in is more important here than ou..
        fadeInStarted += al_get_time() - pauseStartTime;

        // Or are we just going to play new file
    }
    else
    {
        // This is done for example If we'r playing new song ->

        // Hide playbutton and show pausebutton
        pauseButton.showAndActivate();
        playButton.hideAndUnactivate();

        // Stop the DJ MODE!
        startTimeToPlaySongInDjMode = UN_ACTIVE;

        // Either clicked to play on dj mode or normal panel to record and just chilly play one by one.. It would make sense to just use the playMode as well..
        if (playModeClicked == RECORD_PLAY_PANEL) {
            startPlayingNewSongPos(playListPanel.getSelectionItemId(),0);
        }
         else
            playDJMode();

    }
}

void setSongLoopMode()
{
    if (songPlaying)
    {
        if (loopingCheckBox.isChecked())
            stream->setRepeat(true);
        else
            stream->setRepeat(false);
    }
}

void playNextTrack()
{

    if (playModeClicked == RECORD_PLAY_PANEL)
        playListPanel.selectNextItem();

    playSelectedSong();

    if (playModeClicked == RECORD_PLAY_PANEL)
        playListPanel.highlightNextItem();

    // If in dj mode, show what song we'r playing. So choose it from the menu automatically
    if (playModeClicked == DJ_PANEL)
        playListPanel.selectItemById(playThis.getId());
}

void playSameTrackAgain()
{
    playSelectedSong();
    playListPanel.highlightNextItem();
}

void updateSongStatus()
{
    setSongLoopMode(); // Update looping mode according the looping button
    // Are we playing, but song has ended? And we'r not paused?
    if (songPlaying && pausedAtPoint<0)
    {
        if (!stream->isPlaying())   // Then save bw data of this and new track!
        {
            // Save the brainwaves
            saveBWTable();
            // Next track..
            if (startTimeToPlaySongInDjMode!=UN_ACTIVE) // In Dj-mode or..
                playDJMode();
            else // Normal->
                playNextTrack();
        }
    }
    // Are we just playing the song? Then update the known songposition also
    if (songPlaying)
        songPositionInSec = playThis.getPositionInSecs(stream->getPosition());
}

void saveMusicLibrary()
{
    // create and open a character archive for output
    // If library is empty it's just same to delete it.
    if (!musicLibrary.isEmpty()) {
        std::ofstream ofs(musicLibraryFileName.c_str());

        // save data to archive
        {
            boost::archive::text_oarchive oa(ofs);
            // write class instance to archive
            oa << musicLibrary;
            // archive and stream closed when destructors are called
        }

    } else {
        if (boost::filesystem::exists(path(musicLibraryFileName)))
            boost::filesystem::remove(path(musicLibraryFileName));
    }
}

void loadMusicLibrary()
{
    if ( boost::filesystem::exists( musicLibraryFileName.c_str() ) )
    {
        // ... some time later restore the class instance to its orginal state
        // create and open an archive for input
        std::ifstream ifs(musicLibraryFileName.c_str());
        boost::archive::text_iarchive ia(ifs);
        // read class state from archive
        ia >> musicLibrary;
        // archive and stream closed when destructors are called
        // Refresh preinformation also!
        refreshBWPreInformations();
        refreshPlayList();
        // Refresh the view with play and pause If there are songs left.
        if (!musicLibrary.isEmpty()) {
            playSelectedSong();
            pauseSelectedSong();
        }
    }
    else
    {
        playerLog("Empty Music Library.. Ready to bring some audiofiles? :)");
    }
}

void printSelectedSongProfileInfo() {
    Song selectedSong = musicLibrary.getSong(playListPanel.getSelectionItemId());
    float prof1Val = musicLibrary.getRelativeValueInAllSongsOfChannel(bwMeter.PROFILE_CHANNEL_1,selectedSong.getChannelAvg(bwMeter.PROFILE_CHANNEL_1));
    float prof2Val = musicLibrary.getRelativeValueInAllSongsOfChannel(bwMeter.PROFILE_CHANNEL_2,selectedSong.getChannelAvg(bwMeter.PROFILE_CHANNEL_2));
    playerLog("Song's Profile: (" + desToStr(prof1Val*100.0f) + ")(" + desToStr(prof2Val*100.0f) + ")");
}

}; // End of namespace
