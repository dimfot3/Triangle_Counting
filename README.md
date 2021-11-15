# Triangle_Counting

This is the first project of Parallel and Distributed Systems lesson of Aristotle University of Thessaloniki, department of Electrical and Computer Engineering. The main of goal of this project is to utilize and optimize the calculation of A*(A^2) formula which is used for triangle counting in graph theory, where A is a sparse symmetric adjency matrix. In this repo there is a sequential masked implementation and also some parallel versions of it written in C using pthreads, OpenMP and OpenCilk. Also there is similar implementation in Julia.
<br />

## Depedencies
- For Sequential masked implementation and Pthread :
1) gcc >= 8.1
2) cmake >= 3.12
 
- For OpenMP implementation additional requirements:
1) OpenMP library ```sudo apt install libomp-dev```

- For OpenCilk implementation additional requirements:
1) Download clang compiler https://github.com/OpenCilk/opencilk-project/releases/tag/opencilk/v1.0. 
   OpenCilk is deprecated in gcc.
   
- For julia implementation additional requirements:
1) install julia >= 1.5.1
2) run the ```julia requirements.jl``` inside julia_implementation folder to install the requirements


## Build
To run this project run
1) create a build directory inside the code folder: ```cd code && mkdir build && cd build``` <br />
2) run ```cmake ..``` <br />
a) to dowload some default datasets add the option ```-DGET_DATASET=ON``` <br />
b) to build tests add option ```-DBUILD_TESTING=ON``` <br />
c) to build cilk implementationadd the full path of your clang and clang++ compiler in lines https://github.com/dimfot3/Triangle_Counting/blob/00c4d2b9964c85f14fd47d31c8ccdb29a8e91718/code/Cilk/CMakeLists.txt#L4
and then add in cmake the option ```-DCILK=ON``` <br />

## RUN
- In order to run the sequential/pthread and openMP implementation:
  1) run ```./tri_counter o b nl nt fm``` 
 where o is the implementation id(0: sequential, 1:pthreads and 2:openmp) <br />
 b is the benchmark options(0:time, 1:scalability (for parallelism only)) <br />
 nl: the number of loops (or samples) <br />
 nt: the number of threads <br />
 fm: full mat(0:gets only the lower triangular matrix, 1:gets the full mat)
  2) the results of the session are saved inside build/results/ with the name of the current day and time <br />
- In order to run the openCilk implementation: <br />
  1) run ```./Cilk/cilk_implementation b nl nt fm``` <br />
     Here the the options b, nt are deprecated as the number of threads changes only with enviroment variable CILK_NWORKERS <br />
  2) To run scalability session run ./run_cilk.sh inside the build folder and the results will saved in buils/results
- In order to run Julia implementation: <br />
  1) run ```./julia_test.sh``` in the julia_imeplementation folder and the results of scalability test will be saved in same folder

## Testing
There are some google tests to verify correctness of basic utilities. 
In order to run the tests:
1) follow the Build guide to build them
2) run make test

