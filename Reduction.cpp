/*
'reduction' specifies one or more thread-private variables
that are subject to a reduction at the end of the parallel region

ex: reduction(+ : sum)
states that 'sum' is the reduction variable and '+' is performed
on all the thread-private sum variables to produce a scalar result

Supported identifiers include: +, -, *, &, |, ^, &&, ||, max, min
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

void sumArray()
{
    constexpr int SIZE = 100;
    int array[SIZE];

    // create 1 to 100
    for (int i = 0; i < SIZE; ++i)
        array[i] = i + 1; // array will contain numbers 1 to 100

    int result = 0;

    // perform a reduction on 'result' variable, use '+' to combine results
    #pragma omp parallel for reduction(+ : result)
    for (int i = 0; i < SIZE; ++i)
    {
        result += array[i];
    }

    cout << "Sum of array elements is " << result << '\n';
    assert(result == 5050);
}

void factorial()
{
    long long result = 1;

    // perform reduction on 'result' variable, combine results using '*' operator
    #pragma omp parallel for reduction(* : result)
    for (int i = 1; i <= 15; ++i)
    {
        result *= i;
    }

    cout << "Factorial is " << result << '\n';
    assert(result == 1307674368000);
}

void maxValue()
{
    constexpr int SIZE = 100;
    int array[SIZE];

    // create 1 to 100
    for (int i = 0; i < SIZE; ++i)
        array[i] = i + 1; // array will contain numbers 1 to 100

    // add a large value inside the array
    constexpr int LARGE_VAL = 1000;
    array[33] = LARGE_VAL;

    int result = numeric_limits<int>::min();

    // each thread has a private copy of 'result'
    // max() is used to combine these private copies
    #pragma omp parallel for reduction(max : result)
    for (int i = 0; i < SIZE; ++i)
    {
        if (array[i] > result)
            result = array[i];
    }

    cout << "Max value is " << result << '\n';
    assert(result == LARGE_VAL);
}

void test()
{
    sumArray();
    factorial();
    maxValue();
}

int main()
{
    test();

    cout << endl
         << __FILE__ " tests passed!" << endl;
    return 0;
}
