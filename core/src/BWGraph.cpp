#include "../include/BWGraph.h"
#include "../include/BWMeter.h"
#include "../include/PlayerCommon.h"

using namespace plrCommon;

CHANNEL_THEME getGraphTheme(int type)
{
    CHANNEL_THEME graphTheme;
    graphTheme.r = 0;
    graphTheme.b = 0;
    graphTheme.b = 0;
    graphTheme.text = "Template Graph";
    graphTheme.useForAvarageCalculation = false;
    graphTheme.showInBarPanel = false;
    graphTheme.useInRelativeCalculation = false;

    // Then override with real values..
    // The ranges has different sources and meanings. In this case they were assumed on previous experiment with neurofeedback.
    if (type==CHANNEL_DELTA)
    {
        graphTheme.r = 0;
        graphTheme.g = 30;
        graphTheme.b = 110;
        graphTheme.text = "Delta";
        graphTheme.bottomFreq = 0.0f;
        graphTheme.topFreq    = 4.5f;
        graphTheme.centerFreq = (graphTheme.topFreq + graphTheme.bottomFreq) / 2.0f;
        graphTheme.useForAvarageCalculation = true;
        graphTheme.showInBarPanel = true;
        graphTheme.useInRelativeCalculation = true;
    }
    if (type==CHANNEL_THETA)
    {
    /*    graphTheme.r = 255;
        graphTheme.g = 0;
        graphTheme.b = 130;*/
        graphTheme.r = 0;
        graphTheme.g = 255;
        graphTheme.b = 0;
        graphTheme.text = "Theta";
        graphTheme.bottomFreq = 4.5f;
        graphTheme.topFreq    = 8.0f;
        graphTheme.centerFreq = (graphTheme.topFreq + graphTheme.bottomFreq) / 2.0f;
        graphTheme.useForAvarageCalculation = true;
        graphTheme.showInBarPanel = true;
        graphTheme.useInRelativeCalculation = true;
    }
    if (type==CHANNEL_ALPHA)
    {
        graphTheme.r = 255;
        graphTheme.g = 255;
        graphTheme.b = 0;
        graphTheme.text = "Alpha";
        graphTheme.bottomFreq = 8.0f;
        graphTheme.topFreq    = 15.0f;
        graphTheme.centerFreq = (graphTheme.topFreq + graphTheme.bottomFreq) / 2.0f;
        graphTheme.useForAvarageCalculation = true;
        graphTheme.showInBarPanel = true;
        graphTheme.useInRelativeCalculation = true;
    }
    if (type==CHANNEL_BETA)
    {
        graphTheme.r = 255;
        graphTheme.g = 0;
        graphTheme.b = 0;
        graphTheme.text = "Beta";
        graphTheme.bottomFreq = 16.0f;
        graphTheme.topFreq    = 31.0f;
        graphTheme.centerFreq = (graphTheme.topFreq + graphTheme.bottomFreq) / 2.0f;
        graphTheme.useForAvarageCalculation = true;
        graphTheme.showInBarPanel = true;
        graphTheme.useInRelativeCalculation = true;
    }
    if (type==CHANNEL_GAMMA)
    {
        graphTheme.r = 255;
        graphTheme.g = 0;
        graphTheme.b = 255;
        graphTheme.text = "Gamma";
        graphTheme.bottomFreq = 32.0f;
        graphTheme.topFreq    = 44.0f;
        graphTheme.centerFreq = (graphTheme.topFreq + graphTheme.bottomFreq) / 2.0f;
        graphTheme.useForAvarageCalculation = true;
        graphTheme.showInBarPanel = true;
        graphTheme.useInRelativeCalculation = true;
    }
    if (type==CHANNEL_AVG)
    {
        // Varies.. Color varies according the dominating state which takes color from here up or down....
        graphTheme.r = 255;
        graphTheme.g = 255;
        graphTheme.b = 255;
        graphTheme.text = "Dominating";
        graphTheme.showInBarPanel = true;
    }
    if (type==CHANNEL_ATT)
    {
        graphTheme.r = 255;
        graphTheme.g = 70;
        graphTheme.b = 0;
        graphTheme.text = "Attention";
        graphTheme.showInBarPanel = true;
    }
    if (type==CHANNEL_MED)
    {
        graphTheme.r = 0;
        graphTheme.g = 60;
        graphTheme.b = 255;
        graphTheme.text = "Meditation";
        graphTheme.showInBarPanel = true;
    }
    if (type == CHANNEL_ALPHA1)
    {
        graphTheme.r = 120;
        graphTheme.g = 45;
        graphTheme.b = 16;
        graphTheme.bottomFreq = 8.0f;
        graphTheme.topFreq    = 11.0f;
        graphTheme.centerFreq = (graphTheme.topFreq + graphTheme.bottomFreq) / 2.0f;
        graphTheme.text = "Alpha 1";
        graphTheme.showInBarPanel = true;
        graphTheme.useInRelativeCalculation = true;
    }
    if (type == CHANNEL_ALPHA2)
    {
        graphTheme.r = 122;
        graphTheme.g = 90;
        graphTheme.b = 51;
        graphTheme.bottomFreq = 11.0f;
        graphTheme.topFreq    = 14.0f;
        graphTheme.centerFreq = (graphTheme.topFreq + graphTheme.bottomFreq) / 2.0f;
        graphTheme.text = "Alpha 2";
        graphTheme.showInBarPanel = true;
        graphTheme.useInRelativeCalculation = true;
    }
    if (type == CHANNEL_BETA1)
    {
        graphTheme.r = 90;
        graphTheme.bottomFreq = 14.0f;
        graphTheme.topFreq    = 25.0f;
        graphTheme.centerFreq = (graphTheme.topFreq + graphTheme.bottomFreq) / 2.0f;
        graphTheme.text = "Beta 1";
        graphTheme.showInBarPanel = true;
        graphTheme.useInRelativeCalculation = true;
    }
    if (type == CHANNEL_BETA2)
    {
        graphTheme.r = 75;
        graphTheme.bottomFreq = 25.0f;
        graphTheme.topFreq    = 35.0f;
        graphTheme.centerFreq = (graphTheme.topFreq + graphTheme.bottomFreq) / 2.0f;
        graphTheme.text = "Beta 2";
        graphTheme.showInBarPanel = true;
        graphTheme.useInRelativeCalculation = true;
    }
    if (type == CHANNEL_GAMMA1)
    {
        graphTheme.r = 42;
        graphTheme.g = 0;
        graphTheme.b = 42;
        graphTheme.bottomFreq = 32.0f;
        graphTheme.topFreq    = 38.0f;
        graphTheme.centerFreq = (graphTheme.topFreq + graphTheme.bottomFreq) / 2.0f;
        graphTheme.text = "Gamma 1";
        graphTheme.showInBarPanel = true;
        graphTheme.useInRelativeCalculation = true;
    }
    if (type == CHANNEL_GAMMA2)
    {
        graphTheme.r = 67;
        graphTheme.g = 0;
        graphTheme.b = 67;
        graphTheme.bottomFreq = 39.0f;
        graphTheme.topFreq    = 44.0f;
        graphTheme.centerFreq = (graphTheme.topFreq + graphTheme.bottomFreq) / 2.0f;
        graphTheme.text = "Gamma 2";
        graphTheme.showInBarPanel = true;
        graphTheme.useInRelativeCalculation = true;
    }
    if (type == CHANNEL_POWER_AVG)
    {
        graphTheme.r = 255;
        graphTheme.g = 255;
        graphTheme.b = 255;
        graphTheme.text = "Power Avarage";
        graphTheme.showInBarPanel = true;
    }

    // Adjust the graph theme color indexes from 0-255 to 0-1.0f->
    graphTheme.r = graphTheme.r/255.0f;
    graphTheme.g = graphTheme.g/255.0f;
    graphTheme.b = graphTheme.b/255.0f;

    return graphTheme;
}


ALLEGRO_COLOR getGraphThemeColorOf(int typeP) {
    float alphaq = 1.0f;
    return al_map_rgba_f (getGraphTheme(typeP).r*alphaq,getGraphTheme(typeP).g*alphaq,getGraphTheme(typeP).b*alphaq, alphaq);
}

bool useThisTypeInRelativeCalculations(int type) {
    return getGraphTheme(type).useInRelativeCalculation;
}

string getGraphDescriptionText(int typeP) {
    return getGraphTheme(typeP).text;
}
int getThisTypeCenterFreq(int type)
{
    return getGraphTheme(type).centerFreq;
}

bool useThisTypeForAvarageCalculation(int type)
{
    return getGraphTheme(type).useForAvarageCalculation;
}

// GLobal version of this to use in channelInfo
ALLEGRO_COLOR getGlobalColorOfFrequency(double value)
{
    for (int i=0; i<AMOUNT_OF_CHANNELS; i++)
    {
        if (getGraphTheme(i).useForAvarageCalculation)
            if (value>getGraphTheme(i).bottomFreq && value<=getGraphTheme(i).topFreq)
            {
                return al_map_rgba_f (getGraphTheme(i).r, getGraphTheme(i).g, getGraphTheme(i).b, 1.0f);
            }
    }
}

bool BWGraph::useThisForAvarageCalculation()
{
    return getGraphTheme(type).useForAvarageCalculation;
}

// Returns the current value weighted with the center frequency of this graph.. Used for avarage calculation.
double BWGraph::getCurrentWeightedValue()
{
    return getGraphTheme(type).centerFreq*getCurrentValue();
}


BWGraph::BWGraph()
{
    //ctor
    x = 0;
    y = 0;
}

BWGraph::~BWGraph()
{
    //dtor
}

void BWGraph::create(int x2, int y2, int type2, int secAmount)
{
    x    = x2;
    y    = y2;
    type = type2;

    theLatestScaledValue = 0;
    dontUseLastPositionThisTime=true;

    // Length in seconds..
    lengthInSecs = secAmount;

    // Initialize the data table size according seconds amount..
    rawValues.clear();
    for (int i=0; i<lengthInSecs+1; i++)
        rawValues.push_back(GRAPH_EMPTY_INDEX);

    // Let's calculate other neccessary values..
    w = (int)GRAPH_SIZE_X;
    h = (int)GRAPH_SIZE_Y;

    // Create picture to show the graph information..
    // For some reasong Allegro doesn't create the bitmap sometimes. Maybe problem with libraries that I have added or version or something..
    bool tryingToMakeBitmap=true;
    int trials=0;
    while (tryingToMakeBitmap) {
        graphPic = al_create_bitmap(w, h);
        trials++;
        if(!graphPic)
        {
            // Couldn't do it..
            playerLog("Couldn't create graph image! Trying again.. Damn Bug!");
        } else if (trials>10 || graphPic) {
            tryingToMakeBitmap=false;
        }
    }

    // Then finally let's calculate the scaling factory according to the type
    // because the values differ very much ie. delta is powerful beat and gamma is not so..
    alpha = 1.0f;

    // Copy the information from graph struct according the chosen channel..
    CHANNEL_THEME graphTheme = getGraphTheme(type);
    chosenColor = al_map_rgba_f (graphTheme.r*alpha,graphTheme.g*alpha,graphTheme.b*alpha, alpha);
    text = graphTheme.text;

    // Make zero line and clear it etc..
    clear();

}



void BWGraph::clear()
{
    makeZeroLine();
    //rawValues.clear();
}

// "Zeroline" Which means it's probably just -1 to show it's not recorded yet.
void BWGraph::makeZeroLine()
{
    rawValues.clear();
    // The empty index to in graph. This means this is not recorded point.
    for (int i=0; i<lengthInSecs+1; i++)
        rawValues.push_back(GRAPH_EMPTY_INDEX);

    // Draw also line to resemble zero reading in the picture of graph
    drawZeroLine();
}

void BWGraph::drawWholeGraph()
{
    al_set_target_bitmap(graphPic);
    for (int i=0; i<lengthInSecs; i++)
    {
        p2 = i - 1;
        if (p2<0) p2=0;

        value = (GRAPH_SIZE_Y/100.0f)*valueOf(i)/2.0f; // Asking this time point value of 0-100 scaled.. This should then fit it into the graph size doesn't it? ^__- Divided by two to match drawing both sides

        if (valueOf(i)!=GRAPH_EMPTY_INDEX)
        {
            // Draw the actual area.
            al_draw_filled_rectangle(((float)w/(float)lengthInSecs)*(float)(p2), h/2-value, ((float)w/(float)lengthInSecs)*(float)i, h/2+value, getCurrentDrawColor(valueOf(i)));
        }
        else
        {
            // Mark this as not recorded area. Draw there dotted line to show that next to graph line. .....
            if (i%2)
                al_draw_line( ((float)w/(float)lengthInSecs)*(float)(p2), h/2-1, ((float)w/(float)lengthInSecs)*(float)(p2), h/2+1, chosenColor, 1 );
        }
    }
    al_set_target_bitmap(tripleBuffer);
}

void BWGraph::drawZeroLine()
{
    //playerLog("Width: " + intToStr(w) + " Height: " + intToStr(h));
    if(!graphPic)
    {
        graphPic = al_create_bitmap(w, h);
    } else {

    }

    // Draw also line to resemble zero reading in the picture of graph
    al_set_target_bitmap(graphPic);

    alpha       = 1.0f;
    al_clear_to_color(al_map_rgba(0        ,         0,         0,    0));
    // This is the actual drawn line in the picture.
    al_draw_line( 0, h/2, w, h/2, chosenColor, 1 );
    al_set_target_bitmap(tripleBuffer);
}

// This will take out the value from given index
double BWGraph::valueOf(int gIndex)
{
    if (gIndex<rawValues.size()&&gIndex>=0)
        return rawValues[gIndex];
    else
    {
        return GRAPH_EMPTY_INDEX;
    }
}

void BWGraph::dontUseLastPosition()
{
    dontUseLastPositionThisTime = true;
}




ALLEGRO_COLOR BWGraph::getColorOfFrequency(double value)
{
    for (int i=0; i<AMOUNT_OF_CHANNELS; i++)
    {
        if (getGraphTheme(i).useForAvarageCalculation)
            if (value>getGraphTheme(i).bottomFreq && value<=getGraphTheme(i).topFreq)
            {
                return al_map_rgba_f (getGraphTheme(i).r, getGraphTheme(i).g, getGraphTheme(i).b, 1.0f);
            }
    }
}

ALLEGRO_COLOR BWGraph::getCurrentDrawColor(double value)
{
    if (type==CHANNEL_AVG)
    {
        // For avarage we check in which range this is and return the corresponding color..
        return getColorOfFrequency(value);
    }
    else
    {
        // Normal coloring
        float cAlpha = value/100.0f;
        float fableScaleLightSuperb=1.0f;
        float cR = getGraphTheme(type).r*(cAlpha*cAlpha*fableScaleLightSuperb);
        float cG = getGraphTheme(type).g*(cAlpha*cAlpha*fableScaleLightSuperb);
        float cB = getGraphTheme(type).b*(cAlpha*cAlpha*fableScaleLightSuperb);
        cR += cAlpha*cAlpha*0.25f;
        cG += cAlpha*cAlpha*0.25f;
        cB += cAlpha*cAlpha*0.25f;
        if (cR>1.0f) cR = 1.0f;
        if (cG>1.0f) cG = 1.0f;
        if (cB>1.0f) cB = 1.0f;

        float superAlpha = cAlpha*cAlpha*fableScaleLightSuperb;
        if (superAlpha>1.0f) superAlpha = 1.0f;

        ALLEGRO_COLOR color = al_map_rgba_f (cR,cG,cB, superAlpha);
        return color;
    }
}

void BWGraph::refresh()
{
    refresh(bwMeter.getCurrentEEGChannelSlowBarValue(type)); // Get the data from meter according this graph's id
}

void BWGraph::refresh(double newValue)
{
    //playerLog("New node added to graph!");

    theLatestScaledValue=newValue;

    if (dontUseLastPositionThisTime)
    {
        dontUseLastPositionThisTime = false;
        recordToTimePointLast       = songPositionInSec;
    }
    else
        recordToTimePointLast       = recordToTimePoint;

    recordToTimePoint               = songPositionInSec;

    // Do something only IF we'r surely inside the vector.. Songs are static length files so we don't have to worry about expanding it.
    if (recordToTimePoint<lengthInSecs)
    {

        // Everytime new value is brought from the device via bwmeter..?
        // If this point had been drawn once. Let's draw everything again to make sure it's correctly drawn..
        if ( rawValues.at(recordToTimePoint) != GRAPH_EMPTY_INDEX)
        {
            drawZeroLine();
            // Remove the old value offcourse
            rawValues[recordToTimePoint] = (newValue + rawValues[recordToTimePoint]) / 2.0f; // *Mean value
            drawWholeGraph();
        }
        else
        {
            // SO we'r drawing first time.. Anyway remove the old values which is 0
            rawValues[recordToTimePoint] = newValue;
        }

        // Check also that raw values is not overboard, or cut it fit.
        if (rawValues.size() > lengthInSecs + 1)
        {
            rawValues.erase (rawValues.begin()+lengthInSecs+1,rawValues.end());
        }

        // Update also the bitmap which shows the graph
        alpha = 1.0f;

        al_set_target_bitmap(graphPic);

        // This is the actual drawn line in the picture-> Check the scaling to make it look nice..
        value = (GRAPH_SIZE_Y/100.0f)*valueOf(recordToTimePoint)/2.0f; // Asking this time point value of 0-100 scaled.. This should then fit it into the graph size doesn't it? ^__- Divided by two to match drawing both sides

        //al_draw_filled_rectangle(((float)w/(float)lengthInSecs)*(float)(recordToTimePointLast), -h/2, ((float)w/(float)lengthInSecs)*(float)recordToTimePoint, h/2, getCurrentDrawColor(getScaledValue(recordToTimePoint)));
        al_draw_filled_rectangle(((float)w/(float)lengthInSecs)*(float)(recordToTimePointLast), h/2-value, ((float)w/(float)lengthInSecs)*(float)recordToTimePoint, h/2+value, getCurrentDrawColor(valueOf(recordToTimePoint)));

        al_set_target_bitmap(tripleBuffer);


    }

}

double BWGraph::getHzCenter()
{
    return getGraphTheme(type).centerFreq;
}

double BWGraph::getCurrentValue()
{

    // Everytime new value is brought from the device(0-1 sec?).. shoud be 1 sec?
    if (rawValues.size()>recordToTimePoint) // CAREFULL don't read empty array!
        return rawValues.at(recordToTimePoint);
    else
        return 0.0f;

}

// I just got pissed off with this shitbag and I try to make this output the latest value this way IF vector doesn't work out..
double BWGraph::getLatestUpdatedValue()
{
    return theLatestScaledValue;
}

double BWGraph::getValueOfCurrentSongsPlayedPos()
{
    return valueOf(songPositionInSec);
}

double BWGraph::getCurrentRawValue()
{
    // Everytime new value is brought from the device(0-2 sec?).. shoud be 1 sec?
    return rawValues.at(recordToTimePoint);
}

void BWGraph::draw(float vShade)
{

    // Draw the basic graph..
    if (graphPic && vShade>0.0f)
    {

        if (!showDividedBox.isChecked()) // If we want to draw the pure raws then let's draw
            drawTintedPicture(graphPic, al_map_rgba_f(vShade,vShade,vShade,vShade), x+bwManager.graphUIX, y+bwManager.graphUIY-h/2, 0);

        // Select the color also
        CHANNEL_THEME graphTheme = getGraphTheme(type);
        chosenColor = al_map_rgba_f (graphTheme.r*alpha,graphTheme.g*alpha,graphTheme.b*alpha, alpha);

        al_draw_text(buttonFont,al_map_rgba_f(chosenColor.r,chosenColor.g,chosenColor.b,vShade), x+bwManager.graphUIX+w-70, y+bwManager.graphUIY, 0, text.c_str());

        // Draw anyway always the cursor / song time position.
        al_draw_filled_rectangle(w*songScrollBar.getPosition()+x+bwManager.graphUIX, y-h/2+bwManager.graphUIY, w*songScrollBar.getPosition()+1+x+bwManager.graphUIX, y+h/2+bwManager.graphUIY, al_map_rgba_f (vShade, vShade, vShade, vShade) );
    }

}

