#include "../include/BWManager.h"
#include "../include/thinkgear.h"
#include "../include/PlayerCommon.h"


using namespace plrCommon;

BWManager::BWManager(string s, int posX, int posY, int width, int height, string toolTipText)
{
    reset();
    graphUIX = posX;
    graphUIY = posY;
    graphWidth = width;
    graphHeight = height;
    // Initialize graphs to basic.
    /*for (int i=0;i<AMOUNT_OF_CHANNELS;i++) {
        channel[i] = BWGraph();
    }*/
    GUIElement::setToolTipArea(posX,posY,GRAPH_SIZE_X,GRAPH_SIZE_Y*8,toolTipText);
}

BWManager::BWManager()
{
    //ctor
    reset();
}

/*
* Reset()
*::::::::
*
* This method just resets some basic stuff and load MindWave(it shoold be actually loaded already some-elsewhere.)
*
*/
void BWManager::reset()
{
    visibilityShade=0.0f;
    graphUIX = 0;
    graphUIY = 0;
    graphWidth = 0;
    graphHeight = 0;
}

// Stops recording of brainwaves
void BWManager::stopRecording()
{
    recording = false;
}

// Starts recording of brainwaves
void BWManager::startRecording()
{
    recording = true;
    lastGraphUpdate = int( std::clock() );
}

// Draws all channels' images again
void BWManager::drawAllAgain()
{
    for (int i=0; i<AMOUNT_OF_CHANNELS; i++)
    {
        channel[i].drawZeroLine();
        channel[i].drawWholeGraph();
    }
}

/*
* newBWTable(string songId,int songLengthInSecs)
*::::::::
*
* This method just creates all the graphs of this song. This seems to be used only when the process starts for new song. Otherwise the graphs are loaded from serialization system.
*
* songId : The id of the song for what the graphs are created.
* songLengthInSecs : The length of the song in seconds.
*
*/
void BWManager::newBWTable(string songId,int songLengthInSecs)
{

    // Are we already recording the same brainwave?
    if (bwRecordId != songId)
    {
        initClearTable(songId,songLengthInSecs);
    }

}

// This function doesn't care If its already made or not. It makes it again. This is used straigtly to trash the graph.
void BWManager::initClearTable(string songId,int songLengthInSecs) {
        bwRecordId = songId;

        bwRecordLengthInSecs = songLengthInSecs;

        playerLog("Creating new Brainwave Table Information..");

        lastGraphUpdate = int( clock() );

        /* initialize random seed: */
        srand (time(NULL));

        // Create the channels and their drawing information
        //int graphUIY     = 220;
        int i=1;

        // GRaphs are placed relative to bw manager's position :)
        channel[CHANNEL_DELTA].create(0,0+(GRAPH_SIZE_Y*0+GRAPH_SPACING*i), CHANNEL_DELTA,songLengthInSecs);
        channel[CHANNEL_THETA].create(0,0+(GRAPH_SIZE_Y*1+GRAPH_SPACING*i), CHANNEL_THETA,songLengthInSecs);

        channel[CHANNEL_ALPHA].create(0,0+(GRAPH_SIZE_Y*2+GRAPH_SPACING*i),CHANNEL_ALPHA,songLengthInSecs);

        channel[CHANNEL_ALPHA1].create(0,0-50,CHANNEL_ALPHA1,songLengthInSecs); // 1. and 2. graphs are not displayed. We use just their avarage and show that.
        channel[CHANNEL_ALPHA2].create(0,0-100,CHANNEL_ALPHA2,songLengthInSecs);

        channel[CHANNEL_BETA].create(0,0+(GRAPH_SIZE_Y*3+GRAPH_SPACING*i), CHANNEL_BETA,songLengthInSecs);

        channel[CHANNEL_BETA1].create(0,0-50, CHANNEL_BETA1,songLengthInSecs);  // 1. and 2. graphs are not displayed. We use just their avarage and show that.
        channel[CHANNEL_BETA2].create(0,0-100, CHANNEL_BETA2,songLengthInSecs);

        channel[CHANNEL_GAMMA].create(0,0+(GRAPH_SIZE_Y*4+GRAPH_SPACING*i),CHANNEL_GAMMA,songLengthInSecs);

        channel[CHANNEL_GAMMA1].create(0,0-50,CHANNEL_GAMMA1,songLengthInSecs);  // 1. and 2. graphs are not displayed. We use just their avarage and show that.
        channel[CHANNEL_GAMMA2].create(0,0-100,CHANNEL_GAMMA2,songLengthInSecs);

        // TSome of these maybe placed on top of each other because lack of space.. So it's hard to show em at the same time..->
        channel[CHANNEL_ATT].create(0,0+(GRAPH_SIZE_Y*5+GRAPH_SPACING*i),CHANNEL_ATT,songLengthInSecs);
        channel[CHANNEL_MED].create(0,0+(GRAPH_SIZE_Y*6+GRAPH_SPACING*i),CHANNEL_MED,songLengthInSecs);
        channel[CHANNEL_AVG].create(0,0+(GRAPH_SIZE_Y*7+GRAPH_SPACING*i),CHANNEL_AVG,songLengthInSecs);
        channel[CHANNEL_POWER_AVG].create(0,0+(GRAPH_SIZE_Y*7+GRAPH_SPACING*i),CHANNEL_POWER_AVG,songLengthInSecs);


}

// Get the lst virtual current flow value.
float BWManager::getLastVirtualCurrentFlowValue()
{
    return ((channel[CHANNEL_ATT].getValueOfCurrentSongsPlayedPos() - channel[CHANNEL_MED].getValueOfCurrentSongsPlayedPos())/100.0f);
}

// Get the last virtual profile channel value.
float BWManager::getLastVirtualRelativeStateOfChannel(int chanIndxp)
{
    return musicLibrary.getRelativeValueInAllSongsOfChannel(chanIndxp, channel[chanIndxp].getValueOfCurrentSongsPlayedPos() );
}

BWManager::~BWManager()
{
    //dtor
}

// Position changed.. Tell graphs to avoid thinking that the last pos was actually last ps but now we have new..
void BWManager::positionChanged()
{
    for (int i=0; i<AMOUNT_OF_CHANNELS; i++)
        channel[i].dontUseLastPosition();
}

// Called when initializing gui elements..
void BWManager::init()
{
    playerLog("This was BWManager.. Do you copy?");
}

// Adjusts visibility
void BWManager::adjustVisibility(int direction)
{
    visibilityShade += direction*deltaTime*0.3f;
    if (visibilityShade>1.0f) visibilityShade=1.0f;
    if (visibilityShade<0.0f) visibilityShade=0.0f;
    bwManagerFade = visibilityShade; // Update this way-round-around solution value for peak position drawing also..
}


/*
* refresh()
*::::::::
*
* To refresh all the channels data. Usually this is done in one second interval. Here also is calculated the avarage channel's value of all graphs(dominating freq).
*
*/
void BWManager::refresh()
{

    // If visible then do some tricks.. Ie. it can be clicked to choose position.
    if (isVisible()) {
        // It's possible to choose timeline position and view graph information.
        if (mouseInsideManager()) {
            //int secondsInOneScreenX = graphWidth/graphScaleX;
            int secondsInOneScreenX = bwRecordLengthInSecs;
            int firstSecToDraw = 0;
            //int firstSecToDraw = bwScrollBar.getBarPosition()*(bwRecordLengthInSecs-secondsInOneScreenX);
            if (firstSecToDraw<0.0f)
                firstSecToDraw=0.0f;
            float selectedAtTimeLine = int((float(guiMouse.getMouseX()-graphUIX)/float(graphWidth)*float(secondsInOneScreenX)) + firstSecToDraw); // -1 because we will move it soon again to +1 when second is reached..
            // Set tool tip text for to show time.
            int minutes = int(selectedAtTimeLine / 60);
            int seconds = int(selectedAtTimeLine) % 60;
            if (minutes==0) {
                setToolTipTextOnly(intToStr(selectedAtTimeLine) + " s");
            } else {
                setToolTipTextOnly(intToStr(minutes) + " min " + intToStr(seconds) + " s");
            }

            if (mouseClickedInsideManager()) {
                if (selectedAtTimeLine!=songPositionInSec) {
                    // Choose only If it's different position. Not to flood console..
                    //songPositionInSec = selectedAtTimeLine;
                    //if (songPositionInSec<0) songPositionInSec=0;
                    //songScrollBar.setPosition(selectedAtTimeLine/bwRecordLengthInSecs);
                    stream->setPosition(int( (selectedAtTimeLine/bwRecordLengthInSecs) *float(stream->getLength())));
                    positionChanged(); // Tell to start editinr from different point and have a new "last ppoint"
                }
            }
        }
    }

    // Check for mousehover
    if (!showVisualization.isChecked())
    {
        adjustVisibility(1);
    }
    else
    {
        if (mouseInsideManager())
        {
            adjustVisibility(1);
        }
        else
        {
            adjustVisibility(-1);
        }
    }

    if (recording && bwMeter.EEGDeviceIsWorking() && recordBrainWaveBox.isChecked())
    {

        double timeFromLastGraphUpdate = double((std::clock()-lastGraphUpdate)/((double)CLOCKS_PER_SEC));

        if (timeFromLastGraphUpdate > 1.0 && recording)
        {

            lastGraphUpdate = int( std::clock() );

            // Fille the graphs with new data values.. Or old If nothing is updated..
            // Thhis is now done in this way. All information should be transfgered still properly from bwmeter.. :)
            for (int i=0; i<AMOUNT_OF_CHANNELS; i++)
                channel[i].refresh();


        } // If time from last update is one second.. Go ahdea and try record!

    } // Recording?<-

}

bool BWManager::mouseInsideManager() {
    if (guiMouse.getMouseX()>graphUIX && guiMouse.getMouseY()>graphUIY-GRAPH_SIZE_Y/2 && guiMouse.getMouseX()<graphUIX+GRAPH_SIZE_X && guiMouse.getMouseY()<graphUIY+GRAPH_SIZE_Y*9-GRAPH_SIZE_Y/2)
        return true;
    else
        return false;
}

bool BWManager::mouseClickedInsideManager() {
    if (guiMouse.getMouseClickX()>graphUIX && guiMouse.getMouseClickY()>graphUIY-GRAPH_SIZE_Y/2 && guiMouse.getMouseClickX()<graphUIX+GRAPH_SIZE_X && guiMouse.getMouseClickY()<graphUIY+GRAPH_SIZE_Y*9-GRAPH_SIZE_Y/2) {
        if (!guiMouse.getLeftButtonState() && guiMouse.getLeftButtonStatePrevious())
            return true;
        else
            return false;
    }
}


// Unload all
void BWManager::unLoad()
{
    /* Clean up */
    TG_FreeConnection( connectionId );
}

/*
* draw()
*::::::::
*
* Simply draws all the channels that this manager holds for the song. This is the screen of the graphs seen in the program.
*
*/
void BWManager::draw()
{
    // Draw all channels to screen that are wanted to be shown.
    if (isVisible())
    {

        float graphLookAtX = 0;

        channel[CHANNEL_DELTA].draw(visibilityShade);

        channel[CHANNEL_THETA].draw(visibilityShade);

        channel[CHANNEL_ALPHA].draw(visibilityShade);

        //   channel[CHANNEL_ALPHA1].draw(surface);
        //   channel[CHANNEL_ALPHA2].draw(surface);

        channel[CHANNEL_BETA].draw(visibilityShade);

        //   channel[CHANNEL_BETA1].draw(surface);
        //   channel[CHANNEL_BETA2].draw(surface);

        channel[CHANNEL_GAMMA].draw(visibilityShade);

        //   channel[CHANNEL_GAMMA1].draw(surface);
        //   channel[CHANNEL_GAMMA2].draw(surface);

        // Choose which of these three! --> No let's juust draw all!s

        channel[CHANNEL_ATT].draw(visibilityShade);
        channel[CHANNEL_AVG].draw(visibilityShade);
        channel[CHANNEL_MED].draw(visibilityShade);
    // Draw power avg?

    }
    GUIElement::draw();
}
