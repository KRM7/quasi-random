/* Copyright (c) 2022 Kriszti�n Rug�si. Subject to the MIT license. */

#ifndef QUASIRAND_HPP
#define QUASIRAND_HPP

#include <vector>
#include <concepts>
#include <cstddef>

namespace quasirand
{
    /*
    * Quasi-random number generator based on: http://extremelearning.com.au/unreasonable-effectiveness-of-quasirandom-sequences/
    * Generates points on the unit hypercube in dim dimensions.
    */
    template<std::floating_point RealType = double>
    class QuasiRandom final
    {
    public:
        using result_type = std::vector<RealType>;
        using state_type  = std::vector<RealType>;
        using size_type   = std::size_t;

        /* Construct the generator in dim dimensions. */
        explicit QuasiRandom(size_type dim, RealType seed = 0.5);

        /* Generate the next random point in the sequence. */
        [[nodiscard]]
        result_type operator()();

        /* Generate the n-th point in the sequence. */
        [[nodiscard]]
        result_type operator()(size_type n) const;

        /* Discard the next n points of the sequence. */
        void discard(size_type n = 1);

        /* Set a new seed for the generator. */
        void reset(RealType new_seed);

        /* Return the generator's number of dimensions. */
        [[nodiscard]]
        size_type dim() const noexcept;

    private:
        size_type dim_;         /* The dimension of the generated points of the sequence. */
        RealType seed_;         /* The seed used. */
        state_type alpha_;      /* The initial point of the sequence (without considering the seed). */
        state_type point_;      /* The current/last point generated in the sequence. */

        /* Approximate the generalized golden ratio in dim dimensions. */
        static constexpr RealType phi(size_t dim, size_t n = 30) noexcept;
    };

} // namespace quasirand


/* IMPLEMENTATION */

#include <cmath>
#include <tuple>
#include <algorithm>
#include <stdexcept>

namespace quasirand
{
    template<std::floating_point T>
    QuasiRandom<T>::QuasiRandom(size_type dim, T seed) :
        dim_(dim), seed_(seed), alpha_(dim, 0.0), point_(dim, 0.0)
    {
        if (dim == 0) throw std::invalid_argument("The dimension of the generator must be at least 1.");
        if (seed < 0.0) throw std::invalid_argument("The seed value can't be negative.");

        const T phid = phi(dim);
        for (size_type i = 0; i < dim; i++)
        {
            alpha_[i] = 1.0 / std::pow(phid, i + 1);
            point_[i] = seed_;
        }
    }

    template<std::floating_point T>
    inline auto QuasiRandom<T>::operator()() -> result_type
    {
        for (size_type i = 0; i < point_.size(); i++)
        {
            point_[i] += alpha_[i];
            point_[i] -= size_type(point_[i]);
        }

        return point_;
    }

    template<std::floating_point RealType>
    inline auto QuasiRandom<RealType>::operator()(size_type n) const -> result_type
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
    inline void QuasiRandom<RealType>::discard(size_type n)
    {
        while (n--) (void)operator()();
    }

    template<std::floating_point RealType>
    inline void QuasiRandom<RealType>::reset(RealType new_seed)
    {
        if (new_seed < 0.0) throw std::invalid_argument("The seed value can't be negative.");

        seed_ = new_seed;
        std::fill(point_.begin(), point_.end(), seed_);
    }

    template<std::floating_point RealType>
    inline auto QuasiRandom<RealType>::dim() const noexcept -> size_type
    {
        return dim_;
    }

    template<std::floating_point RealType>
    constexpr inline RealType QuasiRandom<RealType>::phi(size_type dim, size_t n) noexcept
    {
        RealType phid = 1.0;
        while (n--)
        {
            phid = std::pow(1.0 + phid, 1.0 / (dim + 1.0));
        }

        return phid;
    }

} // namespace quasirand

#endif