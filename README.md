# Lock-free k-LSM Relaxed Priority Queue

### Folder contents

Here are the contents of the root folder:

```
.
├── MidtermReport.pdf
├── README.md
├── README.pdf
├── a.out
├── compile.bat
├── example
├── include
│   ├── BTree.h
│   ├── BTree.h.gch
│   ├── LSM.h
│   ├── SkipList.h
│   └── SkipList.h.gch
├── irrelevant
│   └── klsm.cpp
├── klsm
├── makefile
├── obj
│   ├── BTree.o
│   ├── LSM.o
│   ├── SkipList.o
│   └── main.o
├── outputCPP.txt
├── src
│   ├── BTree.cpp
│   ├── LSM.cpp
│   ├── SkipList.cpp
│   └── main.cpp
└── testing
    └── skiplist-testing.cpp

```

### Compiling and running the program 

To compile, run `make` as shown here:
```bash
$ make
```
*This has been tested on gcc 8.2.0 on MacOS 10.14.2 and Ubuntu 18.04

### Testing

Tests are included in `skiplist-testing.cpp`. A better-structured and separate unit testing is in progress.

Compile the file as usual like so, and run the executable. Tests are included in the file mentioned above.

```bash
$ g++-8 tester skiplist-testing.cpp
```
*This has been tested on gcc 8.2.0 on MacOS 10.14.2 and Ubuntu 18.04






