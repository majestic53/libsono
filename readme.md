LIBSONO
=======

LIBSONO is a lightweight library used to control Sono devices, written in C++.

Table of Contents
===============

1. [Usage](https://github.com/majestic53/libsono#usage)
	1. [Build](https://github.com/majestic53/libsono#build)
	2. [Sonoctl](https://github.com/majestic53/libsono#sonoctl)
	2. [Examples](https://github.com/majestic53/libsono#examples)
2. [Changelog](https://github.com/majestic53/libsono#changelog)
	1. [Version 0.1.1609](https://github.com/majestic53/libsono#version-011609)
	1. [Version 0.1.1608](https://github.com/majestic53/libsono#version-011608)
3. [License](https://github.com/majestic53/libsono#license)

Usage
=====

###Build

__Prereq__: Building this project requires GNU and/or Clang compilers, Boost library, and Python headers.

To build the project, simply run the commands below in the projects root directory:

```
make all               --- Build libraries and sonoctl
make clean             --- Clean build
make lib               --- Build libraries
make py                --- Build libraries and python module
make shared            --- Build shared library
make static            --- Build static library
make tool              --- Build libraries and sonoctl
```

###Sonoctl

Sonoctl is a CLI application, which can be invoked using the interface below:

```
sonoctl [-h|-v][[[-b][[-a args][-d args][-s args]]][-l[[a|s] args]]]
```

```
-a    --action         <action> <key>=<value>...   --- Specify target action/arguments
-d    --device         <addr>:<port>               --- Specify target device
-h    --help                                       --- Display help information
-l    --list                                       --- List available devices
-la   --list-actions   <addr>:<port> <service>     --- List available device service actions
-ls   --list-services  <addr>:<port>               --- List available device services
-s    --service        <service>                   --- Specify target service
-b    --verbose                                    --- Display verbose information
-v    --version                                    --- Display version information
```

###Examples

* List all available devices
```
sonoctl --list
```

* List all available services advertised by a target device
```
sonoctl --list-services 192.168.1.2:1400
```

* List all available actions advertiesed by a target device's service
```
sonoctl --list-actions 192.168.1.2:1400 DeviceProperties
```

* Set a target device's LED state
```
sonoctl -d 192.168.1.2:1400 -s DeviceProperties -a SetLEDState DesiredLEDState=On
```

Changelog
=========

###Version 0.1.1609
*Updated: 2/24/2016*

* Added python binding to makefile

*Updated: 2/23/2016*

* Completed sonoctl interface
* Added Python binding (incomplete)

*Updated: 2/22/2016*

* Added sonoctl interface (incomplete)
* Updated documentation

###Version 0.1.1608
*Updated: 2/21/2016*

* Added c-style interface

*Updated: 2/17/2016*

* Modified device services to be indexed by string
* Added c-style header

*Updated: 2/16/2016*

* Modified actions to be indexed by string

*Updated: 2/15/2016*

* Added device service actions

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
