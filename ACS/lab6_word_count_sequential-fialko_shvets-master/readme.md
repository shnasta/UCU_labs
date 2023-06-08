# Lab work 6: Consecutive word count - archives, Unicode
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
* C - bool whether clean cash
```
$ python3 ./prog_runner.py N C
```

### Run C++ code
Where 
* P - path to configuration file
```
$ ./bin/countwords_seq P
```
#### Example
```
$ ./bin/countwords_seq ../config_file.cfg
Total=1353123
Writing=40027
```

### Results

<mark>In this lab we learned how to index directories, work with Unicode, tokenize words with boost.locale, and count them concequtively. 

We checked its accuracy by ciunting by hands :D, so it works just right!</mark>

