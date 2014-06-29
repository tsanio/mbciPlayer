#include <iostream>
#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <fftw3.h>
#include <vector>
#include "PlayerCommon.h"


#ifndef OPENEEGDEVICE_H
#define OPENEEGDEVICE_H

//! One channel of Open EEG spectrum. Uses same ranges as other graphs.
class EEGSpectrumChannel {
    public:
    double currentValue;
    double getCurrentValue() { return currentValue; }
};

using namespace std;

// Different Open EEG Channels. Olimex has only 2
enum OPEN_EEG_RAW_CHANNEL { OPEN_EEG_RAW_CHAN1,
                            OPEN_EEG_RAW_CHAN2,
                            OPEN_EEG_RAW_AMOUNT_OF_CHANNELS
                            }; // Notice that the last one tells the amount.

class OpenEEGDevice
{
    public:
        OpenEEGDevice();
        EEGSpectrumChannel specChan[OPEN_EEG_RAW_AMOUNT_OF_CHANNELS][AMOUNT_OF_CHANNELS];
        bool enabled[AMOUNT_OF_CHANNELS];
        void closeDevice();
        void init();
        void readPacketStream(char *readBuffer, int bytesRead);
        void addNewRawValueToChannel(int chanIndex, int chanhigh, int chanlow);
        double getSpectrumAmplitudeOf(int c, int i);
        void calculateSpectrum();
        bool isWorkingProperly() { return isWorking; }
        double getRawChannelValueOfTimePoint(int chanIndex, double requestTime);
        double getRawScaledValue(double val);
        double getRawChannelValue(int chanIndex);
        virtual ~OpenEEGDevice();
        double lastPacketsReceivedTime;
    protected:
    private:
        fftw_plan p;
        double maxAmplitude;
        double *in, *out;
        vector< vector<int> > rawValues;
        ALLEGRO_THREAD *thread;
        int rawSampleSize;
        int sampleRate;
        bool isWorking;
};

#endif // OPENEEGDEVICE_H
