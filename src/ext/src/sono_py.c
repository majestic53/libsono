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

#include <python.h>
#include "../../lib/sono.h"

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
	PyObject *result = NULL;
	sono_evt_cb handler = NULL;
	sono_err_t status = SONO_ERR_NONE;
	
	if(!PyArg_ParseTuple(args, "w#", &handler, &length)
			|| (length != sizeof(sono_evt_cb))) {
		status = SONO_ERR_INVALID;
		goto exit;
	}
	
	status = sono_init(handle);
	
exit:
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
	{"sono_init_func", (PyCFunction) sono_init_func, METH_VARARGS, "initialize sono library" },
	{"sono_err_func", (PyCFunction) sono_err_func, METH_NOARGS, "sono library error string" },
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
