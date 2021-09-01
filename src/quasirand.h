/* Copyright (c) 2021 Krisztián Rugási. Subject to the MIT license. */

/*
* A simple quasi-random number generator implementation for generating low-discrepancy
* sequences.
* 
* See: Martin Roberts, 2018. "The Unreasonable Effectiveness of Quasirandom Sequences", 
* http://extremelearning.com.au/unreasonable-effectiveness-of-quasirandom-sequences/
*/

#ifndef QUASIRAND_H
#define QUASIRAND_H

#include <vector>
#include <cstddef>

namespace quasirand
{
    /*
    * Quasi-random number generator based on: http://extremelearning.com.au/unreasonable-effectiveness-of-quasirandom-sequences/
    * Generates points on the unit hypercube in dim dimensions.
    */
    template<typename RealType = double>
    class QuasiRandom
    {
    public:

        /* Construct the generator in dim dimensions. */
        QuasiRandom(size_t dim, RealType seed = 0.5);

        /* Generate the next random point in the sequence. */
        std::vector<RealType> operator()();

        /* Generate the n-th point in the sequence. */
        std::vector<RealType> operator()(size_t n) const;

        /* Discard the next n points of the sequence. */
        void discard(size_t n = 1);

        /* Set the seed of the generator. */
        void seed(RealType new_seed);

    private:
        const size_t dim_;              /* The dimension of the generated points of the sequence. */
        RealType seed_;                 /* The seed used. */
        std::vector<RealType> alpha_;   /* The initial point of the sequence (without considering the seed). */
        std::vector<RealType> point_;   /* The current/last point generated in the sequence. */

        /* Approximate the generalized golden ratio in dim dimensions. */
        static RealType phi(size_t dim, size_t n = 30);
    };

} // namespace quasirand


/* IMPLEMENTATION */

#include <cmath>
#include <vector>
#include <stdexcept>

namespace quasirand
{
    template<typename RealType>
    inline QuasiRandom<RealType>::QuasiRandom(size_t dim, RealType seed) : dim_(dim), seed_(seed)
    {
        if (dim == 0) throw std::invalid_argument("The dimension of the generator must be at least 1.");

        alpha_.reserve(dim);
        point_.reserve(dim);

        RealType phid = phi(dim);
        for (size_t i = 0; i < dim; i++)
        {
            alpha_.push_back(1.0 / std::pow(phid, i + 1));
            point_.push_back(seed_);
        }
    }

    template<typename RealType>
    inline std::vector<RealType> QuasiRandom<RealType>::operator()()
    {
        for (size_t i = 0; i < point_.size(); i++)
        {
            point_[i] += alpha_[i];
            point_[i] -= std::floor(point_[i]);
        }

        return point_;
    }

    template<typename RealType>
    inline std::vector<RealType> QuasiRandom<RealType>::operator()(size_t n) const
    {
        std::vector<RealType> nth_point(dim_);

        for (size_t i = 0; i < nth_point.size(); i++)
        {
            nth_point[i] = seed_ + alpha_[i] * n;
            nth_point[i] -= std::floor(nth_point[i]);
        }

        return nth_point;
    }

    template<typename RealType>
    inline void QuasiRandom<RealType>::discard(size_t n)
    {
        for (; n > 0; n--)
        {
            operator()();
        }
    }

    template<typename RealType>
    inline void QuasiRandom<RealType>::seed(RealType new_seed)
    {
        seed_ = new_seed;
    }

    template<typename RealType>
    inline RealType QuasiRandom<RealType>::phi(size_t dim, size_t n)
    {
        RealType phid = 1.0;
        for (; n > 0; n--)
        {
            phid = std::pow(1.0 + phid, 1.0 / (dim + 1.0));
        }

        return phid;
    }

} // namespace quasirand

#endif