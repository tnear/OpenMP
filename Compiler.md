# Compiler

Compiling with the `-fopenmp` flag translates the `#pragma omp` constructs into native threads.

https://www.openmp.org/wp-content/uploads/Intro_To_OpenMP_Mattson.pdf

For example, when the compiler sees this function:
```cpp
#pragma omp parallel num_threads(4)
{
    myFcn();
}
```

...it may translate it into:
```cpp
void thunk()
{
    myFcn();
}

pthread_t tid[4];
for (int i = 0; i < 4; ++i)
    pthread_create(&tid[i], 0, thunk, 0);

for (int i = 0; i < 4; ++i)
    pthread_join(tid[i]);
```
