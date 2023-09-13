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
#include "poly.h"

#ifdef redsp_cxx20
#include <concepts>
#endif

// add pi, sin, etc impls
namespace redsp
{

struct remath
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
        return std::sin(x);
    }

    //! returns an approximation of sin(x), diverging outside ().
    template <redsp_arithmetic T>
    static T sin_fast(T x)
    {
        redsp_arithmetic_assert(T)
        polydiv<T, 10> sin_f1({0.0, 1.0, 0.0, -53272705.0/360869676.0, 0.0, 38518909.0/7217393520.0, 0.0, -269197963.0/3940696861920.0, 0.0, 4585922449.0/15605159573203200.0}, {1.0, 0.0, 2290747.0/120289892.0, 0.0, 1281433.0/7217393520.0, 0.0, 560401.0/562956694560.0, 0.0, 1029037.0/346781323848960.0, 0.0});

        return sin_f1(x);
    }

    //! returns an approximation of sin(x), diverging outside ().
    template <redsp_arithmetic T>
    static T sin_faster(T x)
    {
        redsp_arithmetic_assert(T)
        polydiv<T, 6> sin_f2({0.0, 1.0, 0.0, -53.0/396.0, 0.0, 551.0/166320.0}, {1.0, 0.0, 13.0/396.0, 0.0, 5.0/11088.0, 0.0});

        return sin_f2(x);
    }

    //! returns cos(x) using stl implementation
    template <redsp_arithmetic T>
    static T cos(T x)
    {
        redsp_arithmetic_assert(T)
        return static_cast<T>(std::cos(x));
    }

    //! returns an approximation of cos(x), diverging outside ().
    template <redsp_arithmetic T>
    static T cos_fast(T x)
    {
        redsp_arithmetic_assert(T)
        return cos(fmod(x - pi<T>(), twopi<T>()) - pi<T>()); // todo: add pade approx of cos
    }

    //! returns an approximation of cos(x), diverging outside ().
    template <redsp_arithmetic T>
    static T cos_faster(T x)
    {
        redsp_arithmetic_assert(T)
        return cos(fmod(x - pi<T>(), twopi<T>()) - pi<T>()); // todo: add faster approx of cos
    }

    template <redsp_arithmetic T>
    static T cos_fastest(T x)
    {
        redsp_arithmetic_assert(T)
        return (T) 0; // extremely fast
    }

    //! returns tan(x) using stl implementation
    template <redsp_arithmetic T>
    static T tan(T x)
    {
        redsp_arithmetic_assert(T)
        return static_cast<T>(std::tan(x));
    }

    /**
     * returns a good approximation of log, pretty quickly
     * @tparam T
     * @param x
     * @return
     */
    template <redsp_arithmetic T>
    static T tan_fast(T x)
    {
        redsp_arithmetic_assert(T)
        polydiv<T, 10> tf({0.0, 1.0, 0.0, -7.0/51.0, 0.0, 1.0/255.0, 0.0, -2.0/69615.0, 0.0, 1.0/34459425.0}, {1.0, 0.0, -8.0/17.0, 0.0, 7.0/255.0, 0.0, -4.0/9945.0, 0.0, 1.0/765765.0, 0.0});

        return tf(x);
    }

    /**
     * returns a good approximation of log, very quickly.
     * for `flaot`, this returns a total error of 10.0732344252 (i.e. the integral of the residual between nearest representable values of float above -pi/2 and below pi/2)
     * for `double`, this returns a total error of
     * @tparam T type of input
     * @param x input
     * @return approximantly tan(x)
     */
    template <redsp_arithmetic T>
    static T tan_faster(T x)
    {
        redsp_arithmetic_assert(T)
        polydiv<T, 6> tf({0.0, 1.0, 0.0, -1.0/9.0, 0.0, 1.0/945.0}, {1.0, 0.0, -4.0/9.0, 0.0, 1.0/63.0, 0.0});

        return tf(x);
    }

    //! returns tanh(x) using stl implementation
    template <redsp_arithmetic T>
    static T tanh(T x)
    {
        redsp_arithmetic_assert(T)
        return static_cast<T>(std::tanh(x));
    }

    //! returns an approximation of tanh(x), diverging outside ()
    template <redsp_arithmetic T>
    static T tanh_fast(T x)
    {
        redsp_arithmetic_assert(T)
        return static_cast<T>(std::tanh(x)); // todo: add fast approx of tanh
    }


    //! returns an approximation of tanh(x), diverging outside ()
    template <redsp_arithmetic T>
    static T tanh_faster(T x)
    {
        redsp_arithmetic_assert(T)
        return static_cast<T>(std::tanh(x)); // todo: add faster approx of tanh
    }

    template <redsp_arithmetic T>
    static T log2(T x)
    {
        return std::log2(x);
    }

    //! pretty gross, but returns a good log approximation fast.
    template<typename T>
    static std::enable_if_t<std::is_same<T, float>::value, float> log2_fast(float x)
    {
        static_assert(std::numeric_limits<float>::is_iec559, "floats must be ieee for this to work");
        uint32_t vi, mi;
        float mf, y;
        std::memcpy(&vi, &x, 4);
        mi = (vi & 0x007fffff) | 0x3f000000;
        std::memcpy(&mf, &mi, 4);
        y = (float) vi * 1.1920928955078125e-7f; // bruh

        return y - 124.22551499f // newton-raphson iteration
               - 1.498030302f * mf
               - 1.72587999f / (0.3520887068f + mf);
    }

    template<typename T>
    static std::enable_if_t<std::is_same<T, double>::value, double> log2_fast(double x)
    {
        // this is ok for now until I figure out how to write a dedicated double version of this
        return static_cast<double>(log2_fast<float>(static_cast<float>(x)));
    }

    template<typename T>
    static std::enable_if_t<std::is_same<T, long double>::value, long double> log2_fast(long double x)
    {
        // see above
        return static_cast<long double>(log2_fast<float>(static_cast<float>(x)));
    }


    template <redsp_arithmetic T>
    static T sqrt(T x)
    {
        return std::sqrt(x);
    }

//
//    template<typename T>
//    static std::enable_if_t<std::is_same<T, float>::value, float> sqrt_fast(float x)
//    {
//        union { float f; uint32_t i; } val = {x};	/* Convert type, preserving bit pattern */
//
//        uint32_t vi;
//        float vf = x;
//
//        memcpy(&vi, &vf, 4);
//
//        vi -= 1 << 23;	/* Subtract 2^m. */
//        vi >>= 1;		/* Divide by 2. */
//        vi += 1 << 29;	/* Add ((b + 1) / 2) * 2^m. */
//    }
//
//    template <redsp_arithmetic T>
//    static T sqrt_fast(T x)
//    {
//
//    }

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

    template <redsp_arithmetic T>
    static T floor(T x) { return std::floor(x); }

    template <redsp_arithmetic T>
    static T ceil(T x) { return std::ceil(x); }

    //================================================================================================================//
    //==                                                                                                            ==//
    //==                                                 CONSTANTS                                                  ==//
    //==                                                                                                            ==//
    //================================================================================================================//


    //! returns pi
    template <redsp_arithmetic T>
    inline static constexpr T pi()
    {
        return static_cast<T>(3.14159265358979323846264338327950288419716939937510L);
    }

    //! returns two pi
    template <redsp_arithmetic T>
    inline static constexpr T twopi()
    {
        return static_cast<T>(pi<long double>() * 2.0L);
    }

    //! returns pi over two
    template <redsp_arithmetic T>
    inline static constexpr T halfpi()
    {
        return static_cast<T>(1.5707963267948966192313216916397514420985846996875529104874722961539082031431044993140L);
    }

    //! returns negative pi over two - this is here because some of my analytics require getting this value without rounding it to the nearest long double
    template <redsp_arithmetic T>
    inline static constexpr T neghalfpi()
    {
        return static_cast<T>(-1.5707963267948966192313216916397514420985846996875529104874722961539082031431044993140L);
    }

    //! returns pi over two
    template <redsp_arithmetic T>
    inline static constexpr T threepiovertwo()
    {
        return static_cast<T>(pi<long double>() * 2.5L);
    }


    //! returns e
    template <redsp_arithmetic T>
    inline static constexpr T e()
    {
        return static_cast<T>(2.71828182845904523536028747135266249775724709369995L);
    }

    template <redsp_arithmetic T>
    inline static constexpr T sqrt2()
    {
        return static_cast<T>(1.41421356237309504880168872420969807856967187537694L);
    }

};

} // namespace redsp

#endif // REDSP_MATH_HEADERGUARD