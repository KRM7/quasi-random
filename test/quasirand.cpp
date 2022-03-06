#include "../src/quasirand.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/generators/catch_generators.hpp>

using namespace quasirand;


TEST_CASE("dimensions of the generator")
{
    SECTION("good")
    {
        size_t dim = GENERATE(1, 2, 3, 5, 10, 5123, 31242025);

        QuasiRandom qrng(dim);

        REQUIRE(qrng.dim() == dim);
        REQUIRE(qrng().size() == dim);
        REQUIRE(qrng(0).size() == dim);
        REQUIRE(qrng(101).size() == dim);
    }

    SECTION("bad")
    {
        REQUIRE_THROWS_AS(QuasiRandom(0), std::invalid_argument);
    }
}

TEST_CASE("seeding")
{
    SECTION("good")
    {
        double seed = GENERATE(0.0, 0.1, 0.23, 0.7, 1.3, 351.24);

        QuasiRandom qrng(2, seed);
        auto null = qrng(0);

        REQUIRE(std::all_of(null.begin(), null.end(), [seed](double p) { return p == Catch::Approx(seed - std::floor(seed)); }));
    }

    SECTION("bad")
    {
        QuasiRandom qrng(2);

        REQUIRE_THROWS_AS(qrng.reset(-1.0), std::invalid_argument);
        REQUIRE_THROWS_AS(QuasiRandom(3, -900.5), std::invalid_argument);
    }
}

TEST_CASE("generated values")
{
    size_t dim = GENERATE(1, 2, 3, 5, 10, 500);

    QuasiRandom qrng(dim);

    for (size_t i = 0; i < 13; i++)
    {
        std::ignore = qrng();
    }

    auto val1 = qrng();
    auto val2 = qrng(14);
    for (size_t i = 0; i < val1.size(); i++)
    {
        REQUIRE(val1[i] == Catch::Approx(val2[i]));
    }

    qrng.discard();
    val1 = qrng();
    val2 = qrng(16);
    for (size_t i = 0; i < val1.size(); i++)
    {
        REQUIRE(val1[i] == Catch::Approx(val2[i]));
    }

    qrng.discard(3);
    val1 = qrng();
    val2 = qrng(20);
    for (size_t i = 0; i < val1.size(); i++)
    {
        REQUIRE(val1[i] == Catch::Approx(val2[i]));
    }
}