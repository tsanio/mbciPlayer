//#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <math.h>
#include <limits>
#include <string>

#include <audiere.h>

#include <boost/filesystem.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"

#include "include/PlayerCommon.h"

#include "include/GUI/GUIMouse.h"
#include "include/GUI/GUIScrollPanel.h"
#include "include/GUI/GUIButton.h"
#include "include/GUI/GUICheckBox.h"
#include "include/GUI/GUIScroller.h"
#include "include/GUI/GUIRadioButton.h"
#include "include/GUI/GUIRollButton.h"

#include "include/BWManager.h"
#include "include/BWMeter.h"
#include "include/BgEffect.h"

#include "include/PlrFunctionality.h"
#include "include/PlrGUI.h"

using namespace audiere;
using namespace boost::filesystem;
using namespace std;
using namespace plrCommon;


int main(int argc, char **argv)
{

    // Initializing different things for start.
    pauseButton.hideAndUnactivate();
    songPlaying = false;

    // Do we have audiodevice ready also?
    if (!device)
    {
        playerLog("FAILED TO LOAD AUDIOSYSTEM !");// failure
    }

    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *queue;
    bool redraw = true;

    srand(time(NULL));

    /* Init Allegro addons. */
    al_init();
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_mouse();
    al_install_keyboard();
    al_init_font_addon();
    al_init_ttf_addon();

    // For playing stream starting up.. starting up..
    initStreamStartUpValues();

    if(!al_init())
    {
        playerLog("failed to initialize allegro!\n");
        return -1;
    }

    if(!al_init_image_addon())
    {
        playerLog("Failed to initialize al_init_image_addon!\n");
        return -1;
    }

    // Frameless but it's not working in this version!.. I should try later in new allegro?
    al_set_new_display_flags(ALLEGRO_NOFRAME);

    if (fullScreen)
    {
        al_set_new_display_flags(ALLEGRO_FULLSCREEN);
        display = al_create_display(1024, 768);
        mirrorBottom = 570;
    }
    else
    {
        al_set_new_display_flags(ALLEGRO_WINDOWED);
        display = al_create_display(1000, 600);
        mirrorBottom = 565;
    }

    if(!display)
    {
        playerLog("failed to create display!\n");
        return -1;
    }

    // Make the program to run in the background also..
    //set_display_switch_mode(SWITCH_BACKGROUND);

    // Asettaa ikonin, titlen jne ikkunaan
    iconImg = imageLibrary.loadImage("media/gfx/programWindowIcon.png");
    al_set_display_icon(display,iconImg);

    al_set_window_title(display, "BCI Music Player..");
    //al_set_window_position(display,0, 0);


    // Here we load all fonts for player like button fonts, normal fonts, tooltip whatever fonts to avoid instantiating them multiple times..
    if (!loadFonts())
        return -1;

    timer = al_create_timer(1.0f / 150.0f); // This is the FPS .. 1 / FPS -> ie 120 means 1 / 120 so it's 120 frames per second.
    queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    // *** Adding all checkboxes into radiobuttons ***
    // These has to be added here..
    djModeRadioButton.addOption(&djModeOp1);
    djModeRadioButton.addOption(&djModeOp2);
    djModeRadioButton.setCheckedIndex(0);

    //playerLog("djModeRadiobutton text " + djModeRadioButton.getSelectionTxt());

    djPlayPartitions.addOption(&partOp1);
    djPlayPartitions.addOption(&partOp2);
    djPlayPartitions.setCheckedIndex(0);

    // Add these checkboses inside radio button for later usage.
    visualizationParameterRadioButton.addOption(&actOp1);
    visualizationParameterRadioButton.addOption(&actOp2);
    visualizationParameterRadioButton.setCheckedIndex(0);

    // Add buttons for EEG Device choise:
    EEGDeviceModeRadioButton.addOption(&EEGDevicegOp1);
    EEGDeviceModeRadioButton.addOption(&EEGDevicegOp2);
    EEGDeviceModeRadioButton.addOption(&EEGDevicegOp3);
    EEGDeviceModeRadioButton.setCheckedIndex(2);

    // Initialize components. Fonts, images etc that needs allegro..
    elementList.initElements();

    // Load old configurations.
    config.load();

    playerLog("Initializing complete..\n");

    // Show elements info just for fun .. before it was to see z-ordering. :)
    elementList.showElementsInfo();

    // Choose the initial panel to show..
    showPanel(RECORD_PLAY_PANEL);

    // Triplebuffering. This should be changed later If optimization is desired!
   // tripleBuffer = imageLibrary.createImage(al_get_display_width(display), al_get_display_height(display));
    tripleBuffer = al_create_bitmap(al_get_display_width(display), al_get_display_height(display));

    // Background image.
    bgImg = imageLibrary.loadImage("media/gfx/playerBg1.png");

    // Load graphic base image.
    graphBaseImg = imageLibrary.loadImage("media/gfx/graphBase.png");

    // Triple buffer as target.
    al_set_target_bitmap(tripleBuffer);

    // Final thing! Let's open the medialibrary already! :)
    loadMusicLibrary();

    // Finally load devices.
    openEEGDevice.init();
    mindWaveDevice.init();

    /* * * * * * * * * *
    * Program Main Loop *
    * * * * * * * * * */
    while (1)
    {

        al_set_target_bitmap(tripleBuffer);

        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);
        //al_get_next_event(queue, &event);

        guiMouse.mouseUpdate(&event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;
        if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                break;
        }

        // Allegro functions.

        // Draw again If desired fps time reached.
        if (event.type == ALLEGRO_EVENT_TIMER && al_get_time()-lastGfxUpdate>gfxDesiredFps) {
            redraw = true;
        }

        if (event.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
        {
            al_acknowledge_resize(display);
            redraw = true;
        }

        // Special thing for this dragging
        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            if (event.mouse.button == 1 && event.mouse.x)
            {
                down = true;
                down_x = event.mouse.x;
                down_y = event.mouse.y;
            }
        }
        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        {
            if (event.mouse.button == 1)
                down = false;
        }
        else if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            if (down && down_y < 30)
            {
                int cx, cy;
                if (al_get_mouse_cursor_position(&cx, &cy))
                {
                    al_set_window_position(display, cx - down_x, cy - down_y);
                }
            }
        }
        // <- Dragging of window..

        // Update All Components And Element States
        elementList.refresh();

        // NORMAL KEYS -->
        // Handle most of the events and actions user may do with the editor..
        if (playButton.wasClicked() || (playListPanel.mouseIsInsideScrollPanelTextPart() && guiMouse.doubleClicked()))
        {
            if (!musicLibrary.isEmpty())
                playSongButtonPressed();
        }
        if (playListPanel.newSelectionWasMade()) {
            printSelectedSongProfileInfo();
        }
        else if (pauseButton.wasClicked())
        {
            pauseSelectedSong();
        }

        if (songFolderOpenButton.wasClicked())
        {
            musicLibrary.openSongFilesImportDialog(BROWSE_FOLDER);
            // After all is imported. Let's see IF we have old bw info for imported songs so it will be up to date immediately.
            refreshBWPreInformations();
            refreshPlayList();
            // Save the list at this point to add new added stuff right away.
            saveMusicLibrary();
        }

        if (songFileOpenButton.wasClicked())
        {
            musicLibrary.openSongFilesImportDialog(BROWSE_MULTIPLE_FILES);
            // After all is imported. Let's see IF we have old bw info for imported songs so it will be up to date immediately.
            refreshBWPreInformations();
            refreshPlayList();
            // Save the list at this point to add new added stuff right away.
            saveMusicLibrary();
        }

        if (songTrashButton.wasClicked())
        {
            removeSelectedSong();
        }

        if (libraryNewButton.wasClicked())
        {
            musicLibrary.reset();
            refreshPlayList();
        }
        if (bwTrashButton.wasClicked()) {
            if (!playThis.isNull()) {
                bwManager.initClearTable(playThis.getId(),playThis.getLengthInSecs());
                saveBWTable();
                playerLog("New clean brainwave created and saved.");
            }
        }
        // Mode-Buttons for changing controlPanelMode
        if (RecordModeButton.wasClicked())
        {
            showPanel(RECORD_PLAY_PANEL);
        }
        if (DJModeButton.wasClicked())
        {
            showPanel(DJ_PANEL);
        }
        if (ConfigureButton.wasClicked())
        {
            showPanel(SETUP_PANEL);
        }
        if (ConfigureEEGButton.wasClicked())
        {
            showPanel(SETUP_EEG_PANEL);
        }

        if (ChangeToRecordModeButton.wasClicked())
        {
            playModeClicked = RECORD_PLAY_PANEL;
            showPanel(RECORD_PLAY_PANEL);
        }
        if (ChangeToDJModeButton.wasClicked())
        {
            playModeClicked = DJ_PANEL;
            showPanel(DJ_PANEL);
        }
        if (JumpDJButton.wasClicked())
        {
            playSongButtonPressed();
        }

        // Reboot mindwave button
        if (reBootMindWave.wasClicked())
            mindWaveDevice.forceLoadThinkGearConnection();

        if (reBootOpenEEG.wasClicked())
            openEEGDevice.init();


        // Update playlist outfit If the coloring thing was changed.
        if (brainWaveColoredInMenuBox.wasClicked())
            refreshPlayList();

        // Check that if min or max buttons are changed, it may affect the other one!
        if (djModeMinLengthOfPartRollButton.valueWasChanged())
            if (djModeMinLengthOfPartRollButton.getValue()>djModeMaxLengthOfPartRollButton.getValue())
                djModeMaxLengthOfPartRollButton.limitUpper(djModeMinLengthOfPartRollButton.getConvertedValue());
        if (djModeMaxLengthOfPartRollButton.valueWasChanged())
            if (djModeMaxLengthOfPartRollButton.getValue()<djModeMinLengthOfPartRollButton.getValue())
                djModeMinLengthOfPartRollButton.limitDowner(djModeMaxLengthOfPartRollButton.getConvertedValue());

        if (exitButton.wasClicked())
            break;

        // Handle some options.. One of these must be selected always!
        if (!djModeUseXAxis.isChecked() && !djModeUseYAxis.isChecked())
        {
            if (djModeUseXAxis.wasClicked())
                djModeUseYAxis.checkMe();
            else
                djModeUseXAxis.checkMe();
        }

        // Resizing from the resize button or from F5
        if (resizeButton.wasClicked())
        {

            if (fullScreen)
            {
                al_set_new_display_flags(ALLEGRO_WINDOWED);
                fullScreen = false;
            }
            else
            {
                al_set_new_display_flags(ALLEGRO_FULLSCREEN);
                fullScreen = true;
            }

            unLoadFonts();
            imageLibrary.destroyImages();
            elementList.unLoadElements();
            al_set_target_bitmap(NULL);
            al_destroy_display(display);

            if (fullScreen)
                display = al_create_display(1024, 768);
            else
                display = al_create_display(1000, 600);

            if(!display)
            {
                playerLog("failed to create display!\n");
                return -1;
            }
            loadFonts();

            tripleBuffer = al_create_bitmap(al_get_display_width(display), al_get_display_height(display));
            bgImg = imageLibrary.loadImage("media/gfx/playerBg1.png");
            graphBaseImg = imageLibrary.loadImage("media/gfx/graphBase.png");
            elementList.initElements();


        }


        // Changed volume? Update :)!
        if (guiVolumeRollButton.valueWasChanged() && stream)
        {
            stream->setVolume(guiVolumeRollButton.getValue()); // Set volume according rolled value
        }

        // Update song status If there's some new changes ie in looping mode or whatever..
        updateSongStatus();

        // Update Stream Updaters
        updateStreamUpdaters();

        // Update autosaving for configurations.
        config.updateRunTimeTaskes();

        // Update music library while playing or recording new stuff also.. For example the CES-information is updated there. I don't even remember what the hell is that :D Oh yea.. It's the compressed information of brainwaves for indexing etc..
        if (songPlaying)
            musicLibrary.refresh();

        // Self explanatory.
        if (libraryOpenButton.wasClicked())
        {
            loadMusicLibrary();
        }

        // Save library.
        if (librarySaveButton.wasClicked())
        {
            saveMusicLibrary();
        }

        // Are we playing? Show the time bar :)
        if (songPlaying)
        {
            // Did we set position or should we scroll the bar to it's right place according the song?
            // Songscrollbar is the bar showing the played duration of the current song:
            if (songScrollBar.positionChanged())
            {
                stream->setPosition(int(songScrollBar.getPosition()*float(stream->getLength())));
                bwManager.positionChanged(); // Tell to start editinr from different point and have a new "last ppoint"
            }
            else
            {
                if (stream->isPlaying())
                    if (stream->isSeekable())
                        songScrollBar.setPosition(float(stream->getPosition())/float(stream->getLength()));
            }

        }

        // Update delta time.. According the main update time, not the graphical update time.
        if (lastUpdate>0) {
            deltaTime = al_get_time()-lastUpdate;
            //playerLog("deltaTime " + desToStr(deltaTime));
        }
        else
            deltaTime = 0;

        // The main update time
        lastUpdate = al_get_time();

        /************************
        * UPDATE GRAPHICS / GUI *
        *************************/
        //if (redraw && al_is_event_queue_empty(queue) && !drawOnlySongNumber)   // Normal update
        if (redraw && al_is_event_queue_empty(queue))   // Normal update
        {
            updateMainGUI();
            redraw = false; // Wait for next graphic update..
        }


    }

    // Okay quitting ! Pause selected song!
    pauseSelectedSong();

    // Save Brainwavetable before quitting..
    saveBWTable();

    // Save configurations.
    config.save();

    // Save also the whole library and extracted information about brainwaves..
    playerLog("Going to.. save .. library..");
    saveMusicLibrary();
    playerLog("Saved!");

    // Destroy and unload all gui elements
    elementList.unLoadElements();
    //mindWaveDevice.closeDevice();
    openEEGDevice.closeDevice();
    al_destroy_display(display);
    imageLibrary.destroyImages();
    al_destroy_bitmap(copyImg);

    // I'm done! ..
    return 0;
}
