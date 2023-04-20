# redsp
r3's dsp library

i'm working slowly on this dsp library and trying to demonstrate how effective unit testing can be on audio projects. please feel free to add, modify, or suggest changes to the library and i'll check them out!

this is as much an exercise in working through these sorts of problems as it is a project that is meant for others to use - as such, please consider forking this before using it in production. perhaps i'll begin adding tagged releases that are known to work.

# todo
### biquad
- add proper cxx20 support
- add concepts <3
### remath
- improve pi() so that it returns pi with the accuracy of different types when needed
- make e() not rely on non-constexpr library function
- add pade approx of sin
- add faster approx of sin 
- add pade approx of cos
- add faster approx of cos 
- add pade approx of tan
- add fast approx of tan
- add fast approx of tanh
- add faster approx of tanh
### project
- add math tests
- finish svf
- add svftests
- add a SIMD abstraction with various intel and arm intrinsics, and the ability to work on processors without simd
- add a FFT implementation which doesn't rely on fast FFT options available on the hardware, while also hopefully falling back on those for the sake of efficiency.
- add a SIMD-implemented polynomial class using Estrin's scheme, as well as a novel way of running dividends of polynomials (such as pade approximants)
- add a graph processor which allows the user to specify a graph of processors to run on samples in turn
- add integrators and various other filters, including physical models of e.g. ladder filters
- add a mechanism that automatically uses ADAA on nonlinear processors, when provided with the static nonlinearity's antiderivatives
- add an arbitrary static nonlinearity processor with low/no abstraction cost
### tests
- add utilities that make writing tests easier
- add template tests file that allows others to write tests easily
- add tests that reasonably ensure audio callbacks are hard realtime safe, at the very least ensuring that (de)allocations don't occur during audio callbacks