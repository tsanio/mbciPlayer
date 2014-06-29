//#include "PlayerCommon.h"
#include "GUI/GUIElement.h"
#include "ImageLibrary.h"

#ifndef BWMETER_H
#define BWMETER_H

// These are used for getting values to basic graph
enum BW_CHANNELS {  CHANNEL_DELTA,
                       CHANNEL_THETA,
                       CHANNEL_ALPHA,
                       CHANNEL_ALPHA1,
                       CHANNEL_ALPHA2,
                       CHANNEL_BETA,
                       CHANNEL_BETA1,
                       CHANNEL_BETA2,
                       CHANNEL_GAMMA,
                       CHANNEL_GAMMA1,
                       CHANNEL_GAMMA2,
                       CHANNEL_AVG,
                       CHANNEL_ATT,
                       CHANNEL_MED,
                       CHANNEL_POWER_AVG,
                    AMOUNT_OF_CHANNELS
                  }; // Notice that the last one tells the amount of these things..



/**
 * \brief BWMeter.h ...:::Brainwave Meter As Virtual Input Device:::...
*
* This class is supposed to be a virtual offerer for all BCI data that is brought into the application. It acts as an 'interface'.
* In other words all BCI data is brought and processed first here. This can be safely used in every part of the application to gather
* realible data.
*
* author: tonis @ 2013
*
*/

class SlowBar {
   public:
    SlowBar();
    double getCurrentValue() {
        return currentValue;
    }
    void refresh(double goToV);
    void reset(int typeP);
    void reset();

private:
    int    type;
    double goToValue;
    double currentValue;
    double speed;
    double maxSpeed;
};



class BWMeter : public GUIElement
{
public:
    BWMeter();
    BWMeter(float px, float py, string toolTipText);
    void reset();
    void refresh();
    void draw();
    void init();
    bool EEGDeviceIsWorking();
    SlowBar channelSlowBar[AMOUNT_OF_CHANNELS];
    double  channelCurrentValue[AMOUNT_OF_CHANNELS];
    double getCurrentEEGChannelValue(int graphChannelId);
    void  updateChannelData(int graphId, double valToUpdate);
    double getCurrentProfileMindState(int profileChan);
    double getCurrentRelativeStateOfChannel(int chanIndxp);
    void updateProfileValues();
    bool isMindWaveActivated();

    double getCurrentEEGChannelSlowBarValue(int graphChannelId);

    int PROFILE_CHANNEL_1;
    int PROFILE_CHANNEL_2;
    double chan1v;
    double chan2v;

    virtual ~BWMeter();
protected:
private:
    float x,y;
    vector<double> meterValues;
    double rawMeterMultipliesBWMeter;
    double timeFromLastUpdate;
    double lastProfileUpdateTime;
    double lastBWUpdate;
    int    METER_SAMPLE_SIZE;
    ALLEGRO_BITMAP *bwMeterBG;
    ALLEGRO_BITMAP *circleImg;
    ALLEGRO_BITMAP *mindWaveImg;

        // <-
    // Profile of the guy for both channels. Used for nice purposes.
    float profChannel1Val;
    float profChannel2Val;
    float profChannel1ValSpeed;
    float profChannel2ValSpeed;
    float profChannelAcc;

};

#endif // BWMETER_H
