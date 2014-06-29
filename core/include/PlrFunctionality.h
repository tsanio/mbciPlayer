#ifndef PLRFUNCTIONALITY_H
#define PLRFUNCTIONALITY_H

#include "PlayerCommon.h"

/**
 * \brief PlrFunctionality.h ...::: Player Functionality - The player's functionality :::...
*
* Most of the BCI MUSIC PLAYER functionality is here. Like what happens after clicking something etc.
*
* Here is also the experiment data handling. A lots of it.
*
* author: tonis @ 2013
*
*/
class PlrFunctionality
{
public:
    PlrFunctionality();
    virtual ~PlrFunctionality();
protected:
private:
};

namespace plrCommon
{

void updateFadingOut();
void updateFadingIn();
void initStreamStartUpValues();
void loadBWTable(Song thisSong);
// This is a bit heavy way to reset colors to the list.. Only used when loading the list :)
void refreshBWPreInformations();
// This just refreshes the elements.. No changes to colors..(except the just played song?)
void refreshPlayList ();
void loadBWTable();
void saveBWTable();
// I actually don't know why this f.. works but it seems to work fine :D
void fadeOutToStream(OutputStreamPtr streamToFade);
void pauseSelectedSong();
void removeSelectedSong();
void initializeBrainWaveTable();
void updateDJMode();
void findDJModeSongAndPlayIt();
void playDJMode();
bool validateSongToPlay();
// Update stream statuses, djmodes, fading, etc..
void updateStreamUpdaters();
// Opens audiosource from absolute or relative path from given song.. Returns true on success.
bool openSampleSourceToSong(Song songToLoad);
// This is used after fading in to correctly continue playing with normal volume..
void startPlayingNewSongPos(string playSongId, int playSongPos);
// Play button.. Unite for all..
void playSongButtonPressed();
// This is actually for both-> DJ And recordplay
void playSelectedSong();
void setSongLoopMode();
void playNextTrack();
void playSameTrackAgain();
void updateSongStatus();
void saveMusicLibrary();
void updateFadingsGeneral();
void printSelectedSongProfileInfo();
void loadMusicLibrary();

};

#endif // PLRFUNCTIONALITY_H
