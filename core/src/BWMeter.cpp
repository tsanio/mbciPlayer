/********************************************************************************
* Meter to show the current state of Brain Computer Interface -device and some  *
* Nice graph of the EEG.                                                        *
*                                                                               *
* I guess this is used for only reading RAW Values.. Later perhaps this will be *
* the interface for all BCI devices before taking data to graphs. So the data   *
* would be handled here a bit already..                                         *
*                                                                               *
* !Update. Now it supports OpenEEG (5.2014)                                     *
*                                                                               *
* Additionally:                                                                 *
* Modified for NFB program now..                                                *
********************************************************************************/
#include "../include/BWMeter.h"
#include "../include/thinkgear.h"
#include "../include/PlayerCommon.h"

using namespace std;
using namespace plrCommon;

// Slow bar holds the information of current state of one channel. It's slow because it's smoothed and not real time accurate absolutely.
// This is why it's good for nfb .. It also captures the maximum values of each channel.
SlowBar::SlowBar()
{
    reset();
}

void SlowBar::refresh(double goToV)
{

    goToValue = goToV;

    double timeFactor = deltaTime;
    double distToGoTo = getPositiveOfThis(goToValue-currentValue);

    if (goToValue>currentValue)
        speed = distToGoTo*0.1f;
    if (goToValue<currentValue)
        speed = -distToGoTo*0.1f;
    if (speed>maxSpeed)
        speed = maxSpeed;
    if (speed<-maxSpeed)
        speed = -maxSpeed;
     // This could be made more beatiful taking the positive of ^ and replacing some if clauses etc.

    currentValue += speed*timeFactor;
    if (currentValue<0)
        currentValue=0;
    if (currentValue>100)
        currentValue=100;

}

// This is for specifying the type also.
void SlowBar::reset(int typeP) {
    reset();
    type = typeP;
}

void SlowBar::reset() {
    type = 0;
    goToValue = 0;
    currentValue = 0;
    speed = 0.0f;
    maxSpeed = 10.0f;
}

BWMeter::BWMeter()
{
    //ctor
    reset();
}

BWMeter::BWMeter(float px, float py, string toolTipText)
{
    //ctor
    reset();
    x = px;
    y = py;
    GUIElement::setToolTipArea(0,0,0,0,toolTipText);
}

BWMeter::~BWMeter()
{
    //dtor
}

/*
* Init()
*:::::::::::::
*
* Initializes pictures for the meter and other stuff ie. MindWave and devices where data is imported.
*
*/
void BWMeter::init()
{
    playerLog("Initializing BWMeter..");
    bwMeterBG   = imageLibrary.loadImage("media/gfx/BWMeterBg.png");

    mindWaveImg = imageLibrary.loadImage("media/gfx/MindWave.png");

    circleImg   = imageLibrary.loadImage("media/gfx/circle.png");

    GUIElement::setToolTipAreaOnly(x,y,al_get_bitmap_width(bwMeterBG),al_get_bitmap_height(bwMeterBG));
}




/* Reset Brainwave Meter */
void BWMeter::reset()
{
    playerLog("Resetting BrainWave Meter..");
    METER_SAMPLE_SIZE = 500;
    meterValues.clear();
    lastBWUpdate = 0;
    lastProfileUpdateTime=0;
    for (int i=0; i<AMOUNT_OF_CHANNELS; i++)
    {
        channelCurrentValue[i] = 0;
        channelSlowBar[i].reset(i);
    }


    rawMeterMultipliesBWMeter = 3; // This tells how many units of data is in one pixel width in raw.. for example 5 means 5 different new y-positions of the graph inside 1 real pixel.
    // This is good. It shows the data more slowly and thicker thand drawing update per pixel....

    profChannel1Val  = 0.0f;
    profChannel2Val  = 0.0f;
    profChannel1ValSpeed = 0.0f;
    profChannel2ValSpeed = 0.0f;
    profChannelAcc    = 0.1f;

    // Very important attributes. These determine the channels used for song profiles in 2D-mapping. Now they are beta and theta.
    PROFILE_CHANNEL_1 = CHANNEL_BETA;
    PROFILE_CHANNEL_2 = CHANNEL_THETA;
    // These are the targets of the profile which is updated in slower interval to make sure no lagging is occuring. Could be tested otherwise also.
    chan1v = 0;
    chan2v = 0;

}



/* Returns the current newest slow value as scaled to 0 - 100.0f */
double BWMeter::getCurrentEEGChannelSlowBarValue(int graphChannelId)
{
    return channelSlowBar[graphChannelId].getCurrentValue();
}



/* Returns the newest value taken from device 0-100. */
double BWMeter::getCurrentEEGChannelValue(int graphChannelId)
{
    if (useDebugInput) {
        return getRandomZeroToOne()*100000;
    }
    double chanValueToReturn = channelCurrentValue[graphChannelId];
    if (chanValueToReturn == GRAPH_EMPTY_INDEX) {
        playerLog("For some reasong the lottery ticket was won.."); // This is to make sure that empty index cannot really never be given from device's input :)
        // If it was the smallest in number.. increase by one and return.. :)
        chanValueToReturn++;
    }
    return double(chanValueToReturn);
}


/* Update channel data of given id and value */
void BWMeter::updateChannelData(int graphId, double value)
{
    // Do we have new information or not?
    channelCurrentValue[graphId] = value; // Updated!
}



bool BWMeter::EEGDeviceIsWorking()  {
    if (mindWaveDevice.isWorkingProperly() || openEEGDevice.isWorkingProperly())
        return true;
    else
        return false;
}

// This should be smootheden and also visible in the DJ deck or some other place as 2d point going around on the screen.
/* Returns the current state of the monitored and smoothened values as scaled to 0-1.0f */
double BWMeter::getCurrentProfileMindState(int profileChan)
{
    if (profileChan == PROFILE_CHANNEL_1)
        return profChannel1Val;
    else
        return profChannel2Val;
}


double BWMeter::getCurrentRelativeStateOfChannel(int profileChan)
{
    //return musicLibrary.getRelativeValueInAllSongsOfChannel(profileChan, getCurrentEEGChannelValue(profileChan) ); UPDATED TO USE SMOOTH SLOW VALUES.
    return musicLibrary.getRelativeValueInAllSongsOfChannel(profileChan, getCurrentEEGChannelSlowBarValue(profileChan) );
}

//! Update the profile for two channels.  These can be used for visualization and getting the user's current state for example according his profile of all songs.
//! Updates the knowledge of current profile values given from device or played song If device is not in use.
void BWMeter::updateProfileValues()
{

    if (EEGDeviceIsWorking()) {
        // Make some interval not to make all the times calculations. To avoid lag If that happens with big lists(not tested actually?)
        if (al_get_time() - lastProfileUpdateTime > 0.1) {
            lastProfileUpdateTime = al_get_time();
            chan1v = getCurrentRelativeStateOfChannel(PROFILE_CHANNEL_1);
            chan2v = getCurrentRelativeStateOfChannel(PROFILE_CHANNEL_2);
        }
    } else if (playThis.isBwInfoCreated()) {
        chan1v = bwManager.getLastVirtualRelativeStateOfChannel(PROFILE_CHANNEL_1);
        chan2v = bwManager.getLastVirtualRelativeStateOfChannel(PROFILE_CHANNEL_2);
    }

    if (profChannel1Val<chan1v)
        profChannel1ValSpeed+=profChannelAcc*deltaTime;
    else
        profChannel1ValSpeed-=profChannelAcc*deltaTime;

    if (profChannel2Val<chan2v)
        profChannel2ValSpeed+=profChannelAcc*deltaTime;
    else
        profChannel2ValSpeed-=profChannelAcc*deltaTime;

    profChannel1Val += profChannel1ValSpeed*deltaTime;
    profChannel2Val += profChannel2ValSpeed*deltaTime;

    profChannel1ValSpeed *= 0.98f; // Break a bit.
    profChannel2ValSpeed *= 0.98f; // Break a bit.

    // Maybe cumulate these values also to make it look reasonable? :)
    //cumulateValue(profChannel1Val);
    //cumulateValue(profChannel2Val);

    getValueInsideBoundaries(profChannel1Val, 0.0f, 1.0f);
    getValueInsideBoundaries(profChannel2Val, 0.0f, 1.0f);

}

/*
* refresh()
*:::::::::::::
*
* Refresh the Brainwave Meter and it's all channels with information offered by bci devices.
*
*/
void BWMeter::refresh()
{

    //timeFromLastUpdate = double((std::clock()-lastBWUpdate)/((double)CLOCKS_PER_SEC));
    timeFromLastUpdate = al_get_time()-lastBWUpdate;
    if (timeFromLastUpdate > 0.004)   // Update every 5ms max? 0.005 would.. now it's almost limitless..
    {

        // Reading MindWave If chosen..
        if (usingMindWave()) {

            // Update the bwmeter raw signal.. the classical EEG-view.
            meterValues.insert ( meterValues.begin() , mindWaveDevice.getRawValue() );
            //meterValues.insert ( meterValues.begin() , cos(al_get_time()*10.0d)*1000.0d );
            if (meterValues.size()>al_get_bitmap_width(bwMeterBG)*rawMeterMultipliesBWMeter)
                meterValues.pop_back(); // Keep the sample size tidy.

            // Read packets etc.
            mindWaveDevice.readPackets();
            for (int i=0;i<AMOUNT_OF_CHANNELS;i++) {
                updateChannelData(i, mindWaveDevice.getValueOf(i));
            }


        }

        // OpenEEGDevice spectrums from device handler If chosen..
        if (usingOpenEEG()) {
            // Loop through all channels and make calculate avarage of two channels IF it's the option to do so.
            for (int i=0;i<AMOUNT_OF_CHANNELS;i++)
            {
                // How many channels is selected to be used= 1 or 2?
                if (howManyChannelsInOpenEEG()==1)
                    updateChannelData(i, openEEGDevice.getSpectrumAmplitudeOf(0,i) );
                else
                    updateChannelData(i, (openEEGDevice.getSpectrumAmplitudeOf(0,i) + openEEGDevice.getSpectrumAmplitudeOf(1,i)) / 2.0d );
            }
        }

        lastBWUpdate = al_get_time();

    }

    // Then calculate avarage of power amplitudes and avarage or just the dominating frequency. This should be changed to use weighted avarage brobably.
    float realAvarageSum=0;
    float realAvarageCount=0;
    float avgV = 0.1f;
    float currentBiggest;
    for (int i=0; i<AMOUNT_OF_CHANNELS; i++)
    {
        if (useThisTypeForAvarageCalculation(i)) {
            realAvarageCount++;
            realAvarageSum+=getCurrentEEGChannelValue(i);
            if (getCurrentEEGChannelValue(i)>currentBiggest)   // compare 0-100 to 0-100
            {
                currentBiggest    = getCurrentEEGChannelValue(i); // 0-100
                avgV              = getThisTypeCenterFreq(i); // collect the frequency center.. ..
            }
        }
    }
    if (realAvarageCount!=0) {
        bwMeter.updateChannelData(CHANNEL_AVG, avgV); // Update also bwmeter value.
        bwMeter.updateChannelData(CHANNEL_POWER_AVG, realAvarageSum/realAvarageCount); // Update also bwmeter value.
    }


    // Refresh all smooth bars
    for (int i=0; i<AMOUNT_OF_CHANNELS; i++)
    {
        channelSlowBar[i].refresh(getCurrentEEGChannelValue(i));
    }

    // Update profile values.
    updateProfileValues();

}

/* Draw the BrainWave Meter. Background, emotionicons etc.. */
void BWMeter::draw()
{
    float bAlpha = 0.6f;
    drawTintedPicture(bwMeterBG, al_map_rgba_f(1.0f*bAlpha, 1.0f*bAlpha, 1.0f*bAlpha, bAlpha), x, y, 0);

    float rawAlpha = 0.35f;
    float boarderSize = 5;
    if (EEGDeviceModeRadioButton.getSelectionTxt()==EEGDevicegOp1.getText()) {
     for (int ix = boarderSize*rawMeterMultipliesBWMeter ; ix < al_get_bitmap_width(bwMeterBG)*rawMeterMultipliesBWMeter - boarderSize*rawMeterMultipliesBWMeter; ix++ )
     {
        // Meter values.. Meter means the raw value
        // Draw MindWave.
            if (ix+1<meterValues.size())
            {
                al_draw_line(x+al_get_bitmap_width(bwMeterBG)-ix/rawMeterMultipliesBWMeter, y+al_get_bitmap_height(bwMeterBG)/2+((meterValues.at(ix) / 1000.0f) * (al_get_bitmap_height(bwMeterBG)/2.0f) ),
                             x+al_get_bitmap_width(bwMeterBG)-(ix/rawMeterMultipliesBWMeter+1), y+al_get_bitmap_height(bwMeterBG)/2+((meterValues.at(ix+1) / 1000.0f) * (al_get_bitmap_height(bwMeterBG)/2.0f) ),
                             al_map_rgba_f(1.0*bAlpha*rawAlpha, 1.0*bAlpha*rawAlpha, 1.0*bAlpha*rawAlpha, bAlpha*rawAlpha), 2.0f);
            }
     }
    }

    // Draw open eeg meter.
    if (EEGDeviceModeRadioButton.getSelectionTxt()==EEGDevicegOp2.getText() || EEGDeviceModeRadioButton.getSelectionTxt()==EEGDevicegOp3.getText()) {
        double chan2YAdd = al_get_bitmap_height(bwMeterBG)/2.0d;
        for (int ix = 0 ; ix < al_get_bitmap_width(bwMeterBG); ix++ )
        {
            // First channel of Open EEG:
            double valA = openEEGDevice.getRawChannelValueOfTimePoint(OPEN_EEG_RAW_CHAN1, double(ix)   / al_get_bitmap_width(bwMeterBG));
            double valB = openEEGDevice.getRawChannelValueOfTimePoint(OPEN_EEG_RAW_CHAN1, double(ix+1) / al_get_bitmap_width(bwMeterBG));
            al_draw_line(x+ix,   y+al_get_bitmap_height(bwMeterBG)/2+((valA) * ((double)al_get_bitmap_height(bwMeterBG)/2.0f) ),
                         x+ix+1, y+al_get_bitmap_height(bwMeterBG)/2+((valB) * ((double)al_get_bitmap_height(bwMeterBG)/2.0f) ),
                         al_map_rgba_f(1.0*bAlpha*rawAlpha, 1.0*bAlpha*rawAlpha, 1.0*bAlpha*rawAlpha, bAlpha*rawAlpha), 2.0f);

            // Second channel of Open EEG If chosen:
            if (EEGDeviceModeRadioButton.getSelectionTxt()==EEGDevicegOp3.getText()) {
                double valA = openEEGDevice.getRawChannelValueOfTimePoint(OPEN_EEG_RAW_CHAN2, double(ix)   / al_get_bitmap_width(bwMeterBG));
                double valB = openEEGDevice.getRawChannelValueOfTimePoint(OPEN_EEG_RAW_CHAN2, double(ix+1) / al_get_bitmap_width(bwMeterBG));
                al_draw_line(x+ix,   y+al_get_bitmap_height(bwMeterBG)/2+((valA) * ((double)al_get_bitmap_height(bwMeterBG)/2.0f) ) + chan2YAdd,
                             x+ix+1, y+al_get_bitmap_height(bwMeterBG)/2+((valB) * ((double)al_get_bitmap_height(bwMeterBG)/2.0f) ) + chan2YAdd,
                             al_map_rgba_f(1.0*bAlpha*rawAlpha, 1.0*bAlpha*rawAlpha, 1.0*bAlpha*rawAlpha, bAlpha*rawAlpha), 2.0f);
            }
        }
    }

    // Draw the current state IF available also.
    if (songPlaying || EEGDeviceIsWorking())
        drawRotatedTintedPicture(circleImg,getMapColor(getCurrentProfileMindState(PROFILE_CHANNEL_1),getCurrentProfileMindState(PROFILE_CHANNEL_2)),x+getCurrentProfileMindState(PROFILE_CHANNEL_1)*al_get_bitmap_width(bwMeterBG),y+(1.0f-getCurrentProfileMindState(PROFILE_CHANNEL_2))*al_get_bitmap_height(bwMeterBG),totalFrameTime);

    // Draw the mindwave on top also
    drawTintedPicture(mindWaveImg, al_map_rgba_f(1.0f*bAlpha, 1.0f*bAlpha, 1.0f*bAlpha, bAlpha),  x + al_get_bitmap_width(bwMeterBG) - al_get_bitmap_width(mindWaveImg)*0.75f , y-al_get_bitmap_height(mindWaveImg)*0.15f, 0);

    // Draw the power meter to show signal strength or condition..
    float powerMeterX = x + al_get_bitmap_width(bwMeterBG) + 20;
    float powerMeterY = y + al_get_bitmap_height(bwMeterBG)*0.95f;
    drawPowerMeter(powerMeterX, powerMeterY , al_get_bitmap_width(bwMeterBG)*0.3f, al_get_bitmap_height(bwMeterBG)/3,mindWaveDevice.getSignalStrength());

    float alpha = 0.7f;
    ALLEGRO_COLOR bFontColor  = al_map_rgba_f(0.9*alpha, 0.9*alpha, 1.0*alpha, alpha); // <-- correct
    al_draw_text(buttonFont, bFontColor, powerMeterX-70, powerMeterY + 5, NULL, "Signal Strength");

    GUIElement::draw();
}





