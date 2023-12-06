/*
Tasks allow you to create independent units of work that OpenMP can
schedule for execution on different threads. Tasks work well for jobs
which don't fit nearly into the regular structure of parallel loops.
https://www.openmp.org/spec-html/5.0/openmpsu46.html

Task vs Section:
- Tasks are more suited for irregular parallelism such as through recursion.
- Tasks have higher overhead to create and schedule
- Sections are more suited for rigid parallelism where the # of parallel units is
    known during compilation.
- Sections have lower overhead.

Syntax:
#pragma omp parallel
{
    #pragma omp task
    ...

    #pragma omp task
    ...
}
*/

#include "omp.h"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <numeric>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

int fibonacci(int n)
{
    int x = -1, y = -1;

    if (n <= 2)
        return n;

    #pragma omp parallel
    {
        #pragma omp task shared(x)
        x = fibonacci(n - 1);

        #pragma omp task shared(y)
        y = fibonacci(n - 2);
    } // <-- implicit barrier for all tasks to complete

    return x + y;
}

void testFibonacci()
{
    int result = fibonacci(30);
    assert(result == 1346269);
}

int main()
{
    testFibonacci();

    cout << endl
         << __FILE__ " tests passed!" << endl;
    return 0;
}
