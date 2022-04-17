/**
 * Created on April 15, 2022 by Ariadne Lewis-Towbes
 * TODO:
 * - add propert cxx20 support
 * - add concepts <3
 * See redsp/LICENSE for license information.
*/
#ifndef REDSP_MATH_HEADERGUARD
#define REDSP_MATH_HEADERGUARD

#include <type_traits>
#include <cmath>
#include "universal.h"

#ifdef redsp_cxx20
#include <concepts>
#endif

// add pi, sin, etc impls
namespace redsp
{

struct math
{
    //================================================================================================================//
    //==                                                                                                            ==//
    //==                                                   UNARY                                                    ==//
    //==                                                                                                            ==//
    //================================================================================================================//

    //! Returns sin(x) using stl implementation
    template <redsp_arithmetic T>
    static T sin(T x)
    {
        redsp_arithmetic_assert(T)
        return sin(x);
    }

    //! returns an approximation of sin(x), diverging outside ().
    template <redsp_arithmetic T>
    static T sin_fast(T x)
    {
        redsp_arithmetic_assert(T)
//        return ;
    }

    //! returns cos(x) using stl implementation
    template <redsp_arithmetic T>
    static T cos(T x)
    {
        redsp_arithmetic_assert(T)
        return static_cast<T>(cos(x));
    }

    //! returns an approximation of cos(x), diverging outside ().
    template <redsp_arithmetic T>
    static T cos_fast(T x)
    {
        redsp_arithmetic_assert(T)
        // return ;
    }

    //! returns tan(x) using stl implementation
    template <redsp_arithmetic T>
    static T tan(T x)
    {
        redsp_arithmetic_assert(T)
        return static_cast<T>(tan(x));
    }

    //! returns an approximation of tan(x), diverging outside ()
    template <redsp_arithmetic T>
    static T tan_fast(T x)
    {
        redsp_arithmetic_assert(T)
        return static_cast<T>(tanh(x));
    }

    //! returns tanh(x) using stl implementation
    template <redsp_arithmetic T>
    static T tanh(T x)
    {
        redsp_arithmetic_assert(T)
        return static_cast<T>(tanh(x));
    }

    //! returns an approximation of tanh(x), diverging outside
    template <redsp_arithmetic T>
    static T tanh_fast(T x)
    {
        redsp_arithmetic_assert(T)
        return static_cast<T>(tanh(x));
    }


    template <redsp_arithmetic T>
    static T log_fast(T x)
    {
        return 0; // only use specializations!
    }

    //================================================================================================================//
    //==                                                                                                            ==//
    //==                                                   OTHER                                                    ==//
    //==                                                                                                            ==//
    //================================================================================================================//

    template <redsp_arithmetic T>
    static T min(T x, T y) { return x < y ? x : y; }

    template <redsp_arithmetic T>
    static T max(T x, T y) { return x > y ? x : y; }

    template <redsp_arithmetic T>
    static T clip(T low, T high, T x) { return max(low, min(x, high)); }

    template <redsp_arithmetic T, redsp_arithmetic T2>
    static bool within(T x, T y, T2 lim) { return abs(x - y) < lim; }

    //================================================================================================================//
    //==                                                                                                            ==//
    //==                                                 CONSTANTS                                                  ==//
    //==                                                                                                            ==//
    //================================================================================================================//


    // todo: improve me
    //! returns pi
    template <redsp_arithmetic T>
    inline static constexpr T pi()
    {
        return T(3.1415926535);
    }

    //! returns two pi
    template <redsp_arithmetic T>
    inline static constexpr T twopi()
    {
        return pi<T>() * 2;
    }

    //! returns pi over two
    template <redsp_arithmetic T>
    inline static constexpr T halfpi()
    {
        return pi<T>() * 0.5;
    }

    // todo: make me not rely on non-constexpr library function
    //! returns e
    template <redsp_arithmetic T>
    inline static constexpr T e()
    {
        return exp(1);
    }


};

//! pretty gross, but returns a good log approximation fast. Inspired by Q library (unsure of origin).
template<>
float math::log_fast(float x)
{
    static_assert(std::numeric_limits<float>::is_iec559, "floats must be ieee for this to work");
    uint32_t vi, mi;
    float mf, y;
    std::memcpy(&vi, &x, 4);
    mi = (vi & 0x007fffff) | 0x3f000000;
    std::memcpy(&mf, &mi, 4);
    y = (float) vi * 1.1920928955078125e-7f; // bruh

    return y - 124.22551499f
           - 1.498030302f * mf
           - 1.72587999f / (0.3520887068f + mf);
}

template<>
double math::log_fast(double x)
{
    // this is ok for now until I figure out how to write a dedicated double version of this
    return static_cast<double>(log_fast(static_cast<float>(x)));
}



} // namespace redsp

#endif // REDSP_MATH_HEADERGUARD