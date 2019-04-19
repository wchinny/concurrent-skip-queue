# Lock-free k-LSM Relaxed Priority Queue

### Folder contents

Here are the contents of the root folder:

```
.
├── README.md
├── bench-data
│   ├── concurrent_version
│   └── transactional_version
├── bench.py
├── concurrent
│   ├── BTree.cpp
│   ├── BTree.h
│   ├── DLSM.h
│   ├── LockFreeSkiplist.h
│   ├── SkipList.h
│   └── main.cpp
├── documents
├── makefile [recommended]
├── transactional
│   ├── DLSM.h
│   ├── SharedSkipList.h
│   ├── SkipList.h
│   ├── a.out
│   ├── main.cpp
│   └── main.o
├── windows_compile_concurrent.bat [not guaranteed, this project is recommended to be run on Linux/ MacOS]
└── windows_compile_stm.bat [not guaranteed, this project is recommended to be run on Linux/ MacOS]

```
`README.md` -> This file

`bench-data` -> Folder containing benchmark graphs, for both concurrent and transactional versions

`bench.py` -> Benchmark tool. Usage is detailed in the `.py` file as comments

`concurrent` -> Contains the source code for the concurrent version of this algorithm

`documents` -> Related documents

`makefile` -> Makefile

`transactional` -> Contains the source code for the software transactional memory (STM) version of this algorithm

`windows_compile_concurrent.bat` -> Compiling script for the conccurent version for windows machines (where `make` is not available)

`windows_compile_stm.bat` -> Compiling script for the STM version for windows machines (where `make` is not available)

### Compiling the program (Linux/MacOS)

To compile, run `make` as shown here:
```bash
$ make concurrent # build the concurrent version --or--
$ make transactional_version # build the transactional (STM) version
```

The executable will be name `klsm`, which will be in the root folder. 

### Running the executable

`klsm` takes two command-line arguments-- first one being the number of threads, and the second one being the number of operations
```bash
$ ./klsm 4 100000

```








