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

#ifndef LIBSONO_H_
#define LIBSONO_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define SONO_DEV_ADDR_LEN 24
#define SONO_PARAM_LEN 64
#define SONO_SUCCESS(_ERR_) ((_ERR_) == SONO_ERR_NONE)

typedef enum {
	SONO_ERR_NONE = 0,
	SONO_ERR_FAILED,
	SONO_ERR_INVALID,
} sono_err_t;

typedef struct {
	uint8_t id;
	char addr[SONO_DEV_ADDR_LEN];
	uint16_t port;
} sono_dev;

typedef struct {
	char name[SONO_PARAM_LEN];
	char val[SONO_PARAM_LEN];
} sono_dev_param;

typedef void (*sono_evt_cb)(
	/*__in*/ uint8_t dev_id,
	/*__in*/ const char *svc,
	/*__in*/ const char *act,
	/*__in*/ const char *data
	);

sono_err_t sono_init(
	/*__in*/ sono_evt_cb evt_hdl
	);

sono_err_t sono_dev_act(
	/*__in*/ const sono_dev *dev,
	/*__in*/ const char *dev_svc,
	/*__in*/ const char *dev_act,
	/*__in_opt*/ const sono_dev_param *param_in,
	/*__in_opt*/ uint8_t param_in_cnt,
	/*__inout_opt*/ sono_dev_param *param_out,
	/*__inout_opt*/ uint8_t *param_out_cnt
	);

sono_err_t sono_dev_disc(
	/*__out*/ uint8_t *dev_cnt
	);

sono_err_t sono_dev_list(
	/*__inout*/ sono_dev *dev_lst,
	/*__inout*/ uint8_t *dev_cnt
	);

const char *sono_err(void);

sono_err_t sono_uninit(void);

const char *sono_version(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // LIBSONO_H_
