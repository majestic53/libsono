Libsono Python Documentation
---

Usage
---
The following steps must be taken in-order to use the library:

1) Import libsono into your python project
2) Call sono_init_func() to initialize the library
	*) This step must occur prior to any other library calls
3) Call any of the avaliable sono_act_Xxx() routines
4) Call sono_uninit_funct() to uninitialize the library
	*) This step mst occur after any other library calls


Device Discovery
---
To discover avaliable devices:

1) Call sono_dev_disc_func() to retrieve the number of devices found
2) Call sono_dev_list_func() to retrieve the device information


Error Handling
---
To handle errors, follow the steps below:

1) Check all returns against SONO_ERR_NONE(0) to confirm that no error has occured
2) If an error has occurred, retrieve additional information by calling sono_err_func()


API
---
The following commands are supported:

1) LIBRARY

	*) [err] sono_init_func(void)

		Description:
			Initialize the library

		Output:
			[err] 	-- Error code


	*) [err] sono_uninit_func(void)

		Description:
			Uninitialize the library

		Output:
			[err] 	-- Error code


	*) [err] sono_err_func(void)

		Decription:
			Retrieve the last error as a string

		Output:
			[err] 	-- Last error as string


	*) [ver] sono_ver_func(void)

		Description:
			Retrieve the library version as a string

		Output:
			[ver] 	-- Version as string

2) DEVICE

	*) [err, cnt] sono_dev_disc_func(void)

		Description:
			Discover avaliable devices

		Output:
			[err] 	-- Error code
			[cnt] 	-- Device count


	*) [err, dict{id, [addr, port]}] sono_dev_list_func(void)

		Description:
			Retrieve a list of discovered devices

		Output:
			[err]			-- Error code
			dict{id, [addr, port]}	-- Device id, addr:port dictionary

3) ACTION

	*) [err] sono_act_add_func(addr, port, path)

		Description:
			Add a file to a target device's queue

		Output:
			[err] 	-- Error code

		Input:
			addr 	-- Device address
			port 	-- Device port
			path 	-- File path


	*) [err] sono_act_clear_func(addr, port)

		Description:
			Clear a target device's queue

		Output:
			[err] 	-- Error code

		Input:
			addr 	-- Device address
			port 	-- Device port


	*) [err, mute] sono_act_get_mute_func(addr, port)

		Description:
			Retrieve a target device's current mute setting

		Output:
			[err] 	-- Error code
			[mute] 	-- Current mute (0: unmute, 1: mute)

		Input:
			addr 	-- Device address
			port 	-- Device port


	*) [err, state] sono_act_get_playback_state_func(addr, port)

		Description:
			Retrieve a target device's current playback state

		Output:
			[err] 	-- Error code
			[state] -- Current state (0: playing, 1: paused, 2: stopped)

		Input:
			addr 	-- Device address
			port 	-- Device port


	*) [err, vol] sono_act_get_volume_func(addr, port)

		Description:
			Retrieve a target device's current volume

		Output:
			[err] 	-- Error code
			[vol] 	-- Current volume (0 - 100)

		Input:
			addr 	-- Device address
			port 	-- Device port


	*) [err] sono_act_next_func(addr, port)

		Description:
			Move to a target device's next track

		Output:
			[err] 	-- Error code

		Input:
			addr 	-- Device address
			port 	-- Device port


	*) [err] sono_act_pause_func(addr, port)

		Description:
			Pause playback on a target device

		Output:
			[err] 	-- Error code

		Input:
			addr 	-- Device address
			port 	-- Device port


	*) [err] sono_act_play_func(addr, port)

		Description:
			Continue playback on a target device

		Output:
			[err] 	-- Error code

		Input:
			addr 	-- Device address
			port 	-- Device port


	*) [err] sono_act_previous_func(addr, port)

		Description:
			Move to a target device's previous track

		Output:
			[err] 	-- Error code

		Input:
			addr 	-- Device address
			port 	-- Device port


	*) [err] sono_act_restart_func(addr, port)

		Description:
			Restart playback on a target device

		Output:
			[err] 	-- Error code

		Input:
			addr 	-- Device address
			port 	-- Device port


	*) [err] sono_act_seek_func(addr, port, hour, min, sec)

		Description:
			Seek playback on a target device

		Output:
			[err] 	-- Error code

		Input:
			addr 	-- Device address
			port 	-- Device port
			hour	-- Track hours
			min	-- Track minutes
			sec	-- Track seconds


	*) [err] sono_act_set_mute_func(addr, port, mute)

		Description:
			Mute/unmute a target device

		Output:
			[err] 	-- Error code

		Input:
			addr 	-- Device address
			port 	-- Device port
			mute	-- Desired mute


	*) [err] sono_act_set_volume_func(addr, port, vol)

		Description:
			Set a target device's volume

		Output:
			[err] 	-- Error code

		Input:
			addr 	-- Device address
			port 	-- Device port
			vol 	-- Desired volume


	*) [err] sono_act_stop_func(addr, port)

		Descrioption:
			Stop playback on a target device

		Output:
			[err] 	-- Error code

		Input:
			addr 	-- Device address
			port 	-- Device port
