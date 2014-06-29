#include "../include/MindWaveDevice.h"
//#include "../include/thinkgear.h"

using namespace plrCommon;

MindWaveDevice::MindWaveDevice()
{
    //ctor
    init();
}

MindWaveDevice::~MindWaveDevice()
{
    //dtor
}


void MindWaveDevice::forceLoadThinkGearConnection() {
    TG_FreeConnection(connectionId);
    connectionId = -1;
    mindWaveReady = false;
    loadThinkGearConnection();
}

void MindWaveDevice::init() {
    // Set the scaling attributes. And reset last values to 0.
    for (int i=0;i<TG_AMOUNT_OF_CHANNELS;i++) {
        NeuroSkyScaleFactor[i]=1.0d;
        lastValue[i]=0.0d;
    }

    // Scaling to make it usable.
    NeuroSkyScaleFactor[TG_TMP_DELTA]  = 0.00001d;
    NeuroSkyScaleFactor[TG_TMP_THETA]  = 0.0001d;
    NeuroSkyScaleFactor[TG_TMP_ALPHA1] = 0.001d;
    NeuroSkyScaleFactor[TG_TMP_ALPHA2] = 0.001d;
    NeuroSkyScaleFactor[TG_TMP_BETA1]  = 0.001d;
    NeuroSkyScaleFactor[TG_TMP_BETA2]  = 0.001d;
    NeuroSkyScaleFactor[TG_TMP_GAMMA1] = 0.001d;
    NeuroSkyScaleFactor[TG_TMP_GAMMA2] = 0.001d;

    // The addresses of these values.
    TGRequest[TG_TMP_BATTERY] = TG_DATA_BATTERY;
    TGRequest[TG_TMP_POOR_SIGNAL] = TG_DATA_POOR_SIGNAL;
    TGRequest[TG_TMP_ATTENTION] = TG_DATA_ATTENTION;
    TGRequest[TG_TMP_MEDITATION] = TG_DATA_MEDITATION;
    TGRequest[TG_TMP_RAW] = TG_DATA_RAW;
    TGRequest[TG_TMP_DELTA] = TG_DATA_DELTA;
    TGRequest[TG_TMP_THETA] = TG_DATA_THETA;
    TGRequest[TG_TMP_ALPHA1] = TG_DATA_ALPHA1;
    TGRequest[TG_TMP_ALPHA2] = TG_DATA_ALPHA2;
    TGRequest[TG_TMP_BETA1] = TG_DATA_BETA1;
    TGRequest[TG_TMP_BETA2] = TG_DATA_BETA2;
    TGRequest[TG_TMP_GAMMA1] = TG_DATA_GAMMA1;
    TGRequest[TG_TMP_GAMMA2] = TG_DATA_GAMMA2;
    TGRequest[TG_TMP_BLINK_STRENGTH] = TG_DATA_BLINK_STRENGTH;

    connectionId  = -1;
    mindWaveReady = false;

    // Open connection.
    loadThinkGearConnection();
}

void MindWaveDevice::readPackets() {
    if (connectionId!=-1) {
        mindWaveReady = true;
    }

    int   packetsRead  = 0;
    /* Read all currently available Packets, one at a time... I hope CPU is fast enough */
    do {

        /* Read a single Packet from the connection */
        packetsRead = TG_ReadPackets( connectionId, 1 );
        // USe -1 instead of 1 ^ to read all packets at once IF desired.

        /* If TG_ReadPackets() was able to read a Packet of data... */
        if( packetsRead == 1 ) {

            /* If the Packet containted a new raw wave value... */
            if( TG_GetValueStatus(connectionId, TG_DATA_RAW) != 0 ) {

                lastValue[TG_TMP_RAW] = TG_GetValue(connectionId, TG_DATA_RAW);

            } else {

                for (int i=0;i<TG_AMOUNT_OF_CHANNELS;i++) {
                    // Read only values IF there is something new to add..
                    if( TG_GetValueStatus(connectionId, TGRequest[i]) != 0 )
                        lastValue[i] = getTG_Value(TGRequest[i]); // NOTICE: The requested is sometimes different than original index. ie blinking is different.
                }

            }

        } /* end "If TG_ReadPackets() was able to read a Packet..." */

    } while( packetsRead > 0 ); /* Keep looping until all Packets read */

}

// Return channel's newest value. Use this outside of this class. This should be modified when moving to another project.
double MindWaveDevice::getValueOf(int i) {
    // Modified to meet my requirements here. This is to make sure these are done correctly!
    if (i == CHANNEL_DELTA) {
       return lastValue[TG_TMP_DELTA];
    }
    if (i == CHANNEL_THETA) {
        return lastValue[TG_TMP_THETA];
    }
    if (i == CHANNEL_ALPHA) {
        return (lastValue[TG_TMP_ALPHA1]+lastValue[TG_TMP_ALPHA2]) / 2.0d;
    }
    if (i == CHANNEL_ALPHA1) {
        return lastValue[TG_TMP_ALPHA1];
    }
    if (i == CHANNEL_ALPHA2) {
        return lastValue[TG_TMP_ALPHA2];
    }
    if (i == CHANNEL_BETA) {
        return (lastValue[TG_TMP_BETA1]+lastValue[TG_TMP_BETA2]) / 2.0d; // Mix
    }
    if (i == CHANNEL_BETA1) {
       return lastValue[TG_TMP_BETA1];
    }
    if (i == CHANNEL_BETA2) {
       return lastValue[TG_TMP_BETA2];
    }
    if (i == CHANNEL_GAMMA) {
        return (lastValue[TG_TMP_GAMMA1]+lastValue[TG_TMP_GAMMA2]) / 2.0d; // Mix
    }
    if (i == CHANNEL_GAMMA1) {
        return lastValue[TG_TMP_GAMMA1];
    }
    if (i == CHANNEL_GAMMA2) {
        return lastValue[TG_TMP_GAMMA2];
    }
    if (i == CHANNEL_ATT) {
        return lastValue[TG_TMP_ATTENTION];
    }
    if (i == CHANNEL_MED) {
        return lastValue[TG_TMP_MEDITATION];
    }
    return 0; // latestValue[i] check that the i correspons the TG_TMP_value of asked value..

}

// This will bring value from device and invert negative to positive etc.
double MindWaveDevice::getTG_Value(int i) {
    double val = 0.0d;
    if (NeuroSkyScaleFactor[i] != 1.0d)
        val = TG_GetValue(connectionId, i) * NeuroSkyScaleFactor[i] * mindWaveScaleFactor.getConvertedValue();
    else
        val = TG_GetValue(connectionId, i);

    if (val<0)
        val = -val;
    if (val>100.0d)
        val = 100.0d;
    return val;
}


float MindWaveDevice::getSignalStrength() {
    if (!mindWaveReady) // No connection at all yet..
        return 0.0f;
    if (poorSignalValue==0) // VERY GOOD!
        return 1.0f;
    if (poorSignalValue>0 && poorSignalValue<150) // Lousy..
        return 0.5f;
    if (poorSignalValue>150) // Shiddy..
        return 0.0f;
}

/*
* Starting thinkgear etc.. This can be called just to make sure it's connected and If NOT Then it tries to coonnect!
*/
void MindWaveDevice::loadThinkGearConnection() {

    if (connectionId != 0) { // If zero, we have not connected yet! Doo it! :)
         char *comPortName   = NULL;
         char comPortNameBuf[1024];
         int  dllVersion     = 0;
         int  attPacketsRead = 0;
         int errCode;
         errCode             = 0;
         connectionId        = -1;


        /* Print driver version number */
        dllVersion = TG_GetDriverVersion();
        playerLog("ThinkGear DLL version: "+dllVersion);

        /* Get a connection ID handle to ThinkGear */
        connectionId = TG_GetNewConnectionId();

        if( connectionId < 0 ) {
            playerLog("ERROR: TG_GetNewConnectionId() returned "+connectionId);
        }
        /* Set/open stream (raw bytes) log file for connection */
        errCode = TG_SetStreamLog( connectionId, "streamLog.txt" );
        if( errCode < 0 ) {
            playerLog("ERROR: TG_SetStreamLog() returned "+errCode);
        }

        /* Set/open data (ThinkGear values) log file for connection */
        errCode = TG_SetDataLog( connectionId, "dataLog.txt" );
        if( errCode < 0 ) {
            playerLog("ERROR: TG_SetDataLog() returned "+errCode);
       }

        // Let's loop the ports to find the correct
        int errCodeResult=0;
        int startingPort = int(mindWavePortRollButton.getConvertedValue());
        playerLog("Starting from port " + intToStr(startingPort));
        //int PORTS=int(mindWavePortRollButton.getMax() - mindWavePortRollButton.getMin())-startingPort;
        int PORTS=int(mindWavePortRollButton.getMax() - mindWavePortRollButton.getMin());
        int portResult=0;
        /* Attempt to connect the connection ID handle to serial port "COM5" */
        /*comPortName = "\\\\.\\COM19";
        errCode = TG_Connect( connectionId,
                              comPortName,
                              TG_BAUD_9600,
                              TG_STREAM_PACKETS );
        if( errCode < 0 ) {
            //fprintf( stderr, "ERROR: TG_Connect() returned %d.\n", errCode );
            //wait();
            //exit( EXIT_FAILURE );
            playerLog("MindWave message to you.. F U C K  Y O U I'm not co-operating hahaa :D!");
        }*/
        //playerLog("max " + intToStr(mindWavePortRollButton.getMax()));
        for (int q=startingPort;q<PORTS;q++) {
           //playerLog("Trying port " + intToStr(q));
           sprintf(comPortNameBuf, "\\\\.\\COM%d", q);
           /*errCode = TG_Connect( connectionId,
                              comPortNameBuf,  THIS WAS OLD STUFF:: This is why this device didn't get my best grading at all........  .\___/. WHY? WHY There was this code in the neurosky's example code?! WHY!?
                              TG_BAUD_9600,
                              TG_STREAM_PACKETS );*/
            errCode = TG_Connect( connectionId,
                              comPortNameBuf,
                              TG_BAUD_57600,
                              TG_STREAM_PACKETS );
           if( errCode >= 0 ) {
                   // Right one! Quit the loop!
                   portResult=q;
                   q=PORTS+1;
                   errCodeResult = errCode;
           }

        }

        // Did we found device?
        if( errCodeResult < 0 ) {
             playerLog("ERROR: TG_Connect() returned "+intToStr(errCode));
        } else {
             playerLog("TGConnection eshtablished on COM-port "+intToStr(portResult));
             // Set also the roller to that point. So it's already pointing the new port :)?
             playerLog("TGConnection connectionId is "+intToStr(connectionId));
        }

    } else { // Need for new connection?
        playerLog("TGConnection already.. No need for new connection..");
    }
}

void MindWaveDevice::closeDevice() {
    TG_FreeConnection( connectionId );
}
