/*
omp_set_num_threads sets the # of threads to be used for parallel regions
that do not specify a num_threads clause.
https://www.openmp.org/spec-html/5.0/openmpsu110.html
void omp_set_num_threads(int num_threads);
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

/*
Example output:
Thread num: 0
Thread num: 2
Thread num: 1
*/
void testSetNumThreads()
{
    // limit to 3 threads
    omp_set_num_threads(3);
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        cout << "omp_set_num_threads num: " << id << "\n";
    }
}

void testNumThreads()
{
    // limit to 3 threads using the parallel construct
    // similar output to example above
    #pragma omp parallel num_threads(3)
    {
        int id = omp_get_thread_num();
        cout << "num_threads num: " << id << "\n";
    }
}

void test()
{
    testSetNumThreads();
    testNumThreads();
}

int main()
{
    test();

    cout << endl
         << __FILE__ " tests passed!" << endl;
    return 0;
}
