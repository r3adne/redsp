/**
 * Created on April 15, 2022 by Ariadne Lewis-Towbes
 * TODO:
 * - make correct math implementations in remath.h (see calc_stable)
 * - check stability limit
 * - implement L2 norm
 * - write tests
 * - ensure correct operation if SampleType is non-float
 *
 * See redsp/LICENSE for license information.
*/
#ifndef REDSP_SVF_HEADERGUARD
#define REDSP_SVF_HEADERGUARD

#include <type_traits>

#include "../internal/remath.h"
#include "../internal/universal.h"



#ifdef redsp_cxx20
#include <concepts>
#endif

namespace redsp {
/**
 * @brief 2nd-order state-variable filter implementation. Note instability in response to rapid cutoff modulation
 * This is a reference implementation of the SVF described by Udo Zolzer in DAFx (2nd edition). see fastsvf for a filter
 * with appropriate modulation handling for fast cutoff modulation.
 */
#ifdef redsp_cxx20
template <std::arithmetic SampleType, positive Channels = 1, std::floating_point CoeffType>
#else
template<typename SampleType = double, size_t Channels = 1, typename CoeffType = double>
#endif
struct svf
{
    enum class SVFType
    {
        Highpass = 0,
        Bandpass,
        Lowpass
    };

    CoeffType _fs;
    CoeffType f1, q1;
    // c is: h, b, l
    std::array<std::array<SampleType, 3>, Channels> c;


#ifndef redsp_cxx20
    static_assert(std::is_arithmetic<SampleType>::value,
                  "SampleType must be arithmetic (normally float, double, int, long, etc... (see simd.h for simd)");
    static_assert(std::is_floating_point<CoeffType>::value,
                  "CoeffType must be floating-point (probably just use double unless your platform doesn't support it)");
    static_assert(Channels > 0, "It doesn't make sense to have zero/negative channels");
#endif

    explicit svf(CoeffType fs = 0)
    {
        reset(fs);
    }

    /**
     * Processes a given sample `@param sample` for the given Channel `@param N`, returning a frame of output samples (highpass, bandpass, lowpass).
     * @param outframe (in/out) The frame of processed (highpass, bandpass, lowpass) outputs for the input sample and channel. Ensure this has 3*SampleType allocated.
     * @param sample The input sample
     * @param N The channel index
     */
    void process(SampleType* outframe, SampleType const& sample, int const N = 0)
    {
        auto &C = c[N];
        SampleType h, b, l;
        h = sample - C[SVFType::Highpass] - q1 * C[SVFType::Bandpass];
        b = f1 * h + C[SVFType::Bandpass];
        l = f1 * b + C[SVFType::Lowpass];
        C[SVFType::Highpass] = h;
        C[SVFType::Bandpass] = b;
        C[SVFType::Lowpass] = l;

        memcpy(C, outframe, 3);
    }

    /**
     * Processes a given sample on the given channel `@param N`, returning the type of filter specified by @tparam t
     * @tparam t The desired SVF filter type
     * @param sample The input sample
     * @param N The channel to process on
     * @return The output sample
     */
    template <SVFType t>
    SampleType process(SampleType const& sample, int const N = 0)
    {
        std::array<SampleType, 3> frame;
        process(frame.data(), sample, N);

        return frame[static_cast<size_t>(t)];
    }

    /**
     * Replaces a given buffer of samples `@param samples` with filtered samples according the the filter type @tparam t, on channel `@param N`. Assumes @param samples has `@param N` samples to process.
     * @tparam t The desired SVF filter type
     * @param samples (in/out) The samples to process and replace
     * @param count The number of input samples
     * @param N The channel to process on
     */
    template <SVFType t>
    void process(SampleType *samples, int count, int const N = 0)
    {
        for (auto* sample = samples; sample != samples + count; ++sample)
        {
            process<t>(*sample, N);
        }
    }

    /**
     * Processes multi-channel samples. Disabled when Channels is 1.
     * @param samples input samples, of shape (Channels, samples)
     * @param count the number of samples per channel in the input frame (@param samples should be length `count * N`)
     */
    template <SVFType t, typename std::enable_if<Channels != 1, void>::type>
    void process(SampleType **samples, int count)
    {
        for (int i = 0; i < Channels - 1; ++i) { process(samples[i * count], count, i); }
    }

    /**
     * Resets the filter, zeroing coefficients and replacing fs with @param fs
     * @param fs new sampling rate
     */
    void reset(CoeffType fs)
    {
        q1 = CoeffType(0);
        f1 = CoeffType(0);
        _fs = fs;
        c.fill(CoeffType(0));
    }

    /**
     * Directly sets the coefficients f1 and q1 if the stability criteria is met.
     * @param F1 2 * sin(pi * (fc/fs));
     * @param Q1 1 / Q
     * @return true if coefficients were changed, otherwise returns false.
     */
    bool calc_stable_direct(CoeffType const& F1, CoeffType const& Q1)
    {
        if (F1 < 2 - Q1)
        {
            f1 = F1;
            q1 = Q1;
            return true;
        }
        else
            return false;
    }

    /**
     * Sets the coefficients F1 and Q1 via conversions from cutoff frequency @param fc and Q @param Q
     * @param fc Cutoff frequency of the filter, in Hz.
     * @param Q Resonance of the filter (usually normalised within 0,1)
     * @return true if coefficients were changed, otherwise returns false.
     */
    bool calc_stable(CoeffType const& fc, CoeffType const& Q)
    {
        auto F1 = CoeffType(2) * remath::sin_fast(remath::pi<CoeffType>() * (fc / _fs));
        return calc_stable_direct(F1, 1 / Q);
    }

    /**
     * Sets the coefficients F1 and Q1 without checking filter stability
     * @param F1 2 * sin(pi * (fc/fs));
     * @param Q1 1 / Q
     */
    void calc_unsafe_direct(CoeffType const& F1, CoeffType const& Q1)
    {
        q1 = Q1;
        f1 = F1;
    }

    /**
     * Sets the coefficients F1 and Q1 via conversions from cutoff frequency @param fc and Q @param Q
     * @param fc Cutoff frequency of the filter, in Hz.
     * @param Q Resonance of the filter (usually normalised within 0,1)
     */
    void calc_unsafe(CoeffType const& fc, CoeffType const& Q)
    {

        auto F1 = CoeffType(2) * remath::sin(remath::pi<CoeffType>() * (fc / _fs));
        calc_unsafe_direct(F1, 1/Q);
    }


};

} // namespace redsp

#endif // REDSP_SVF_HEADERGUARD