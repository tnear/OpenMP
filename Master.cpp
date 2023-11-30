/*
The master construct specifies a structured block that
is executed *only* by the master thread. The master
thread is typically thread_id=0.

Master is different from 'single' as there is no implied
barrier at the end of a master block. Other threads will
*not* wait for the master thread to complete the master block.
https://www.openmp.org/spec-html/5.0/openmpse24.html

Syntax:
#pragma omp master
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
Thread 1 is starting work.
Thread 2 is starting work.
Thread 1 is finishing work.
Thread 2 is finishing work.
Only the master (Thread 0) performs this task.
Thread 0 is finishing work.
*/
void master()
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

        // Only the master thread executes (other threads skip and do not wait)
        #pragma omp master
        {
            cout << "Only the master (Thread " << thread_id << ") performs this task.\n";
        }

        // all threads execute this
        #pragma omp critical
        {
            cout << "Thread " << thread_id << " is finishing work.\n";
        }
    }
}

void test()
{
    master();
}

int main()
{
    test();

    cout << endl
         << __FILE__ " tests passed!" << endl;
    return 0;
}
