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
sonoctl [-h|-v][[-a args][-d args][-s args]][-l[[a|s] args]]
```

* __-a | --action__ _\<action\> \<key\>=\<value\>..._ --- Specify target action/arguments
* __-d | --device__ _\<addr\>:\<port\>_ --- Specify target device
* __-h | --help__ --- Display help information
* __-l | --list__ --- List available devices
* __-la | --list-actions__ _\<addr\>:\<port\> \<service\>_ --- List available device service actions
* __-ld | --list-services__ _\<addr\>:\<port\>_ --- List available device services
* __-s | --service__ _\<service\>_ --- Specify target service
* __-v | --version__ --- Display version information

###Examples

* Set a target device's LED state
```
sonoctl -d 192.168.1.2:1400 -s DeviceProperties -a SetLEDState DesiredLEDState=On
```

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

Changelog
=========

###Version 0.1.1609
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
