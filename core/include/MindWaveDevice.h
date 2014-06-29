#include "PlayerCommon.h"

#ifndef MINDWAVEDEVICE_H
#define MINDWAVEDEVICE_H

#include "thinkgear.h"

enum TG_CHANNELS {  TG_TMP_BATTERY,
                    TG_TMP_POOR_SIGNAL,
                    TG_TMP_ATTENTION,
                    TG_TMP_MEDITATION,
                    TG_TMP_RAW,
                    TG_TMP_DELTA,
                    TG_TMP_THETA,
                    TG_TMP_ALPHA1,
                    TG_TMP_ALPHA2,
                    TG_TMP_BETA1,
                    TG_TMP_BETA2,
                    TG_TMP_GAMMA1,
                    TG_TMP_GAMMA2,
                    TG_TMP_BLINK_STRENGTH,
                    TG_AMOUNT_OF_CHANNELS
                 }; // Notice that the last one tells the amount of these things..

using namespace std;

class MindWaveDevice
{
    public:
    protected:
    private:
    public:
        MindWaveDevice();
        virtual ~MindWaveDevice();
        void closeDevice();
        void init();
        void readPackets();
        double getRawValue() { return lastValue[TG_TMP_RAW]; }
        bool isWorkingProperly() { return mindWaveReady; }
        double getValueOf(int i);
        float getSignalStrength();
        void forceLoadThinkGearConnection();
        void loadThinkGearConnection();
    protected:
    private:
        double bringValueOfTGChannel(int i);
        double getTG_Value(int i);
        double NeuroSkyScaleFactor[AMOUNT_OF_CHANNELS];
        double TGRequest[AMOUNT_OF_CHANNELS];
        double lastValue[AMOUNT_OF_CHANNELS];
        // Thinkgear connection id. 0 If not initialized yet.
        int connectionId;
        bool mindWaveReady;
        int errCode;
        float poorSignalValue;

};

#endif // MINDWAVEDEVICE_H
