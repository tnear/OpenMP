/*
The omp_get_num_procs routine returns the number of processors available to the device.
https://www.openmp.org/spec-html/5.0/openmpsu114.html

Syntax:
int omp_get_num_procs(void);
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

void numProcesses()
{
    int numP = omp_get_num_procs();
    cout << "Num processes on this device: " << numP << endl;
    // on current machine: 12
}

void test()
{
    numProcesses();
}

int main()
{
    test();

    cout << endl
         << __FILE__ " tests passed!" << endl;
    return 0;
}
