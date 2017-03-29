## cpptest
A simple set of programs that I use to check the compiler and cmake setup on each new machine.

### Basic check
Hello world - requires just a working compiler and cmake

### Basic check + MPI
Hello world mpi - cpp and mpi check
```
mpiexec -n 4 ./hellompi
```

### Boost checks

#### serialize 
simple test from boost examples

#### program options 
allows a list to be passed in : try 
```
./options -i 2 3 1 4 5 -j 3 2 4 5 6
```
#### filestem checks
fs-tut1 fs-tut2 fs-tut3 fs-tut4 fs-tut5 : taken from boost examples : try 
```
./fs-tut4 $HOME
```

#### Threads and lockfree tests
threads, atomic queue stack waitfreequeue atomicity atomic_ordering lockfree stdqueue

All taken from boost examples off the web

#### Regex
regex-recursion : basic unit test. Don't need this any more, use std::regex instead

#### Accumulators/statistics
histogram : simple demo of a histogram generated from random data.

Also includes a demo of a very nice fixed bin size histogram found here [boost issue 12832](https://svn.boost.org/trac/boost/ticket/12832)
  
### Raspberry Pi test
pi-lockfree : dumps out some setting that the cross compiling framework should set, 
this was caused by boost::lockfree not working on Pi at first and needing a way to check alignment settings.
Also extended with a few other #defines that occasionally are interesting.
