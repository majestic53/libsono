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

#include <cstring>
#include "../sono.h"
#include "../include/sono_manager.h"

#define SONO_INVALID_ARGUMENT "Invalid argument"

static std::string g_err;
static sono_evt_cb g_hdl = NULL;

void 
sono_evt_hdl(
	__in sono_uid_t id,
	__in const std::string &svc,
	__in const std::string &act,
	__in const std::string &data
	)
{

	if(g_hdl) {
		g_hdl((uint8_t) id, svc.c_str(), act.c_str(), data.c_str());
	}
}

sono_err_t 
sono_init(
	__in sono_evt_cb hdl
	)
{
	sono_err_t result = SONO_ERR_NONE;

	g_err.clear();

	try {
		sono_manager::acquire()->initialize(sono_evt_hdl);
	} catch(sono_exception &exc) {
		g_err = exc.to_string(true);
		result = SONO_ERR_FAILED;
		goto exit;
	} catch(std::exception &exc) {
		g_err = exc.what();
		result = SONO_ERR_FAILED;
		goto exit;
	}

	if(hdl) {
		g_hdl = hdl;
	}

exit:
	return result;
}

sono_err_t 
sono_dev_act(
	__in const sono_dev *dev,
	__in const char *svc,
	__in const char *act,
	__in_opt const sono_dev_arg *in,
	__in_opt uint8_t in_cnt,
	__inout_opt sono_dev_arg *out,
	__inout_opt uint8_t *out_cnt,
	/*__in_opt*/ uint32_t tmout
	)
{
	sono_err_t result = SONO_ERR_NONE;
	sono_action_argument input, output;

	g_err.clear();

	if(!dev || !svc || !act || (in && !in_cnt) || (!in && in_cnt) 
			|| (out && (!out_cnt || !*out_cnt)) || (!out && out_cnt)) {
		g_err = SONO_INVALID_ARGUMENT;
		result = SONO_ERR_INVALID;
		goto exit;
	}

	try {

		// TODO: populate input map

		output = sono_manager::acquire()->device(dev->addr, dev->port).service(svc).run(
			act, input, tmout);

		// TODO: populate outout map

	} catch(sono_exception &exc) {
		g_err = exc.to_string(true);
		result = SONO_ERR_FAILED;
		goto exit;
	} catch(std::exception &exc) {
		g_err = exc.what();
		result = SONO_ERR_FAILED;
		goto exit;
	}

exit:

	if(!SONO_SUCCESS(result) && out && out_cnt && *out_cnt) {
		memset(out, 0, sizeof(sono_dev_arg) * *out_cnt);
		*out_cnt = 0;
	}

	return result;
}

sono_err_t 
sono_dev_disc(
	__out uint8_t *cnt,
	/*__in_opt*/ uint32_t tmout
	)
{
	
	sono_device_list dev_list;
	sono_service_list svc_list;
	sono_manager *instance = NULL;
	sono_err_t result = SONO_ERR_NONE;
	sono_device_list::iterator dev_iter;

	g_err.clear();

	if(!cnt) {
		g_err = SONO_INVALID_ARGUMENT;
		result = SONO_ERR_INVALID;
		goto exit;
	}

	try {
		instance = sono_manager::acquire();

		dev_list = instance->device_discovery(tmout);
		for(dev_iter = dev_list.begin(); dev_iter != dev_list.end(); ++dev_iter) {
			instance->device(dev_iter->second.second.first, dev_iter->second.second.second)
				.service_discovery(tmout);
		}

		*cnt = (uint8_t) dev_list.size();
	} catch(sono_exception &exc) {
		g_err = exc.to_string(true);
		result = SONO_ERR_FAILED;
		goto exit;
	} catch(std::exception &exc) {
		g_err = exc.what();
		result = SONO_ERR_FAILED;
		goto exit;
	}

exit:
	return result;
}

const char *
sono_err(void)
{
	return g_err.c_str();
}

sono_err_t 
sono_dev_list(
	__inout sono_dev *lst,
	__inout uint8_t *cnt
	)
{
	uint8_t iter = 0;
	sono_device_list dev_list;
	sono_err_t result = SONO_ERR_NONE;
	sono_device_list::iterator iter_dev;

	g_err.clear();

	if(!lst || !cnt || !*cnt) {
		g_err = SONO_INVALID_ARGUMENT;
		result = SONO_ERR_INVALID;
		goto exit;
	}

	try {
		dev_list = sono_manager::acquire()->device_list();

		for(iter_dev = dev_list.begin(); iter_dev != dev_list.end(); ++iter_dev, ++iter) {

			if(iter >= *cnt) {
				break;
			}

			memcpy(lst[iter].addr, (const char *) &iter_dev->second.second.first[0], 
				iter_dev->second.second.first.size());
			lst[iter].id = iter_dev->first;
			lst[iter].port = iter_dev->second.second.second;
		}

		*cnt = iter;
	} catch(sono_exception &exc) {
		g_err = exc.to_string(true);
		result = SONO_ERR_FAILED;
		goto exit;
	} catch(std::exception &exc) {
		g_err = exc.what();
		result = SONO_ERR_FAILED;
		goto exit;
	}

exit:

	if(!SONO_SUCCESS(result) && lst && cnt) {
		memset(lst, 0, sizeof(sono_dev) * *cnt);
		*cnt = 0;
	}

	return result;
}

sono_err_t 
sono_uninit(void)
{
	sono_err_t result = SONO_ERR_NONE;

	g_err.clear();

	try {
		sono_manager::acquire()->uninitialize();
	} catch(sono_exception &exc) {
		g_err = exc.to_string(true);
		result = SONO_ERR_FAILED;
		goto exit;
	} catch(std::exception &exc) {
		g_err = exc.what();
		result = SONO_ERR_FAILED;
		goto exit;
	}

	g_hdl = NULL;

exit:
	return result;
}

const char *
sono_ver(void)
{
	return sono_manager::version().c_str();
}
