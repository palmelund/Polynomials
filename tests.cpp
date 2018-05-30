#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "Polynomial.hpp"

using namespace polynomial;

/**
 * Main function for catch.
 * This is present to resolve some problems when running the Catch test suite in CLion.
 * @param argc Number of arguments passed from command line
 * @param argv Arguments passed from command line
 * @return Status code.
 */
int main(int argc, char* argv[]) {
    // Setup
    auto result = Catch::Session().run(argc, argv);
  // Tear down
    return 0; //result;
}

using namespace polynomial;

TEST_CASE("Create integer polynomials", "[polynomial]") {
    Polynomial<int> z{};
    REQUIRE(z.degree() == 0);

    Polynomial<int> p{0};
    REQUIRE(p.degree() == 0);

    Polynomial<int> q{2, 3};
    REQUIRE(q.degree() == 1);

    Polynomial<int> r{4, 2, 3};
    REQUIRE(r.degree() == 2);
}

TEST_CASE("Create float polynomials", "[polynomial]") {
    Polynomial<float> z{};
    REQUIRE(z.degree() == 0);

    Polynomial<float> p{0.0};
    REQUIRE(p.degree() == 0);

    Polynomial<float> q{2.0, 3.0};
    REQUIRE(q.degree() == 1);

    Polynomial<float> r{4.0, 2.0, 3.0};
    REQUIRE(r.degree() == 2);
}

TEST_CASE("Create complex polynomials", "[polynomial]") {
    Polynomial<complex<int>> z{};
    REQUIRE(z.degree() == 0);

    Polynomial<complex<int>> p{complex<int>{1}};
    REQUIRE(p.degree() == 0);

    Polynomial<complex<int>> q{complex<int>{1}, complex<int>{2}};
    REQUIRE(q.degree() == 1);

    Polynomial<complex<int>> r{complex<int>{1}, complex<int>{2}, complex<int>{3}};
    REQUIRE(r.degree() == 2);
}

TEST_CASE("Scale polynomials", "[polynomial]") {
    Polynomial<int> p{2, 4};
    p.scale(2);
    REQUIRE(p.value_at_degree(0) == 4);
    REQUIRE(p.value_at_degree(1) == 8);

    Polynomial<float> q{2.0, 4.0};
    q.scale(2.0);
    REQUIRE(q.value_at_degree(0) == 4.0);
    REQUIRE(q.value_at_degree(1) == 8.0);

    Polynomial<complex<int>> r{complex<int>{2}, complex<int>{4}};
    r.scale(2);
    REQUIRE(r.value_at_degree(0) == complex<int>{4});
    REQUIRE(r.value_at_degree(1) == complex<int>{8});
}

TEST_CASE("Add roots to polynomials", "[polynomial]") {
    Polynomial<int> p{2, 3};
    p.add_term(7);
    REQUIRE(p.degree() == 2);

    Polynomial<float> q{2.0, 3.7};
    q.add_term(7.9, -77.8);
    REQUIRE(q.degree() == 3);
}

TEST_CASE("Evaluate polynomials", "[polynomial]") {
    Polynomial<int> p{1, 2, 3};
    auto res = p.evaluate(0);
    REQUIRE(res == 3);

    Polynomial<int> q{1, -1, -2};
    auto res2 = q.evaluate(2);
    REQUIRE(res2 == 0);
}

TEST_CASE("Derive polynomials", "[polynomial]") {
    Polynomial<int> p{1, -1, -2};
    auto expected = Polynomial<int>{2, -1};
    auto actual = p.derive();
    REQUIRE(actual == expected);
}

TEST_CASE("Integral polynomials", "[polynomial]") {
    Polynomial<float> p{1.0, -1.0, -2.0};
    REQUIRE(abs(p.integral(0, 2) - -3.33f) < 0.1f);   // Epsilon
}

TEST_CASE("Add polynomials", "[polynomial") {
    Polynomial<int> p{1, 2, 3};
    Polynomial<int> q{7, 3, 2, 1};
    Polynomial<int> expected{7, 4, 4, 4};
    auto actual = p + q;
    REQUIRE(actual == expected);
}

TEST_CASE("Product of polynomials", "[polynomial") {
    Polynomial<int> p{2,1};
    Polynomial<int> q{1,-1};
    Polynomial<int> expected{2,-1,-1};
    auto actual = p * q;
    REQUIRE(actual == expected);
}