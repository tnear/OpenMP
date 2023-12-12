/*
Atomic: a form of synchronization where only one thread
at a time can execute a critical region.

Atomic is faster than critical, but only supports these operations
for a single variable:
x binop= expr (ex: x *= 2)
x++
++x
x--
--x

https://www.openmp.org/spec-html/5.0/openmpsu95.html
#pragma omp atomic [clause[[,] clause] ... ] new-line
    expression-stmt
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
    int sum = 0;

    #pragma omp parallel for
    for (int i = 0; i < 100; ++i)
    {
        #pragma omp atomic
        sum += 1; // atomic addition
    }

    cout << sum << '\n';
    assert(sum == 100);
}

void atomicRead()
{
    int sharedVar = 0;

    // parallel region with multiple threads
    #pragma omp parallel num_threads(4) shared(sharedVar)
    {
        // only one thread writes to sharedVar
        #pragma omp single
        {
            sharedVar = 101;
        }

        // simulate some work and wait for threads
        #pragma omp barrier

        int localCopy{};

        // atomic read of sharedVar ensures a consistent value for the shared variable
        #pragma omp atomic read
        localCopy = sharedVar;

        #pragma omp critical
        {
            cout << "Thread " << omp_get_thread_num() << " read: " << localCopy << endl;
        }
    }
}

void test()
{
    testSumInRange();
    atomicRead();
}

int main()
{
    test();

    cout << endl
         << __FILE__ " tests passed!" << endl;
    return 0;
}
