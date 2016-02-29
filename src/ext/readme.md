LIBSONO PYTHON MODULE
=======
LIBSONO is a lightweight library used to control Sono devices, written in C++.

Table of Contents
===============

1. [Usage](https://github.com/majestic53/libsono/src/ext#usage)
2. [Device Discovery](https://github.com/majestic53/libsono/src/ext#device-discovery)
3. [Error Handling](https://github.com/majestic53/libsono/src/ext#error-handling)
4. [API](https://github.com/majestic53/libsono/src/ext#api)

Usage
=======
The following steps must be taken in-order to use the library:

1. Import libsono into your python project
2. Call ```sono_init_func()``` to initialize the library
	1. This step must occur prior to any other library calls
3. Call any of the avaliable ```sono_act_Xxx()``` routines
4. Call ```sono_uninit_funct()``` to uninitialize the library
	1. This step mst occur after any other library calls

Device Discovery
=======
To discover avaliable devices:

1. Call ```sono_dev_disc_func()``` to retrieve the number of devices found
2. Call ```sono_dev_list_func()``` to retrieve the device information


Error Handling
=======
To handle errors, follow the steps below:

1. Check all returns against __SONO_ERR_NONE__(0) to confirm that no error has occured
2. If an error has occurred, retrieve additional information by calling ```sono_err_func()```


API
=======
The following commands are supported:

###LIBRARY

1. sono_init_func
Initialize the library

```
[err] sono_init_func(void)

Output:
	[err]                 -- Error code
```

2. sono_uninit_func
Uninitialize the library

```
[err] sono_uninit_func(void)

Output:
	[err]                 -- Error code
```

3. sono_err_func
Retrieve the last error as a string

```
[err] sono_err_func(void)

Output:
	[err]                 -- Last error as string
```

4. sono_ver_func
Retrieve the library version as a string

```
[ver] sono_ver_func(void)

Output:
	[ver]                 -- Version as string
```

###DEVICE

1. sono_dev_disc_func
Discover avaliable devices

```
[err, cnt] sono_dev_disc_func(void)

Output:
	[err]                 -- Error code
	[cnt]                 -- Device count
```

2. sono_dev_list_func
Retrieve a list of discovered devices

```
[err, dict{id, [addr, port]}] sono_dev_list_func(void)

Output:
	[err]                 -- Error code
	dict{id, [addr, port]}-- Device id, addr:port dictionary
```

###ACTION

1. sono_act_add_func
Add a file to a target device's queue

```
[err] sono_act_add_func(addr, port, path)

Output:
	[err]                 -- Error code

Input:
	addr                  -- Device address
	port                  -- Device port
	path                  -- File path
```

2. sono_act_clear_func
Clear a target device's queue

```
[err] sono_act_clear_func(addr, port)

Output:
	[err]                 -- Error code

Input:
	addr                  -- Device address
	port                  -- Device port
```

3. sono_act_get_mute_func
Retrieve a target device's current mute setting

```
[err, mute] sono_act_get_mute_func(addr, port)

Output:
	[err]                 -- Error code
	[mute]                -- Current mute (0: unmute, 1: mute)

Input:
	addr                  -- Device address
	port                  -- Device port
```

4. sono_act_get_playback_state_func
Retrieve a target device's current playback state

```
[err, state] sono_act_get_playback_state_func(addr, port)

Output:
	[err]                 -- Error code
	[state]               -- Current state (0: playing, 1: paused, 2: stopped)

Input:
	addr                  -- Device address
	port                  -- Device port
```

5. sono_act_get_volume_func
Retrieve a target device's current volume

```
[err, vol] sono_act_get_volume_func(addr, port)

Output:
	[err]                 -- Error code
	[vol]                 -- Current volume (0 - 100)

Input:
	addr                  -- Device address
	port                  -- Device port
```

6. sono_act_next_func
Move to a target device's next track

```
[err] sono_act_next_func(addr, port)

Output:
	[err]                 -- Error code

Input:
	addr                  -- Device address
	port                  -- Device port
```

7. sono_act_pause_func
Pause playback on a target device

```
[err] sono_act_pause_func(addr, port)

Output:
	[err]                 -- Error code

Input:
	addr                  -- Device address
	port                  -- Device port
```

8. sono_act_play_func
Continue playback on a target device

```
[err] sono_act_play_func(addr, port)

Output:
	[err]                 -- Error code

Input:
	addr                  -- Device address
	port                  -- Device port
```

9. sono_act_previous_func
Move to a target device's previous track

```
[err] sono_act_previous_func(addr, port)

Output:
	[err]                 -- Error code

Input:
	addr                  -- Device address
	port                  -- Device port
```

10. sono_act_restart_func
Restart playback on a target device

```
[err] sono_act_restart_func(addr, port)

Output:
	[err]                 -- Error code

Input:
	addr                  -- Device address
	port                  -- Device port
```

11. sono_act_seek_func
Seek playback on a target device

```
[err] sono_act_seek_func(addr, port, hour, min, sec)

Output:
	[err]                 -- Error code

Input:
	addr                  -- Device address
	port                  -- Device port
	hour                  -- Track hours
	min                   -- Track minutes
	sec                   -- Track seconds
```

12. sono_act_set_mute_func
Mute/unmute a target device

```
[err] sono_act_set_mute_func(addr, port, mute)

Output:
	[err]                 -- Error code

Input:
	addr                  -- Device address
	port                  -- Device port
	mute                  -- Desired mute
```

13. sono_act_set_volume_func
Set a target device's volume

```
[err] sono_act_set_volume_func(addr, port, vol)

Output:
	[err]                 -- Error code

Input:
	addr                  -- Device address
	port                  -- Device port
	vol                   -- Desired volume
```

14. sono_act_stop_func
Stop playback on a target device

```
[err] sono_act_stop_func(addr, port)

Output:
	[err]                 -- Error code

Input:
	addr                  -- Device address
	port                  -- Device port
```
