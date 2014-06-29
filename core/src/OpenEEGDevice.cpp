#include "../include/OpenEEGDevice.h"
#include "../include/Serialport.h"

using namespace plrCommon;

OpenEEGDevice::OpenEEGDevice()
{
    //ctor
    thread = NULL;
    init();
}

OpenEEGDevice::~OpenEEGDevice()
{
    //dtor
}

static void *Serial_Thread_Func(ALLEGRO_THREAD *thr, void *arg);

//! Function to read from serial in another thread so it doesn't affect other functionality.
static void *Serial_Thread_Func(ALLEGRO_THREAD *thr, void *arg){

    // Handle is given from the method that called this function.
    HANDLE *h  = (HANDLE*) arg;

    // Buffersize and the actual data buffer.
    int  bufferSize = 256*17;
    char readBuffer[bufferSize];

    // Thread main loop for reading data.
    while(!al_get_thread_should_stop(thr))
    {

        // Read Open EEG Data from serial. This should wait until it happens.
        int bytesRead = readFromSerialPort(h,readBuffer,bufferSize-1);

        // End the buffer with terminal?
        readBuffer[bytesRead]=0;

        // Let's read Open-eeg Olimex's EEG-SMT packet!
        openEEGDevice.readPacketStream(readBuffer, bytesRead);

        // Rest proper time. These packets seems to come about 1 seconds interval so this can be long time.
        al_rest(0.25);

    }

   closeSerialPort(h);
   return NULL;
}

//! Read readPacketStream that is given from serial reading thread. This supports now only Olimex EEG-SMT, but others could be also implemented.
void OpenEEGDevice::readPacketStream(char *readBuffer, int bytesRead)
{

    // Sign last packet received time.
    lastPacketsReceivedTime = al_get_time();

    // Checking success of the reading from serial.
    if (bytesRead>0) {

        isWorking = true;
        int found = 0;

        // Loop through all data.
        for (int i=0;i<bytesRead;i++) {

            // Find the starting point of each packet (90).
            if (int(readBuffer[i])==90 && i+15<=bytesRead) {

                // Single packet found. Check that it's not broken and it's correct point. 15th index should be 0-15 to indicate status.
                if (int(readBuffer[i+15]) >= 0 && int(readBuffer[i+15]) <= 15)
                {

                    addNewRawValueToChannel(OPEN_EEG_RAW_CHAN1, readBuffer[i + 3], readBuffer[i + 4]);
                    addNewRawValueToChannel(OPEN_EEG_RAW_CHAN2, readBuffer[i + 5], readBuffer[i + 6]);

                    // Increase founded packet counter.
                    found++;
                }
            }

        // Reading end.
        }

        // Calculate spectrum from new data.
        calculateSpectrum();

    // Problem reading packet or it was empty.
    } else {

        isWorking=false;

    }

}

void OpenEEGDevice::calculateSpectrum()
{
    // Loop through all channels and make the spectrums for each.
    for (int c=0;c<OPEN_EEG_RAW_AMOUNT_OF_CHANNELS;c++)
    {
        // Copy to input.
        for (int i=0;i<rawSampleSize;i++)
        {
            in[i] = rawValues[c][i];
        }

        // Calculate spectrum.
        fftw_execute(p);

        // Update the channel's spectrum values. Update only neccessary channels.
        for (int i=0;i<AMOUNT_OF_CHANNELS;i++)
            if (enabled[i]) {
                // Calculate avarage of all indexes in the requested range.
                int rangeCount=0;
                double valSum=0.0d;
                CHANNEL_THEME gTheme = getGraphTheme(i);
                for (int r=Freq2Index(gTheme.bottomFreq,sampleRate,rawSampleSize); r < Freq2Index(gTheme.topFreq,sampleRate,rawSampleSize); r++ ) {
                    rangeCount++;
                    double val = getPositiveOfThis(out[Freq2Index(r,sampleRate,rawSampleSize)]) / maxAmplitude;
                    if (val>1.0d)
                        val=1.0d;
                    valSum+=val;
                }
                specChan[c][i].currentValue = (valSum/rangeCount)*100.0d;
                // Let's limit it to 0-100.
                if (specChan[c][i].currentValue<0.0d)
                    specChan[c][i].currentValue = -specChan[c][i].currentValue;
                if (specChan[c][i].currentValue>100.0d)
                    specChan[c][i].currentValue = 100.0d;
            }

    }
}

double OpenEEGDevice::getSpectrumAmplitudeOf(int c, int i) {
    if (isWorkingProperly() && enabled[i]) {
        return specChan[c][i].getCurrentValue();
    }
}

void OpenEEGDevice::addNewRawValueToChannel(int chanIndex, int chanhigh, int chanlow)
{
    // Calculate each channel data from lowbit and highbit. It's a 16bit value divided to 8bits :)
    int chanval  = chanhigh * 256 + chanlow; // 256 x chan high bytes + chanlow bytes. Google If you are unsure about it.

    // Insert raw readings into the tables.
    rawValues[chanIndex].insert ( rawValues[chanIndex].begin() , chanval );

     // Keep the samples size tidy.
    if (rawValues[chanIndex].size()>rawSampleSize)
        rawValues[chanIndex].pop_back();

}

//! Give graph position Y of requested time. 0.0-1.0 second probably. Calculates between two points because the timepoint asked is usually between two asked values.
//! This is used for example to draw the graph in meter (the classical EEG-looking graph). It really gives just the raw values.
double OpenEEGDevice::getRawChannelValueOfTimePoint(int chanIndex, double requestTime) {

    if (requestTime<0.0d)
        requestTime = 0.0d;
    if (requestTime>1.0d)
        requestTime = 1.0d;

    // The readed values are given in 1 second delay. Calculate between the points.
    int positionOnRawTableA = floor(requestTime*((double)rawSampleSize));
    int positionOnRawTableB = floor(requestTime*((double)rawSampleSize))+1;

    if (positionOnRawTableB>rawSampleSize) {
        positionOnRawTableA --;
        positionOnRawTableB --;
    }

    double valA = getRawScaledValue(rawValues[chanIndex][positionOnRawTableA]);
    double valB = getRawScaledValue(rawValues[chanIndex][positionOnRawTableB]);

    double deltaVal = valB-valA;

    // Time to next index.
    double oneIndexTime = 1.0d/rawSampleSize;
    double lastIndexTime = positionOnRawTableA*oneIndexTime;
    double timeAfterPreviousIndex = requestTime - lastIndexTime;
    double closeToNextPoint = timeAfterPreviousIndex/oneIndexTime;

    double val = valA + closeToNextPoint * deltaVal;

    // Return the value.
    return val;
}

//! This determines how the raw value is scaled to -1.0d to 1.0d.
double OpenEEGDevice::getRawScaledValue(double val) {

    // The estimated amplitude range.
    double maxAmplitude = 768.0d;

    // Scaled.
    val = val/maxAmplitude;

    // Let's turn it upwards to look better as Y-coordinate goes downwards here otherwise.
    val = -val;

    if (val<-1.0d)
            val=-1.0d;
    if (val>1.0d)
            val=1.0d;

    return val;
}

double OpenEEGDevice::getRawChannelValue(int chanIndex) {
    return getRawChannelValueOfTimePoint(chanIndex, al_get_time()-lastPacketsReceivedTime);
}

void OpenEEGDevice::init()
{

    // Set intial values.
    isWorking               = false;
    rawSampleSize           = 256;
    sampleRate              = 256;
    lastPacketsReceivedTime = 0;
    maxAmplitude            = 20000;

    // Which channels are used by this EEG-Device?
    for (int i=0;i<AMOUNT_OF_CHANNELS;i++)
        enabled[i]=false;

    enabled[CHANNEL_DELTA]  = true;
    enabled[CHANNEL_THETA]  = true;
    enabled[CHANNEL_ALPHA]  = true;
    enabled[CHANNEL_BETA]   = true;
    enabled[CHANNEL_GAMMA]  = true;
    enabled[CHANNEL_ALPHA1] = true;
    enabled[CHANNEL_ALPHA2] = true;
    enabled[CHANNEL_BETA1]  = true;
    enabled[CHANNEL_BETA2]  = true;
    enabled[CHANNEL_GAMMA1] = true;
    enabled[CHANNEL_GAMMA2] = true;


    // Allocate fftw arrays.
    in   = fftw_alloc_real(sizeof(double) * rawSampleSize);
    out  = fftw_alloc_real(sizeof(double) * rawSampleSize);

    // Create plan for fftw already for calculating spectrum.
    p    = fftw_plan_r2r_1d(rawSampleSize, in, out, FFTW_R2HC, FFTW_ESTIMATE);

    // Init spectrum values.
    for (int c = 0; c < OPEN_EEG_RAW_AMOUNT_OF_CHANNELS; c++)
    {
        for (int i = 0; i < AMOUNT_OF_CHANNELS; i++)
        {
            specChan[c][i].currentValue = 0;
        }
    }

    // Expand the raw channel sizes first.
    rawValues.resize(OPEN_EEG_RAW_AMOUNT_OF_CHANNELS);
    for(int i = 0 ; i < OPEN_EEG_RAW_AMOUNT_OF_CHANNELS ; ++i)
    {
    	//Grow Columns to fit sampleSize.
    	rawValues[i].resize(rawSampleSize);
    }

    /* MAKE DEBUG SINE LINES TO CHECK IF FFTW WORKS PROPERLY ETC.
    rawValues[0].clear();
    rawValues[1].clear();
    for(int i = 0 ; i < rawSampleSize ; i++)
    {
    	//Grow Columns to fit sampleSize.
    	rawValues[0].push_back ( cos(double(i)/rawSampleSize*3.14d*2.0d*10.0d)*500.0d );
    	rawValues[1].push_back ( cos(double(i)/rawSampleSize*3.14d*2.0d*40.0d)*500.0d );
    }*/

    if (thread!=NULL)
        al_destroy_thread(thread);

    // Open COMPort and thread.
    int openEEGPort = int(OpenEEGPortRollButton.getConvertedValue());
    HANDLE h = openSerialPort(("COM"+intToStr(openEEGPort)).c_str(),B57600,one,off); // Comports with a bigger number (I believe bigger than 9) can be used by with their UNC Paths \\\\.\\COM11 (thanks to jaui)
    // If serial port stuff failed in intialization, make output in log but don't crash app.
    if (h == INVALID_HANDLE_VALUE) {
        playerLog("OpenEEG wasn't found on port " + intToStr(openEEGPort));
    } else {
        thread   = al_create_thread(Serial_Thread_Func, h);
        al_start_thread(thread);
    }

}

void OpenEEGDevice::closeDevice()
{
    al_destroy_thread(thread);
}
