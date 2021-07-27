#include "quasirand.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace quasirand;

int main()
{
    /* Create the generator. */
    size_t num_dimensions = 2;

    QuasiRandom qrng(num_dimensions);

    /* Generate and print out some points. */
    for (size_t i = 0; i < 100; i++)
    {
        vector<double> point = qrng();

        for (const auto& co : point)
        {
            cout << co << "\t";
        }
        cout << "\n";
    }

    /* Get the 500th element of the sequence. */
    vector<double> point = qrng(500);

    for (const auto& co : point)
    {
        cout << co << "\t";
    }
    cout << "\n";

    //qrng.seed(0.5);
    //qrng.discard();

    return 0;
}