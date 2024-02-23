/*
'parallel for' spawns threads and divides them by iteration.
Iterations should be independent.
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

void testLoopDependence()
{
    // this poorly written example has iteration dependence on the 'j' variable
    int result = 0;
    int j = 5;
    for (int i = 0; i < 1000; ++i)
    {
        j += 2;
        result += j;
    }

    cout << result << '\n';
    assert(result == 1'006'000);
}

void testLoopIndependence()
{
    int result = 0;

    // spawn threads and run iterations in parallel
    #pragma omp parallel for
    for (int i = 0; i < 1000; ++i)
    {
        // make j independent of each iteration
        int j = 5 + 2 * (i + 1);

        #pragma omp critical
        {
            // protect shared variable 'result'
            result += j;
        }
    }

    cout << result << '\n';
    assert(result == 1'006'000);
}

void matrixMultiply(const vector<vector<int>> &A, const vector<vector<int>> &B,
                    vector<vector<int>> &C)
{
    int N = A.size();
    #pragma omp parallel for
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void testMatrixMultiply()
{
    const int N = 3;
    vector<vector<int>> A = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}};

    vector<vector<int>> B = {
        {9, 8, 7},
        {6, 5, 4},
        {3, 2, 1}};

    vector<vector<int>> C(N, vector<int>(N));

    matrixMultiply(A, B, C);

    assert(C[0] == vector<int>({30, 24, 18}));
    assert(C[1] == vector<int>({84, 69, 54}));
    assert(C[2] == vector<int>({138, 114, 90}));
}

void test()
{
    testLoopDependence();
    testLoopIndependence();
    testMatrixMultiply();
}

int main()
{
    test();

    cout << endl
         << __FILE__ " tests passed!" << endl;
    return 0;
}
