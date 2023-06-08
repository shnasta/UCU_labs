# Lab 4. Assembly
Team: [Andrii Yaroshevych](https://github.com/andylvua), [Anastasiia Shvets](https://github.com/shnasta)<br>
Variant func_1: 2<br>
Variant func_2: 7<br>
OS: LINUX

## Prerequisites

* GCC 4.9 or higher
* FASMG - _provided in the repository_
* Git
* Make

## Compilation

To compile the project, you need to clone the repository first:
```bash
$ mkdir ~/workspace
$ cd ~/workspace
$ git clone https://github.com/ucu-cs/lab4_asm_func-asm_yaroshevych_shvets
$ cd lab4_asm_func-asm_yaroshevych_shvets
```
> Replace `~/workspace` with the path to your workspace.
>
> Remember, if you want to change the name of the folder you clone the repository to,
> you can simply add the name of the folder after the repository URL.
> ```bash
> $ git clone https://github.com/ucu-cs/lab4_asm_func-asm_yaroshevych_shvets folder_name
> ```

Then, you can compile the project using the following command from the root of the project:
```bash
$ make
```

This will automatically run compilation of the project files. 
Executable files will be placed in the `bin` folders of the corresponding tasks.

## Usage

To run the program after compilation, you need to execute the following command from the root of the project:
```bash
$ ./func_1/bin/call_from_c
$ ./func_1/bin/call_from_a
```

and

```bash
$ ./func_2/bin/call_from_c
$ ./func_2/bin/call_from_a
```

## License
The [MIT](https://choosealicense.com/licenses/mit/) License (MIT)

Copyright © 2022. Andrii Yaroshevych, Anastasiia Shvets
