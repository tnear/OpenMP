/*
'parallel for' spawns threads and divides them by iteration.
Iterations should be independent.
*/

#include "omp.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

void testLoopDependence()
{
    // this poorly written example has iteration dependence on the 'j' variable
    int result = 0;
    int j = 5;
    for (int i = 0; i < 1000; ++i)
    {
        j += 2;
        result += j;
    }

    cout << result << '\n';
    assert(result == 1'006'000);
}

void testLoopIndependence()
{
    int result = 0;

    // spawn threads and run iterations in parallel
    #pragma omp parallel for
    for (int i = 0; i < 1000; ++i)
    {
        // make j independent of each iteration
        int j = 5 + 2 * (i + 1);

        #pragma omp critical
        {
            // protect shared variable 'result'
            result += j;
        }
    }

    cout << result << '\n';
    assert(result == 1'006'000);
}

void test()
{
    testLoopDependence();
    testLoopIndependence();
}

int main()
{
    test();

    cout << endl
         << __FILE__ " tests passed!" << endl;
    return 0;
}
