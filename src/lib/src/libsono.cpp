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
#include "../libsono.h"
#include "../include/sono.h"

#define SONO_INVALID_PARAMETER "Invalid parameter"

#define DEVICE_DISCOVERY_TIMEOUT 1 // sec
#define SERVICE_DISCOVERY_TIMEOUT 2 // sec

static std::string g_err;
static sono_evt_cb g_evt_hdl = NULL;

void 
sono_evt_hdl(
	__in sono_uid_t id,
	__in const std::string &svc,
	__in const std::string &act,
	__in const std::string &data
	)
{

	if(g_evt_hdl) {
		g_evt_hdl((uint8_t) id, svc.c_str(), act.c_str(), data.c_str());
	}
}

sono_err_t 
sono_init(
	__in sono_evt_cb evt_hdl
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

	if(evt_hdl) {
		g_evt_hdl = evt_hdl;
	}

exit:
	return result;
}

sono_err_t 
sono_dev_act(
	__in const sono_dev *dev,
	__in const char *dev_svc,
	__in const char *dev_act,
	__in_opt const sono_dev_param *param_in,
	__in_opt uint8_t param_in_cnt,
	__inout_opt sono_dev_param *param_out,
	__inout_opt uint8_t *param_out_cnt
	)
{
	sono_manager *instance = NULL;
	sono_err_t result = SONO_ERR_NONE;

	g_err.clear();

	if(!dev || !dev_svc || !dev_act 
			|| (param_in && !param_in_cnt) || (!param_in && param_in_cnt) 
			|| (param_out && (!param_out_cnt || !*param_out_cnt)) || (!param_out && param_out_cnt)) {
		g_err = SONO_INVALID_PARAMETER;
		result = SONO_ERR_INVALID;
		goto exit;
	}

	try {
		instance = sono_manager::acquire();

		// TODO

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

	if(!SONO_SUCCESS(result) && param_out && param_out_cnt && *param_out_cnt) {
		memset(param_out, 0, sizeof(sono_dev_param) * *param_out_cnt);
		*param_out_cnt = 0;
	}

	return result;
}

sono_err_t 
sono_dev_disc(
	__out uint8_t *dev_cnt
	)
{
	
	sono_device_list dev_list;
	sono_service_list svc_list;
	sono_manager *instance = NULL;
	sono_err_t result = SONO_ERR_NONE;
	sono_device_list::iterator dev_iter;

	g_err.clear();

	if(!dev_cnt) {
		g_err = SONO_INVALID_PARAMETER;
		result = SONO_ERR_INVALID;
		goto exit;
	}

	try {
		instance = sono_manager::acquire();

		dev_list = instance->device_discovery(DEVICE_DISCOVERY_TIMEOUT);
		for(dev_iter = dev_list.begin(); dev_iter != dev_list.end(); ++dev_iter) {
			instance->device(dev_iter->second.second.first, dev_iter->second.second.second).service_discovery(
				SERVICE_DISCOVERY_TIMEOUT);
		}

		*dev_cnt = (uint8_t) dev_list.size();
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
	__inout sono_dev *dev_lst,
	__inout uint8_t *dev_cnt
	)
{
	uint8_t iter = 0;
	sono_device_list dev_list;
	sono_err_t result = SONO_ERR_NONE;
	sono_device_list::iterator iter_dev;

	g_err.clear();

	if(!dev_lst || !dev_cnt || !*dev_cnt) {
		g_err = SONO_INVALID_PARAMETER;
		result = SONO_ERR_INVALID;
		goto exit;
	}

	try {
		dev_list = sono_manager::acquire()->device_list();

		for(iter_dev = dev_list.begin(); iter_dev != dev_list.end(); ++iter_dev, ++iter) {

			if(iter >= *dev_cnt) {
				break;
			}

			memcpy(dev_lst[iter].addr, (const char *) &iter_dev->second.second.first[0], 
				iter_dev->second.second.first.size());
			dev_lst[iter].id = iter_dev->first;
			dev_lst[iter].port = iter_dev->second.second.second;
		}

		*dev_cnt = iter;
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

	if(!SONO_SUCCESS(result) && dev_lst && dev_cnt) {
		memset(dev_lst, 0, sizeof(sono_dev) * *dev_cnt);
		*dev_cnt = 0;
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

	g_evt_hdl = NULL;

exit:
	return result;
}

const char *
sono_version(void)
{
	return sono_manager::version().c_str();
}
