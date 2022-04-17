#ifndef REDSP_UNIVERSAL_HEADERGUARD
#define REDSP_UNIVERSAL_HEADERGUARD

#ifdef redsp_cxx20
#define redsp_arithmetic std::arithmetic
#define redsp_arithmetic_assert
#define redsp_inttype
#define redsp_uinttype
#else
#define redsp_arithmetic typename
#define redsp_inttype typename
#define redsp_uinttype typename
#define redsp_arithmetic_assert(T) static_assert(std::is_arithmetic<T>::value, "T must be arithmetic silly.");
#endif

enum class redsp_enabler_t {};



#endif // REDSP_UNIVERSAL_HEADERGUARD