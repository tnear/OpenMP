/*
Critical: a form of synchronization where only one thread
at a time can execute a critical region.

Critical is slower than Atomic, but supports all operations.
See Atomic.cpp for examples of what it supports

https://www.openmp.org/spec-html/5.0/openmpsu89.html
#pragma omp critical [(name) [[,] hint(hint-expression)] ]
    structured-block
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

void testSumInRange()
{
    int sharedSum = 0;

    #pragma omp parallel for
    for (int i = 0; i < 101; ++i)
    {
        // assume each thread performs some computation and updates sharedSum
        int computedValue = i;

        // because sharedSum is a shared variable across threads,
        // only one thread at a time can execute this block
        #pragma omp critical
        {
            sharedSum += computedValue;
        }
    }

    cout << sharedSum << '\n';
    assert(sharedSum == 5050);
}

void test()
{
    testSumInRange();
}

int main()
{
    test();

    cout << endl
         << __FILE__ " tests passed!" << endl;
    return 0;
}
