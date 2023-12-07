/*
flush is used to ensure memory consistency between threads. flush enforces an
immediate synchronization of the thread's temporary view of memory with the
main memory. This is crucial when you need to ensure that all threads have a
consistent view of shared variables.
https://www.openmp.org/spec-html/5.0/openmpsu96.html

Syntax:
#pragma omp flush [memory-order-clause] [(list)] new-line
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

void flush()
{
    int data = 0;
    int flag = 0;

    #pragma omp parallel num_threads(2)
    {
        if (omp_get_thread_num() == 0)
        {
            // thread 0 initializes data and then sets flag
            data = 10;
            #pragma omp flush(data) // ensure data value is flushed to main memory

            flag = 1;
            #pragma omp flush(flag) // ensure flag value is flushed to main memory
        }
        else
        {
            // thread 1 waits until flag is set and then reads data
            while (true)
            {
                #pragma omp flush(flag)
                if (flag == 1)
                {
                    #pragma omp flush(data)
                    break;
                }
            }

            assert(data == 10);
            cout << "Thread 1 sees data as " << data << endl;
        }
    }
}

int main()
{
    flush();

    cout << endl
         << __FILE__ " tests passed!" << endl;
    return 0;
}
