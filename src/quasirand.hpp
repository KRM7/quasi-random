/* Copyright (c) 2022 Krisztián Rugási. Subject to the MIT license. */

#ifndef QUASIRAND_HPP
#define QUASIRAND_HPP

#include <array>
#include <vector>
#include <concepts>
#include <cstddef>

namespace quasirand::detail
{
    /* Approximation of the generalized golden ratio in dim dimensions. */
    template<std::floating_point RealType>
    RealType phi(std::size_t dim, std::size_t n = 30) noexcept;

} // namespace quasirand::detail

namespace quasirand
{
    /* Special dimension value for the dynamically sized generators. */
    inline constexpr std::size_t DYNAMIC = std::size_t(-1);

    /*
    * Quasi-random number generator based on: http://extremelearning.com.au/unreasonable-effectiveness-of-quasirandom-sequences/
    * Generates points in a unit hypercube in Dim dimensions.
    * 
    * The dimension of the generator can be chosen at runtime if the Dim argument is set to DYNAMIC.
    */
    template<std::size_t Dim, std::floating_point RealType = double>
    class QuasiRandom
    {
    public:
        using result_type = std::array<RealType, Dim>;
        using state_type  = std::array<RealType, Dim>;
        using size_type   = std::size_t;

        /* Construct a generator. */
        QuasiRandom() noexcept;

        /* Construct a generator using the specified seed. The seed must be in the range [0.0, 1.0). */
        explicit QuasiRandom(RealType seed);

        /* Generate the next point in the sequence. */
        [[nodiscard]]
        constexpr result_type operator()() noexcept;

        /* Return the n-th point of the sequence. Doesn't affect the state of the generator. */
        [[nodiscard]]
        constexpr result_type operator()(size_type n) const noexcept;
        
        /* Discard the next n points of the sequence. */
        constexpr void discard(size_type n = 1) noexcept;

        /* Restart the sequence. */
        constexpr void reset() noexcept;

        /* Restart the sequence using a new seed. The seed must be in the range [0.0, 1.0). */
        constexpr void reset(RealType new_seed);

        /* Return the generator's number of dimensions. */
        [[nodiscard]]
        constexpr size_type dim() const noexcept;

    private:
        RealType seed_;
        state_type alpha_;      /* The initial point of the sequence (without considering the seed). */
        state_type point_;      /* The previous point of the sequence. */
    };

    /* Can't generate points in 0 dimensions. */
    template<std::floating_point RealType>
    class QuasiRandom<0, RealType>;


    /* DYNAMIC GENERATOR */

    /* Specialization of the generator with a dynamic number of dimensions. */
    template<std::floating_point RealType>
    class QuasiRandom<DYNAMIC, RealType>
    {
    public:
        using result_type = std::vector<RealType>;
        using state_type  = std::vector<RealType>;
        using size_type   = std::size_t;

        /* Construct a generator in dim dimensions. The seed must be in the range [0.0, 1.0). */
        explicit QuasiRandom(size_type dim, RealType seed = 0.5);

        /* Generate the next point in the sequence. */
        [[nodiscard]]
        constexpr result_type operator()();

        /* Return the n-th point of the sequence. Doesn't affect the state of the generator. */
        [[nodiscard]]
        constexpr result_type operator()(size_type n) const;

        /* Discard the next n points of the sequence. */
        constexpr void discard(size_type n = 1);

        /* Restart the sequence using a new seed. The seed must be in the range [0.0, 1.0). */
        constexpr void reset(RealType new_seed = 0.5);

        /* Return the generator's number of dimensions. */
        [[nodiscard]]
        constexpr size_type dim() const noexcept;

    private:
        size_type dim_;         /* The dimension of the sequence's points. */
        RealType seed_;
        state_type alpha_;      /* The initial point of the sequence (without considering the seed). */
        state_type point_;      /* The previous point of the sequence. */
    };

    /* DEDUCTION GUIDES */

    QuasiRandom(std::size_t dim) -> QuasiRandom<DYNAMIC, double>;

    template<std::floating_point RealType>
    QuasiRandom(std::size_t dim, RealType seed) -> QuasiRandom<DYNAMIC, RealType>;

} // namespace quasirand


/* IMPLEMENTATION */

#include <cmath>
#include <tuple>
#include <algorithm>
#include <stdexcept>

namespace quasirand
{
    /* STATIC GENERATOR IMPLEMENTATION */

    template<std::size_t Dim, std::floating_point RealType>
    QuasiRandom<Dim, RealType>::QuasiRandom() noexcept :
        seed_(0.5)
    {
        const RealType phid = detail::phi<RealType>(Dim);
        for (size_type i = 0; i < Dim; i++)
        {
            alpha_[i] = 1.0 / std::pow(phid, i + 1);
            point_[i] = seed_;
        }
    }

    template<std::size_t Dim, std::floating_point RealType>
    QuasiRandom<Dim, RealType>::QuasiRandom(RealType seed) :
        seed_(seed)
    {
        if (!(0.0 <= seed && seed < 1.0)) throw std::invalid_argument("The seed must be in the range [0.0, 1.0).");

        const RealType phid = detail::phi<RealType>(Dim);
        for (size_type i = 0; i < Dim; i++)
        {
            alpha_[i] = 1.0 / std::pow(phid, i + 1);
            point_[i] = seed_;
        }
    }

    template<std::size_t Dim, std::floating_point RealType>
    constexpr auto QuasiRandom<Dim, RealType>::operator()() noexcept -> result_type
    {
        for (size_type i = 0; i < point_.size(); i++)
        {
            point_[i] += alpha_[i];
            point_[i] -= size_type(point_[i]);
        }

        return point_;
    }

    template<std::size_t Dim, std::floating_point RealType>
    constexpr auto QuasiRandom<Dim, RealType>::operator()(size_type n) const noexcept -> result_type
    {
        result_type nth_point;

        for (size_type i = 0; i < Dim; i++)
        {
            nth_point[i] = seed_ + alpha_[i] * n;
            nth_point[i] -= size_type(nth_point[i]);
        }

        return nth_point;
    }

    template<std::size_t Dim, std::floating_point RealType>
    constexpr void QuasiRandom<Dim, RealType>::discard(size_type n) noexcept
    {
        while (n--) (void)operator()();
    }

    template<std::size_t Dim, std::floating_point RealType>
    constexpr void QuasiRandom<Dim, RealType>::reset() noexcept
    {
        std::fill(point_.begin(), point_.end(), seed_);
    }

    template<std::size_t Dim, std::floating_point RealType>
    constexpr void QuasiRandom<Dim, RealType>::reset(RealType new_seed)
    {
        if (!(0.0 <= new_seed && new_seed < 1.0)) throw std::invalid_argument("The seed must be in the range [0.0, 1.0).");

        seed_ = new_seed;
        std::fill(point_.begin(), point_.end(), seed_);
    }

    template<std::size_t Dim, std::floating_point RealType>
    constexpr auto QuasiRandom<Dim, RealType>::dim() const noexcept -> size_type
    {
        return Dim;
    }


    /* DYNAMIC GENERATOR IMPLEMENTATION */

    template<std::floating_point RealType>
    QuasiRandom<DYNAMIC, RealType>::QuasiRandom(size_type dim, RealType seed) :
        dim_(dim), seed_(seed), alpha_(dim, 0.0), point_(dim, 0.0)
    {
        if (dim == 0) throw std::invalid_argument("The dimension of the generator must be at least 1.");
        if (!(0.0 <= seed && seed < 1.0)) throw std::invalid_argument("The seed must be in the range [0.0, 1.0).");

        const RealType phid = detail::phi<RealType>(dim);
        for (size_type i = 0; i < dim; i++)
        {
            alpha_[i] = 1.0 / std::pow(phid, i + 1);
            point_[i] = seed_;
        }
    }

    template<std::floating_point RealType>
    constexpr auto QuasiRandom<DYNAMIC, RealType>::operator()() -> result_type
    {
        for (size_type i = 0; i < point_.size(); i++)
        {
            point_[i] += alpha_[i];
            point_[i] -= size_type(point_[i]);
        }

        return point_;
    }

    template<std::floating_point RealType>
    constexpr auto QuasiRandom<DYNAMIC, RealType>::operator()(size_type n) const -> result_type
    {
        result_type nth_point(dim_);

        for (size_type i = 0; i < dim_; i++)
        {
            nth_point[i] = seed_ + alpha_[i] * n;
            nth_point[i] -= size_type(nth_point[i]);
        }

        return nth_point;
    }

    template<std::floating_point RealType>
    constexpr void QuasiRandom<DYNAMIC, RealType>::discard(size_type n)
    {
        while (n--) (void)operator()();
    }

    template<std::floating_point RealType>
    constexpr void QuasiRandom<DYNAMIC, RealType>::reset(RealType new_seed)
    {
        if (!(0.0 <= new_seed && new_seed < 1.0)) throw std::invalid_argument("The seed must be in the range [0.0, 1.0).");

        seed_ = new_seed;
        std::fill(point_.begin(), point_.end(), seed_);
    }

    template<std::floating_point RealType>
    constexpr auto QuasiRandom<DYNAMIC, RealType>::dim() const noexcept -> size_type
    {
        return dim_;
    }

} // namespace quasirand


namespace quasirand::detail
{
    template<std::floating_point RealType>
    RealType phi(std::size_t dim, std::size_t n) noexcept
    {
        RealType phid = 1.0;
        RealType exponent = 1.0 / (dim + 1.0);

        while (n--) phid = std::pow(1.0 + phid, exponent);

        return phid;
    }

} // namespace quasirand::detail

#endif