#Manak: C++ Unit Benchmarking Library

[![Gitter](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/Manak-org/Manak?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)        ![build status](https://travis-ci.org/Manak-org/Manak.svg?branch=master)

###Latest Stable Release: [Manak 2.0.0](https://github.com/Manak-org/Manak/archive/v2.0.0.zip)

###Contact Us:
 - Join IRC channel #manak on Freenode
 - Get in touch with Gitter
 - Or you can just drop a mail on sumedhghaisas@gmail.com

##Introduction

Manak unit benchmarking and testing library provides both easy to use and flexible 
API. As both benchmarking and testing framework are combined into one, Manak comes
handy for projects with continuous integration. Manak implements xUnit achitecture
and improves upon it.

Manak offers following unique features - 
 - Timing and testing both in a single unit
 - Comparison of implementation along time(for checking improvent)
 - Manak groups for removing code redundancy
 - Implements framework for randomized tests
 - Complete regular expression support for running specific cases
 - Supports HTML output generating, supports mulyipe output generating
 - Supports both Static and Dynamic Linkage
 - Implemented in C++11 and scalable to C++14
 - No external dependency(Specially for Windows Users :D)
 
##The Idea Behind Manak
'Manak' in Sanskrit means standard. Manak aims at a framework for standardization
of code. Currently Manak combines the 2 most important requirements for any C++
project, testing and benchmarking. Manak understands that benchmarking is not just 
timing, it must also be supported with a rich comparison framework. And this is 
not the end. The next milestone is to integrate the memory mesurement tools. With 
proper steps, we will build a one stop solution for all continuous integration 
issues. 
 
###Check out [Wiki](https://github.com/Manak-org/Manak/wiki) for User Guide.

###Check out [Developer's Guide](http://Manak-org.github.io/Manak/html/index.html).

###Check out [GSOC 2015 Ideas Page](Google Summer of Code 2015 Ideas Page)

##How To Install

1. Create directory build.
2. cd into build and run command _**cmake ../**_
3. For building from source run command _**make**_, this will create executable kif
   and flatten inside bin.
4. To install run command _**sudo make install**_

Of course, all this is unneccessary if you want to use only static linkage. 
Just copy the src/manak folder to the default search path and you are done.

##An Overview

A simple case is given below -   

```cpp
#define MANAK_SIMPLE_MODULE mod1
#define MANAK_AUTO_MAIN
#define MANAK_INIT

#include <manak/manak.hpp>

MANAK_AUTO_BENCHMARK_CASE(B1)
{
  MEASURE
  (
    for(size_t i = 0;i < 10000;i++);
  )
  
  TEST
  (
    MANAK_ASSERT_TRUE(1 == 1);
  )
}
```
This will create a benchmark case with name 'B1' which will time the 
for loop given inside 'MEASURE' and run the tests given inside 'TEST' block. 
The case is run for certain number of iterations, the default is 10. The iterations
help to gt average score and also for randomized tests, where sometimes x out of
n times the test has to be passed.  

The ouput will be written to file 'benchmark_stat.html'. TThe output is easy to
read and verbose. 

```cpp
#define MANAK_MODULE mod1
#define MANAK_AUTO_MAIN
#define MANAK_INIT

#include <manak/manak.hpp>

MANAK_AUTO_BENCHMARK_CASE(B1, With10000)
{
  MEASURE
  (
    for(size_t i = 0;i < 10000;i++);
  )
  
  TEST
  (
    MANAK_ASSERT_TRUE(1 == 1);
  )
}

MANAK_AUTO_BENCHMARK_CASE(B1, With100000)
{
  MEASURE
  (
    for(size_t i = 0;i < 100000;i++);
  )
  
  TEST
  (
    MANAK_ASSERT_TRUE(1 == 1);
  )
}
```

The above code will measure the time taken by the loops and compare them against 
each other in the output. Observe that the case name is same and the second 
argument(the library name in Manak hierarchy) is different. 

See that its a code redundancy. The solution is through Groups - 

```cpp
template<size_t index>
MANAK_GROUP(TestGroup);

size_t iter;
std::string l_name;

GINIT(size_t n, const std::string& str)
{
  iter = n;
  name = str;
}

MANAK_AUTO_GROUP_BENHMARK_CASE("ForLoops", l_name)
{
  MEASURE
  (
    for(size_t i = 0;i < iter;i++);
  )
}

MANAK_AUTO_GROUP_BENCHMARK_CASE("Test", "Lib1")
{
  //Can use iter and name here
} 

MANAK_GROUP_END();

MANAK_ADD_GROUP(TestGroup<1>, 1000, "With1000");
MANAK_ADD_GROUP(TestGroup<2>, 10000, "With10000");
```

Another important usgae of groups is that the cases inside can share variables.
Imagine that you are testing a variable which takes lot of time to create. 
So rather than wasting time while creating it in each case create it in 'GINIT'
and usage is freely inside the group. Groups act like c++ Classes. Example of
this is also shown in the above code.

Sometimes you don't want to compare the code against each other but time and test
a function with many different parameters. Maybe while regression testing.
For this parametrized benchmarks can be used.

```cpp
void fun(ize_t a, size_t b)
{

}

MANAK_ADD_CASE(MANAK_CREATE_BENCHMARK_WITH_TEMPLATE(B1, fun)->AddArgs(0, 0)->AddArgs(1, 1));
```

What if you want to test it against 100 set of parameters -

```cpp
std::list<std::tuple<size_t, size_t>> GetArgs()
{
  std::list<std::tuple<size_t, size_t>> out;

  for(size_t i = 0;i < 100;i++)
    out.emplace_back(i, i);
    
  return out;
}

MANAK_ADD_CASE(MANAK_CREATE_BENCHMARK_WITH_TEMPLATE(B1, fun)->AddCustomArgs(GetArgs));
```

Check out the the wiki for many more such features.
