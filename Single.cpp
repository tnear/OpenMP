/*
single is used to specify that a block of code should only
be executed by *one* thread (not necessarily the master).
Unlike master, 'single' *does* include an implicit barrier.
https://www.openmp.org/spec-html/5.0/openmpse24.html

Syntax:
#pragma omp single
    <structured-block>
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
Sample output:

Thread 0 is starting work.
This is executed by a single thread (Thread 0).
Thread 1 is starting work.
Thread 2 is starting work.
Thread 2 is finishing work.
Thread 0 is finishing work.
Thread 1 is finishing work.
*/
void single()
{
    const int NUM_THREADS = 3;
    omp_set_num_threads(NUM_THREADS);
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();

        // all threads execute this
        #pragma omp critical
        {
            cout << "Thread " << thread_id << " is starting work.\n";
        }

        // only one thread (not necessarily the master) executes this
        #pragma omp single
        {
            cout << "This is executed by a single thread (Thread " << thread_id << ").\n";
        } // <-- implicit barrier here for single (unlike master)

        // all threads execute this part after the single block is completed
        #pragma omp critical
        {
            cout << "Thread " << thread_id << " is finishing work.\n";
        }
    }
}

void test()
{
    single();
}

int main()
{
    test();

    cout << endl
         << __FILE__ " tests passed!" << endl;
    return 0;
}
