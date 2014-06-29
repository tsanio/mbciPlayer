#ifndef PLAYERCOMMON_H
#define PLAYERCOMMON_H

#include <string>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <audiere.h>
#include "Config.h"
#include "BWMeter.h"
#include "MindWaveDevice.h"
#include "OpenEEGDevice.h"
#include "Song.h"
#include "GUI/GUIMouse.h"
#include "GUI/GUIElementList.h"
#include "GUI/GUIScrollPanel.h"
#include "GUI/GUISpectrumPanel.h"
#include "GUI/GUICheckBox.h"
#include "GUI/GUIRollButton.h"
#include "GUI/GUIRadioButton.h"
#include "GUI/GUIXYPicker.h"
#include "GUI/GUIScroller.h"
#include "ImageLibrary.h"
#include "MusicLibrary.h"
#include "BoostRelativePath.h"
#include "BgEffect.h"

using namespace boost::filesystem;
using namespace audiere;
using namespace std;


/**
 * \brief PlayerCommon.h ...::: Player Commons - All is here :::...
*
* This class holds the variable definitions of globally used classes such as BWManager, BWMeter, musicLibrary and GUIElements.
*
* author: tonis @ 2013
*
*/
class PlayerCommon
{
public:
    PlayerCommon();

    virtual ~PlayerCommon();

protected:
private:
};

//! NOTICE For future: Represent the value here and also in .cpp file. These are for global usage.
namespace plrCommon
{

//! Configuration.
extern Config config;

// Autosave configuration time.
extern int autoSaveConfigurationTime;

//! Debug input which is random just for testing..
extern bool useDebugInput;

//! MindWaveDevice which is used in the program to get data from mindwave.
extern MindWaveDevice mindWaveDevice;

//! Open EEG Device .. This is Olimex specifically.
extern OpenEEGDevice openEEGDevice;

//! Set up Brainwave Manager
extern BWManager bwManager;

//! List to show spectrum in eeg conf device panel.
extern GUISpectrumPanel SpectrumPanel;

//! Song information. Current played song.
extern Song playThis; //! Currently played song. Good to know for all..

extern Song *playThisPtr; //! Address to the medialibrary version of the song.

//! The Music Library to hold songs of the music list and some basic information about the brainwaves(for searching).
extern MusicLibrary musicLibrary;

//! Variable to tell which control panel is visible.(standard playmode-panel, djmode panel, settings panel).
extern int controlPanelMode;

//! Variable to tell which playmode is chosen. (standard playmode, djmode ?)
extern int playMode;

//! Variable to tell which playmode is chosen when play was clicked.
extern int playModeClicked;

//! Our player's list of all components.
extern GUIElementList elementList;

//! Our common display
extern ALLEGRO_DISPLAY *display;

//! Our triple buffer..
extern ALLEGRO_BITMAP *tripleBuffer;

//! Image library also as extern to help loading all images and destroying them!
extern ImageLibrary imageLibrary;

//! Some fonts that are used commonly.
extern ALLEGRO_FONT *buttonFont;

//! The size of the button font.
extern int buttonFontSize;

//! Font again used in playlist?.
extern ALLEGRO_FONT *libraryFont;

//! Our common mouse for gui to help this world a bit again..
extern GUIMouse guiMouse;

//! Global variables..
extern AudioDevicePtr device;

//! Player log text collected in this variable..
extern string playerLogTxt;

//! Music library file. All music and some raw information abour recorded brainwaves.
extern string musicLibraryFileName;

//! Name of the result file..
extern string resultsFileName;

//! Name of the result file..
extern string resultsFileName2;

//! Confugration filename.
extern string configurationFileName;

//! The Manual Configuration file name.
extern string manualConfigurationFileName;

//! AMount of peaks in channel for help finding the top notch points in song.
extern int AMOUNT_OF_PEAKS_IN_CHANNEL;

//! Division of smoothened graphs. 5-10 seconds usually. This helps to ignore harsh peaks.
extern int SMOOTH_GRAPH_DIVISION_IN_SECS;

//! Dj Mode top ranking. How many of the best peaks will be used to pick the random choise for listener.
extern double DJModeTopRankingRate;

//! Something for DJ Mode.. The size of the last played list: So we don't repeatily play the same songs.
extern double DJModeLastPlayedItemsListSize;

//! Folder for saved brainwaves.
extern string BWFolder;

//! Time between each frame..
extern double deltaTime;

//! Time between each frame of graphical updates..
extern double deltaGraphicsTime;

//! This tells which channel is used to estimate the amount of recorded bwdata in %.
extern int generalEstimateChannel;

//! Globally usable connection Id which tells the id for MindWave or similar device. 0 If it's not initialized!
extern int connectionId;

//! Globally told song position
extern int songPositionInSec;

//! Takes away excess decimals from giben number with giben amount of decimals.
extern double showDecimals(const double& x, const int& numDecimals);

//! Global function that changes integer to string.
string intToStr(int number);

//! Converts string to decimal.
double strToDes(string txt);

//! Changes float to String repsresentation.
string desToStr(float number);

//! String to bool 0 or 1.
bool strToBool(string str);

//! Bool to str 0 or 1.
string boolToStr(bool val);

//! Changes str to int.
int strToInt(string strToConvert);

//! Checks wether giben filepath is proper music file.
bool isValidMusicPlayerFile(path p);

//! Font loading function.
bool loadFonts();

//! Font unloading functin.
bool unLoadFonts();

//! Player log. This is used everywhere to print debug information.
/*!
      Details: Give string to log something.
*/
void playerLog(string s);

//! Calculates distance between points 1 and 2.
/*!
      Details: Give point 1 x and y and point 2 x and y and function returns the distance.
*/
float getDistance(float x,float y,float x2,float y2);

//! Remove double empty spaces like these "  ".
char* removeDoubleEmptySpaces(string txt);

//! This tells whether we'r in fullscreen or in windowed mode.
extern bool fullScreen;

//! Some checkboxes for common usage in setup panel:)
extern GUICheckBox showDividedBox;

//! Show peakposition option to choose.
extern GUICheckBox showPeakPosBox;

//! Colored songs according brainwave information or just neutral?
extern GUICheckBox brainWaveColoredInMenuBox;

//! Recording new brainwave. Sometimes good to take it off when using DJ Mode.
extern GUICheckBox recordBrainWaveBox;

//! Feding length time start and end. Nice.. Smooth.. Mellow. Fellow.. Rolling!
extern GUIRollButton fadingLengthRollButton;

//! Min length of the dj mode's picked song.
extern GUIRollButton djModeMinLengthOfPartRollButton;

//! Max length of dj mode songs parts.
extern GUIRollButton djModeMaxLengthOfPartRollButton;

//! How much we rewind before playing the peak pos that DJ Mode is going to play.
extern GUIRollButton delayBeforePeakPosRollButton;

//! Picker for choosing X,Y position of the desired DJ Mode or seen by user which is the BCI state in 2D-mode.
extern GUIXYPicker XYDJPicker;

//! MindWave Port options.
extern GUIRollButton mindWavePortRollButton;

//! Scaling for mindwave values.
extern GUIRollButton mindWaveScaleFactor;

//! Reboot mindwave.
extern GUIButton reBootMindWave;

//! OpenEEG Port and reboot.
extern GUIRollButton OpenEEGPortRollButton;

//! Reboot Open EEG device.
extern GUIButton reBootOpenEEG;

//! EEGDevice options.
extern GUIRadioButton EEGDeviceModeRadioButton;
extern GUICheckBox    EEGDevicegOp1;
extern GUICheckBox    EEGDevicegOp2;
extern GUICheckBox    EEGDevicegOp3;

//! Show visualization checkbox.
extern GUICheckBox showVisualization;

//! Show mirror down there?
extern GUICheckBox showMirror;

//! Should Dj-Mode use X-Axis for deciding songs. (Activity-beta usually)
extern GUICheckBox djModeUseXAxis;

//! Should Dj-Mode use Y-Axis for deciding song. (Liking-theta usually)
extern GUICheckBox djModeUseYAxis;

//! Playlist to show songs for choosing song and editing the list..
extern GUIScrollPanel playListPanel;

//! Scroller for song position.. length was 440 originally
extern GUIScroller songScrollBar;

//! Volume rollbutton after song scrollbar in the right corner
extern GUIRollButton guiVolumeRollButton;

//! Exit, resize, minimize
extern GUIButton exitButton;
extern GUIButton resizeButton;
extern GUIButton minimizeButton;

//! Playbutton and pause button :)
extern GUIButton playButton;
extern GUIButton pauseButton;

//! These are the three big page-options to choose [Record(normal play)][DJMode][Configurations]
extern GUIButton RecordModeButton;
extern GUIButton DJModeButton;
extern GUIButton ConfigureButton;
extern GUIButton ConfigureEEGButton;

//! Buttons to change normal mode or to change DJ-mode
extern GUIButton ChangeToDJModeButton;
extern GUIButton ChangeToRecordModeButton;
extern GUIButton JumpDJButton;

//! Looping of songs.
extern GUICheckBox loopingCheckBox;

//! Song handling.
extern GUIButton songFileOpenButton;
extern GUIButton songFolderOpenButton;
extern GUIButton songTrashButton;

//! Trash bwinformation.
extern GUIButton bwTrashButton;

//! Library handling.
extern GUIButton libraryNewButton;
extern GUIButton libraryOpenButton;
extern GUIButton librarySaveButton;

//! Different mode choosing radio-choose menu. Modes for BCI or user chosen options.
extern GUIRadioButton djModeRadioButton;

//! Options to choose.. Added later in the main-void to the radiobutton. See main.cpp.
extern GUICheckBox    djModeOp1;
extern GUICheckBox    djModeOp2;

//! Different options of playing whole songs or just song partions in Djmode: This means to play full songs or parts.
extern GUIRadioButton djPlayPartitions;
extern GUICheckBox    partOp1;
extern GUICheckBox    partOp2;

extern GUIRadioButton visualizationParameterRadioButton;
extern GUICheckBox    actOp1;
extern GUICheckBox    actOp2;


//! Visualization mode. There are some fun things in these. Whether o show it with the GUI usual elements or not. I don't remember anymore how did this work.
extern bool onTopVisualization;

//! Wtf is this..
extern float bwManagerFade;

//! This is the Brainwave Meter which is interface for data to program from devices.
extern BWMeter bwMeter;

//! Attributes used in main mostly..

/* Keep track of pressed mouse button. */
extern int mouse;

//! Get the minimum values of integer of whatever we need here
extern double dmin; //! minimum value
extern double dmax;

extern int down;
extern int down_x;
extern int down_y;

/* Simple FPS counter. */
extern int fps, fps_accum;
extern double fps_time;
extern double totalFrameTime;
//! Important global functions to cause smooth movement etc.
extern double lastUpdate;
extern double lastGfxUpdate;
extern double gfxDesiredFps; // This is the main gfx desired fps. Ie 80 frames per second for drawing again and again.
extern double pauseStartTime;

extern bool songPlaying;
extern int pausedAtPoint;

//! This is the panel text "Normal record mode-panel" "Dj Mode-panel" "..-panel"
extern string panelText;

extern ALLEGRO_BITMAP *graphBaseImg;
extern ALLEGRO_BITMAP *iconImg;

//! Player samplesource and stream for streaming audio
extern SampleSourcePtr samplesource;
extern OutputStreamPtr stream;

//! For DJ Mode two streams to fade in and fade the other out at the same time..
extern OutputStreamPtr fadeInStream;
extern OutputStreamPtr fadeOutStream;

extern double fadeOutStarted;
extern float  fadeOutStartVolume;
extern bool   fadeOut;
extern double fadeInStarted;
extern bool   fadeIn;


extern double startTimeToPlaySongInDjMode;
extern float  djModeCurrentPieceDuration;

extern float mirrorBottom;

extern float songInfoTextX;

//! Allegro bitmaps. For background
extern ALLEGRO_BITMAP *bgImg;
extern ALLEGRO_BITMAP *copyImg;

extern ALLEGRO_TIMER *timerMs;

//! Background visualization..
extern BgEffect backgroundVisualization;

//! Function to get scaled image of giben image.
ALLEGRO_BITMAP* getScaledImage(ALLEGRO_BITMAP* src, float scaleFactorX, float scaleFactorY);

//! Nice function to split string or something. Where did I get this? Anyway thank you!
class splitstring : public string
{
    vector<string> flds;
public:
    splitstring(const char *s) : string(s) { };
    vector<string>& split(char delim, int rep=0);
};

//! Draw glowing dots.. Ahh Staars.. I can see them! @_@ *_* This is used to draw the glowing bars in the meters next to rollbuttons.
void drawGlowDot(float px, float py, float glowValue);

//! ^  Mostly same. This is used to draw the glowing bars in the meters next to rollbuttons.
void drawPowerMeter(float px, float py, float width, float height, float value);

//! Get positive value.
float getPositiveOfThis(float num);

//! Get negation of this.
float getNegationOfThisPrefix(float num);

//! Get's the prefix +- from value.
float getPrefixOf(float num);

//! Draws scaled picture of given image.
void drawScaledPicture(ALLEGRO_BITMAP *img, float dx, float dy, float width, float height);

//! Draws rotated tinted picture.
void drawRotatedTintedPicture(ALLEGRO_BITMAP *img, ALLEGRO_COLOR tint, float dx, float dy, float angle);

//! Draw rotated picture.
void drawRotatedPicture(ALLEGRO_BITMAP *img, float dx, float dy, float angle);

//!  Draw tinted picture with given color.
void drawTintedPicture(ALLEGRO_BITMAP *img, ALLEGRO_COLOR color, float x, float y, int flags);

//! Delta-value of values.
float getDistanceBetweenValues(float val1, float val2);

//! Limits upper part of value.
float limitValueUp(float tval, float limitV);

//! Limits value down.
float limitValueDown(float tval, float limitV);

//! Limits value inside both upper and down part.
void getValueInsideBoundaries(float& tval, float limitVDown, float limitVUp);

//! Remove excess characters. Used to cut middle character from too large string. Useful when we can assume that in the middle there is not so importatn info. Artist - album - songtitle <- album is not so important.
string removeExcessFromMiddle(string sourceStr, int limitStrSize);

//! Get 3D translation of the colors. Color from to color with scaling factor.
ALLEGRO_COLOR getFromColorToColor(ALLEGRO_COLOR colFrom, ALLEGRO_COLOR colTo, double fac);

//! Get's the 2D-map color according desired visualization of given values 0-1f , 0-1f
ALLEGRO_COLOR getMapColor(float valX, float valY);

//! Get's timestamp.
string getTimeStamp();

//! Remove asked characters. Thank you Stack overflow people!
string removeChars(string str, char *chars);

//! Same but uses more force :D.
string forceRemoveAllDoubleEmptySpaces(string txt);

//! Easy way to ask which device is used.
bool usingOpenEEG();

//! How many channels in openEEG to use now?
int howManyChannelsInOpenEEG();

//! Easy way to ask which device is used.
bool usingMindWave();

extern double getEffectiveValueOfEEG();

//! Get frequency of asked index in spectrum.
double Index2Freq(int i, double sampleRate, int n);

//! Get index of asked frequency in spectrum.
int Freq2Index(double freq, double sampleRate, int n);

};

#endif //! PLAYERCOMMON_H

