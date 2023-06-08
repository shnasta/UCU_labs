# Lab work 1: Сpp performance
Authors (team):
- <a href="https://github.com/trlumph">_Tymur Krasnianskyi_</a>
- <a href="https://github.com/shnasta">_Anastasiia Shvets_</a>
- <a href="https://github.com/YarynaFialko">_Fialko Yaryna_</a>

## Prerequisites
### C++ prerequisites:
#### - cmake
#### - g++
#### - python3
### Python prerequisites
```
$ ./install_prerequisites.sh
```

## Compilation
```
$ ./compile.sh
```

## Usage
### Generate data
```
# python3 dataGenerator.py <N>
# Where N is the numbers of words. For example:
python3 dataGenerator.py 50000000
# It will generate file `data/numbers_50000000.txt`
```

<strong>OR take from here:</strong>
https://drive.google.com/drive/folders/1EW1LQNNY5z6-OzB5qaSFQuq7h8meNBdk


### Run C++ code

Read data from file
#### Usage: bin/performance_strings <method_number (1-5)> <input_file>
```
$ bin/performance_strings 4 data/numbers_50000000.txt
3865
394446189
```


Parse data from string to unsigned int
#### Usage: bin/performance_strings <method_number (1-5)> <input_file>
```
$ bin/performance_uints 4 data/numbers_50000000.txt
1417
51
```

### Run Python code
#### Usage: python prog1_runner.py <text_file> <num_executions> <flush_buffers> <output_file>

### Machine:
![](https://imgur.com/BTAsosm.png)
```
$ python3 prog1_runner.py data/numbers_50000000.txt 30 0 pvalues.txt
Method 1:
Min execution time: 4991 ms
Avg execution time: 7475.333333333333 ms
Corrected sample standard deviation: 1776.06 ms
Method 2:
Min execution time: 7502 ms
Avg execution time: 9565.2 ms
Corrected sample standard deviation: 1537.86 ms
Method 3:
Min execution time: 6892 ms
Avg execution time: 8167.266666666666 ms
Corrected sample standard deviation: 1262.18 ms
Method 4:
Min execution time: 3256 ms
Avg execution time: 4092.4666666666667 ms
Corrected sample standard deviation: 562.02 ms
Method 5:
Min execution time: 4061 ms
Avg execution time: 5198.566666666667 ms
Corrected sample standard deviation: 666.65 ms
```

```
$ python3 prog2_runner.py data/numbers_50000000.txt 30 0 pvalues.txt
Method 1:
Min execution time: 18307 ms
Avg execution time: 22725.0 ms
Corrected sample standard deviation: 3645.27 ms
Method 2:
Min execution time: 1210 ms
Avg execution time: 1809.6666666666667 ms
Corrected sample standard deviation: 331.94 ms
Method 3:
Min execution time: 2438 ms
Avg execution time: 2771.866666666667 ms
Corrected sample standard deviation: 570.36 ms
Method 4:
Min execution time: 905 ms
Avg execution time: 983.1666666666666 ms
Corrected sample standard deviation: 44.11 ms
Method 5:
Min execution time: 7314 ms
Avg execution time: 7589.633333333333 ms
Corrected sample standard deviation: 216.43 ms
```



## Results

### PVS-Studio results
Generally, PVS-Studio suggests us to avoid using some of the chosen methods for analysis.

### Valgrind results
Memcheck, a memory error detector
Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
Using Valgrind-3.18.1 and LibVEX;

### Machine:
![](https://imgur.com/2UURJtX.png)

### Parse from file
File `data/numbers_1000000.txt`

Method 1


==3928== HEAP SUMMARY:
==3928==     in use at exit: 0 bytes in 0 blocks
==3928==   total heap usage: 80 allocs, 80 frees, 67,193,909 bytes allocated
==3928== 
==3928== All heap blocks were freed -- no leaks are possible
==3928== 
==3928== <div style="color: green;">ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)</div>


Method 2

==3935== HEAP SUMMARY:
==3935==     in use at exit: 0 bytes in 0 blocks
==3935==   total heap usage: 96 allocs, 96 frees, 100,747,845 bytes allocated
==3935== 
==3935== All heap blocks were freed -- no leaks are possible
==3935== 
==3935== <div style="color: green;">ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)</div>

---
Method 3

==3938== HEAP SUMMARY:
==3938==     in use at exit: 0 bytes in 0 blocks
==3938==   total heap usage: 96 allocs, 96 frees, 100,747,845 bytes allocated
==3938== 
==3938== All heap blocks were freed -- no leaks are possible
==3938== 
==3938== <div style="color: green;">ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)</div>


---
Method 4

==3939== HEAP SUMMARY:
==3939==     in use at exit: 0 bytes in 0 blocks
==3939==   total heap usage: 81 allocs, 81 frees, 76,082,602 bytes allocated
==3939== 
==3939== All heap blocks were freed -- no leaks are possible
==3939== 
==3939== <div style="color: green;">ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)</div>

---
Method 5

==3942== HEAP SUMMARY:
==3942==     in use at exit: 0 bytes in 0 blocks
==3942==   total heap usage: 101 allocs, 101 frees, 107,539,872 bytes allocated
==3942== 
==3942== All heap blocks were freed -- no leaks are possible
==3942== 
==3942== <div style="color: green;">ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)</div>

---
### Parse from string
File `data/numbers_1000000.txt`

---
Method 1

==3958== HEAP SUMMARY:
==3958==     in use at exit: 0 bytes in 0 blocks
==3958==   total heap usage: 102 allocs, 102 frees, 84,471,206 bytes allocated
==3958== 
==3958== All heap blocks were freed -- no leaks are possible
==3958== 
==3958== <div style="color: green;">ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)</div>

---
Method 2

==3960== HEAP SUMMARY:
==3960==     in use at exit: 0 bytes in 0 blocks
==3960==   total heap usage: 102 allocs, 102 frees, 84,471,206 bytes allocated
==3960== 
==3960== All heap blocks were freed -- no leaks are possible
==3960== 
==3960== <div style="color: green;">ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)</div>

---
Method 3

==3963== HEAP SUMMARY:
==3963==     in use at exit: 0 bytes in 0 blocks
==3963==   total heap usage: 82 allocs, 82 frees, 80,082,602 bytes allocated
==3963== 
==3963== All heap blocks were freed -- no leaks are possible
==3963== 
==3963== <div style="color: green;">ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)</div>

---
Method 4

==3965== HEAP SUMMARY:
==3965==     in use at exit: 0 bytes in 0 blocks
==3965==   total heap usage: 102 allocs, 102 frees, 84,471,206 bytes allocated
==3965== 
==3965== All heap blocks were freed -- no leaks are possible
==3965== 
==3965== <div style="color: green;">ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)</div>

---
Method 5

==3970== HEAP SUMMARY:
==3970==     in use at exit: 0 bytes in 0 blocks
==3970==   total heap usage: 102 allocs, 102 frees, 84,471,206 bytes allocated
==3970== 
==3970== All heap blocks were freed -- no leaks are possible
==3970== 
==3970== <div style="color: green;">ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)</div>

### Profiling results

#### Parse from file
Method 1:

![Method 1](https://imgur.com/ox8QBUj.png)

Method 2:
![Method 2](https://i.imgur.com/1XQp3nH.png)

Method 3:
![Method 3](https://i.imgur.com/5TrNnFF.png)

Method 4:
![Method 4](https://imgur.com/kSranTi.png)

Method 5:
![Method 5](https://imgur.com/GksZAR4.png)

#### Parse from string
Method 1:
![Method 1](https://imgur.com/luhcum4.png)

Method 2:
![Method 2](https://imgur.com/GksZAR4.png)

Method 3:
![Method 3](https://imgur.com/dzAOsa1.png)

Method 4:
![Method 4](https://imgur.com/pE6bXiU.png)

Method 5:
![Method 5](https://imgur.com/sQ88BFD.png)

# Conclusion

Based on the results presented, we can observe that parsing from strings was generally faster and more memory efficient than parsing from files. Among the methods used to parse from files, Method 4 was the fastest and most memory efficient, with an average execution time of 4092.47 ms and a total heap usage of 76,082,602 bytes allocated. Among the methods used to parse from strings, Method 4 was also the fastest and most memory efficient, with an average execution time of 983.17 ms and a total heap usage of 84,471,206 bytes allocated.

It is also worth noting that there was significant variation in the execution times and total heap usage between the different methods used for parsing, indicating that the efficiency of a parsing method can depend on various factors such as the size of the input data and the format of the input data. Therefore, it is important to carefully consider the specific requirements and constraints of a given task when selecting a parsing method.






