# Lab work 10: Two-dimensional heat conduction problem using MPI
Authors (team):
* Yaryna Fialko
* Anastasiia Shvets

> Link to the report: https://docs.google.com/document/d/1FofHkk4AXTspQOSoltkr-gVgV-uTNj5MKYqKRjZFWc4/edit#heading=h.i9nydzu35j87

## Prerequisites
- cmake
- mpi
- png++
- Magick++

### Compilation

You can compile using cmake or by the following command from the project root directory:
```
./compile.sh -O
```
### Configuration file

You can configure the program by changing the configuration file `config_file.cfg` in the project root directory. The file contains the following parameters:
- dens - density of the material 
- h_cap - heat capacity of the material
- conduct - thermal conductivity of the material
- height - height of the matrix
- width - width of the matrix
- dx - step by x
- dy - step by y
- dt - step by time
- dt_gif - step by time for .png generation
- first_state_path - path to the file with the first state of the matrix
- total_time - total time of the simulation relating to the step by time

### Usage

Run from the project root directory. This will use your computer two run for 3 different services:
```
mpirun -np 3 ./bin/mpi ./config_file.cfg  
```
Or you can specify other host computer for executing servises:
```
mpirun -np 3 --host nod1 --host node2 --host node3 ./bin/mpi ./config_file.cfg  
```

### Result

#### Gif animation example
![point near border](./img/animation.gif)
![point at the center](./img/animation1.gif)

#### Execution time

Time of execution for 3 services on 1 computer: `202s`

Time of execution for 3 services on 3 different computers: `278s`
