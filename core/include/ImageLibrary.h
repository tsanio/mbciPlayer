#include <string>
#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"
#ifndef IMAGELIBRARY_H
#define IMAGELIBRARY_H

using namespace std;

/**
 * \brief ImageLibrary.h ...::: Library of images :::...
 *
 * This handles the image requests and saves them as library so they can be managed easily later(ie. destroyed and re-created).
 * + Easier to debug.
 *
 * author: tonis @ 2013
 *
 */
class ImageLibrary
{
public:
    ImageLibrary();
    virtual ~ImageLibrary();
    ALLEGRO_BITMAP *loadImage(string folder);
    ALLEGRO_BITMAP *createImage(int width, int height);
    void destroyImages();
protected:
private:
    vector <ALLEGRO_BITMAP*>bitmap;
};

#endif // IMAGELIBRARY_H
