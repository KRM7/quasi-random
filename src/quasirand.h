/*
* A simple quasi-random number generator implementation for generating low-discrepancy
* sequences.
* 
* See: Martin Roberts, 2018. "The Unreasonable Effectiveness of Quasirandom Sequences", 
* http://extremelearning.com.au/unreasonable-effectiveness-of-quasirandom-sequences/
*/

#ifndef QUASIRAND_H
#define QUASIRAND_H

#include <cmath>
#include <vector>
#include <stdexcept>

namespace quasirand
{
    /*
    * Quasi-random number generator based on: http://extremelearning.com.au/unreasonable-effectiveness-of-quasirandom-sequences/
    * Generates points on the unit hypercube in dim dimensions.
    */
    class QuasiRandom
    {
    public:

        /* Construct the generator in dim dimensions. */
        QuasiRandom(size_t dim) : dim(dim)
        {
            if (dim == 0)
            {
                throw std::invalid_argument("The dimension of the generator must be at least 1.");
            }

            alpha.reserve(dim);
            point.reserve(dim);
            double phid = phi(dim);
            for (size_t i = 0; i < dim; i++)
            {
                alpha.push_back(1.0/pow(phid, i + 1));
                point.push_back(s);
            }
        }

        /* Generate the next random point in the sequence. */
        std::vector<double> operator()()
        {
            for (size_t i = 0; i < point.size(); i++)
            {
                point[i] += alpha[i];
                point[i] -= floor(point[i]);
            }
            return point;
        }

        /* Generate the n-th point in the sequence. */
        std::vector<double> operator()(size_t n) const
        {
            std::vector<double> nth_point(dim);
            for (size_t i = 0; i < nth_point.size(); i++)
            {
                nth_point[i] = s + alpha[i] * n;
                nth_point[i] -= floor(nth_point[i]);
            }
            return nth_point;
        }

        /* Discard the next n points of the sequence. */
        void discard(size_t n = 1)
        {
            for (size_t i = 0; i < n; i++)
            {
                operator()();
            }
        }

        /* Set the seed of the generator. */
        void seed(double seed)
        {
            s = seed;
        }

    private:
        const size_t dim;
        double s = 0.5;
        std::vector<double> alpha;
        std::vector<double> point;

        /* Approximate the generalized golden ratio in dim dimensions. */
        static double phi(size_t dim, size_t n = 30)
        {
            double phid = 1.0;
            for (; n > 0; n--)
            {
                phid = pow(1.0 + phid, 1.0 / (dim + 1.0));
            }
            return phid;
        }
    };
} // namespace quasirand
#endif