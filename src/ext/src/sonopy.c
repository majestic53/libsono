/**
 * libsono
 * Copyright (C) 2016 David Jolly
 * ----------------------
 *
 * libsono is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libsono is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include "../../lib/sono.h"

#define BUILD_PYTHON_3 // python version 3.x
#include <Python.h>

#define DEVICE_DISCOVERY_TIMEOUT 1 // sec
#define DEVICE_DISCOVERY_TUPLE_COUNT 1
#define SERVICE_ACTION_TIMEOUT 2 // sec
#define SERVICE_DISCOVERY_TIMEOUT 2 // sec

static void
sono_evt_callback(
	/*__in*/ uint32_t id,
	/*__in*/ const char *svc,
	/*__in*/ const char *act,
	/*__in*/ const char *data
	)
{
	return;
}

/*
 * Perform add to queue device action
 * @param self Python object
 * @param args Python object
 * @return Python object (error code)
 */
static PyObject *
sono_act_add_func(
	/*__in*/ PyObject *self, 
	/*__in*/ PyObject *args
	)
{	
	size_t addr_len;
	sono_dev dev = { 0 };
	char *addr = NULL, *path = NULL;
	sono_err_t status = SONO_ERR_NONE;

	if(!PyArg_ParseTuple(args, "shs", &addr, &dev.port, &path) 
			|| !addr || !path) {
		status = SONO_ERR_INVALID;
		goto exit;
	}

	addr_len = strlen(addr);
	if(addr_len > SONO_DEV_ADDR_LEN) {
		addr_len = SONO_DEV_ADDR_LEN;
	}

	memcpy(dev.addr, addr, addr_len);
	status = sono_dev_act_add(&dev, path, SERVICE_DISCOVERY_TIMEOUT, SERVICE_ACTION_TIMEOUT);

exit:
	return Py_BuildValue("l", status);
}

/*
 * Perform clear queue device action
 * @param self Python object
 * @param args Python object
 * @return Python object (error code)
 */
static PyObject *
sono_act_clear_func(
	/*__in*/ PyObject *self, 
	/*__in*/ PyObject *args
	)
{
	size_t addr_len;
	char *addr = NULL;
	sono_dev dev = { 0 };
	sono_err_t status = SONO_ERR_NONE;

	if(!PyArg_ParseTuple(args, "sh", &addr, &dev.port)
			|| !addr) {
		status = SONO_ERR_INVALID;
		goto exit;
	}

	addr_len = strlen(addr);
	if(addr_len > SONO_DEV_ADDR_LEN) {
		addr_len = SONO_DEV_ADDR_LEN;
	}

	memcpy(dev.addr, addr, addr_len);
	status = sono_dev_act_clear(&dev, SERVICE_DISCOVERY_TIMEOUT, SERVICE_ACTION_TIMEOUT);

exit:
	return Py_BuildValue("l", status);
}

/*
 * Perform get mute device action
 * @param self Python object
 * @param args Python object
 * @return Python object (error code)
 */
static PyObject *
sono_act_get_mute_func(
	/*__in*/ PyObject *self, 
	/*__in*/ PyObject *args
	)
{
	size_t addr_len;
	uint8_t result = 0;
	char *addr = NULL;
	sono_dev dev = { 0 };
	sono_err_t status = SONO_ERR_NONE;

	if(!PyArg_ParseTuple(args, "sh", &addr, &dev.port)
			|| !addr) {
		status = SONO_ERR_INVALID;
		goto exit;
	}

	addr_len = strlen(addr);
	if(addr_len > SONO_DEV_ADDR_LEN) {
		addr_len = SONO_DEV_ADDR_LEN;
	}

	memcpy(dev.addr, addr, addr_len);
	status = sono_dev_act_get_mute(&result, &dev, SERVICE_DISCOVERY_TIMEOUT, 
		SERVICE_ACTION_TIMEOUT);

exit:
	return Py_BuildValue("lb", status, result);
}

/*
 * Perform get playback state device action
 * @param self Python object
 * @param args Python object
 * @return Python object (error code)
 */
static PyObject *
sono_act_get_playback_state_func(
	/*__in*/ PyObject *self, 
	/*__in*/ PyObject *args
	)
{
	size_t addr_len;
	char *addr = NULL;
	uint32_t result = 0;
	sono_dev dev = { 0 };
	sono_err_t status = SONO_ERR_NONE;

	if(!PyArg_ParseTuple(args, "sh", &addr, &dev.port)
			|| !addr) {
		status = SONO_ERR_INVALID;
		goto exit;
	}

	addr_len = strlen(addr);
	if(addr_len > SONO_DEV_ADDR_LEN) {
		addr_len = SONO_DEV_ADDR_LEN;
	}

	memcpy(dev.addr, addr, addr_len);
	status = sono_dev_act_get_playback_state(&result, &dev, SERVICE_DISCOVERY_TIMEOUT, 
		SERVICE_ACTION_TIMEOUT);

exit:
	return Py_BuildValue("ll", status, result);
}

/*
 * Perform get volume device action
 * @param self Python object
 * @param args Python object
 * @return Python object (error code)
 */
static PyObject *
sono_act_get_volume_func(
	/*__in*/ PyObject *self, 
	/*__in*/ PyObject *args
	)
{
	size_t addr_len;
	char *addr = NULL;
	uint32_t result = 0;
	sono_dev dev = { 0 };
	sono_err_t status = SONO_ERR_NONE;

	if(!PyArg_ParseTuple(args, "sh", &addr, &dev.port)
			|| !addr) {
		status = SONO_ERR_INVALID;
		goto exit;
	}

	addr_len = strlen(addr);
	if(addr_len > SONO_DEV_ADDR_LEN) {
		addr_len = SONO_DEV_ADDR_LEN;
	}

	memcpy(dev.addr, addr, addr_len);
	status = sono_dev_act_get_volume(&result, &dev, SERVICE_DISCOVERY_TIMEOUT, 
		SERVICE_ACTION_TIMEOUT);

exit:
	return Py_BuildValue("ll", status, result);
}

/*
 * Perform next track device action
 * @param self Python object
 * @param args Python object
 * @return Python object (error code)
 */
static PyObject *
sono_act_next_func(
	/*__in*/ PyObject *self, 
	/*__in*/ PyObject *args
	)
{
	size_t addr_len;
	char *addr = NULL;
	sono_dev dev = { 0 };
	sono_err_t status = SONO_ERR_NONE;

	if(!PyArg_ParseTuple(args, "sh", &addr, &dev.port)
			|| !addr) {
		status = SONO_ERR_INVALID;
		goto exit;
	}

	addr_len = strlen(addr);
	if(addr_len > SONO_DEV_ADDR_LEN) {
		addr_len = SONO_DEV_ADDR_LEN;
	}

	memcpy(dev.addr, addr, addr_len);
	status = sono_dev_act_next(&dev, SERVICE_DISCOVERY_TIMEOUT, SERVICE_ACTION_TIMEOUT);

exit:
	return Py_BuildValue("l", status);
}

/*
 * Perform pause track device action
 * @param self Python object
 * @param args Python object
 * @return Python object (error code)
 */
static PyObject *
sono_act_pause_func(
	/*__in*/ PyObject *self, 
	/*__in*/ PyObject *args
	)
{
	size_t addr_len;
	char *addr = NULL;
	sono_dev dev = { 0 };
	sono_err_t status = SONO_ERR_NONE;

	if(!PyArg_ParseTuple(args, "sh", &addr, &dev.port)
			|| !addr) {
		status = SONO_ERR_INVALID;
		goto exit;
	}

	addr_len = strlen(addr);
	if(addr_len > SONO_DEV_ADDR_LEN) {
		addr_len = SONO_DEV_ADDR_LEN;
	}

	memcpy(dev.addr, addr, addr_len);
	status = sono_dev_act_pause(&dev, SERVICE_DISCOVERY_TIMEOUT, SERVICE_ACTION_TIMEOUT);

exit:
	return Py_BuildValue("l", status);
}

/*
 * Perform play track device action
 * @param self Python object
 * @param args Python object
 * @return Python object (error code)
 */
static PyObject *
sono_act_play_func(
	/*__in*/ PyObject *self, 
	/*__in*/ PyObject *args
	)
{
	size_t addr_len;
	char *addr = NULL;
	sono_dev dev = { 0 };
	sono_err_t status = SONO_ERR_NONE;

	if(!PyArg_ParseTuple(args, "sh", &addr, &dev.port)
			|| !addr) {
		status = SONO_ERR_INVALID;
		goto exit;
	}

	addr_len = strlen(addr);
	if(addr_len > SONO_DEV_ADDR_LEN) {
		addr_len = SONO_DEV_ADDR_LEN;
	}

	memcpy(dev.addr, addr, addr_len);
	status = sono_dev_act_play(&dev, SERVICE_DISCOVERY_TIMEOUT, SERVICE_ACTION_TIMEOUT);

exit:
	return Py_BuildValue("l", status);
}

/*
 * Perform previous track device action
 * @param self Python object
 * @param args Python object
 * @return Python object (error code)
 */
static PyObject *
sono_act_previous_func(
	/*__in*/ PyObject *self, 
	/*__in*/ PyObject *args
	)
{
	size_t addr_len;
	char *addr = NULL;
	sono_dev dev = { 0 };
	sono_err_t status = SONO_ERR_NONE;

	if(!PyArg_ParseTuple(args, "sh", &addr, &dev.port)
			|| !addr) {
		status = SONO_ERR_INVALID;
		goto exit;
	}

	addr_len = strlen(addr);
	if(addr_len > SONO_DEV_ADDR_LEN) {
		addr_len = SONO_DEV_ADDR_LEN;
	}

	memcpy(dev.addr, addr, addr_len);
	status = sono_dev_act_previous(&dev, SERVICE_DISCOVERY_TIMEOUT, SERVICE_ACTION_TIMEOUT);

exit:
	return Py_BuildValue("l", status);
}

/*
 * Perform restart track device action
 * @param self Python object
 * @param args Python object
 * @return Python object (error code)
 */
static PyObject *
sono_act_restart_func(
	/*__in*/ PyObject *self, 
	/*__in*/ PyObject *args
	)
{
	size_t addr_len;
	char *addr = NULL;
	sono_dev dev = { 0 };
	sono_err_t status = SONO_ERR_NONE;

	if(!PyArg_ParseTuple(args, "sh", &addr, &dev.port)
			|| !addr) {
		status = SONO_ERR_INVALID;
		goto exit;
	}

	addr_len = strlen(addr);
	if(addr_len > SONO_DEV_ADDR_LEN) {
		addr_len = SONO_DEV_ADDR_LEN;
	}

	memcpy(dev.addr, addr, addr_len);
	status = sono_dev_act_restart(&dev, SERVICE_DISCOVERY_TIMEOUT, SERVICE_ACTION_TIMEOUT);

exit:
	return Py_BuildValue("l", status);
}

/*
 * Perform seek within track device action
 * @param self Python object
 * @param args Python object
 * @return Python object (error code)
 */
static PyObject *
sono_act_seek_func(
	/*__in*/ PyObject *self, 
	/*__in*/ PyObject *args
	)
{
	size_t addr_len;
	char *addr = NULL;
	sono_dev dev = { 0 };
	uint32_t hour, minute, second;
	sono_err_t status = SONO_ERR_NONE;

	if(!PyArg_ParseTuple(args, "shlll", &addr, &dev.port, &hour, &minute, &second)
			|| !addr) {
		status = SONO_ERR_INVALID;
		goto exit;
	}

	addr_len = strlen(addr);
	if(addr_len > SONO_DEV_ADDR_LEN) {
		addr_len = SONO_DEV_ADDR_LEN;
	}

	memcpy(dev.addr, addr, addr_len);
	status = sono_dev_act_seek(hour, minute, second, &dev, SERVICE_DISCOVERY_TIMEOUT, 
		SERVICE_ACTION_TIMEOUT);

exit:
	return Py_BuildValue("l", status);
}

/*
 * Perform set mute device action
 * @param self Python object
 * @param args Python object
 * @return Python object (error code)
 */
static PyObject *
sono_act_set_mute_func(
	/*__in*/ PyObject *self, 
	/*__in*/ PyObject *args
	)
{
	uint8_t mute;
	size_t addr_len;
	char *addr = NULL;
	sono_dev dev = { 0 };
	sono_err_t status = SONO_ERR_NONE;

	if(!PyArg_ParseTuple(args, "shb", &addr, &dev.port, &mute)
			|| !addr) {
		status = SONO_ERR_INVALID;
		goto exit;
	}

	addr_len = strlen(addr);
	if(addr_len > SONO_DEV_ADDR_LEN) {
		addr_len = SONO_DEV_ADDR_LEN;
	}

	memcpy(dev.addr, addr, addr_len);
	status = sono_dev_act_set_mute(mute, &dev, SERVICE_DISCOVERY_TIMEOUT, 
		SERVICE_ACTION_TIMEOUT);

exit:
	return Py_BuildValue("l", status);
}

/*
 * Perform set volume device action
 * @param self Python object
 * @param args Python object
 * @return Python object (error code)
 */
static PyObject *
sono_act_set_volume_func(
	/*__in*/ PyObject *self, 
	/*__in*/ PyObject *args
	)
{
	size_t addr_len;
	uint32_t volume;
	char *addr = NULL;
	sono_dev dev = { 0 };
	sono_err_t status = SONO_ERR_NONE;

	if(!PyArg_ParseTuple(args, "shl", &addr, &dev.port, &volume)
			|| !addr) {
		status = SONO_ERR_INVALID;
		goto exit;
	}

	addr_len = strlen(addr);
	if(addr_len > SONO_DEV_ADDR_LEN) {
		addr_len = SONO_DEV_ADDR_LEN;
	}

	memcpy(dev.addr, addr, addr_len);
	status = sono_dev_act_set_volume(volume, &dev, SERVICE_DISCOVERY_TIMEOUT, 
		SERVICE_ACTION_TIMEOUT);

exit:
	return Py_BuildValue("l", status);
}

/*
 * Perform stop track device action
 * @param self Python object
 * @param args Python object
 * @return Python object (error code)
 */
static PyObject *
sono_act_stop_func(
	/*__in*/ PyObject *self, 
	/*__in*/ PyObject *args
	)
{
	size_t addr_len;
	char *addr = NULL;
	sono_dev dev = { 0 };
	sono_err_t status = SONO_ERR_NONE;

	if(!PyArg_ParseTuple(args, "sh", &addr, &dev.port)
			|| !addr) {
		status = SONO_ERR_INVALID;
		goto exit;
	}

	addr_len = strlen(addr);
	if(addr_len > SONO_DEV_ADDR_LEN) {
		addr_len = SONO_DEV_ADDR_LEN;
	}

	memcpy(dev.addr, addr, addr_len);
	status = sono_dev_act_stop(&dev, SERVICE_DISCOVERY_TIMEOUT, SERVICE_ACTION_TIMEOUT);

exit:
	return Py_BuildValue("l", status);
}

/*
 * Discover sono devices
 * @param self Python object
 * @param args Python object
 * @return Python object (error code)
 */
static PyObject *
sono_dev_disc_func(
	/*__in*/ PyObject *self, 
	/*__in*/ PyObject *args
	)
{
	uint8_t count = 0;

	return Py_BuildValue("lb", sono_dev_disc(&count, DEVICE_DISCOVERY_TIMEOUT, 
		SERVICE_DISCOVERY_TIMEOUT), count);
}

/*
 * Retrieve a list of sono devices
 * @param self Python object
 * @param args Python object
 * @return Python object (error code)
 */
static PyObject *
sono_dev_list_func(
	/*__in*/ PyObject *self, 
	/*__in*/ PyObject *args
	)
{
	sono_dev *list = NULL;
	uint8_t count, iter = 0;
	sono_err_t status = SONO_ERR_NONE;
	PyObject *key = NULL, *map = NULL, *result = NULL, *value = NULL;

	if(!PyArg_ParseTuple(args, "b", &count)
			|| !count) {
		status = SONO_ERR_INVALID;
		goto exit;
	}

	list = (sono_dev *) malloc(sizeof(sono_dev) * count);
	if(!list) {
		status = SONO_ERR_FAILED;
		goto exit;
	}

	status = sono_dev_list(list, &count);
	if(!SONO_SUCCESS(status)) {
		status = SONO_ERR_INVALID;
		goto exit;
	}

	map = PyDict_New();
	if(!map
			|| !PyDict_Check(map)) {
		status = SONO_ERR_FAILED;
		goto exit;
	}

	for(; iter < count; ++iter) {
		key = Py_BuildValue("i", list[iter].id);
		value = Py_BuildValue("si", (char *) list[iter].addr, list[iter].port);
		PyDict_SetItem(map, key, value);

		if(key) {
			Py_DECREF(key);
			key = NULL;
		}

		if(value) {
			Py_DECREF(value);
			value = NULL;
		}
	}
	
	result = Py_BuildValue("lO", status, map);

exit:

	if(key) {
		Py_DECREF(key);
		key = NULL;
	}

	if(value) {
		Py_DECREF(value);
		value = NULL;
	}

	if(map) {
		Py_DECREF(map);
		map = NULL;
	}

	if(list) {
		free(list);
		list = NULL;
	}

	return result;
}

/*
 * Library last error code
 * @param self Python object
 * @return Python object (error code)
 */
static PyObject *
sono_err_func(
	/*__in*/ PyObject *self
	)
{
	return Py_BuildValue("s", sono_err());
}

/*
 * Initialize library
 * (NOTE: This routine must be called prior to calling any other library routines)
 * @param self Python object
 * @param args Python object
 * @return Python object (error code)
 */
static PyObject *
sono_init_func(
	/*__in*/ PyObject *self, 
	/*__in*/ PyObject *args
	)
{
	return Py_BuildValue("l", sono_init(sono_evt_callback));
}

/*
 * Uninitialize library
 * (NOTE: This routine must be called after all other library routines)
 * @param self Python object
 * @return Python object (error code)
 */
static PyObject *
sono_uninit_func(
	/*__in*/ PyObject *self
	)

{
	return Py_BuildValue("l", sono_uninit());
}

/*
 * Library version
 * @param self Python object
 * @return Python object (version as string)
 */
static PyObject *
sono_ver_func(
	/*__in*/ PyObject *self
	)
{
	return Py_BuildValue("s", sono_ver());
}

// Library method table
static PyMethodDef sonopyMethods[] = {
	{ "add", (PyCFunction) sono_act_add_func, METH_VARARGS, "perform add to queue device action" },
	{ "clear", (PyCFunction) sono_act_clear_func, METH_VARARGS, "perform clear queue device action" },
	{ "discover", (PyCFunction) sono_dev_disc_func, METH_NOARGS, "discover devices" },
	{ "error", (PyCFunction) sono_err_func, METH_NOARGS, "error string" },
	{ "get_mute", (PyCFunction) sono_act_get_mute_func, METH_VARARGS, "perform get mute device action" },
	{ "get_list", (PyCFunction) sono_dev_list_func, METH_VARARGS, "retrieve a list of devices" },
	{ "get_playback_state", (PyCFunction) sono_act_get_playback_state_func, METH_VARARGS, "perform get playback state device action" },
	{ "get_volume", (PyCFunction) sono_act_get_volume_func, METH_VARARGS, "perform get volume device action" },
	{ "initialize", (PyCFunction) sono_init_func, METH_NOARGS, "initialize sono library" },
	{ "next", (PyCFunction) sono_act_next_func, METH_VARARGS, "perform next track device action" },
	{ "pause", (PyCFunction) sono_act_pause_func, METH_VARARGS, "perform pause track device action" },
	{ "play", (PyCFunction) sono_act_play_func, METH_VARARGS, "perform play track device action" },
	{ "previous", (PyCFunction) sono_act_previous_func, METH_VARARGS, "perform previous track device action" },
	{ "restart", (PyCFunction) sono_act_restart_func, METH_VARARGS, "perform restart track device action" },
	{ "seek", (PyCFunction) sono_act_seek_func, METH_VARARGS, "perform seek within track device action" },
	{ "mute", (PyCFunction) sono_act_set_mute_func, METH_VARARGS, "perform set mute device action" },
	{ "set_volume", (PyCFunction) sono_act_set_volume_func, METH_VARARGS, "perform set volume device action" },
	{ "stop", (PyCFunction) sono_act_stop_func, METH_VARARGS, "perform stop track device action" },
	{ "uninitialize", (PyCFunction) sono_uninit_func, METH_NOARGS, "uninitialize sono library" },
	{ "version", (PyCFunction) sono_ver_func, METH_NOARGS, "version string" },
	{ NULL, NULL, 0, NULL },
	};

#ifdef BUILD_PYTHON_3
// library definition table
static struct PyModuleDef sonopyDef = {
	PyModuleDef_HEAD_INIT,
	"sonopy",
	"a lightweight library used to control Sono devices",
	-1,
	sonopyMethods
	};
#endif // BUILD_PYTHON_3

// Module initialization routine
PyMODINIT_FUNC
initsono_module(void)
{
#ifdef BUILD_PYTHON_3
	return PyModule_Create(&sonopyDef);
#else
	(void) Py_InitModule("sonopy", sonopyMethods);
#endif // BUILD_PYTHON_3
}
