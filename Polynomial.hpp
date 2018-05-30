#pragma once

#include <tuple>
#include <typeinfo>
#include <vector>
#include <iostream>
#include <complex>
#include <type_traits>
#include <memory>
#include <algorithm>
#include <functional>

using namespace std;

namespace polynomial {
    // https://stackoverflow.com/questions/41055292/c-stdenable-if-for-more-types
    // https://stackoverflow.com/questions/41438493/how-to-identifying-whether-a-template-argument-is-stdcomplex

    template<typename T>
    struct is_complex_t : public false_type {
    };

    template<typename T>
    struct is_complex_t<complex<T>> : public true_type {
    };

    template<typename T>
    constexpr bool is_complex() { return is_complex_t<T>::value; }

    template <typename T>
    constexpr bool polytype() {
        return is_convertible<T, int>::value ||
               is_convertible<T, float>::value ||
               is_complex<T>();
    }

    template<typename T>
    class Polynomial {
    public:

        // Copy constructor
        Polynomial(const Polynomial<T> &other) = default;

        // Copy assignment
        Polynomial &operator=(const Polynomial<T> &other) = default;


        Polynomial();

        template<typename First, typename ... Rest>
        explicit Polynomial(First first, Rest... rest);

        T value_at_degree(int index) const;

        int degree() const;

        void scale(T scalar);

        template<typename Term>
        void add_term(Term term) ;

        template<typename First, typename ... Rest>
        void add_term(First first, Rest... rest) ;

        T evaluate(T point) const;

        Polynomial derive() const;

        T integral(T lowerbound, T upperbound) const;

        bool operator==(const Polynomial<T> &other) const;

        Polynomial<T> operator+(const Polynomial<T> &other) const;

        Polynomial<T> operator*(const Polynomial<T> &other) const;

    private:
        vector<T> v;

        // TODO: FIX ME MAYBE?
        void derive_helper();

        T integral_helper(T limit) const;

        template<typename First>
        void variadic_to_front(First first) ;

        template<typename First, typename ... Rest>
        void variadic_to_front(First first, Rest... rest) ;

        template<typename First>
        void variadic_to_back(First first) ;

        template<typename First, typename... Rest>
        void variadic_to_back(First first, Rest... rest) ;
    };

    /*
 * PUBLIC
 */

    template<typename T>
    polynomial::Polynomial<T>::Polynomial()  {
        v.emplace_back(0);
    }

    template<typename T>
    template<typename First, typename ... Rest>
    polynomial::Polynomial<T>::Polynomial(First first, Rest... rest)  {
        variadic_to_back(first, rest...);
    };

    template <class T>
    T polynomial::Polynomial<T>::value_at_degree(const int index) const  {
        return v.at(index);
    }

    template <class T>
    int polynomial::Polynomial<T>::degree() const {
        return v.size() - 1;
    }

    template <class T>
    void polynomial::Polynomial<T>::scale(const T scalar) {
        for (int i = 0; i < v.size(); ++i) {
            v[i] *= scalar;
        }
    }

    template <class T>
    template <typename Term>
    void polynomial::Polynomial<T>::add_term(Term term) {
        variadic_to_front(term);
    }

    template<typename T>
    template<typename First, typename ... Rest>
    void polynomial::Polynomial<T>::add_term(First first, Rest... rest) {
        variadic_to_front(rest...);
        variadic_to_front(first);
    };

    template <class T>
    T polynomial::Polynomial<T>::evaluate(T point) const {
        // https://www.geeksforgeeks.org/horners-method-polynomial-evaluation/
        auto res = v.at(0);
        auto size = v.size();

        for (int i = 1; i < size; ++i) {
            res = res * point + v.at(i);
        }

        return res;
    }

    template <class T>
    polynomial::Polynomial<T> polynomial::Polynomial<T>::derive() const  {
        auto derives = Polynomial<T>(*this);

        derives.derive_helper();

        return derives;
    }

    template <class T>
    T polynomial::Polynomial<T>::integral(T lowerbound, T upperbound) const {
        return integral_helper(upperbound) - integral_helper(lowerbound);
    }

    template <class T>
    bool polynomial::Polynomial<T>::operator==(const polynomial::Polynomial<T> &other) const {
        return other.v == v;
    }

    template <class T>
    polynomial::Polynomial<T> polynomial::Polynomial<T>::operator+(const polynomial::Polynomial<T> &other) const {
        auto& a = v.size() > other.v.size() ? *this : other;
        auto& b = v.size() > other.v.size() ? other : *this;

        Polynomial p(a);

        for (int i = p.v.size()-1, j = b.v.size()-1; j >= 0; --i, --j) {
            p.v[i] += b.v.at(j);
        }

        return p;
    }

    template <class T>
    polynomial::Polynomial<T> polynomial::Polynomial<T>::operator*(const polynomial::Polynomial<T> &other) const  {
        // https://www.geeksforgeeks.org/multiply-two-polynomials-2/
        Polynomial p{};
        p.v.resize(v.size() + other.v.size() - 1);

        for(int i = 0; i < v.size(); i++) {
            for (int j = 0; j < other.v.size(); ++j) {
                p.v[i+j] += v[i] * other.v[j];
            }
        }

        return p;
    }

/*
 * PRIVATE
 */

    template <class T>
    void polynomial::Polynomial<T>::derive_helper() {
        auto size = v.size()-1;

        for (int i = 0; i < size; ++i) {
            v[i] *= (size - i);
        }

        v.resize(v.size()-1);
    }

    template <class T>
    T polynomial::Polynomial<T>::integral_helper(T limit) const {
        T res{};
        auto size = v.size();

        for (int i = 0; i < size; ++i) {
            res += v[i] / (size-i) * pow(limit, size-i);
        }

        return res;
    }

    template <class T>
    template <typename First>
    void polynomial::Polynomial<T>::variadic_to_front(First first) {
        v.insert(v.begin(), first);
    }

    template <class T>
    template <typename First, typename ... Rest>
    void polynomial::Polynomial<T>::variadic_to_front(First first, Rest... rest) {
        variadic_to_front(rest...);
        variadic_to_front(first);
    };

    template <class T>
    template <typename First>
    void polynomial::Polynomial<T>::variadic_to_back(First first) {
        v.emplace_back(first);
    }

    template <class T>
    template <typename First, typename ... Rest>
    void polynomial::Polynomial<T>::variadic_to_back(First first, Rest... rest) {
        v.emplace_back(first);
        variadic_to_back(rest...);
    }
}