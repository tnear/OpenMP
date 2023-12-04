/*
The sections construct is a non-iterative work sharing construct that
contains a set of structured blocks that are distributed among threads.
Each structured block is executed once by one of the threads.

Sections are often used when different code blocks can be executed in
parallel, and these tasks are independent of each other.
https://www.openmp.org/spec-html/5.1/openmpsu42.html

Syntax:
#pragma omp sections [clause[ [,] clause] ... ]
{
    #pragma omp section
      structured-block-sequence
    #pragma omp section
      structured-block-sequence
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

/*
Sample output:
Section 2, Thread ID: 1
Section 1, Thread ID: 0
*/
void sections()
{
    #pragma omp parallel
    {
        #pragma omp sections // begin sections block
        {
            #pragma omp section // section 1
            {
                // this is executed by one thread
                cout << "Section 1, Thread ID: " << omp_get_thread_num() << endl;
            }

            #pragma omp section // section 2
            {
                // this is executed by another thread
                cout << "Section 2, Thread ID: " << omp_get_thread_num() << endl;
            }

            // (additional sections can be added here)
        }
    }
}

void sumAndDoubleSum()
{
    vector<int> nums;
    int numel = 1000;
    for (int i = 0; i < numel; ++i)
    {
        nums.push_back(i+1);
    }

    int sum = -1;
    int doubled = -1;

    // sum and double sum are independent operations and could be done in parallel
    // note: it's likely a better design to pass in an execution policy directly to reduce
    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                sum = reduce(nums.begin(), nums.end());
            }

            #pragma omp section
            {
                auto doubler = [] (int previousResult, int newNumber) { return previousResult + newNumber * 2; };
                doubled = reduce(nums.begin(), nums.end(), 0, doubler);
            }
        }
    }

    assert(sum == 500'500);
    assert(doubled == 2 * 500'500);
}

void test()
{
    sections();
    sumAndDoubleSum();
}

int main()
{
    test();

    cout << endl
         << __FILE__ " tests passed!" << endl;
    return 0;
}
