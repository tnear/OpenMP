/*
OpenMP barrier is a synchronization point where each thread waits
until all of the threads in the team reach the barrier. Once all
threads have reached this point, they can then proceed with their
execution. Barrier is often used to synchronize work done in
parallel sections to ensure that a certain stage of processing is
complete for all threads.

Syntax:
#pragma omp barrier
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
example output:
Thread 0 is working...
Thread 1 is working...
Thread 2 is working...
Thread 2 passed the barrier.
Thread 0 passed the barrier.
Thread 1 passed the barrier.
*/
void barrierExample()
{
    // use 3 threads to simplify output
    #pragma omp parallel num_threads(3)
    {
        int thread_id = omp_get_thread_num();

        cout << "Thread " << thread_id << " is working...\n";

        // use a barrier to wait for all threads to complete their work
        #pragma omp barrier

        // this part is executed by each thread once all have passed the barrier
        cout << "Thread " << thread_id << " passed the barrier.\n";
    }
}

void test()
{
    barrierExample();
}

int main()
{
    test();

    cout << endl
         << __FILE__ " tests passed!" << endl;
    return 0;
}
