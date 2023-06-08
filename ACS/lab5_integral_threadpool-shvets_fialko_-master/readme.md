# Lab work 5: Integration using threadpool
Authors (team):
- <a href="https://github.com/shnasta">_Anastasiia Shvets_</a>
- <a href="https://github.com/YarynaFialko">_Yaryna Fialko_</a><br>
## Prerequisites

### C++ prerequisites:
#### - cmake
#### - g++
#### - python3
### Compilation
```
$ ./compile.sh -O
```

### Run Python code

Python script run where
* N - number of executions
* M - number of threads
* P - points per task:
```
$ python3 ./prog_runner.py N M P
```

#### Example
```
$ python3 ./prog_runner.py 10 6 100
Function 1:
Result of integration: 4545450.0
Absolute error: 0.0230791
Relative error: 5.07742e-09
Min execution time: 39 ms
Avg execution time: 39.9 ms
Corrected sample standard deviation: 1.20 ms

Function 2:
Result of integration: 857208.0
Absolute error: 0.0011704
Relative error: 1.36536e-09
Min execution time: 256 ms
Avg execution time: 388.1 ms
Corrected sample standard deviation: 57.27 ms

Function 3:
Result of integration: -1.60467
Absolute error: 3.71871e-05
Relative error: 2.31743e-05
Min execution time: 6 ms
Avg execution time: 6.1 ms
Corrected sample standard deviation: 0.32 ms
```
### Run C++ code
Where 
* N - number of function.
* M - number of threads.
* P - points per task.
```
$ ./bin/integrate_parallel_tpool N ./funcN.cfg M P
```
#### Example
```
$ ./bin/integrate_parallel_tpool 1 func1.cfg 3 100
4.54545e+06
0.0230791
5.07742e-09
91
```

### General time of execution
> For func 2 it is used 500 points per task, for func 1 and 3 - 100 points per task.

#### Thread pool
We can see that the time of execution generally is the smallest for number of threads between 4 and 16.
```
function 1 with 1 threads: 153 ms
function 1 with 2 threads: 77 ms
function 1 with 3 threads: 66 ms
function 1 with 4 threads: 47 ms
function 1 with 5 threads: 45 ms
function 1 with 6 threads: 42 ms
function 1 with 7 threads: 38 ms
function 1 with 8 threads: 36 ms
function 1 with 9 threads: 35 ms
function 1 with 10 threads: 39 ms
function 1 with 11 threads: 36 ms
function 1 with 16 threads: 50 ms
function 1 with 20 threads: 46 ms
function 1 with 25 threads: 44 ms
function 1 with 100 threads: 251 ms

function 2 with 1 threads: 511 ms
function 2 with 2 threads: 234 ms
function 2 with 3 threads: 161 ms
function 2 with 4 threads: 203 ms
function 2 with 5 threads: 206 ms
function 2 with 6 threads: 206 ms
function 2 with 7 threads: 208 ms
function 2 with 8 threads: 213 ms
function 2 with 9 threads: 211 ms
function 2 with 10 threads: 227 ms
function 2 with 11 threads: 210 ms
function 2 with 16 threads: 251 ms
function 2 with 20 threads: 348 ms
function 2 with 25 threads: 644 ms
function 2 with 100 threads: 1181 ms

function 3 with 1 threads: 58 ms
function 3 with 2 threads: 12 ms
function 3 with 3 threads: 7 ms
function 3 with 4 threads: 12 ms
function 3 with 5 threads: 6 ms
function 3 with 6 threads: 8 ms
function 3 with 7 threads: 6 ms
function 3 with 8 threads: 6 ms
function 3 with 9 threads: 8 ms
function 3 with 10 threads: 7 ms
function 3 with 11 threads: 6 ms
function 3 with 16 threads: 10 ms
function 3 with 20 threads: 10 ms
function 3 with 25 threads: 61 ms
function 3 with 100 threads: 101 ms
```


#### Threadsafe queues
Results from previous lab using threadsafe queues directly:
```
funtion 1 with 1 threads: 166 ms
funtion 1 with 2 threads: 86 ms
funtion 1 with 4 threads: 59 ms
funtion 1 with 8 threads: 338 ms
funtion 1 with 16 threads: 327 ms
funtion 1 with 100 threads: 304 ms

funtion 2 with 1 threads: 1256 ms
funtion 2 with 2 threads: 1366 ms
funtion 2 with 4 threads: 1798 ms
funtion 2 with 8 threads: 20325 ms
funtion 2 with 16 threads: 20430 ms
funtion 2 with 100 threads: 19055 ms

funtion 3 with 1 threads: 32 ms
funtion 3 with 2 threads: 29 ms
funtion 3 with 4 threads: 37 ms
funtion 3 with 8 threads: 328 ms
funtion 3 with 16 threads: 408 ms
funtion 3 with 100 threads: 321 ms
```

We can see that threadpool is more efficient than threadsafe queues.
### Efficiency
![res_plot1.png](plots%2Fres_plot2_1.png)
![res_plot2.png](plots%2Fres_plot2_2.png)
![res_plot3.png](plots%2Fres_plot2_3.png)

### Results
In this lab we learned how to implement a threadpool and applied it to in parallel integrate functions. Also, we visualized the efficiency of the method via graph and compared it with the previous approach from lab3 and lab4.
