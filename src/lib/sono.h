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

#ifndef SONO_H_
#define SONO_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Argument name/value max length (in bytes)
#define SONO_ARG_LEN 32

// Device address max length (in bytes)
#define SONO_DEV_ADDR_LEN 24

/*
 * Determine if call to library was successful
 * @param _ERR_ Library error code return
 * @return Boolean
 */
#define SONO_SUCCESS(_ERR_) ((_ERR_) == SONO_ERR_NONE)

// Library error codes
typedef enum {
	SONO_ERR_NONE = 0,	// no error
	SONO_ERR_FAILED,	// call failed
	SONO_ERR_INVALID,	// user passed invalid arguments
} sono_err_t;

/*
 * Device information structure
 */
typedef struct {

	// device uid
	uint8_t id;

	// device address (as string)
	char addr[SONO_DEV_ADDR_LEN];

	// device port
	uint16_t port;
} sono_dev;

/*
 * Device action arguments structure
 */
typedef struct {

	// action argument name (as string)
	char name[SONO_ARG_LEN];

	// action argument value (as string)
	char val[SONO_ARG_LEN];
} sono_dev_arg;

/*
 * Device service event callback
 * @param id Device uid
 * @param svc Device service name
 * @param act Device action name
 * @param data Event data
 */
typedef void (*sono_evt_cb)(
	/*__in*/ uint32_t id,
	/*__in*/ const char *svc,
	/*__in*/ const char *act,
	/*__in*/ const char *data
	);

/*
 * Initialize library
 * (NOTE: This routine must be called prior to calling any other library routines)
 * @param hdl Device service event callback
 * @return Error code
 */
sono_err_t sono_init(
	/*__in*/ sono_evt_cb hdl
	);

/*
 * Perfom device service action
 * @param dev Target device
 * @param svc Target service name
 * @param act Target action name
 * @param in Input arguments
 * @param in_cnt Input argument count
 * @param out Output arguments
 * @param out_cnt Output argument count
 * @param tmout Timeout (in seconds)
 * @return Error code
 */
sono_err_t sono_dev_act(
	/*__in*/ const sono_dev *dev,
	/*__in*/ const char *svc,
	/*__in*/ const char *act,
	/*__in_opt*/ const sono_dev_arg *in,
	/*__in_opt*/ uint8_t in_cnt,
	/*__inout_opt*/ sono_dev_arg *out,
	/*__inout_opt*/ uint8_t *out_cnt,
	/*__in_opt*/ uint32_t tmout
	);

/*
 * Discover available devices
 * @param cnt Device count
 * @param disc_tmout Device discovery timeout (in seconds)
 * @param svc_tmout Device service discovery timeout (in seconds)
 * @return Error code
 */
sono_err_t sono_dev_disc(
	/*__out*/ uint8_t *cnt,
	/*__in_opt*/ uint32_t disc_tmout,
	/*__in_opt*/ uint32_t svc_tmout
	);

/*
 * List available devices
 * @param lst Device list
 * @param cnt Device count
 * @return Error code
 */
sono_err_t sono_dev_list(
	/*__inout*/ sono_dev *lst,
	/*__inout*/ uint8_t *cnt
	);

/*
 * Library last error code
 * @return Error code
 */
const char *sono_err(void);

/*
 * Uninitialize library
 * (NOTE: This routine must be called after all other library routines)
 * @return Error code
 */
sono_err_t sono_uninit(void);

/*
 * Library version
 * @return version as a string
 */
const char *sono_ver(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SONO_H_
