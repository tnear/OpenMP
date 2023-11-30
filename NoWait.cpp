/*
nowait is used to specify that threads do not need to wait at the end of
a work-sharing construct. Normally, OpenMP includes an implicit barrier
at the end of these constructs to ensure all threads have completed their
work before proceeding. The nowait clause removes this implicit barrier,
allowing threads to continue without waiting for others.
https://www.openmp.org/spec-html/5.2/openmpse94.html

Syntax:
#pragma omp for nowait
*/

#include "omp.h"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

/*
Example output:
Thread 0 processing iteration 0
Thread 0 reached the end of the parallel for.
Thread 2 reached the end of the parallel for.
Thread 1 processing iteration 1
Thread 1 reached the end of the parallel for.
*/
// nowait is most useful for nested omp sections where the inner section
// does not need to wait at the end of its scope
void noWait()
{
    const int NUM_THREADS = 3;
    omp_set_num_threads(NUM_THREADS);

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();

        #pragma omp for nowait
        for (int i = 0; i < 2; ++i)
        {
            cout << "Thread " << thread_id << " processing iteration " << i << endl;
            //this_thread::sleep_for(chrono::milliseconds(1));
        }

        // 'nowait' allows execution to reach here without waiting for other threads
        cout << "Thread " << thread_id << " reached the end of the parallel for." << endl;
    }
}

void test()
{
    noWait();
}

int main()
{
    test();

    cout << endl
         << __FILE__ " tests passed!" << endl;
    return 0;
}
