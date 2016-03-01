LIBSONO PYTHON MODULE
=======
LIBSONO is made avaliable to python through a C python extension. See the Build/Usage section below for details.

Table of Contents
===============
1. [Build](https://github.com/majestic53/libsono/tree/master/src/ext#build)
2. [Usage](https://github.com/majestic53/libsono/tree/master/src/ext#usage)
3. [Device Discovery](https://github.com/majestic53/libsono/tree/master/src/ext#device-discovery)
4. [Error Handling](https://github.com/majestic53/libsono/tree/master/src/ext#error-handling)
5. [API Reference](https://github.com/majestic53/libsono/tree/master/src/ext#api-reference)
	1. [Library Routines](https://github.com/majestic53/libsono/tree/master/src/ext#library-routines)
		1. [Initialize](https://github.com/majestic53/libsono/tree/master/src/ext#initialize)
		2. [Uninitialize](https://github.com/majestic53/libsono/tree/master/src/ext#uninitialize)
		3. [Error](https://github.com/majestic53/libsono/tree/master/src/ext#error)
		4. [Version](https://github.com/majestic53/libsono/tree/master/src/ext#version)
	2. [Device Routines](https://github.com/majestic53/libsono/tree/master/src/ext#device-routines)
		1. [Discover](https://github.com/majestic53/libsono/tree/master/src/ext#discover)
		2. [List](https://github.com/majestic53/libsono/tree/master/src/ext#list)
	3. [Action Routines](https://github.com/majestic53/libsono/tree/master/src/ext#action-routines)
		1. [Add](https://github.com/majestic53/libsono/tree/master/src/ext#add)
		2. [Clear](https://github.com/majestic53/libsono/tree/master/src/ext#clear)
		3. [Get Mute](https://github.com/majestic53/libsono/tree/master/src/ext#get-mute)
		4. [Get Playback State](https://github.com/majestic53/libsono/tree/master/src/ext#get-playback-state)
		5. [Get Volume](https://github.com/majestic53/libsono/tree/master/src/ext#get-volume)
		6. [Next](https://github.com/majestic53/libsono/tree/master/src/ext#next)
		7. [Pause](https://github.com/majestic53/libsono/tree/master/src/ext#pause)
		8. [Play](https://github.com/majestic53/libsono/tree/master/src/ext#play)
		9. [Previous](https://github.com/majestic53/libsono/tree/master/src/ext#previous)
		10. [Restart](https://github.com/majestic53/libsono/tree/master/src/ext#restart)
		11. [Seek](https://github.com/majestic53/libsono/tree/master/src/ext#seek)
		12. [Set Mute](https://github.com/majestic53/libsono/tree/master/src/ext#set-mute)
		13. [Set Volume](https://github.com/majestic53/libsono/tree/master/src/ext#set-volume)
		14. [Stop](https://github.com/majestic53/libsono/tree/master/src/ext#stop)

Build
=======
__Prereq__: Building this project requires GNU and/or Clang compilers, Boost library, and Python headers.

To build the project, simply run the commands below in the projects root directory:

```
make py                --- Build libraries and python module
```

Upon completion, a python module (a dynamic/shared library) can be found at ```./bin/sonopy(.dll/.so)```

Usage
=======
The following steps must be taken in-order to use the library:

1. Import libsono into your python project
2. Call ```sonopy.initialize()``` to initialize the library (__Note__: This step must occur prior to any other library calls)
3. Call any of the avaliable ```sonopy.Xxx()``` routines
4. Call ```sonopy.uninitialize()``` to uninitialize the library (__Note__: This step mst occur after any other library calls)

Device Discovery
=======
To discover avaliable devices:

1. Call ```sonopy.discover()``` to retrieve the number of devices found
2. Call ```sonopy.list()``` to retrieve the device information


Error Handling
=======
To handle errors, follow the steps below:

1. Check all returns against __SONO_ERR_NONE__ to confirm that no error has occured
2. If an error has occurred, retrieve additional information by calling ```sonopy.error()```


API Reference
=======
The following commands are supported:

###Library Routines

Routines used to setup/teardown the library:

####Initialize
Initialize the library

```
[err] sonopy.initialize(void)

Output:
	[err]                 -- Error code
```

####Uninitialize
Uninitialize the library

```
[err] sonopy.uninitialize(void)

Output:
	[err]                 -- Error code
```

####Error
Retrieve the last error as a string

```
[err] sonopy.error(void)

Output:
	[err]                 -- Last error as string
```

####Version
Retrieve the library version as a string

```
[ver] sonopy.version(void)

Output:
	[ver]                 -- Version as string
```

###Device Routines

Routines used to query device information:

####Discover
Discover avaliable devices

```
[err, cnt] sonopy.discover(void)

Output:
	[err]                 -- Error code
	[cnt]                 -- Device count
```

####List
Retrieve a list of discovered devices

```
[err, dict{id, [addr, port]}] sonopy.list(void)

Output:
	[err]                  -- Error code
	dict{id, [addr, port]} -- Device id, addr:port dictionary
```

###Action Routines

Routines used to perform device actions and set device properties

####Add
Add a file to a target device's queue

```
[err] sonopy.add(addr, port, path)

Output:
	[err]                 -- Error code

Input:
	addr                  -- Device address
	port                  -- Device port
	path                  -- File path
```

####Clear
Clear a target device's queue

```
[err] sonopy.clear(addr, port)

Output:
	[err]                 -- Error code

Input:
	addr                  -- Device address
	port                  -- Device port
```

####Get Mute
Retrieve a target device's current mute setting

```
[err, mute] sonopy.get_mute(addr, port)

Output:
	[err]                 -- Error code
	[mute]                -- Current mute (0: unmute, 1: mute)

Input:
	addr                  -- Device address
	port                  -- Device port
```

####Get Playback State
Retrieve a target device's current playback state

```
[err, state] sonopy.get_playback_state(addr, port)

Output:
	[err]                 -- Error code
	[state]               -- Current state (0: playing, 1: paused, 2: stopped)

Input:
	addr                  -- Device address
	port                  -- Device port
```

####Get Volume
Retrieve a target device's current volume

```
[err, vol] sonopy.get_volume(addr, port)

Output:
	[err]                 -- Error code
	[vol]                 -- Current volume (0 - 100)

Input:
	addr                  -- Device address
	port                  -- Device port
```

####Next
Move to a target device's next track

```
[err] sonopy.next(addr, port)

Output:
	[err]                 -- Error code

Input:
	addr                  -- Device address
	port                  -- Device port
```

####Pause
Pause playback on a target device

```
[err] sonopy.pause(addr, port)

Output:
	[err]                 -- Error code

Input:
	addr                  -- Device address
	port                  -- Device port
```

####Play
Continue playback on a target device

```
[err] sonopy.play(addr, port)

Output:
	[err]                 -- Error code

Input:
	addr                  -- Device address
	port                  -- Device port
```

####Previous
Move to a target device's previous track

```
[err] sonopy.previous(addr, port)

Output:
	[err]                 -- Error code

Input:
	addr                  -- Device address
	port                  -- Device port
```

####Restart
Restart playback on a target device

```
[err] sonopy.restart(addr, port)

Output:
	[err]                 -- Error code

Input:
	addr                  -- Device address
	port                  -- Device port
```

####Seek
Seek playback on a target device

```
[err] sonopy.seek(addr, port, hour, min, sec)

Output:
	[err]                 -- Error code

Input:
	addr                  -- Device address
	port                  -- Device port
	hour                  -- Track hours
	min                   -- Track minutes
	sec                   -- Track seconds
```

####Set Mute
Mute/unmute a target device

```
[err] sonopy.set_mute(addr, port, mute)

Output:
	[err]                 -- Error code

Input:
	addr                  -- Device address
	port                  -- Device port
	mute                  -- Desired mute
```

####Set Volume
Set a target device's volume

```
[err] sonopy.set_volume(addr, port, vol)

Output:
	[err]                 -- Error code

Input:
	addr                  -- Device address
	port                  -- Device port
	vol                   -- Desired volume
```

####Stop
Stop playback on a target device

```
[err] sonopy.stop(addr, port)

Output:
	[err]                 -- Error code

Input:
	addr                  -- Device address
	port                  -- Device port
```
