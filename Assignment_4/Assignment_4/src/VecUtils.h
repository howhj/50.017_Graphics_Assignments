#ifndef VEC_UTILS_H
#define VEC_UTILS_H

#include "vecmath.h"

class VecUtils
{
public:

    static vec3 min( const vec3& b, const vec3& c )
    {
        vec3 out;

        for( int i = 0; i < 3; ++i )
        {
            out[ i ] = ( b[i] < c[i] ) ? b[i] : c[i];
        }

        return out;
    }

    static vec3 max( const vec3& b, const vec3& c )
    {
        vec3 out;

        for( int i = 0; i < 3; ++i )
        {
            out[ i ] = ( b[i] > c[i] ) ? b[i] : c[i];
        }

        return out;
    }

    static vec3 clamp( const vec3& data, float low = 0, float high = 1 )
    {
        vec3 out = data;
        for( int i = 0; i < 3; ++i )
        {
            if( out[ i ] < low )
            {
                out[ i ] = low;
            }
            if( out[ i ] > high )
            {
                out[ i ] = high;
            }
        }

        return out;
    }

    // transforms a 3D point using a matrix, returning a 3D point
    static vec3 transformPoint( const mat4& mat, const vec3& point )
    {
        return vec3( mat * vec4( point, 1 )[0], mat * vec4( point, 1 )[1], mat * vec4( point, 1 )[2] );
    }

    // transform a 3D directino using a matrix, returning a direction
    // This function *does not* take the inverse tranpose for you.
    static vec3 transformDirection( const mat4& mat, const vec3& dir )
    {
        return vec3( mat * vec4( dir, 0 )[0], mat * vec4( dir, 0 )[1], mat * vec4( dir, 0 )[2] );
    }
};

#endif // VEC_UTILS_H
