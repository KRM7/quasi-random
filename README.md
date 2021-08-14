# Quasi-random number generator

<p>A simple quasi-random number generator implementation in C++ for generating low-discrepancy sequences. <br>
It generates points in any number of dimensions on a unit hypercube.  
</p>

The generator is based on:  
&nbsp;&nbsp; *Martin Roberts, 2018. "[The Unreasonable Effectiveness of Quasirandom Sequences](http://extremelearning.com.au/unreasonable-effectiveness-of-quasirandom-sequences/)"*  

## Usage example

```cpp
#include "quasirand.h"

using namespace quasirand;

int main()
{
    /* Initialize the generator in 3 dimensions. */
    QuasiRandom qrng(3);

    /* Generate the next point of the sequence. */
    vector<double> point = qrng();

    /* Generate the 500th point of the sequence. */
    vector<double> point500 = qrng(500);

    /* Discard the next point from the sequence. */
    qrng.discard();

    /* Set a seed different from the default one. */
    qrng.seed(0.13);

    return 0;
}
```

## Examples of generated sequences

#### <center>2-dimensions</center>

|   ![](plots/2d_50.png)	|   ![](plots/2d_100.png)	|
|           ---	            |           ---	            |
|   ![](plots/2d_500.png)	|   ![](plots/2d_2000.png)	|

#### <center>3-dimensions</center>

|   ![](plots/3d_50.png)	|   ![](plots/3d_100.png)	|
|           ---	            |           ---	            |
|   ![](plots/3d_500.png)	|   ![](plots/3d_2000.png)	|
