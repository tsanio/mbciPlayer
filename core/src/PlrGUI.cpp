#include "../include/PlrFunctionality.h"
#include "../include/PlrGUI.h"
#include "../include/PlayerCommon.h"

PlrGUI::PlrGUI()
{
    //ctor
}

PlrGUI::~PlrGUI()
{
    //dtor
}

using namespace boost::filesystem;
using namespace audiere;
using namespace std;

namespace plrCommon
{


void hideAllPanels()
{
    loopingCheckBox.hide();
    bwManager.hide();
    showDividedBox.hide();
    djModeRadioButton.hide();
    djPlayPartitions.hide();
    XYDJPicker.hide();
    djModeUseXAxis.hide();
    djModeUseYAxis.hide();
    ChangeToDJModeButton.hide();
    ChangeToRecordModeButton.hide();
    JumpDJButton.hide();

    visualizationParameterRadioButton.hide();
    showPeakPosBox.hide();
    brainWaveColoredInMenuBox.hide();
    recordBrainWaveBox.hide();
    fadingLengthRollButton.hide();
    djModeMinLengthOfPartRollButton.hide();
    djModeMaxLengthOfPartRollButton.hide();
    delayBeforePeakPosRollButton.hide();
    showVisualization.hide();
    showMirror.hide();
    bwTrashButton.hide();

    mindWavePortRollButton.hide();
    reBootMindWave.hide();
    EEGDeviceModeRadioButton.hide();
    OpenEEGPortRollButton.hide();
    reBootOpenEEG.hide();
    SpectrumPanel.hide();
    mindWaveScaleFactor.hide();

    panelText="";

}

void showPanel(int panelIndex)
{
    playMode = panelIndex;
    hideAllPanels();
    controlPanelMode = panelIndex;
    if (panelIndex == RECORD_PLAY_PANEL)
    {
        if (playModeClicked == RECORD_PLAY_PANEL)   // Note that we want to hide everything in this basic panel.. To have more view to the actual visualization
        {
            panelText = "Record Play Panel";
            if (!showVisualization.isChecked() ) {
                loopingCheckBox.show();
                bwTrashButton.show();
            }
            bwManager.show();
        } else
            ChangeToRecordModeButton.show();


    }
    if (panelIndex == DJ_PANEL)
    {
        if (playModeClicked == DJ_PANEL)
        {
            // if plapla !showVisualization.isChecked()
            panelText = "Disc Jockey Panel";
            XYDJPicker.show();
            djModeUseXAxis.show();
            djModeUseYAxis.show();
            bwManager.show();
            JumpDJButton.show();
        }
        else
            ChangeToDJModeButton.show();
    }
    if (panelIndex == SETUP_PANEL)
    {
        panelText = "Setup Panel";
        showDividedBox.show();
        showPeakPosBox.show();
        brainWaveColoredInMenuBox.show();
        recordBrainWaveBox.show();
        fadingLengthRollButton.show();
        djModeMinLengthOfPartRollButton.show();
        djModeMaxLengthOfPartRollButton.show();
        delayBeforePeakPosRollButton.show();
        showVisualization.show();
        showMirror.show();
        // DJmode also set here to be seen.
        djModeRadioButton.show();
        djPlayPartitions.show();
        visualizationParameterRadioButton.show();
    }
    if (panelIndex == SETUP_EEG_PANEL)
    {
        panelText = "Setup EEG Panel";
        mindWavePortRollButton.show();
        reBootMindWave.show();
        EEGDeviceModeRadioButton.show();
        OpenEEGPortRollButton.show();
        reBootOpenEEG.show();
        SpectrumPanel.show();
        mindWaveScaleFactor.show();
    }

}


void drawSongInfoText() {
    songInfoTextX += deltaGraphicsTime*50.0f;
    float songInfoTextBoxX = songScrollBar.getX();
    //float songInfoTextBoxY = songScrollBar.getY()-32;
    float songInfoTextBoxY = songScrollBar.getY()+32;
    float songInfoTextBoxWidth  = songScrollBar.getLength();
    float songInfoTextBoxHeight =  30;
    float letterWidth = 12;
    float alpha = 0.9f;
    al_set_clipping_rectangle(songInfoTextBoxX, songInfoTextBoxY, songInfoTextBoxWidth, songInfoTextBoxHeight); // Notice that this is made different later for that scrollbar
    ALLEGRO_COLOR color   = al_map_rgba_f(0.4*alpha, 0.7*alpha, 0.97*alpha, alpha); // <-- correct
    string songStatusText = "Playing " + forceRemoveAllDoubleEmptySpaces(playThis.getArtist()) + " - " + forceRemoveAllDoubleEmptySpaces(playThis.getAlbum()) + " - " + forceRemoveAllDoubleEmptySpaces(playThis.getTitle());
    if (songInfoTextX>songStatusText.length()*letterWidth)
        songInfoTextX = -songScrollBar.getLength();
    al_draw_text(libraryFont, color, songInfoTextBoxX-songInfoTextX, songInfoTextBoxY, ALLEGRO_ALIGN_LEFT, songStatusText.c_str());
    // Return clipping are to normal!
    al_set_clipping_rectangle(0, 0, al_get_display_width(display), al_get_display_height(display));
    al_draw_text(libraryFont, color, songInfoTextBoxX, songInfoTextBoxY-15, ALLEGRO_ALIGN_LEFT, playThis.getSongPositionStr(stream->getPosition()).c_str());
}

/************************
* UPDATE GRAPHICS / GUI *
*************************/
void updateMainGUI()
{
    /*
    * DRAWING
    */
    // Let's start drawing to the old flipped as our new double buffer :)
    al_clear_to_color(al_map_rgb(0,0,0));

    // Draw the background visualization if it's in background mode..
    backgroundVisualization.drawVisualization();

    // Draw background..
    //onTopVisualization = true;
    if (!showVisualization.isChecked())
        al_draw_bitmap(bgImg,0,0,0);

    // Draw base for graphs
    if (bwManager.isVisible() && !showVisualization.isChecked())
        drawScaledPicture(graphBaseImg,476,220,505,260); //al_draw_bitmap(graphBaseImg,476,180,0);

    if (libraryFont)
    {
        float alpha           = 0.5f+cos(totalFrameTime)*0.5f;
        ALLEGRO_COLOR color   = al_map_rgba_f(1.0*alpha, 0.4*alpha, 0.6*alpha, alpha); // <-- correct
        al_draw_textf(libraryFont, al_map_rgb(255, 255, 255),54, 8, ALLEGRO_ALIGN_CENTRE, "FPS: %d", fps);

        // Draw panel text
        if (!showVisualization.isChecked())
            al_draw_textf(libraryFont, al_map_rgb(255, 255, 255),474, 90, ALLEGRO_ALIGN_LEFT, panelText.c_str());

        // Update some special visibilities
        if ((showVisualization.isChecked() && playListPanel.mouseIsInsideScrollPanel()) || !showVisualization.isChecked())
            playListPanel.adjustVisibility(1);
        else
            playListPanel.adjustVisibility(-1);

        // Draw all basic GUI Elements
        elementList.draw();

        // Then draw some additional things that are otherwise hard to connect to guielements ie peaks..
        // The call should be made directly to the one which is in medialibrary so we will see the updates all the time
        //playThis.drawSongExtractedBwInformation();
        if (!playThis.isNull() && showPeakPosBox.isChecked())
            playThisPtr->drawSongExtractedBwInformation(); // It will be faded according bwmanager's shading indo

        if (!playThis.isNull()) {
            // Draw song text and song time text. Set clipping so that font doesn't go over board.
            drawSongInfoText();
        }

    }


    // Hey let's play and try to make the mirror thing :)
    // Draw mirror If desired so..:P
    if (showMirror.isChecked())
    {
        copyImg = al_create_sub_bitmap(tripleBuffer, 0, mirrorBottom-(al_get_display_height(display)-mirrorBottom), al_get_display_width(display), (al_get_display_height(display)-mirrorBottom));
        //al_draw_bitmap(copyImg,74,554,ALLEGRO_FLIP_VERTICAL);
        float mirrorAlpha = 0.4f;
        // Draw black before mirror..
        al_draw_filled_rectangle(0, mirrorBottom, al_get_display_width(display), al_get_display_height(display), al_map_rgb(0,0,0));
        //al_draw_tinted_bitmap(copyImg, al_map_rgba_f(0.0f*mirrorAlpha, 0.5f*mirrorAlpha, 1.0f*mirrorAlpha, mirrorAlpha), 74,554, ALLEGRO_FLIP_VERTICAL);
        al_draw_tinted_bitmap(copyImg, al_map_rgba_f(0.0f*mirrorAlpha, 0.5f*mirrorAlpha, 1.0f*mirrorAlpha, mirrorAlpha), 0, mirrorBottom, ALLEGRO_FLIP_VERTICAL);
    }

    al_set_target_bitmap(al_get_backbuffer(display));
    al_draw_bitmap(tripleBuffer,0, 0, 0);

    // Flip the backbuffer to display! :P
    al_flip_display();

    // Now how much  time did we spent drawing stuff?
    double t = al_get_time();

    deltaGraphicsTime = al_get_time()-lastGfxUpdate;

    //deltaTime       = t-lastUpdate;
    totalFrameTime += deltaTime;
    if (totalFrameTime>dmax) totalFrameTime = 0; // When maximum.. go to zero. This maybe problem in some special cases in long run but.. not important now

    fps_accum++;
    if (t - fps_time >= 1)
    {
        fps       = fps_accum;
        fps_accum = 0;
        fps_time  = t;
    }

    // And last merge the last gfx update
    lastGfxUpdate = t;

}




};
