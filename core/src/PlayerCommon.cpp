#include <sstream>
#include <stdio.h>
#include <math.h>
#include <limits>
#include <string>
#include <boost/filesystem.hpp>
#include "../include/PlayerCommon.h"
#include "../include/GUI/GUIMouse.h"
#include "../include/GUI/GUIElementList.h"
#include "../include/GUI/GUIScrollPanel.h"
#include "../include/GUI/GUICheckBox.h"
#include "../include/MusicLibrary.h"
#include "../include/BWMeter.h"
#include "../include/thinkgear.h"
#include "../include/Config.h"
#include <audiere.h>

using namespace boost::filesystem;
using namespace audiere;
using namespace std;


PlayerCommon::PlayerCommon()
{
    //ctor
}

PlayerCommon::~PlayerCommon()
{
    //dtor
}

namespace plrCommon
{

    // Configuration for program.
    Config config;

    // To use debug input for testing.
    bool useDebugInput = false;

    // Player current played song information..
    Song playThis = Song();  // Currently played song. Good to know for all..
    Song *playThisPtr;       // Address to the medialibrary version of the song

    // The musical library of all songs and some extracted harsh brainwave information
    MusicLibrary musicLibrary("C:\\Users\\Maagi\\Documents\\muzaik\\Bonobo");

    // How many peaks are in one channel to locate DJ Mode playing positions.
    int AMOUNT_OF_PEAKS_IN_CHANNEL       = 5;

    // Smoothing of the graphs. How many seconds per block.
    int SMOOTH_GRAPH_DIVISION_IN_SECS    = 5;

    // Some DJMode values:
    // Top list of DJ Mode. How many percent of the found solutions will be used to pick random of the very best.
    double DJModeTopRankingRate          = 0.3;

    //  Size of the list that keeps track of last played songs.
    double DJModeLastPlayedItemsListSize = 0.25; // Percentage of the whole song list..

    // Autosave configuration time. -1 menas don't save at all. time in seconds.
    int autoSaveConfigurationTime = 30;

    // Variable to tell which control panel is visible.
    int controlPanelMode;

    // Variable to tell which playmode is chosen.
    int playMode;

    // Variable to tell which playmode is chosen when play was clicked
    int playModeClicked;

    // Use this channel to calculate how much brainwaves are recorded.
    int generalEstimateChannel = CHANNEL_ALPHA;

    // Our list of elements.. To help them drawing and refreshing more handy.
    GUIElementList elementList;

    // Our common display
    ALLEGRO_DISPLAY *display = NULL;

    // Triple Buffer
    ALLEGRO_BITMAP *tripleBuffer = NULL;

    // Our image loader..
    ImageLibrary imageLibrary;

    // The delta time to tell difference between frames and sync everything nicely..
    double deltaTime=0;
    double deltaGraphicsTime=0; // This is the deltaTime for graphics.

    // Song position that can be readed from any class..
    int songPositionInSec=0;

    // Our global mouse!
    GUIMouse guiMouse;

    // Thinkgear connection id. 0 If not initialized yet.
    int connectionId = -1;

    // Global variables(notice that we have to give still the type again):
    string playerLogTxt = "";

    // The gloobal music library file name
    string musicLibraryFileName = "mlibrary.txt";

    // Brainwaves folder, for bw data..
    string BWFolder = "brainwaves/";

    // The results txt file
    string resultsFileName = "results.txt";

    // The results txt file 2
    string resultsFileName2 = "results2.txt";

    // The Configuration file name.
    string configurationFileName = "config.txt";

    // The Manual Configuration file name.
    string manualConfigurationFileName = "manualConfiguration.txt";

    // Player's audio device
    AudioDevicePtr device(OpenDevice());

    // The font.. As it must be said that it's here defined.. njah.. I
    ALLEGRO_FONT *buttonFont;
    ALLEGRO_FONT *libraryFont;

    int buttonFontSize;

    bool fullScreen=false;

    float bwManagerFade = 0.0f;


    /* * * * * * * * * * * * * * * * * * * *
    * S e t u p  P a n e l and GUI ELEMENTS *
    ****************************************/

    // GUI Checkboxes For Setup-Panel. Good to be here, so they can be used from anywhere/any class later on..
    GUICheckBox showDividedBox(487,112,"Show divided data", "Show smooth graphs.", STORABLE);
    GUICheckBox showPeakPosBox(487,112+15*2,"Show peakpositions of graphs", false, "Show maximum peak positions of graphs.", STORABLE);

    GUICheckBox brainWaveColoredInMenuBox(487,112+15*4, "Brainwave colored names in playlist", true, "User relatively colored songs in playlist.", STORABLE);

    GUICheckBox recordBrainWaveBox(   487, 112+15*6, "Record New Brainwave Data", true, "Keep this on to record new brainwave data.", STORABLE);

    // Volume rollbutton after song scrollbar in the right corner
    GUIRollButton fadingLengthRollButton(500, 112+15*7+60*0.7,       20, "Length Of Mix Fading",0.0f,60.0f,"s",12.0f, "The fading time of song end and beginning(s).", STORABLE); // Rouull
    GUIRollButton delayBeforePeakPosRollButton(500, 112+15*7+60*1.7, 20, "Delay Before Peak Positions(DjMode)",0.0f,60.0f,"s",10.0f, "How long to play the track before desired position in DJ Mode(s).", STORABLE); // Rouull

    GUIRollButton djModeMinLengthOfPartRollButton(500, 112+15*7+60*2.7, 20, "Min Length Of DJMode Song Part",0.0f,1000.0f,"s", 30.0f, "Minimum length of DJ Mode song partition(s).", STORABLE); // Rouull
    GUIRollButton djModeMaxLengthOfPartRollButton(500, 112+15*7+60*3.7, 20, "Max Length Of DJMode Song Part",0.0f,1000.0f,"s", 60.0f*3.0f, "Maximum length of DJ Mode song partition(s).", STORABLE); // Rouull

    GUIRollButton mindWavePortRollButton(487+290, 170+15*6.5, 20,"MindWave Search Start Port.", 0, 30, " COM", 3, "Use this to choose right port for your MindWave.", STORABLE);

    GUIRollButton mindWaveScaleFactor(487+320, 170+13*13.5, 20,"MindWave Amplitude Scaling.", 0.0f, 10.0f, " x", 3.0f, "Use this to scale amplitude of graphs", STORABLE);

    GUIButton reBootMindWave(487+320, 170+13*10, 28, 28, 15, "media/gfx/mindWaveBoot.png", "Reboot MindWave!", "Reboot mindwave(rebooting this software may also help).");

    GUIRollButton OpenEEGPortRollButton(487+340, 255+15*11, 20,"OpenEEG Port.", 0, 30, " COM", 2, "Use this to choose right port for your OpenEEG.", STORABLE);

    GUIButton reBootOpenEEG(487+320, 255+13*15.5, 28, 28, 15, "media/gfx/mindWaveBoot.png", "Reboot Open EEG!", "Reboot Open EEG.");

    // Options for EEG -device.
    GUIRadioButton EEGDeviceModeRadioButton(780, 180, "EEG Input Device", "Choose device to use for EEG input.", STORABLE);
    GUICheckBox    EEGDevicegOp1( "MindWave", "NeuroSky MindWave.");
    GUICheckBox    EEGDevicegOp2( "Olimex EEG-SMT 1 Chan", "Olimex EEG-SMT 1 Channel(Open EEG)." );
    GUICheckBox    EEGDevicegOp3( "Olimex EEG-SMT 2 Chan", "Olimex EEG-SMT 2 Channels(Open EEG)." );


    GUICheckBox showVisualization(487+250, 112+15*23, "Show Visualization", false, "Show visualization.", STORABLE);
    GUICheckBox showMirror(487+250, 112+15*24, "Show Mirror", true, "The mirror effect below. Off to increase performance.", STORABLE);


    // Playlist to show songs for choosing song and editing the list..
    GUIScrollPanel playListPanel(62,125,20,352,20,12,"Choose song to play.");

    // Scroller for song position.. length was 440 originally
    GUIScroller songScrollBar(414+94+37,520,HORISONTAL_AXIS,340,15,500/10, "Timeline of the song.");

    // Volume rollbutton after song scrollbar in the right corner
    GUIRollButton guiVolumeRollButton(414+94+37+340+30,520,20,"Volume", "Adjust volume.", STORABLE); // Rouull

    // Exit, resize, minimize
    GUIButton exitButton(1000-2-25, 4, 25, 25, 15,"media/gfx/quitIcon.png", "Exit.");
    GUIButton resizeButton(1000-4-25*2, 4, 25, 25, 15,"media/gfx/resizeScreenIcon.png", "Resize.");
    GUIButton minimizeButton(1000-6-25*3, 4, 25, 25, 15,"media/gfx/minimizeIcon.png", "Minimize.");


    // Playbutton :)
    GUIButton playButton(414+94,520,35, "media/gfx/play_icon.png", "Play song.");
    GUIButton pauseButton(414+94,520,35, "media/gfx/pause_icon.png", "Pause song.");

    GUIButton RecordModeButton(725, 54, 51, 28, 15, "media/gfx/RecordMode.png", "Record or play music manually.");
    GUIButton DJModeButton(725+51+4, 54, 51, 28, 15, "media/gfx/DJPlay.png", "Use DJ Mode to choose music.");
    GUIButton ConfigureButton(725+51*2+4*2, 54, 51, 28, 15, "media/gfx/Configure.png", "Configuration panel.");
    GUIButton ConfigureEEGButton(725+51*3+5*2, 54, 51, 28, 15, "media/gfx/ConfigureEEG.png", "EEG Configuration panel.");

    // Change to diferent play-mode buttons:
    GUIButton ChangeToDJModeButton(500, 120, 68, 68, 15, "media/gfx/changeToMod_Dj.png", "Change to this DJ Mode.", true, "Change using DJ Mode(Works better the more brainwaves are recorded).");
    GUIButton ChangeToRecordModeButton(500, 120, 68, 68, 15, "media/gfx/changeToMod_Normal.png", "Change to this Normal(/record) Mode." , true, "Change to recording and manual playing mode(Do this before using DJ Mode with your songs).");

    GUIButton JumpDJButton(500, 170, 34, 34, 15, "media/gfx/jump_button.png", "Find & Play next song now!", true, "Hurry up DJ I want Music NOW!");

    GUICheckBox loopingCheckBox(480,150,"Loop songs", "Song is played again and again.", STORABLE);
    GUIButton bwTrashButton(480,175, 28, 28, 15,"media/gfx/trash_bw_icon.png", "Clear current song's brainwave information.");

    GUIButton songFileOpenButton(56, 55, 28, 28, 15,"media/gfx/open_file_icon.png", "Import Song (mp3).");
    GUIButton songFolderOpenButton(56+35,55, 28, 28, 15,"media/gfx/open_folder_icon.png", "Import folder of Song(s) and it's subfolders.");
    GUIButton songTrashButton(56+35*2,55, 28, 28, 15,"media/gfx/trash_file_icon.png", "Delete selected song from list.");

    GUIButton libraryNewButton(213, 55, 28, 28, 15,"media/gfx/new_library_icon.png", "Clear library (Use this If you want to make new list of music).");
    GUIButton libraryOpenButton(213+35,55, 28, 28, 15,"media/gfx/open_library_icon.png", "Open library (This is done automatically when opening application).");
    GUIButton librarySaveButton(213+35*2,55, 28, 28, 15,"media/gfx/save_library_icon.png", "Save library (This is done automatically when quiting application).");

    // Different mode choosing radio-choose menu
    GUIRadioButton djModeRadioButton(487+250, 112+16*14.5, "Disk Jockey Mode", "DJ Mode uses your brainwaves or your manually chosen ideal for picking songs.", STORABLE);
    // Options to choose.. Added later in the main-void to the radiobutton
    GUICheckBox    djModeOp1( "Chosen State List", "DJ Mode use your choice from the DJ PANEL to find songs." );
    GUICheckBox    djModeOp2( "BCI State List", "DJ Mode will use your current brainwave state to find proper songs." );

    GUICheckBox    djModeUseXAxis( 480, 110, "Use X-axis (Beta)", true, "If this is enabled DJ Mode will find songs according beta-channel of recordings.", STORABLE );
    GUICheckBox    djModeUseYAxis( 480, 110+18,"Use Y-axis (Theta)", true, "If this is enabled DJ Mode will find songs according theta-channel of recordings.", STORABLE );

    // Different options of playing whole songs or just song partions in Djmode
    GUIRadioButton djPlayPartitions(487+250, 112+19*15, "DJ-mode Part Size", "DJ Mode will play full songs or parts of the songs.", STORABLE);
    GUICheckBox    partOp1( "Play Parts", "For letting DJ Mode play parts of the songs."  );
    GUICheckBox    partOp2( "Play Full Songs", "For letting DJ Mode play full songs." );

    // Options for different listening flow graph. Standard it's using profile channel 1(which is usually beta channel - activity)
    GUIRadioButton visualizationParameterRadioButton(487+250, 112+12*15, "Visualization channel", "This tells which channel visualization is reacting to.", STORABLE);
    GUICheckBox    actOp1( "Use profile channel 1", "Default - activity linked(ie. beta).");
    GUICheckBox    actOp2( "Use profile channel 2", "Use second channel." );


    GUIXYPicker XYDJPicker(650,88,88,88, "Choose desired music or let brain choose it.");

    // Background visualization..
    BgEffect backgroundVisualization(1000,600);

    bool onTopVisualization = false;

    // Set up Brainwave Manager and it's GUI (to the previously declared elementlist!)
    BWManager bwManager("Standard", 478, 240, 500, 260, "Different brainwave readings.");

    // Playlist to show songs for choosing song and editing the list..
    GUISpectrumPanel SpectrumPanel(480,125,20,272,17,10,"Neurofeedback panel showing bw values.");
    //int xP, int yP, int panelItemSizeYP, int widthP, int heightP, int scrollBarWidthP, string toolTipText)

    /* * * * *
    * E n d  *
    * * * * */

    // Create MindWave Device..
    MindWaveDevice mindWaveDevice;

    // Create Open EEG Device..
    OpenEEGDevice openEEGDevice;


    // Attributes used in main..

    double dmin = std::numeric_limits<double>::min(); // minimum value
    double dmax = std::numeric_limits<double>::max();

    /* Simple FPS counter. */
    int fps, fps_accum;
    double fps_time;
    double totalFrameTime;
    // Important global functions to cause smooth movement etc.
    double lastUpdate=0;
    double lastGfxUpdate=0;
    double pauseStartTime;

    double gfxDesiredFps = 1.0f/160.0f; // Ie 80 fps. Put something like double the desired.. Because it's not tuned to work properly.

    bool songPlaying;
    int pausedAtPoint=-1;

    int down;
    int down_x;
    int down_y;

    string panelText = "Chosen Panel Text";

    ALLEGRO_BITMAP *graphBaseImg;
    ALLEGRO_BITMAP *iconImg;

    // Player samplesource and stream for streaming audio
    SampleSourcePtr samplesource;
    OutputStreamPtr stream;

    // For DJ Mode two streams to fade in and fade the other out at the same time..
    OutputStreamPtr fadeInStream;
    OutputStreamPtr fadeOutStream;

    double fadeOutStarted;
    float  fadeOutStartVolume;
    bool   fadeOut;
    double fadeInStarted;
    bool   fadeIn;

    double startTimeToPlaySongInDjMode;
    float  djModeCurrentPieceDuration;

    float mirrorBottom;

    float songInfoTextX = 0;

    // Allegro bitmaps. For background
    ALLEGRO_BITMAP *bgImg   = NULL;
    ALLEGRO_BITMAP *copyImg = NULL;


    // Our common BWMeter to get some nice current Brainwave-data and show it on the gui :) Start it up candyy!
    BWMeter bwMeter(820,93, "Brainwave meter."); //795

    // Converts int to string. Very nice.
    string intToStr(int number)
    {
       stringstream ss;//create a stringstream
       ss << number;//add number to the stream
       return ss.str();//return a string with the contents of the stream
    }

    // Converts float to string. Very nice.
    string desToStr(float number)
    {
       stringstream ss;//create a stringstream
       ss << number;//add number to the stream
       return ss.str();//return a string with the contents of the stream
    }

    // Str to integer :)
    int strToInt(string strToConvert) {
        return atoi(strToConvert.c_str());
    }

    double strToDes(string strToConvert) {
        double value;
        try
        {
            value = boost::lexical_cast<double>(strToConvert);
        }
        catch (boost::bad_lexical_cast const&)
        {
            value = 0;
        }
        return value;
    }

    double Index2Freq(int i, double sampleRate, int n) {
      return (double) i * (sampleRate / n);
    }

    int Freq2Index(double freq, double sampleRate, int n) {
      return (int) (freq / (sampleRate / n));
    }


    // Tells wether a given file in the path is a valid file for music or not..
    bool isValidMusicPlayerFile(path p) {
        bool answer  = false;
        if ( extension(p) == ".mp3" || extension(p) == ".ogg" || extension(p) == ".wav" ||
             extension(p) == ".flac" || extension(p) == ".aiff" || extension(p) == ".mod" ||
             extension(p) == ".s3m" || extension(p) == ".xm" || extension(p) == ".it" )
            answer  = true;
        else
            playerLog("Wasn't valid music file! isValidMusicPlayerFile returned false. ");
        return answer;
    }

    // Logs stuff.. Change this to allegro or windows console depending desire:)
    void playerLog(string s) {
        fprintf(stderr,(s+"\n").c_str());
    }


    bool unLoadFonts() {
        al_destroy_font(buttonFont);
        if (buttonFont){
            playerLog("Button font still alive. It shouldnt be...\n");
            return false;
        }

        al_destroy_font(libraryFont );
        if (libraryFont){
            playerLog("libraryfont font still alive. It shouldnt be...\n");
            return false;
        }

        return true;

    }

    bool loadFonts() {
        buttonFontSize = 8;
        buttonFont = al_load_ttf_font("pirulen.ttf",buttonFontSize,0 );
        if (!buttonFont){
            playerLog("Could not load 'pirulen.ttf'.\n");
            return false;
        }

        // Load font also at the same time..
        libraryFont = al_load_ttf_font("pirulen.ttf",12,0 ); //UnionBold
        if (!libraryFont){
            playerLog("Could not load 'pirulen.ttf '.\n");
            return false;
        }

        return true;
    }

    float getDistance(float x,float y,float x2,float y2) {
        float dx = x-x2;
        float dy = y-y2;
        // And using the Pythagoras we can achieve the distance between two points.. Clever :)
        return sqrt(dx*dx+dy*dy);
    }

    ALLEGRO_BITMAP* getScaledImage(ALLEGRO_BITMAP* src, float scaleFactorX, float scaleFactorY) {
        ALLEGRO_BITMAP* dest = NULL;
        float originalWidth=al_get_bitmap_width(src);
        float originalHeight=al_get_bitmap_height(src);
        //dest = imageLibrary.createImage(originalWidth*scaleFactorX, originalHeight*scaleFactorY);
        dest = al_create_bitmap(originalWidth*scaleFactorX, originalHeight*scaleFactorY);
        if(!dest) {
            playerLog("Failed to create destination image in copyScaledImage()-function..'.\n");
        }
        al_set_target_bitmap(dest);
        al_draw_scaled_bitmap(src, 0, 0, originalWidth, originalHeight, 0, 0, originalWidth*scaleFactorX, originalHeight*scaleFactorY, 0);
        al_set_target_bitmap(al_get_backbuffer(display));
        return dest;
    }


    // split: receives a char delimiter; returns a vector of strings
    // By default ignores repeated delimiters, unless argument rep == 1.
    vector<string>& splitstring::split(char delim, int rep) {
        if (!flds.empty()) flds.clear();  // empty vector if necessary
        string work = data();
        string buf = "";
        int i = 0;
        while (i < work.length()) {
            if (work[i] != delim)
                buf += work[i];
            else if (rep == 1) {
                flds.push_back(buf);
                buf = "";
            } else if (buf.length() > 0) {
                flds.push_back(buf);
                buf = "";
            }
            i++;
        }
        if (!buf.empty())
            flds.push_back(buf);
        return flds;
    }

    string forceRemoveAllDoubleEmptySpaces(string txt) {
        // I think this 10 trial will trim or remove all empty spaces :D
        for (int i=0;i<10;i++) {
            txt = string(removeDoubleEmptySpaces(txt));
        }
        return txt;
    }

    string removeExcessFromMiddle(string sourceStr, int limitStrSize)
    {
        if(sourceStr.length() < limitStrSize)
            return sourceStr;
        // Else.. Remove from middle.. Count how many excess and remove..
        int excess = sourceStr.length()-limitStrSize;
        int middlePoint = int(sourceStr.length()/2);
        int startPoint = middlePoint-int(excess/2);
        int endPoint = middlePoint+int(excess/2);
        if (startPoint<0 || endPoint>sourceStr.length())
            return "trimAndRemoveExcess..Too small string to handle.";
        return (sourceStr.substr(0,startPoint)+sourceStr.substr(endPoint,sourceStr.length()-endPoint));
    }

    // Hey does that hmm.. Is that destroyed.. damnd.. when it goes out of this function
    // No it is not As far as I tested it..
    char* removeDoubleEmptySpaces(string txt) {
        char *cstr = new char[txt.length() + 1];
        strcpy(cstr, txt.c_str());
        int len = (int)strlen(cstr);

        int i = 0;
        int j = 0;

        while (i != len-1) {
          if ( !(cstr[i] == ' ' && cstr[i+1] == ' ') )
            cstr[j++] = cstr[i];
          i++;
        }
        return cstr;
    }


    void drawGlowDot(float px, float py, float glowValue) {
        float alpha = 0.9f;
        al_draw_pixel(px, py,   al_map_rgba_f(0.0*alpha*glowValue, 0.0*alpha*glowValue, 1.0*glowValue*alpha, alpha));
        alpha = 0.2f;
        al_draw_pixel(px, py,   al_map_rgba_f(0.3*alpha*glowValue, 0.3*alpha*glowValue, 1.0*alpha*glowValue, alpha));
        alpha = 0.9f;
        al_draw_pixel(px-1, py, al_map_rgba_f(.0*alpha*glowValue, .0*alpha*glowValue, 1.0*glowValue*alpha, alpha));
        al_draw_pixel(px+1, py, al_map_rgba_f(.0*alpha*glowValue, .0*alpha*glowValue, 1.0*glowValue*alpha, alpha));
        al_draw_pixel(px, py-1, al_map_rgba_f(.0*alpha*glowValue, .0*alpha*glowValue, 1.0*glowValue*alpha, alpha));
        al_draw_pixel(px, py+1, al_map_rgba_f(.0*alpha*glowValue, .0*alpha*glowValue, 1.0*glowValue*alpha, alpha));

        alpha = 0.8f;
        al_draw_pixel(px-1, py-1, al_map_rgba_f(0.0*alpha, 0.0*alpha, 0.8*glowValue*alpha, alpha));
        al_draw_pixel(px+1, py-1, al_map_rgba_f(0.0*alpha, 0.0*alpha, 0.8*glowValue*alpha, alpha));
        al_draw_pixel(px-1, py+1, al_map_rgba_f(0.0*alpha, 0.0*alpha, 0.8*glowValue*alpha, alpha));
        al_draw_pixel(px+1, py+1, al_map_rgba_f(0.0*alpha, 0.0*alpha, 0.8*glowValue*alpha, alpha));

        alpha = 0.3f;
        al_draw_pixel(px-2, py, al_map_rgba_f(0.0*alpha, 0.0*alpha, 0.3*glowValue*alpha, alpha));
        al_draw_pixel(px+2, py, al_map_rgba_f(0.0*alpha, 0.0*alpha, 0.3*glowValue*alpha, alpha));
        al_draw_pixel(px, py-2, al_map_rgba_f(0.0*alpha, 0.0*alpha, 0.3*glowValue*alpha, alpha));
        al_draw_pixel(px, py+2, al_map_rgba_f(0.0*alpha, 0.0*alpha, 0.3*glowValue*alpha, alpha));
    }

    void drawPowerMeter(float px, float py, float width, float height, float value) {
        // Triangle
        float alpha = 0.7f;
        float heightNow = 1;
        float raiseHeightStep = height/width;
        int lightTheseBars = (width * value);
        int skipper=0;
        for (int ix = 0; ix < width; ix++) {
            if (skipper==4) {
                for (int iy = 0; iy < heightNow; iy++) {
                    if (ix<lightTheseBars)
                        al_draw_pixel(px+ix, py-iy,al_map_rgba_f(0.3*alpha, 0.3*alpha, 1.0*alpha, alpha));
                        //drawGlowDot(px+ix, py-iy,1.0f);
                    else //darker
                        //drawGlowDot(px+ix, py-iy,0.3f);
                        al_draw_pixel(px+ix, py-iy,al_map_rgba_f(0.0*alpha, 0.0*alpha, 0.7*alpha, alpha));
                }
            skipper=0;
            }
            skipper++;
            heightNow += raiseHeightStep;
        }
    }


    float getNegationOfThisPrefix(float num) {
        if (num>0.0f)
            return -1.0f;
        else
            return 1.0f;
    }

    float getPositiveOfThis(float num) {
        if (num<0.0f)
            return -num;
        else
            return num;
    }

    float getPrefixOf(float num) {
        if (num>0.0f)
            return 1.0f;
        else
            return -1.0f;
    }

    float limitValueUp(float tval, float limitV) {
        if (tval>limitV)
            tval=limitV;
        return tval;
    }
    float limitValueDown(float tval, float limitV) {
        if (tval<limitV)
            tval=limitV;
        return tval;
    }

    void getValueInsideBoundaries(float &tval, float limitVDown, float limitVUp) {
        float resultV = tval;
        if (resultV>limitVUp)
           resultV = limitVUp;
        if (resultV<limitVDown)
            resultV = limitVDown;
        tval = resultV;
    }


    // This is a bit weirds. :H
    ALLEGRO_COLOR getFromColorToColor(ALLEGRO_COLOR colFrom, ALLEGRO_COLOR colTo, double fac) {
        // Calculate 3d translation in the color cube. Just add vector to point with scaling factor.
        ALLEGRO_COLOR deltaVec = al_map_rgb_f(colTo.r - colFrom.r, colTo.g - colFrom.g, colTo.b - colFrom.b);
        return al_map_rgb_f(colFrom.r + deltaVec.r * fac, colFrom.g + deltaVec.g * fac, colFrom.b + deltaVec.b * fac);
    }


    // Get color of the text color according this song's brainwave information. Ie calm song is different in color than energetic song.
    // Values are relative from 0.0 - 1.0 and 0.0 - 1.0 of their position in the min to max of the channel's avarages of each song.
    ALLEGRO_COLOR getMapColor(float valX, float valY) {

        ALLEGRO_COLOR startPointColor = al_map_rgb_f(0.0, 0.0, 0.0); // This is init color If no bw data is available.
        ALLEGRO_COLOR defPointColor   = al_map_rgb_f(0.2, 0.2, 0.2);

        // Jump to right positions.
        ALLEGRO_COLOR trans1 = getFromColorToColor(startPointColor,getGraphThemeColorOf(bwMeter.PROFILE_CHANNEL_1),valX);
        ALLEGRO_COLOR trans2 = getFromColorToColor(startPointColor,getGraphThemeColorOf(bwMeter.PROFILE_CHANNEL_2),valY);

        ALLEGRO_COLOR endPointColor = al_map_rgb_f(startPointColor.r + trans1.r + trans2.r, startPointColor.g + trans1.g + trans2.g, startPointColor.b + trans1.b + trans2.b);
        endPointColor = al_map_rgb_f(endPointColor.r*(1.0-defPointColor.r)+defPointColor.r,endPointColor.g*(1.0-defPointColor.g)+defPointColor.g,endPointColor.b*(1.0-defPointColor.b)+defPointColor.b);

        return endPointColor;
    }

    void drawRotatedTintedPicture(ALLEGRO_BITMAP *img, ALLEGRO_COLOR tint, float dx, float dy, float angle) {
        al_draw_tinted_rotated_bitmap(img, tint, al_get_bitmap_width(img)/2, al_get_bitmap_height(img)/2 , dx, dy, angle, 0);
    }


    void drawRotatedPicture(ALLEGRO_BITMAP *img, float dx, float dy, float angle) {
        al_draw_rotated_bitmap(img,  al_get_bitmap_width(img)/2, al_get_bitmap_height(img)/2 , dx, dy, angle, 0);
    }

    void drawScaledPicture(ALLEGRO_BITMAP *img, float dx, float dy, float width, float height) {
        int dflags = 0;
        al_draw_scaled_bitmap(img,0,0, al_get_bitmap_width(img), al_get_bitmap_height(img), dx, dy, width, height, dflags);
    }

    void drawTintedPicture(ALLEGRO_BITMAP *img, ALLEGRO_COLOR color, float x, float y, int flags) {
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE); // Nice tint
            al_draw_tinted_bitmap(img, color, x, y, flags);
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA); // No tint for other drawing..
    }

    float getDistanceBetweenValues(float val1, float val2) {
        float dist = val1 - val2;
        if (dist<0.0f)
            dist = -dist;
        return dist;
    }

    /* Function to control # of decimal places to be output for x */
    double showDecimals(const double& x, const int& numDecimals) {
        int y=x;
        double z=x-y;
        double m=pow(10,numDecimals);
        double q=z*m;
        double r=round(q);

        return static_cast<double>(y)+(1.0/m)*r;
    }


    string getTimeStamp() {
        // Get current date/time, format is YYYY-MM-DD-HH-mm-ss
        time_t     now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d-%H-%M-%S", &tstruct);
        // Damn boost!! Why you had to be so complicated. See! This is what you got! Now I'm using c++ basic stuff! This should also work on multiple platform.
        return string(buf);
    }

    // Modifie from stackoverflow example. Takes of the characters that are requested.
    string removeChars(string str, char *chars) {
       string strOrig = str;
       for (unsigned int i = 0; i < strlen(chars); ++i)
       {
          // you need include <algorithm> to use general algorithms like std::remove()
          str.erase (std::remove(str.begin(), str.end(), chars[i]), str.end());
       }
       playerLog("# Removing chars from string " + strOrig + " to result " + str);
       return str;
    }

    bool strToBool(string val) {
        if (val=="1")
            return true;
        else
            return false;
    }

    string boolToStr(bool val) {
        if (val)
            return "1";
        else
            return "0";
    }

    // This is to give some value to chew on for background effects in visualization..
    double getEffectiveValueOfEEG() {
        if (visualizationParameterRadioButton.getSelectionTxt() == actOp1.getText())
            return bwMeter.getCurrentProfileMindState(bwMeter.PROFILE_CHANNEL_1)*2.0d-1.0d;
        else
            return bwMeter.getCurrentProfileMindState(bwMeter.PROFILE_CHANNEL_2)*2.0d-1.0d;
    }

    bool usingMindWave() {
        if (!usingOpenEEG()) // or EEGDeviceModeRadioButton.getSelectionTxt()==EEGDevicegOp1.getText()
            return true;
        else
            return false;
    }

    bool usingOpenEEG() {
        if (EEGDeviceModeRadioButton.getSelectionTxt()==EEGDevicegOp2.getText() || EEGDeviceModeRadioButton.getSelectionTxt()==EEGDevicegOp3.getText())
            return true;
        else
            return false;
    }

    int howManyChannelsInOpenEEG() {
        if (EEGDeviceModeRadioButton.getSelectionTxt()==EEGDevicegOp3.getText())
            return 2;
        else
            return 1;
    }
};




