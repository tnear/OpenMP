/*
OpenMP supports these data scope attribute clauses:

private     : private declares the variables in the list to be private to each thread in a team.
firstprivate: firstprivate provides a superset of the functionality provided by the private clause.
              The private variable is initialized by the original value of the variable when the parallel construct is encountered.
lastprivate : lastprivate provides a superset of the functionality provided by the private clause.
              The private variable is updated after the end of the parallel construct.
shared      : shared declares the variables in the list to be shared among all the threads in a team.
              All threads within a team access the same storage area for shared variables.
reduction   : reduction performs a reduction on the scalar variables that appear in the list, with a specified operator.
default     : default allows the user to affect the data-sharing attribute of the variables appeared in the parallel construct.
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
Thread 0 has private sharedVar value: 0
Thread 1 has private sharedVar value: 2
Thread 2 has private sharedVar value: 4
Outside parallel region, sharedVar is: 10
*/
void privateExample()
{
    int sharedVar = -10101; // a shared variable by default

    // set sharedVar to private
    #pragma omp parallel private(sharedVar) num_threads(3)
    {
        // sharedVar is now private to each thread
        // important note: sharedVar is uninitialized in this block.
        // it is not guaranteed to inherit the value before the block.
        assert(sharedVar != -10101);

        // set sharedVar based on thread_id
        int thread_id = omp_get_thread_num();
        sharedVar = thread_id * 2; // Each thread modifies its own copy of sharedVar

        #pragma omp critical
        {
            cout << "Thread " << thread_id << " has private sharedVar value: " << sharedVar << endl;
        }
    }

    // because each thread had a private copy of sharedVar, its value outside
    // the omp directive is unchanged
    assert(sharedVar == -10101);
    cout << "Outside parallel region, sharedVar is: " << sharedVar << "\n\n";
}

// with firstprivate, the value is copy constructed. each thread starts with a copy of the initial value
void firstPrivate()
{
    omp_set_num_threads(3);
    int x = -10101;

    // each thread inherits x=-10101 for firstprivate
    #pragma omp parallel firstprivate(x)
    {
        cout << "Thread " << omp_get_thread_num() << " has x (firstprivate): " << x << endl;
        assert(x == -10101);
    }

    // reset x
    x = -10101;

    // threads do not inherit x=-10101
    #pragma omp parallel private(x)
    {
        assert(x != -10101);
        cout << "Thread " << omp_get_thread_num() << " has x (private): " << x << endl;
    }
}

// lastprivate uses the last value of an omp parallel for execution
void lastPrivate()
{
    omp_set_num_threads(3);

    int last = -1;
    #pragma omp parallel for lastprivate(last)
    {
        for (int i = 0; i < 4; ++i)
        {
            last = i;
        }
    }

    // the last value of the loop above is 3, so last equals that value
    assert(last == 3);
    cout << endl << endl;
}

int globalVar = 101;
// threadprivate gives each thread a separate copy of a variable
#pragma omp threadprivate(globalVar)

void threadPrivate()
{
    #pragma omp parallel num_threads(2)
    {
        // each thread gets its own copy of globalVar
        globalVar = omp_get_thread_num() * 10;

        #pragma omp critical
        {
            cout << "Thread " << omp_get_thread_num() << ": globalVar = " << globalVar << endl;
        }
    }

    // outside the parallel region, globalVar has its original value
    cout << "Outside parallel region: globalVar = " << globalVar << endl;
    //assert(globalVar == 101); // note: not working
}

void test()
{
    privateExample();
    firstPrivate();
    lastPrivate();
    threadPrivate();
}

int main()
{
    test();

    cout << endl
         << __FILE__ " tests passed!" << endl;
    return 0;
}
