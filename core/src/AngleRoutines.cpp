#include <stdlib.h>
#include <time.h>
#include "../include/AngleRoutines.h"

using namespace std;

AngleRoutines::AngleRoutines()
{
    //ctor
}

AngleRoutines::~AngleRoutines()
{
    //dtor
}

const double pi = 3.14159d; // far from accurate value (3.141) updated

double DegToRad(double deg)
{
    return (deg/180.0d) * pi;
}

double RadToDeg(double rad)
{
    return (rad*180.0d) / pi;
}

double getRandomZeroToOne()
{
    return (double)rand()/(double)RAND_MAX;
}

double getRandomRad()
{
    return getRandomZeroToOne()*2.0d*pi;
}

double distanceBetweenPoints(double x, double y, double x2, double y2)
{
    double deltaX=0;
    double deltaY=0;
    // Delta values are neede for calculation.
    if (x > x2) deltaX = x - x2;
    if (x2 > x) deltaX = x2 - x;
    if (y > y2) deltaY = y - y2;
    if (y2 > y) deltaY = y2 - y;
    // These are used to calculate distance.
    if (deltaX == 0 && deltaY == 0)
        return 0;
    else
        return sqrt( deltaX*deltaX+deltaY*deltaY );
}

double angleBetween(double x, double y, double x2, double y2)
{
    // Why?
    if (x2-x != 0)
        x += 1;

    double angle = atan2(y2 - y, x2 - x) ;

    return angle;
}

double angleBetweenInDegrees(double x, double y, double x2, double y2)
{
    return RadToDeg(angleBetween(x,y,x2,y2));
}

int distAngleToAngle(int angle1, int angle2)
{
    // Temporary solution.. I don't know am I using this here anywhere?
    angle1 += 180;
    angle2 += 180;

    int result;
    result = 0;

    if (angle1 > angle2)
    {
        if ( (angle1 - angle2) > (360-angle1+angle2) )
            result = (360-angle1+angle2); // Shortest angle is the wanted angle.
        else
            result = (angle1 - angle2);
    }

    if (angle2 > angle1)
    {
        if ( (angle2 - angle1) > (360-angle2+angle1) )
            result = (360-angle2+angle1); // Shortest angle is the wanted angle.
        else
            result = (angle2 - angle1);
    }

    return result;

}

int shortestWayTo(int angle1, int angle2)
{
    // Temporary.
    angle1 += 180;
    angle2 += 180;

    int result;
    result = 0;

    if (angle1 > angle2)
    {
        if ( (angle1 - angle2) > (360-angle1+angle2) )
            result = 1; // Shortest is the wanted.
        else
            result = -1;
    }

    if (angle2 > angle1)
    {
        if ( (angle2 - angle1) > (360-angle2+angle1) )
            result = -1;
        else
            result = 1;
    }

    return result;
}
