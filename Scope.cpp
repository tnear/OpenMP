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
    int sharedVar = 10; // a shared variable by default

    #pragma omp parallel private(sharedVar) num_threads(3) // sharedVar is now private to each thread
    {
        int thread_id = omp_get_thread_num();
        sharedVar = thread_id * 2; // Each thread modifies its own copy of sharedVar

        #pragma omp critical
        {
            cout << "Thread " << thread_id << " has private sharedVar value: " << sharedVar << endl;
        }
    }

    // because each thread had a private copy of sharedVar, its value outside
    // the omp directive is unchanged
    assert(sharedVar == 10);
    cout << "Outside parallel region, sharedVar is: " << sharedVar << endl;
}

void test()
{
    privateExample();
}

int main()
{
    test();

    cout << endl
         << __FILE__ " tests passed!" << endl;
    return 0;
}
