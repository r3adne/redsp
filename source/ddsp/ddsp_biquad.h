/**
 * Created on April 16, 2022 by Ariadne Lewis-Towbes
 *
 * See redsp/LICENSE for license information.
*/
#ifndef REDSP_DDSP_BIQUAD_HEADERGUARD
#define REDSP_DDSP_BIQUAD_HEADERGUARD

#include <type_traits>
#include <array>
#include "../internal/remath.h"
#include "../internal/universal.h"

#ifdef redsp_cxx20
#include <concepts>
#endif


namespace redsp {

template <redsp_arithmetic SampleType, redsp_arithmetic CoeffType, size_t Channels = 1, bool SingleSampleProcessing = false>
struct dbiquad
{
    redsp_arithmetic_assert(SampleType)
    redsp_arithmetic_assert(CoeffType)
    static_assert(Channels > 0, "It doesn't make sense to have zero/negative channels");

    enum class Type
    {
        // todo: add
    };

    // todo: resize
    CoeffType a1, a2, b0, b1, b2;
    std::array<std::array<SampleType, 2>, Channels> x;
    std::array<std::array<SampleType, 2>, Channels> y;

    dbiquad() = default;
private:
    inline SampleType td2(SampleType const& x0, SampleType const& x1, SampleType const& x2, SampleType const& y1, SampleType const& y2 )
    {
//        todo: this...
    }
public:

    template<class enabled = std::enable_if<SingleSampleProcessing, void>>
    SampleType process(SampleType const& sample, int n = 0)
    {
        auto& X = x[static_cast<typename decltype(x)::size_type>(n)];
        auto& Y = y[static_cast<typename decltype(y)::size_type>(n)];
        auto s = td2(sample, X[0], X[1], Y[0], Y[1]);

        // shuffle - avoided in process(SampleType*, int, int) when
        X[1] = X[0];
        X[0] = sample;
        Y[1] = Y[0];
        Y[0] = s;

        return s;
    }

    template<class enabled = std::enable_if<SingleSampleProcessing, void>>
    void process(SampleType* samples, int count, int n = 0)
    {
        for (auto* s = samples; s != samples + count; ++s)
        {
            *s = process(*s, n);
        }
    }

    /**
     * An optimized way of running the process loop over a series of samples. It's rather weird looking, but it just means two fewer shuffles of samples.
     * @param samples (in/out) samples to process
     * @param count number of samples to process
     * @param n channel to process samples on
     */
    template<class enabled = std::enable_if<! SingleSampleProcessing, void>>
    void process_optimized(SampleType *const samples, int count, int n = 0)
    {
        auto& X = x[n];
        auto& Y = y[n];

        auto* s = samples;

        // process the first two samples normally, so that there's room to process without referencing Y
        for (; s != samples + 2; ++s)
        {
            auto sample = td2(*s, X[0], X[1], Y[0], Y[1]);
            X[1] = X[0];
            X[0] = sample;
            *s = sample;
        }

        for (; s != samples + count; ++s)
        {
            auto sample = td2(*s, X[0], X[1], *(s-1), *(s-2));
            X[1] = X[0];
            X[0] = sample;
            *s = sample;
        }

        // fill Y with the correct samples for the next buffer
        Y[0] = *(samples + count - 1);
        Y[1] = *(samples + count - 2);
    }

    /**
     * @brief Another optimized way of running the process loop over a series of samples, this time even better, assuming you want to process in a non-replacing manner.
     * This one is great because all the shuffles are removed from the processing loop, meaning the only cost which scales by count is the cost of td2 (5 *'s, 4 +'s)
     * @param input input samples
     * @param output output samples
     * @param count number of samples to process
     * @param n channel to process samples on
     */
    template<class enabled = std::enable_if<! SingleSampleProcessing, void>>
    void process_optimized(SampleType const *const input, SampleType *const output, int count, int n = 0)
    {
        auto& X = x[n];
        auto& Y = y[n];

        output[0] = td2(input[0], X[0], X[1], Y[0], Y[1]);
        output[1] = td2(input[1], input[0], X[0], output[0], Y[0]);

        for (int i = 2; i < count; ++i)
        {
            output[i] = td2(input[i], input[i-1], input[i-2], output[i-1], output[i-2]);
        }

        Y[0] = output[count - 1];
        Y[1] = output[count - 2];
        X[0] = input[count - 1];
        X[1] = input[count - 2];
    }


    template<class enabled = std::enable_if<Channels != 1 && ! SingleSampleProcessing>>
    void process_optimized(SampleType** samples, int count)
    {
        for (int i = 0; i < Channels; ++i) { process_optimized(samples[count * i], count); }
    }

    template<class enabled = std::enable_if<Channels != 1 && ! SingleSampleProcessing>>
    void process_optimized(SampleType** input, SampleType** output, int count)
    {
        for (int i = 0; i < Channels; ++i) { process_optimized(input[count * i], output[count * i], count); }
    }

    template<class enabled = std::enable_if<Channels != 1>>
    void process(SampleType** samples, int count )
    {
        for (int i = 0; i < Channels; ++i)
        {
            process(samples[count * i], count);
        }
    }


    /**
     * Calculates lowpass coefficients from normalized frequency f and Q
     * @param f Normalized frequency
     * @param Q Q
     */
    void calc_lp(CoeffType f, CoeffType Q)
    {
        calc_lp_direct(math::tan_fast(math::pi<CoeffType>() * f), Q);
    }

    /**
     * Calculates lowpass coefficients from cutoff, sampling rate, and q
     * @param fc Cutoff frequency
     * @param fs Sampling frequency
     * @param Q Q
     */
    void calc_lp(CoeffType fc, CoeffType fs, CoeffType Q)
    {
        calc_lp(fc/fs, Q);
    }

    /**
     * Calculates lowpass coefficients from k and q:
     * @param k tan(pi * (fc/fs))
     * @param q Q
     */
    void calc_lp_direct(CoeffType k, CoeffType q)
    {
        auto kk = k * k;
        auto den = kk * q + k + q;
        b0 = kk * q;
        b1 = (2 * b0) / den;
        b0 /= den;
        b2 = b0;
        a1 = (2 * q * (kk - 1)) / den;
        a2 = (den-k-k) / den;
    }

    /**
     * Calculates highpass coefficients from normalized frequency f and Q
     * @param f Normalized frequency
     * @param Q Q
     */
    void calc_hp(CoeffType f, CoeffType Q)
    {
        calc_hp_direct(math::tan_fast(math::pi<CoeffType>() * f), Q);
    }

    /**
     * Calculates highpass coefficients from cutoff, sampling rate, and q
     * @param fc Cutoff frequency
     * @param fs Sampling frequency
     * @param Q Q
     */
    void calc_hp(CoeffType fc, CoeffType fs, CoeffType Q)
    {
        calc_hp(fc/fs, Q);
    }

    /**
     * Calculates highpass coefficients from k and q:
     * @param k tan(pi * (fc/fs))
     * @param q Q
     */
    void calc_hp_direct(CoeffType k, CoeffType q)
    {
        auto kk = k * k;
        auto den = kk * q + k + q;
        b0 = q / den;
        b1 = (-2 * q) / den;
        b2 = b0;
        a1 = (2*q*(kk-1)) / den;
        a2 = (den-k-k) / den;
    }

    /**
     * Calculates bandpass coefficients from normalized frequency f and Q
     * @param f Normalized frequency
     * @param Q Q
     */
    void calc_bp(CoeffType f, CoeffType Q)
    {
        calc_bp_direct(math::tan_fast(math::pi<CoeffType>() * f), Q);
    }

    /**
     * Calculates bandpass coefficients from cutoff, sampling rate, and q
     * @param fc Cutoff frequency
     * @param fs Sampling frequency
     * @param Q Q
     */
    void calc_bp(CoeffType fc, CoeffType fs, CoeffType Q)
    {
        calc_bp(fc/fs, Q);
    }

    /**
     * Calculates bandpass coefficients from k and q:
     * @param k tan(pi * (fc/fs))
     * @param q Q
     */
    void calc_bp_direct(CoeffType k, CoeffType q)
    {
        auto kk = k * k;
        auto den = kk * q + k + q;
        b0 = k / den;
        b1 = 0;
        b2 = -k / den;
        a1 = (2 * q * (kk - 1)) / den;
        a2 = (den-k-k) / den;
    }

    /**
     * Calculates allpass coefficients from normalized frequency f and Q
     * @param f Normalized frequency
     * @param Q Q
     */
    void calc_ap(CoeffType f, CoeffType Q)
    {
        calc_ap_direct(math::tan_fast(math::pi<CoeffType>() * f), Q);
    }

    /**
     * Calculates allpass coefficients from cutoff, sampling rate, and q
     * @param fc Cutoff frequency
     * @param fs Sampling frequency
     * @param Q Q
     */
    void calc_ap(CoeffType fc, CoeffType fs, CoeffType Q)
    {
        calc_ap(fc/fs, Q);
    }

    /**
     * Calculates allpass coefficients from k and q:
     * @param k tan(pi * (fc/fs))
     * @param q Q
     */
    void calc_ap_direct(CoeffType k, CoeffType q)
    {
        auto kk = k * k;
        auto den = kk * q + k + q;
        b0 = (den-k-k) / den;
        b1 = (2 * q * (kk - 1)) / den;
        b2 = 1;
        a1 = b1;
        a2 = b0;
    }

    /**
     * Calculates bandreject coefficients from normalized frequency f and Q
     * @param f Normalized frequency
     * @param Q Q
     */
    void calc_br(CoeffType f, CoeffType Q)
    {
        calc_br_direct(math::tan_fast(math::pi<CoeffType>() * f), Q);
    }

    /**
     * Calculates bandreject coefficients from cutoff, sampling rate, and q
     * @param fc Cutoff frequency
     * @param fs Sampling frequency
     * @param Q Q
     */
    void calc_br(CoeffType fc, CoeffType fs, CoeffType Q)
    {
        calc_br(fc/fs, Q);
    }

    /**
     * Calculates bandreject coefficients from k and q:
     * @param k tan(pi * (fc/fs))
     * @param q Q
     */
    void calc_br_direct(CoeffType k, CoeffType q)
    {
        auto kk = k * k;
        auto den = kk * q + k + q;
        b0 = (q * (1 + kk)) / den;
        b1 = (2 * q * (kk - 1)) / den;
        b2 = b0;
        a1 = (2 * q * (kk - 1)) / den;
        a2 = (den-k-k) / den;
    }

    template <Type FilterType>
    void calc(CoeffType, CoeffType ) { }
    template <Type FilterType>
    void calc(CoeffType fc, CoeffType fs, CoeffType Q) { calc<FilterType>(fc/fs, Q); }

#ifdef redsp_cxx20
    template <>
    void calc<Type::Lowpass>(CoeffType f, CoeffType Q) { calc_lp(f, Q); }
    template <>
    void calc<Type::Highpass>(CoeffType f, CoeffType Q) { calc_hp(f, Q); }
    template <>
    void calc<Type::Bandpass>(CoeffType f, CoeffType Q) { calc_bp(f, Q); }
    template <>
    void calc<Type::Bandreject>(CoeffType f, CoeffType Q) { calc_br(f, Q); }
    template <>
    void calc<Type::Allpass>(CoeffType f, CoeffType Q) { calc_ap(f, Q); }
#endif
};

} // namespace redsp

#endif // REDSP_DDSP_BIQUAD_HEADERGUARD