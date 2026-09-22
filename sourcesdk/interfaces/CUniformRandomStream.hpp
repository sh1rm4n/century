//
// Created by sh1rm4n on 6/26/26.
//

#ifndef CENTURY_CUNIFORMRANDOMSTREAM_HPP
#define CENTURY_CUNIFORMRANDOMSTREAM_HPP

class CUniformRandomStream {
public:
    virtual void	SetSeed( int iSeed );
    virtual float	RandomFloat( float flMinVal = 0.0f, float flMaxVal = 1.0f );
    virtual int		RandomInt( int iMinVal, int iMaxVal );
    virtual float	RandomFloatExp( float flMinVal = 0.0f, float flMaxVal = 1.0f, float flExponent = 1.0f );
};

#endif //CENTURY_CUNIFORMRANDOMSTREAM_HPP
