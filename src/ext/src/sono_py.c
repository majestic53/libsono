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

#include <python2.7/Python.h>
#include "../../lib/sono.h"

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
	sono_err_t status = SONO_ERR_NONE;
	
	// TODO
	
	return Py_BuildValue("I", status);
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
	sono_err_t status = SONO_ERR_NONE;
	
	// TODO
	
	return Py_BuildValue("I", status);
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
	sono_err_t status = SONO_ERR_NONE;
	
	// TODO
	
	return Py_BuildValue("I", status);
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
	sono_err_t status = SONO_ERR_NONE;
	
	// TODO
	
	return Py_BuildValue("I", status);
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
	sono_err_t status = SONO_ERR_NONE;
	
	// TODO
	
	return Py_BuildValue("I", status);
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
	sono_err_t status = SONO_ERR_NONE;
	
	// TODO
	
	return Py_BuildValue("I", status);
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
	sono_err_t status = SONO_ERR_NONE;
	
	// TODO
	
	return Py_BuildValue("I", status);
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
	sono_err_t status = SONO_ERR_NONE;
	
	// TODO
	
	return Py_BuildValue("I", status);
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
	sono_err_t status = SONO_ERR_NONE;
	
	// TODO
	
	return Py_BuildValue("I", status);
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
	sono_err_t status = SONO_ERR_NONE;
	
	// TODO
	
	return Py_BuildValue("I", status);
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
	sono_err_t status = SONO_ERR_NONE;
	
	// TODO
	
	return Py_BuildValue("I", status);
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
	sono_err_t status = SONO_ERR_NONE;
	
	// TODO
	
	return Py_BuildValue("I", status);
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
	sono_err_t status = SONO_ERR_NONE;
	
	// TODO
	
	return Py_BuildValue("I", status);
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
	sono_err_t status = SONO_ERR_NONE;
	
	// TODO
	
	return Py_BuildValue("I", status);
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
	size_t length;
	sono_evt_cb handler = NULL;
	sono_err_t status = SONO_ERR_NONE;
	
	if(!PyArg_ParseTuple(args, "w#", &handler, &length)
			|| (length != sizeof(sono_evt_cb))) {
		status = SONO_ERR_INVALID;
		goto exit;
	}
	
	status = sono_init(handler);
	
exit:
	return Py_BuildValue("I", status);
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
	return Py_BuildValue("I", sono_uninit());
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
static PyMethodDef sonoMethods[] = {
	{"sono_act_add_func", (PyCFunction) sono_act_add_func, METH_VARARGS, "perform add to queue device action" },
	{"sono_act_clear_func", (PyCFunction) sono_act_clear_func, METH_VARARGS, "perform clear queue device action" },
	{"sono_act_get_mute_func", (PyCFunction) sono_act_get_mute_func, METH_VARARGS, "perform get mute device action" },
	{"sono_act_get_playback_state_func", (PyCFunction) sono_act_get_playback_state_func, METH_VARARGS, "perform get playback state device action" },
	{"sono_act_get_volume_func", (PyCFunction) sono_act_get_volume_func, METH_VARARGS, "perform get volume device action" },
	{"sono_act_next_func", (PyCFunction) sono_act_next_func, METH_VARARGS, "perform next track device action" },
	{"sono_act_pause_func", (PyCFunction) sono_act_pause_func, METH_VARARGS, "perform pause track device action" },
	{"sono_act_play_func", (PyCFunction) sono_act_play_func, METH_VARARGS, "perform play track device action" },
	{"sono_act_previous_func", (PyCFunction) sono_act_previous_func, METH_VARARGS, "perform previous track device action" },
	{"sono_act_restart_func", (PyCFunction) sono_act_restart_func, METH_VARARGS, "perform restart track device action" },
	{"sono_act_seek_func", (PyCFunction) sono_act_seek_func, METH_VARARGS, "perform seek within track device action" },
	{"sono_act_set_mute_func", (PyCFunction) sono_act_set_mute_func, METH_VARARGS, "perform set mute device action" },
	{"sono_act_set_volume_func", (PyCFunction) sono_act_set_volume_func, METH_VARARGS, "perform set volume device action" },
	{"sono_act_stop_func", (PyCFunction) sono_act_stop_func, METH_VARARGS, "perform stop track device action" },
	{"sono_err_func", (PyCFunction) sono_err_func, METH_NOARGS, "sono library error string" },
	{"sono_init_func", (PyCFunction) sono_init_func, METH_VARARGS, "initialize sono library" },
	{"sono_uninit_func", (PyCFunction) sono_uninit_func, METH_NOARGS, "uninitialize sono library" },
	{"sono_ver_func", (PyCFunction) sono_ver_func, METH_NOARGS, "sono library version string" },
	{NULL, NULL, 0, NULL},
	};

// Module initialization routine
PyMODINIT_FUNC
initsono_module(void)
{
	(void) Py_InitModule("sono_py", sonoMethods);
}
