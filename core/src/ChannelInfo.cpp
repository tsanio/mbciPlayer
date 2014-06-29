/*
* ...:::ChannelInfo:::...
* The extracted and lightweight infoscreen of individual graph.
* This holds the avarage data of each channel and it's not so heavy to hold in memory even for longer songs.
* Also the idea is to hold the peak-information here which is used to know the different positions to play in
* DJ mode.. Ie to play peak modes of attentive music :)
*
* All data should come more straight here.. The purpose..
*
*/

#include "../include/PlayerCommon.h"
#include "../include/ChannelInfo.h"

using namespace plrCommon;

ChannelInfo::ChannelInfo()
{
    //ctor
    reset();
}

ChannelInfo::~ChannelInfo()
{
    //dtor
}

// Reset method
void ChannelInfo::reset()
{

    //ctor
    avarage.clear();
    peakPos.clear();
    peakPosValue.clear();
    avarageValue = 0;
    recordedPercentage = 0;
    motherGraphType = CHANNEL_BETA; // Check this. I couldn't dare to put this in the arcive yet.. It's okay this way for now also. Next release put it in arcive also.
    x = 1;
    y = 1;
    h = 1;
    w = 1;
    lengthInSecs = 1;
    thisAddedValue=0;
}

/*
* getRndPeakPosInSecs
*:::::::::::::
*
* Returns a random peak position as seconds during the song's timeline. Ie this can be used to get a peak position of attention, theta, meditation or any channel.
*
*/
int ChannelInfo::getRndPeakPosInSecs()
{
    if (peakPos.size()>0)
    {
        srand (time(NULL));
        return peakPos[rand() % peakPos.size()]; // 0 - (peakpossize -1) .. legit? should be :]
    }
    return 0; // Okay this is not good.. No actual answer from peak pos so we give 0 back..
}

/*
* createFromGraph(BWGraph graph, int channelIndexP)
*:::::::::::::
*
* Calculates the peak positions and avarage value data of this particular given graph. Requested when creating song bwinfo. Actually updated almost each frame for one song's information?:)
*
*/
void ChannelInfo::createFromGraph(BWGraph graph, int channelIndexP)
{

    // This is not neccessary and it's a bit cruel.. I still try to reset everytinh while I'm hunting bug here..
    reset();

    // Save the given graph to make the minimap and get all data and channelIndex for later usage.
    // graph = graphP; // This is always just for runtime stuff.. So we can access to any graph in runtime but they are not saved in the indexfile to save diskspace.. hMm O_o Why not save all bw data to the mlibrary then?
    channelIndex = channelIndexP;

    // Grab and initialize stuff for drawing etc.
    x = graph.x;
    y = graph.y;
    h = graph.h;
    w = graph.w;
    motherGraphType = graph.type;
    lengthInSecs = graph.lengthInSecs;

    // Division of things.. This is smooth the graphs. ie 10 seconds blocks.
    divisionInSecs = SMOOTH_GRAPH_DIVISION_IN_SECS;

    // Calculating the avarage values based on the division amount.. ie avarage values from each 5 seconds..
    calculateSmoothAvarageMiniGraph(graph);

    // Find the peakpositions and use the just created minigraph for that purpose.
    findPeakPositions();

    // Calculate avarage value of this channel..
    calculateAvarageValue();

}

// Notice this is not scaled or anything man..
float ChannelInfo::getSmoothValueFromTimePos(int timeP)
{
    if (int(timeP/divisionInSecs)<avarage.size())
    {
        return avarage[int(timeP/divisionInSecs)];
    }
}

/*
* calculateSmoothAvarageMiniGraph()
*:::::::::::::
*
* Calculates the little graph of the bigger graph.
*
*/
void ChannelInfo::calculateSmoothAvarageMiniGraph(BWGraph graph)
{

    avarage.clear();
    int countUs=0;
    int countForRecorded=0;
    for (int i=0; i<graph.lengthInSecs; i++)
    {
        if (graph.rawValues.size()>=i)   // Copy only If there exists raw values in that range :)
        {
            // Save also the percentage of recorded data here.
            if (graph.rawValues[i]!=GRAPH_EMPTY_INDEX)
                countForRecorded++;

            thisAddedValue += graph.rawValues[i];
            countUs++;
            if (countUs>=divisionInSecs)
            {
                if (countUs==0) countUs=1; // Avoid div. by .zero
                //if (thisAddedValue<0) thisAddedValue=0;  LET IT BE -1? Then it will be discarded later.
                avarage.push_back(thisAddedValue/countUs);
                thisAddedValue=0;
                countUs=0;
            }
        }
    }

    // Did we miss the last one? If there exists still counted number's let's make that as the last thing there
    if (countUs>0)
        avarage.push_back(thisAddedValue/countUs);

    if (graph.rawValues.size()>0)
        recordedPercentage = float(countForRecorded/float(graph.rawValues.size()));
    else
        recordedPercentage = 0.0f;

    //playerLog("Calculated percentage " + desToStr(recordedPercentage) );

}


/*
* findPeakPositions()
*:::::::::::::
*
* Find the peakpositions of current graph using the generated mini graph. So it uses avarage values to find a bit bigger peaks etc not just
* single big peaks but rather vast areas of profound values.
*
*/
void ChannelInfo::findPeakPositions()
{

    // Then what happens?.. We need to know the peak positions If there's any ..
    // A maximum happens when the previous area was lower than this one and the next is also lower than this..
    // But PEAK in my book happens when the the previous is just lower or reaches the highest place on that rising curve..
    peakPos.clear();
    peakPosIndex.clear();
    peakPosValue.clear();

    // Find three highest tops If there exists such things. The first is out of calculations?
    float minimumHigh  = 3; // Keep it small. Sometimes channels have very small amplitude :P
    float lastHighest  = minimumHigh;
    int lastHighestPos = 0;
    bool found = false;
    int amountOfTopsWanted = AMOUNT_OF_PEAKS_IN_CHANNEL;
    for (int q=0; q<amountOfTopsWanted; q++)
    {
        found = false;
        lastHighest = minimumHigh;

        for (int i=1; i<avarage.size(); i++)
        {
            // For initial we will try the cases that are just higher than previous and see what happens :)
            if (avarage[i-1]  > lastHighest)
            {
                if (avarage[i-1]  > avarage[i])   // Rule nro 2.. This is not final solution. But This is Life I will try this. The idea is that if next is lower then this was peak  and is acceptable:)
                {
                    if (find(peakPosIndex.begin(), peakPosIndex.end(), i-1) == peakPosIndex.end())   // Is there such element in vector as i or do we reach end?
                    {
                        // Ok no then we can add it! Otherwise just skip it as it was propably chosen al ready previous round
                        lastHighest     = avarage[i-1];
                        lastHighestPos  = i-1;
                        found           = true;
                    }
                }
            }
        }
        // After searching and propably finding the most highest.. Grab it! And look for another one :)
        if (found)
        {
            peakPos.push_back(int(lastHighestPos*divisionInSecs+(divisionInSecs/2.0f))); // Save the exact second position right away..
            peakPosValue.push_back(avarage[lastHighestPos]); // Save the exact second position right away..
            peakPosIndex.push_back(lastHighestPos);
            //playerLog("Found peak in " + intToStr(channelIndex));
        }
    }
}

/*
* calculateAvarageValue()
*:::::::::::::
*
* Calculate the avarage value of this channel.
*
*/
void ChannelInfo::calculateAvarageValue()
{
    // Calculate the total avarage also..
    // We need to separate -1 (GRAPH_EMPTY_INDEX) away because otherwise we would have a lot of false information.
    // The problem comes also with this smoothing. So it's not so accurate. Some unrelevant data can slip into the system.
    // This should be taken into account before -> ie using the last found value instead of GRAPH_EMPTY_INDEX.
    thisAddedValue = 0;
    int divider = 0;
    for (int i=0; i<avarage.size(); i++)
    {
        if (avarage[i]!=GRAPH_EMPTY_INDEX)
        {
            thisAddedValue += avarage[i];
            divider++;
        }
    }

    // Division by zero? BE carefull dude :)
    // This nice value tells the total avarage value.. Very useful..
    if (divider != 0)
        avarageValue = thisAddedValue/divider;
    else
        avarageValue = 0;
}

/*
* draw()
*:::::::::::::
*
* Draws everything.. All extracted graphs. It's a bit similar as drawing the graphs. For some complicated reasons it's done here where the channel info is also.
*
*/
void ChannelInfo::draw()
{
    // This is frozen for a moment.
    //if (thisIsVisibleChannel && (controlPanelMode == RECORD_PLAY_PANEL || controlPanelMode == DJ_PANEL))
    if (y>0 && y<bwManager.graphHeight) {
        if ( (controlPanelMode == RECORD_PLAY_PANEL && playModeClicked == RECORD_PLAY_PANEL) || (controlPanelMode == DJ_PANEL && playModeClicked == DJ_PANEL) )
        {
            // Peak position drwaing
            for (int i=0; i<peakPos.size(); i++)
            {
                if (peakPos[i])
                {
                    float alpha = 0.8f*bwManagerFade;
                    al_draw_filled_rectangle(((float)w/(float)lengthInSecs)*(float)(peakPos[i]-1)+x+bwManager.graphUIX, y-h/2+bwManager.graphUIY, ((float)w/(float)lengthInSecs)*(float)(peakPos[i]+1)+x+bwManager.graphUIX, y+h/2+bwManager.graphUIY, al_map_rgba_f  (0.0*alpha, 0.5*alpha, 1.0*alpha, alpha));
                }
            }
            // The avarage valuegraph drawing
            if (showDividedBox.isChecked())
            {
                for (int i=0; i<avarage.size(); i++)
                {
                    float alpha = 0.8f;
                    float posX2 = (i+1)*divisionInSecs;
                    if (posX2>lengthInSecs) posX2 = lengthInSecs;
                    float value = ((h/100.0f)*(avarage[i]))/2.0f;
                    if (value>h/2)
                        value = h/2;
                    if (value > 0)
                    {
                        if (channelIndex==CHANNEL_AVG)
                            al_draw_filled_rectangle(((float)w/(float)lengthInSecs)*(float)(i*divisionInSecs)+x+bwManager.graphUIX, y-value+bwManager.graphUIY, ((float)w/(float)lengthInSecs)*(float)(posX2)+x+bwManager.graphUIX, y+value+bwManager.graphUIY, getGlobalColorOfFrequency(avarage[i]));
                        else
                            al_draw_filled_rectangle(((float)w/(float)lengthInSecs)*(float)(i*divisionInSecs)+x+bwManager.graphUIX, y-value+bwManager.graphUIY, ((float)w/(float)lengthInSecs)*(float)(posX2)+x+bwManager.graphUIX, y+value+bwManager.graphUIY, al_map_rgba_f  (getGraphTheme(channelIndex).r*alpha, getGraphTheme(channelIndex).g*alpha, getGraphTheme(channelIndex).b*alpha, alpha));
                    }
                    else     // If zero value.. Show just very thin line..
                    {
                        alpha = 0.3f;
                        al_draw_filled_rectangle(((float)w/(float)lengthInSecs)*(float)(i*divisionInSecs)+x+bwManager.graphUIX, y-1+bwManager.graphUIY, ((float)w/(float)lengthInSecs)*(float)(posX2)+x+bwManager.graphUIX, y+bwManager.graphUIY, al_map_rgba_f  (getGraphTheme(channelIndex).r*alpha, getGraphTheme(channelIndex).g*alpha, getGraphTheme(channelIndex).b*alpha, alpha)  );

                    }
                }
            }
        }
    }
}
