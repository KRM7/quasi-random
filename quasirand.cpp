#include "quasirand.h"

#include <iostream>
#include <vector>

using namespace std;

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

    return 0;
}