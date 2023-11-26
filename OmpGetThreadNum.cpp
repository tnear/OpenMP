/*
omp_get_thread_num returns the thread number, within the current team, of the calling thread.
https://www.openmp.org/spec-html/5.0/openmpsu113.html
int omp_get_thread_num(void);
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
Thread num: 0
Thread num: 4
Thread num: 3
Thread num: 7
Thread num: 1Thread num:
2
Thread num: 5
Thread num: 6
Thread num: 8
Thread num: 9
Thread num: 10
Thread num: 11
*/
void testGetThreadNum()
{
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        cout << "Thread num: " << id << "\n";
    }
}

void test()
{
    testGetThreadNum();
}

int main()
{
    test();

    cout << endl
         << __FILE__ " tests passed!" << endl;
    return 0;
}
