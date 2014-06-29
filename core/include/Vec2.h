#ifndef __VEC2__
#define __VEC2__

#include <cmath>
//! A 2d-vector class. Not my handcraft! -Toni
//! USed for BgEffect's particle to make particles have speed and position prolly.
template<typename Val> class Vec2
{
    Val mx,my;
public:
    Vec2(Val xx = Val(), Val yy = Val())
        : mx(xx), my(yy)
    { }

    /*     Vec2(const Vec2& v)
     *         : mx(v.mx), my(v.my)
     *     { }
     *
     *     Vec2& operator=(Vec2 v)
     *     {
     *         mx = v.mx;
     *         my = v.my;
     *         return *this;
     *     }
     */

    void setPolar(Val radians, Val distance)
    {
        mx = distance*cos(radians);
        my = distance*sin(radians);
    }

    Val x() const
    {
        return mx;
    }
    Val y() const
    {
        return my;
    }

    Val radians() const
    {
        if(mx == my == 0.0)
            return 0;
        return atan2(my, mx);
    }


    Val magnitudeSquared() const
    {
        return mx*mx+my*my;
    }

    Val magnitude() const
    {
        return sqrt(magnitudeSquared());
    }

    Vec2& normalize()
    {
        Val m = magnitude();
        if(m != 0.0)
        {
            mx /= m;
            my /= m;
        }
        return *this;
    }

    Vec2& operator+=(const Vec2& rhs)
    {
        mx += rhs.mx;
        my += rhs.my;
        return *this;
    }

    Vec2& operator-=(const Vec2& rhs)
    {
        mx -= rhs.mx;
        my -= rhs.my;
        return *this;
    }

    Vec2& operator*=(const Vec2& rhs)
    {
        mx *= rhs;
        my *= rhs;
        return *this;
    }

    Vec2& operator/=(const Vec2& rhs)
    {
        mx /= rhs;
        my /= rhs;
        return *this;
    }

    //Vector2DAddition / Subtraction

    Vec2 operator+ (const Vec2 &rhs) const
    {
        /*
        * PlayerCommon.h
        *
        * ...::: Player Commons - All is here :::...
        *
        * This class holds the variable definitions of globally used classes such as BWManager, BWMeter, musicLibrary and GUIElements.
        *
        * author: tonis @ 2013
        *
        */
        return Vec2(mx + rhs.mx, my + rhs.my);
    }

    Vec2 operator- (const Vec2 &rhs) const
    {
        return Vec2(mx - rhs.mx, my - rhs.my);
    }

    Vec2 operator* (Val rhs) const
    {
        return Vec2(mx * rhs, my * rhs);
    }

    Vec2 operator/ (Val rhs) const
    {
        return Vec2(mx / rhs, my / rhs);
    }

    //Vector2DDot product
    Val dotproduct (const Vec2 &rhs)
    {
        return mx*rhs.mx + my*rhs.my;
    }

    //Vector2DSign operators

    Vec2 operator- () const
    {
        return Vec2(-mx, -my);
    }

    Vec2 operator+ () const
    {
        return Vec2(+mx, +my);
    }

    //Vector2DComparison

    bool operator== (const Vec2 &rhs) const
    {
        return (mx == rhs.mx) && (my == rhs.my);
    }

    bool operator!= (const Vec2 &rhs) const
    {
        return !(*this == rhs);
    }
};

//Vector2DScalar product
template<typename Val>
inline Vec2<Val> operator*(Val lhs, Vec2<Val> rhs)
{
    return rhs * lhs;
}

//Vector2DAbsolute value / magnitude

template<typename Val>
inline Val abs(const Vec2<Val> &v)
{
    return v.magnitude();
}


//A couple of handy typedefs

typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;

typedef Vec2<int> Vec2i;

#endif
