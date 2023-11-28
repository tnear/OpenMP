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
