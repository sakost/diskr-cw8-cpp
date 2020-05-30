# Discrete course work

This project is my course work of the second term of discrete mathematics course

## Installation

### Dependencies
To build and run this project you need such dependencies as:
* GraphViz
* Qt5 GUI Components
* CMake
* some C++17 compiler

To install them all in Arch Linux just run
```shell script
$ sudo pacman -Syu build-essential cmake qt5-base graphviz
```
To install them in Ubuntu/Debian just run
```shell script
$ sudo apt update
$ sudo apt install build-essential cmake libgl1-mesa-dev
```
and follow [this](https://wiki.qt.io/Install_Qt_5_on_Ubuntu) instruction to install Qt5

### Run
Then just run building process
```shell script
$ cmake .
$ make
```
Then just run a program
```shell script
$ ./diskr-cw8-cpp
```

## LICENSE
MIT

Copyright (c) 2020 Konstantin Sazhenov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
