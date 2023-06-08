# Lab work 2. Make build system, CMake automation. Manual compilation
> Authors (team): [Shvets Anastasiia](https://github.com/shnasta), [Fialko Yaryna](https://github.com/YarynaFialko), [Kazymyr Arsenii](https://github.com/Arsenii6666), [Sofia Knyshoyid](https://github.com/Sofia-Knyshoyid)
> 
> Variant: `1`
## Prerequisites

* make, CMake, and compiler with C11, C++14 support
* bash

## Compilation

You can compile our project natively on Unix systems or using WSL on Windows.
> How to install WSL: https://learn.microsoft.com/en-us/windows/wsl/install

### 1. Bash

Run ```./bashfile.sh``` from the project directory to compile `mystring` project.

Results of compilation will be placed in ```./compiled_files```.

![image](https://user-images.githubusercontent.com/92580927/196960790-e537e510-fd63-44b2-b6f8-c3a9f9f32e62.png)

Run ```./bashfile_jpeg.sh``` from the project directory to compile `jpeg` project.

Results of compilation will be placed in `./compiled_files_jpeg/compiled_library` and `./compiled_files_jpeg/compiled_library`.

![image](https://user-images.githubusercontent.com/49194660/197029470-c1985012-a452-4a68-bc3d-187b5e85fef8.png)



### 2. Make

First, run ```make``` from the ```./library``` directory to compile a library of mystring project.

Results of compilation will be placed in ```./library/compiled_files```.

![image](https://user-images.githubusercontent.com/92580927/196963686-28d2feee-a33c-497b-b0dd-7b74b39eec5b.png)


Next, run ```make``` from the ```./example``` forder to create an executable file. 

Results of compilation will be placed in ```./example/compiled_files```. 

![image](https://user-images.githubusercontent.com/92580927/196964148-4d25dddf-1b9f-4bb0-bfbb-e23e88658f64.png)

> **Warning**
>
> Note that sequence is important here. Running `Makefile` in library path first is obligatory.

#### Optimisation

To compile the examples or library with optimization consider running compilation in release mode by passing `release` option to make 
```bash
cd library
make release
```

![image](https://user-images.githubusercontent.com/49194660/197080809-efc1f2e4-ea40-4c63-b739-72ec2340a0ec.png)

and

```bash
cd ../example
make release
```

![image](https://user-images.githubusercontent.com/49194660/197080854-a7e97454-8b57-41ff-aaf6-2d9b999ff1b1.png)

#### Clean 

To delete the results of compilation run ```make clean``` in ```./library``` directory and in ```./example``` respectively.

![image](https://user-images.githubusercontent.com/92580927/196969048-983779ae-d6d2-488a-9eb7-750567f9fd5d.png)

![image](https://user-images.githubusercontent.com/92580927/196969881-8bafc6f3-39e2-4d1f-8df1-f2e271d7b599.png)


#### Library `jpeg`

Similarly, compilation of `jpeg` library (other popular C library from the task)


First, run ```make``` from the ```./other_library/library``` directory to compile a library.

Results of compilation will be placed in ```./other_library/library/compiled_files```.

![image](https://user-images.githubusercontent.com/92580927/196971272-b7f1ee92-7ba0-40a3-9254-f2b438fa5853.png)

Next, run ```make``` from the ```./other_library/example``` forder to create executable files. 
Results of compilation will be placed in ```./other_library/example/compiled_files```.

![image](https://user-images.githubusercontent.com/92580927/196972087-76e804f7-debb-460e-ac47-71b4388da1a3.png)

#### Optimization

To compile the examples or library with optimization consider running compilation in release mode by passing `release` option to make
```bash
cd other_library/library
make release
```

![image](https://user-images.githubusercontent.com/49194660/197080956-a1517c59-b3fb-433e-94d6-6fe369e73d68.png)

and

```bash
cd ../example
make release
```

![image](https://user-images.githubusercontent.com/49194660/197081026-6c4cd262-a189-46ae-9182-17a886bad142.png)

#### Clean

To delete the results of compilation run ```make clean``` in ```./other_library/library``` directory and in ```./other_library/example``` respectively.

![image](https://user-images.githubusercontent.com/92580927/196973145-c60da516-089d-4df2-b6bf-5716988d7b06.png)


### 3. CMake
Firstly, we can run cmake for our library of mystring project.
create new directory, run "cmake .. && make" from there
![Screenshot from 2022-10-20 20-55-56](https://user-images.githubusercontent.com/87498412/197031190-d4093cf1-c018-4b7c-8a65-c84d6ab4dca7.png)

The compiled executable file, "mystring_example", will be placed in "example" subdirectory
![Screenshot from 2022-10-20 20-56-27](https://user-images.githubusercontent.com/87498412/197031415-f2aa6c7b-4801-4f78-ab3d-e8dbb6525fd8.png)

Similarly, we can follow the same steps for `jpeg` library
![Screenshot from 2022-10-20 21-01-32](https://user-images.githubusercontent.com/87498412/197031651-60df74df-698e-458c-8368-e70749ca4598.png)

The resulting executable files, "cmake_new_lib_cjpeg" and "cmake_new_lib_djpeg", will be placed in "example" subdirectory
![Screenshot from 2022-10-20 21-04-38](https://user-images.githubusercontent.com/87498412/197031865-615ecfbb-7ab2-4620-badf-870e093943a0.png)
