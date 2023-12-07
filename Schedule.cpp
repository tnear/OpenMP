/*
The schedule clause in OpenMP is used to specify how iterations of a
loop are divided among the threads in a parallel region.
https://www.openmp.org/spec-html/5.0/openmpse49.html

There are 4 types:
1. static:  iterations are divided into chunks of a specified size, and
            each chunk is assigned to a thread in a round-robin fashion.
2. dynamic: assigns a chunk of iterations to a thread, and once the thread finishes,
            it requests the next chunk, until all iterations are completed.
3. guided:  similar to dynamic scheduling, but the chunk size decreases over time.
4. auto:    lets OpenMP choose the scheduling method.
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

/* Sample output:
Thread 0 processing iteration 0
Thread 0 processing iteration 0
Thread 0 processing iteration 0
Thread 0 processing iteration 1
Thread 2 processing iteration 4
Thread 2 processing iteration 5
Thread 1 processing iteration 2
Thread 1 processing iteration 3
*/
void staticExample()
{
    omp_set_num_threads(3);

    // iterations are divided *ahead of time* (statically) into chunks of size=2.
    // useful for loops where each iteration takes approximately the same amount of time.
    #pragma omp parallel for schedule(static, 2)
    for (int i = 0; i < 6; ++i)
    {
        cout << "Thread " << omp_get_thread_num() << " processing iteration " << i << endl;
    }

    cout << "Static done\n\n";
}

void dynamic()
{
    omp_set_num_threads(3);

    // iterations are assigned to threads in chunks of 2. when a thread finishes its
    // chunk, it requests the next available chunk.
    // useful for loops where iterations have unpredictable times
    #pragma omp parallel for schedule(dynamic, 2)
    for (int i = 0; i < 6; ++i)
    {
        cout << "Thread " << omp_get_thread_num() << " processing iteration " << i << endl;
    }

    cout << "Dynamic done\n\n";
}

void guided()
{
    omp_set_num_threads(3);

    // guided is similar to dynamic but chunk sizes decrease over time.
    // this example set a min chunk size of 2, which means chunks will start larger than 2.
    #pragma omp parallel for schedule(guided, 2)
    for (int i = 0; i < 6; ++i)
    {
        cout << "Thread " << omp_get_thread_num() << " processing iteration " << i << endl;
    }

    cout << "Guided done\n\n";
}

void test()
{
    staticExample();
    dynamic();
    guided();
}

int main()
{
    test();

    cout << endl
         << __FILE__ " tests passed!" << endl;
    return 0;
}
