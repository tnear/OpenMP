# OpenMP
OpenMP (Open Multi-Processing) is an API for shared-memory multiprocessing programs.

https://www.openmp.org/

OpenMP uses fork-join parallelism where the main thread spawns a team of threads which are joined after completing execution.

## Summary
- atomic: fastest, but only supports ++, --, +=, *=, etc. See also: std::atomic.
- barrier: waits for all threads (joins).
- critical: critical section. Slower than atomic, but supports all operators. See also: std::mutex
- reduction: ex: `omp parallel for reduction(+ : result)`.
- section: used for rigid parallelism where number of threads is known at compile-time.
- task: better for irregular parallelism, such as through recursion

## Building
Use `Ctrl+Shift+B`

## Running
Use either:
- `Ctrl+F5`
- `./<executable>.exe`

## Debugging
Generally not supported.
