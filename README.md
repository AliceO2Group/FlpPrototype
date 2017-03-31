## Project description

This project provides the software and tools necessary to run a data acquisition system for the ALICE O2.

It is made of a number of modules. Once a module is ready, we aim at extracting it in its own repo within AliceO2Group or in an existing repo.

Instructions to setup a machine to run it :
https://alice-o2.web.cern.ch/node/121

The code will gradually be made compliant with the Alice O2 coding guidelines and software code organization. It shall also be built by alibuild in due time.

## Project structure and organisation

    The general structure is
    ├── cmake
    │   └── <all cmake macro files requested by more than one subproject>
    ├── CMakeLists.txt               # main CMakeLists.txt
    ├── Subproject ProjAAA
    │   ├── cmake
    │   │   ├── CPackConfig.cmake    # for Cpack
    │   │   └── ProjAAAConfig.cmake  # for subprojects interdependencies
    │   ├── CMakeLists.txt           # main CmakeLists.txt for ProjA
    │   ├── doc
    │   │   └── <Doxygen files and CMakeLists.txt>
    │   ├── include
    │   │   ├── CMakeLists.txt
    │   │   └── ProjAAA              # project subfolder to appear in includes (e.g. include <ProjA/World.h>)
    │   │       ├── CMakeLists.txt
    │   │       ├── Version.h.in     # template for the Version class
    │   │       └── World.h
    │   ├── src
    │   │   ├── CMakeLists.txt       # this is where we define the key targets for this subproject
    │   │   ├── main.cxx
    │   │   ├── Version.cxx
    │   │   └── World.cxx
    │   └── test
    │       ├── CMakeLists.txt
    │       └── testProjAAA.cxx
    ├── Subproject ProjBBB
    │   └── <same as ProjAAA>
    ├── ...
    └── README

The headers and source files are split in each subproject. The headers are under a directory with
the name of the project. We want that users of our libraries write "include <ProjA/xxx.h>" and not
just "include <xxx.h>" to avoid conflicts.

## Current Requirements

See https://alice-o2.web.cern.ch/node/121

#### The cmake way

The commands to build the project would be:
```
    mkdir build_dir
    cd build_dir
    cmake ..
    make
    make install
```
Installation goes to /usr/local/ by default.

#### More advanced way

To use it run `ccmake ..` in the top
directory of the project. Then turn on the options that you want by
going up and down with arrows, hitting "enter" to edit values and hitting
it again to exit edition.
For example, you might want to enable building documentation for the
sample project. Then press "c" followed by "g" to generate a
makefile. It will bring you back to the terminal. Then type "make"

One can also pass parameters to cmake when configuring it using "-DMY_VARIABLE VALUE".
For example to build a debug version or a release version with debug symbols could look like :
```
    cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
    cmake -DCMAKE_BUILD_TYPE=Debug ..
```

#### List of targets

From the ./build or ./ProjX/build, one can call :
* make [-j<number_of_parallel_jobs>]
* make test
* make install
* make doc
* make package
* make uninstall
* make coverage    (see below the details)

Important note : to be able to compile a subproject alone, one must first make and install all the subprojects it
depends on.
Moreover, one must add their installation directory to PATH and LD_LIBRARY_PATH.

## Docs (Doxygen)

There is a default doc target in the CMakeLists.txt, which is built
when you type "make doc". You must have doxygen
installed on your system for this to work. See source code
for hints on how to write doxygen comments.
You can customize the output by editing config files in the doc directory.

Documentation is placed in: BUILD_DIR/doc BUILD_DIR is the build in which you say "make".
After installation, it will go by default to /usr/local/share.

## Tests

There are 2 dummy tests defined in the CMakeLists.txt. The unit test
source is in the test directory.  To run the dummy test, do "make
test".

For coverage : 
1. Install gcov and lcov
2. Build like this : 
	cmake -DCODE_COVERAGE=ON ..
	make
	make coverage
3. Open in a browser the file indicated in the last line of the output.

Important note about coverage : the number of functions for a given class might seem off. It is because it counts
the compiler generated methods as well, such as the constructors, the copiers etc...
It should encourage you to use the "delete" keyword provided by c++11.

## Boost

We include boost in the CMakeLists.txt using find_package(Boost ...) and
passing a list of components we intend to use, namely unit_test_framework
and program_options.

The former is to ease the development of unit tests and the latter is
to help getting options for your binaries. See apps/hellorunner/main.cxx
for an example.

## Code formatting and beautifier

The file .clang-formatter contains a set of rules that one can use to adapt
the source code to coding conventions. Clang has a number of predefined
formatting rules but here we use a custom one. To use it one must first
install clang.
To run it on a single file and see the modified file content :

    clang-format -style=file <source file> # from the top directory

To run it on all subdirectories and replace the content of the .cxx and .h
files with the modified content :

    find . -name *.cxx -o -name *.h | xargs clang-format -style=file -i
    
To check whether a file complies or not with the format : 

    clang-format -style=file -output-replacements-xml <source file> | grep -c "<replacement " >/dev/null
    # returns 1 if complies, 0 otherwise

## IDEs

### CLion

Clion setup : launch from a terminal where we have the correct env. Add CMake options :
-DBOOST_ROOT=/opt/hackathon/ -DBOOST_LIBRARYDIR=/opt/hackathon/lib64 -DBoost_NO_SYSTEM_PATH=ON