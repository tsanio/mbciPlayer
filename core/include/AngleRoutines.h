#ifndef ANGLEROUTINES_H
#define ANGLEROUTINES_H

#include <math.h>
/**
 * \brief Angleroutines.h ...::: Angle Routines - Routines to handle angle calculations. :::...
*
* This class is used for all different kinds of angle calculations ie. distances between angles or convert from rad to deg or get random angle.
*
* author: tonis @ 2013
*
*/
class AngleRoutines
{
public:
    AngleRoutines();
    virtual ~AngleRoutines();
protected:
private:
};

extern const double pi; // far from accurate value
extern double DegToRad(double deg);
extern double RadToDeg(double rad);
extern double getRandomRad();
extern double distanceBetweenPoints(double x, double y, double x2, double y2);
extern double angleBetween(double x, double y, double x2, double y2);
extern double angleBetweenInDegrees(double x, double y, double x2, double y2);
extern double getRandomZeroToOne();
extern int distAngleToAngle(int angle1, int angle2);
extern int shortestWayTo(int angle1, int angle2);

#endif // ANGLEROUTINES_H
