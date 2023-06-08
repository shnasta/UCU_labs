<mark>Template for your README. Remove all unused parts and instructions</mark>

# Lab work <mark>NUMBER</mark>: <mark>SHORT TOPIC</mark>
Authors (team): <mark>
- <a href="https://github.com/shnasta">_Anastasiia Shvets_</a>
- <a href="https://github.com/YarynaFialko">_Yaryna Fialko_</a></mark><br>

## Prerequisites

### C++ prerequisites:
#### - cmake
#### - g++
#### - python3
### Compilation

```
$ ./compile.sh
```

### Run Python code

Python script run (where n - number of executions): 
```
$ python3 ./prog_runner.py  n
```

### Run C++ code
Where N - number of function.
```
$ ./bin/integrate N ./funcN.cfg
```
### Usage
![image](https://user-images.githubusercontent.com/92580927/221668058-6e69b26d-b528-4ec8-a1b9-0396439f6c50.png)

```
$ python3 ./prog_runner.py  10
Function 1:
Result of integration: 4545450.0
Absolute error: 0.0230791
Relative error: 5.07742e-09
Min execution time: 198 ms
Avg execution time: 203.0 ms
Corrected sample standard deviation: 6.94 ms

Function 2:
Result of integration: 857208.0
Absolute error: 0.00117039
Relative error: 1.36535e-09
Min execution time: 918 ms
Avg execution time: 1033.6 ms
Corrected sample standard deviation: 131.67 ms

Function 3:
Result of integration: -1.60466
Absolute error: 4.82406e-06
Relative error: 3.00629e-06
Min execution time: 71 ms
Avg execution time: 73.6 ms
Corrected sample standard deviation: 2.07 ms
```
### Results

<mark>We wrote a sequential code for the integration of the specified functions and measured the time of its operation.</mark>

