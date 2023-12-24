#include "../src/quasirand.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/generators/catch_generators.hpp>

using namespace quasirand;


TEST_CASE("dynamic generator")
{
    SECTION("ctor")
    {
        REQUIRE_NOTHROW(QuasiRandom(1));
        REQUIRE_NOTHROW(QuasiRandom(2, 0.3));

        REQUIRE_THROWS(QuasiRandom(0, 0.3));
        REQUIRE_THROWS(QuasiRandom(0, 1.3));
        REQUIRE_THROWS(QuasiRandom(1, -1.3));
    }

    SECTION("dimensions")
    {
        size_t dim = GENERATE(1, 2, 3, 5, 10);

        QuasiRandom qrng(dim);

        REQUIRE(qrng.dim()     == dim);
        REQUIRE(qrng().size()  == dim);
        REQUIRE(qrng(0).size() == dim);
    }

    QuasiRandom qrng(3);

    SECTION("discard")
    {
        qrng.discard();
        qrng.discard(3);

        REQUIRE(qrng() == qrng(5));
    }

    SECTION("reset")
    {
        qrng.discard(7);
        qrng.reset();

        REQUIRE(qrng() == qrng(1));
    }

    SECTION("types")
    {
        STATIC_REQUIRE(std::is_same_v<decltype(qrng)::result_type, std::vector<double>>);
        STATIC_REQUIRE(std::is_same_v<decltype(qrng)::state_type, std::vector<double>>);

        QuasiRandom<DYNAMIC, float> fqrng(2);

        STATIC_REQUIRE(std::is_same_v<decltype(fqrng)::result_type, std::vector<float>>);
        STATIC_REQUIRE(std::is_same_v<decltype(fqrng)::state_type, std::vector<float>>);
    }
}

TEST_CASE("static size")
{
    SECTION("ctor")
    {
        REQUIRE_NOTHROW(QuasiRandom<2>{});
        REQUIRE_NOTHROW(QuasiRandom<2>{ 0.3 });

        REQUIRE_THROWS(QuasiRandom<2>{ -3.2 });
    }

    SECTION("dimensions")
    {
        constexpr size_t DIM = 7;
        QuasiRandom<DIM> qrng;

        REQUIRE(qrng.dim()     == DIM);
        REQUIRE(qrng(0).size() == DIM);
    }

    QuasiRandom<3> qrng;

    SECTION("discard")
    {
        qrng.discard();
        qrng.discard(3);

        REQUIRE(qrng() == qrng(5));
    }

    SECTION("reset")
    {
        qrng.discard(7);
        qrng.reset();

        REQUIRE(qrng() == qrng(1));
    }

    SECTION("types")
    {
        STATIC_REQUIRE(std::is_same_v<decltype(qrng)::result_type, std::array<double, qrng.dim()>>);
        STATIC_REQUIRE(std::is_same_v<decltype(qrng)::state_type,  std::array<double, qrng.dim()>>);

        QuasiRandom<5, float> fqrng;

        STATIC_REQUIRE(std::is_same_v<decltype(fqrng)::result_type, std::array<float, 5>>);
        STATIC_REQUIRE(std::is_same_v<decltype(fqrng)::state_type,  std::array<float, 5>>);
    }
}