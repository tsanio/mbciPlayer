#include <string>
#include <allegro5/allegro.h>
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"
#include "../include/ImageLibrary.h"
#include "../include/PlayerCommon.h"

using namespace std;
using namespace plrCommon;

ImageLibrary::ImageLibrary()
{
    //ctor
}

ImageLibrary::~ImageLibrary()
{
    //dtor
}

// Load image to library..
ALLEGRO_BITMAP* ImageLibrary::loadImage(string path)
{
    ALLEGRO_BITMAP* img = NULL;
    img = al_load_bitmap(path.c_str());
    if(!img)
    {
        playerLog("Failed to load image: "+path+" \n");
    }
    bitmap.push_back(img);
    return img;
}

// Create just blank image..
ALLEGRO_BITMAP* ImageLibrary::createImage(int width, int height)
{
    ALLEGRO_BITMAP* img = NULL;
    img = al_create_bitmap(width,height);
    if(!img)
    {
        playerLog("Failed to create image in image library createImage()-function.. \n");
    }
    bitmap.push_back(img);
    return img;
}



void ImageLibrary::destroyImages()
{
    for(size_t i = 0; i < bitmap.size(); ++i)
    {
        if (bitmap[i])
        {
            al_destroy_bitmap(bitmap[i]);
            bitmap[i] = NULL;
        }
        /*if (!bitmap[i])
            playerLog("destroyed properly " + intToStr(i));
        else
            playerLog("width of img was is(this should be impossible) " + intToStr(al_get_bitmap_width(bitmap[i])));*/
    }
    bitmap.clear();
}
