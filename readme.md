LIBSONO
=======

LIBSONO is a lightwight library used to control Sono devices, written in C++.

Table of Contents
===============

1. [Usage](https://github.com/majestic53/libsono#usage)
2. [Changelog](https://github.com/majestic53/libsono#changelog)
3. [License](https://github.com/majestic53/libsono#license)

Usage
=====

###Build

__Prereq__: Building this project requires the GNU and/or Clang c++ compilers (g++/clang++) and the Boost library.

To build the entire project, simply run the command below in the projects root directory.

```
make
```

sonoctl is a CLI application, which can be invoked using the interface below:

###SONOCTL

```
sonoctl [-h|-v][-l]
```

* _-h | --help_			--- Display help information
* _-l | --list_			--- Display a list of accessible Sono devices
* _-v | --version_ 		--- Display version information

###Examples

* sonoctl --help
* sonoctl --list

*More to come...*

Changelog
=========

###Version 0.1.1606
*Updated: 2/11/2016*

* Added device service discovery support (partial)

*Updated: 2/10/2016*

* Finished SSDP discovery/list routines

*Updated: 2/9/2016*

* Added socket classes
* Added xml class (incomplete)
* Added discovery/list routines (incomplete)

*Updated: 2/8/2016*

* Initial commit

*Changelog truncated (see file history for full log)*

License
======

Copyright(C) 2016 David Jolly <majestic53@gmail.com>

libsono is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libsono is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
