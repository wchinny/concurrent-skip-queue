# Lock-free k-LSM Relaxed Priority Queue

### Folder contents

Here are the contents of the root folder:

```
.
├── README.md
├── klsm.cpp
├── skiplist-testing.cpp
└── skiplist.cpp
```

### Compiling and running the program 

To compile, use the g++ compiler as shown here:
```bash
$ g++-8 skiplist skiplist.cpp
```
*This has been tested on gcc 8.2.0 on MacOS 10.14.2 and Ubuntu 18.04

### Testing

Tests are included in `skiplist-testing.cpp`. A better-structured and separate unit testing is in progress.

Compile the file as usual like so, and run the executable. Tests are included in the file mentioned above.

```bash
$ g++-8 tester skiplist-testing.cpp
```
*This has been tested on gcc 8.2.0 on MacOS 10.14.2 and Ubuntu 18.04





