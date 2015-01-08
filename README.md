#Manak: C++ Unit Benchmarking Library

##Introduction

Manak unit benchmarking library provides both easy to use and flexible solution
to the problem of benchmarking. Unit benchmarking comes handy for projects 
with continuous integration. With Manak you can save the benchmarks of your
current implementation and compare them against the later ones. You can also 
benchmark your library against any other library. Manak offers a hierarchical 
structure similar to BOOST Unit Testing and generates a easy to read output file.
Manak supports HTML log generation along with txt logs. Manak is header only 
library and highly uses c++11 features. 

###Check out [Documentation](http://Manak-org.github.io/Manak/) for tutorials on Manak C++ Benchmarking.

##How To Install

###For Linux
1. Create directory build.
2. cd into build and run command _**cmake ../**_
3. For building from source run command _**make**_, this will create executable kif
   and flatten inside bin.
4. To install run command _**sudo make install**_

###For Windows
Manak does not have any external dependency. Hence you can use the CMake GUI to
install the header files to your environment. Or you can simple copy the folder 
named **manak**(_./src/manak_) to the include folder of your IDE.

##Quick Guide

A simple benchmarking case is given below -   

```cpp
#include <iostream>

#define MANAK_SIMPLE_BENCHMARK_MODULE lib
#define MANAK_AUTO_BENCHMARK_MAIN

#include <manak/manak.hpp>

MANAK_AUTO_BENCHMARK_CASE(ForLoops)
{
  for(size_t i = 0;i < 10000;i++);
}
```
This will create a benchmark case with name 'ForLoops' which will time the 
for loop given inside. The default iterations are 10. Run the genrated executable 
by passing '-h' for more options.   

The ouput will be written to file 'benchmark_stat.html'.Here the function code 
inside the auto case will be timed. For More time sensetive code you can use 
'Measure' macro. For example -  

```cpp
#include <iostream>

#define MANAK_SIMPLE_BENCHMARK_MODULE lib
#define MANAK_AUTO_BENCHMARK_MAIN

#include <manak/manak.hpp>

MANAK_AUTO_BENCHMARK_CASE(ForLoops)
{
  Setup code;
  
  Measure
  (
    Code to measure;
  )
  
  More code...
}
```

This usage will only measure the code inside the measure block. There can be 
more that one measure blocks. For more time related options check out the 
complete guide.

To add already existing function to benchmarking -

```cpp
#include <iostream>

#define MANAK_SIMPLE_BENCHMARK_MODULE lib
#define MANAK_AUTO_BENCHMARK_MAIN

#include <manak/manak.hpp>

int fun()
{
  Code;
}

MANAK_ADD_BENCHMARK(MANAK_BENCHMARK_CASE(B1, fun));
```
Remember Measure blocks can also be used in functions such as 'fun' here.

To create benchmark suite -

```cpp
#include <iostream>

#define MANAK_SIMPLE_BENCHMARK_MODULE lib
#define MANAK_AUTO_BENCHMARK_MAIN

#include <manak/manak.hpp>

MANAK_AUTO_BENCHMARK_SUITE(Suite1);

MANAK_AUTO_BENCHMARK_CASE(B1)
{
  for(size_t i = 0;i < 1000;i++);
}
```

To compare 2 or more libraries against each other -

```cpp
#include <iostream>

#define MANAK_BENCHMARK_MODULE lib
#define MANAK_AUTO_BENCHMARK_MAIN

#include <manak/manak.hpp>

MANAK_AUTO_BENCHMARK_CASE(B1, lib1)
{
  for(size_t i = 0;i < 1000;i++);
}

MANAK_AUTO_BENCHMARK_CASE(B2, lib2)
{
  for(size_t i = 0;i < 10000;i++);
}
```

Functions with arguments can be used as templates. Templates are useful when 
the same function is used for benchmarking with different parameters.
Consider the next example -

```cpp
#include <iostream>

#define MANAK_SIMPLE_BENCHMARK_MODULE lib
#define MANAK_AUTO_BENCHMARK_MAIN

#include <manak/manak.hpp>

int fun(int a, int b)
{
  Code
}

MANAK_ADD_BENCHMARK(MANAK_CREATE_BENCHMARK_WITH_TEMPLATE(B1, fun)->AddArgs(0,0)->AddArgs(1,1)...);
```

Here the banechmark case will contain many unnamed cases. When number of argument
sets is large you can add all the argument sets by a function. The function must
return list of tuples.
For example -

```cpp
#include <iostream>
#include <tuple>
#include <list>

#define MANAK_SIMPLE_BENCHMARK_MODULE lib
#define MANAK_AUTO_BENCHMARK_MAIN

#include <manak/manak.hpp>

using namespace std;

int fun(int a, int b)
{
  Code;
}

list<tuple<int, int>> get_args()
{
  list<tuple<int, int>> out;
  for(size_t i = 0;i < 100;i++)
    out.emplace_back(i, i);
  return out;
}

MANAK_ADD_BENCHMARK(MANAK_CREATE_BENCHMARK_WITH_TEMPLATE(B1, fun)->AddCustomArgs(get_args));
```

Check out the complete guide for more template related options.

The genrated timings can be saved and added for comparison for later runs. For
more options check out the Complete Guide.
