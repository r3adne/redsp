//
// Created by ariadne lewis-towbes on 4/22/23.
//

#ifndef REDSP_POLY_H
#define REDSP_POLY_H

#pragma once

#include <type_traits>
#include <cmath>
#include <array>
#include "universal.h"
//#include "../../MIPP/src/mipp.h"


namespace redsp
{

template<redsp_arithmetic T, const int N, redsp_arithmetic C = T>
struct polynomial
{
    std::array<C, N> coeffs;

    virtual T eval(T x) = 0;

    T operator()(T x)
    {
        return eval(x);
    }

    C operator[](int n)
    {
        assert(n < N);
        return coeffs[n];
    }
};


template<redsp_arithmetic T, const int N, redsp_arithmetic C = T>
struct polyNaive : public polynomial<T, N, C>
{
    polyNaive(std::initializer_list<C> coefficients) : polynomial<T, N, C>::coeffs(coefficients) { }

    explicit polyNaive(std::array<C, N> coefficients) : polynomial<T, N, C>::coeffs(coefficients) { }

    T eval(T x) override
    {
        T res = (T) this[0];
        auto localx = (T) 1;

        for (auto a : this->coeffs)
        {
            res += localx * a;
            localx *= x;
        }
    }
};


template<redsp_arithmetic T, const int N, redsp_arithmetic C = T>
struct poly2term : public polynomial<T, N, C>
{
    static_assert(N == 2, "poly2term is an optimized polynomial class that only works with 2-term polynomials");

    poly2term(std::initializer_list<C> coefficients) : polynomial<T, N, C>::coeffs(coefficients) { }

    explicit poly2term(std::array<C, N> coefficients) : polynomial<T, N, C>::coeffs(coefficients) { }

    T eval(T x) override
    {
        return std::fma(x, polynomial<T, N, C>::coeffs[1], polynomial<T, N, C>::coeffs[0]);
    }
};

template<redsp_arithmetic T, const int N, redsp_arithmetic C = T>
struct polyEstrin : public polynomial<T, N, C>
{
    std::array<T, N % 2 == 1 ? N + 1 : N> t;

    polyEstrin(std::initializer_list<C> coefficients)  : polynomial<T, N, C>::coeffs(coefficients) { t.fill((C) 0); }

    explicit polyEstrin(std::array<C, N> coefficients) : polynomial<T, N, C>::coeffs(coefficients) { t.fill((C) 0); }

    T eval(T x) override
    {
        estrin(x, polynomial<T, N, C>::coeffs);
    }

    T estrin(T x, std::vector<T> ts)
    {
        if (t.size() == 1)
        {
            return x;
        }
        std::vector<T> ta;

        for (int i = 0; i < ts.size(); i += 2)
        {
            ta.push_back(ts[i] + ts[i+1] * x);
        }

        return estrin(x*x, ta);
    }
};


template<redsp_arithmetic T, const int N, redsp_arithmetic C = T>
struct polydiv
{
    std::array<C, N> num;
    std::array<C, N> den;

    polydiv(std::initializer_list<C> numerators, std::initializer_list<C> denominators)
    {
        assert(numerators.size() == denominators.size() && numerators.size() == N);

        auto a = numerators.begin();
        auto b = denominators.begin();

        for (int i = 0; i < numerators.size(); ++i)
        {
            num[i] = *a;
            den[i] = *b;

            a++;
            b++;
        }
    }

    polydiv(std::array<C, N> numerators, std::array<C, N> denominators) : num(numerators), den(denominators)
    {

    }

    T eval(T x)
    {
        T numres = (T) 0;
        T denres = (T) 0;

        auto localx = (T) 1;

        for (int i = 0; i < N; ++i)
        {
            auto a = num[i];
            auto b = den[i];

            numres += localx * a;
            denres += localx * b;

            localx *= x;
        }

        return numres / denres;
    }

    T operator()(T x)
    {
        return eval(x);
    }

};


}
#endif //REDSP_POLY_H
