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

    template<typename Q>
    struct is_complex_t : public false_type {
    };

    template<typename Q>
    struct is_complex_t<complex<Q>> : public true_type {
    };

    template<typename Q>
    constexpr bool is_complex() { return is_complex_t<Q>::value; }

    template<typename T, typename = enable_if<
            is_convertible<T, int>::value ||
            is_convertible<T, float>::value ||
            is_complex<T>()>>
    class Polynomial {
    public:

        // Copy constructor
        Polynomial(const Polynomial &other) = default;

        // Copy assignment
        Polynomial &operator=(const Polynomial &other) = default;


        Polynomial() {
            v.emplace_back(0);
        }

        template<typename First, typename ... Rest, typename = enable_if<is_convertible<First, T>::value>>
        explicit Polynomial(First first, Rest... rest) {
            variadic_to_back(first, rest...);
        };

        T value_at_degree(const int index) const {
            return v.at(index);
        }

        int degree() const {
            return v.size() - 1;
        }

        void scale(const T scalar) {
            for (int i = 0; i < v.size(); ++i) {
                v[i] *= scalar;
            }
        }

        template<typename Term>
        void add_term(Term term) {
            variadic_to_front(term);
        }

        template<typename First, typename ... Rest>
        void add_term(First first, Rest... rest) {
            variadic_to_front(rest...);
            variadic_to_front(first);
        };

        T evaluate(T point) const {
            // https://www.geeksforgeeks.org/horners-method-polynomial-evaluation/
            auto res = v.at(0);
            auto size = v.size();

            for (int i = 1; i < size; ++i) {
                res = res * point + v.at(i);
            }

            return res;
        }

        Polynomial derive() const {
            auto derives = Polynomial<T>(*this);

            derives.derive_helper();

            return derives;
        }

        T integral(T lowerbound, T upperbound) {
            return integral_helper(upperbound) - integral_helper(lowerbound);
        }

        bool operator==(const Polynomial &other) const {
            return other.v == v;
        }

        Polynomial operator+(const Polynomial &other) const {
            auto& a = v.size() > other.v.size() ? *this : other;
            auto& b = v.size() > other.v.size() ? other : *this;

            Polynomial p(a);

            for (int i = p.v.size()-1, j = b.v.size()-1; j >= 0; --i, --j) {
                p.v[i] += b.v.at(j);
            }

            return p;
        }

        Polynomial operator*(const Polynomial &other) const {
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

    private:
        vector<T> v;

        void derive_helper() {
            auto size = v.size()-1;

            for (int i = 0; i < size; ++i) {
                v[i] *= (size - i);
            }

            v.resize(v.size()-1);
        }

        T integral_helper(T limit) {
            T res{};
            auto size = v.size();

            for (int i = 0; i < size; ++i) {
               res += v[i] / (size-i) * pow(limit, size-i);
            }

            return res;
        }

        template<typename First>
        void variadic_to_front(First first) {
            v.insert(v.begin(), first);
        }

        template<typename First, typename ... Rest>
        void variadic_to_front(First first, Rest... rest) {
            variadic_to_front(rest...);
            variadic_to_front(first);
        };

        template<typename First>
        void variadic_to_back(First first) {
            v.emplace_back(first);
        }

        template<typename First, typename... Rest>
        void variadic_to_back(First first, Rest... rest) {
            v.emplace_back(first);
            variadic_to_back(rest...);
        }
    };
}