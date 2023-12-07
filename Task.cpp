/*
Tasks allow you to create independent units of work that OpenMP can
schedule for execution on different threads. Tasks work well for jobs
which don't fit nearly into the regular structure of parallel loops.
https://www.openmp.org/spec-html/5.0/openmpsu46.html

Task vs Section:
- Tasks are more suited for irregular parallelism such as through recursion.
- Tasks have higher overhead to create and schedule
- Sections are more suited for rigid parallelism where the # of parallel units is
    known during compilation.
- Sections have lower overhead.

Syntax:
#pragma omp parallel
{
    #pragma omp task
    ...

    #pragma omp task
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

// note how the recursive calls never create a new "#pragma omp parallel" region.
// That is done in the calling function to avoid nesting. nested parallel regions
// hinder performance.
int _fibonacci(int n)
{
    if (n <= 2)
        return n;

    int x = -1, y = -1;
    #pragma omp task shared(x)
    x = _fibonacci(n - 1);

    #pragma omp task shared(y)
    y = _fibonacci(n - 2);

    #pragma omp taskwait // wait for all tasks to finish
    return x + y;
}

int fibonacci(int n)
{
    int result = -1;
    // create one parallel region to use for *all* recursive calls
    #pragma omp parallel
    {
        #pragma omp single // ensure block is called only once
        result = _fibonacci(n);
    }

    return result;
}

void testFibonacci()
{
    int result = fibonacci(30);
    assert(result == 1346269);
}

class TreeNode
{
public:
    int value{};
    TreeNode *left = nullptr, *right = nullptr;

    TreeNode(int val)
        : value(val)
    {
    }
};

// note how the recursive calls never create a new "#pragma omp parallel" region.
// That is done in the calling function to avoid nesting. nested parallel regions
// hinder performance.
int _sumTree(TreeNode* node)
{
    if (node == nullptr)
        return 0;

    int leftSum = 0, rightSum = 0;

    // shared() is used so that the result exists beyond the scope
    // firstprivate is used to preserve node value in omp
    #pragma omp task shared(leftSum) firstprivate(node)
    {
        leftSum = _sumTree(node->left);
    }

    #pragma omp task shared(rightSum) firstprivate(node)
    {
        rightSum = _sumTree(node->right);
    }

    #pragma omp taskwait // wait for tasks to complete

    // combine this node result with child results
    return node->value + leftSum + rightSum;
}

int sumTree(TreeNode *node)
{
    int totalSum = 0;

    // create one parallel region to use for *all* recursive calls
    #pragma omp parallel
    {
        #pragma omp single // ensure block is only called once
        totalSum = _sumTree(node);
    }

    return totalSum;
}

void testSumTree()
{
    TreeNode *root = new TreeNode(10);
    root->left = new TreeNode(20);
    root->right = new TreeNode(30);
    root->left->left = new TreeNode(40);
    root->left->right = new TreeNode(50);

    int totalSum = sumTree(root);
    assert(totalSum == 150);
}

int main()
{
    testFibonacci();
    testSumTree();

    cout << endl
         << __FILE__ " tests passed!" << endl;
    return 0;
}
