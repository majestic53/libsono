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

#include <algorithm>
#include <cstring>
#include "../sono.h"
#include "../include/sono_manager.h"

#define SONO_INVALID_ARGUMENT "Invalid argument"

static std::string g_err, g_ver;
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
sono_dev_act_add(
	__in const sono_dev *dev,
	__in const char *path,
	__in_opt uint32_t svc_tmout,
	__in_opt uint32_t act_tmout
	)
{
	sono_err_t result = SONO_ERR_NONE;

	g_err.clear();

	if(!dev || !path) {
		g_err = SONO_INVALID_ARGUMENT;
		result = SONO_ERR_INVALID;
		goto exit;
	}

	try {
		sono_manager::acquire()->control()->add_to_queue(dev->addr, dev->port, path, 
			svc_tmout, act_tmout);
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

sono_err_t 
sono_dev_act_clear(
	__in const sono_dev *dev,
	__in_opt uint32_t svc_tmout,
	__in_opt uint32_t act_tmout
	)
{
	sono_err_t result = SONO_ERR_NONE;

	g_err.clear();

	if(!dev) {
		g_err = SONO_INVALID_ARGUMENT;
		result = SONO_ERR_INVALID;
		goto exit;
	}

	try {
		sono_manager::acquire()->control()->clear_queue(dev->addr, dev->port, svc_tmout, 
			act_tmout);
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

sono_err_t 
sono_dev_act_get_mute(
	__out uint8_t *mute,
	__in const sono_dev *dev,
	__in_opt uint32_t svc_tmout,
	__in_opt uint32_t act_tmout
	)
{
	sono_err_t result = SONO_ERR_NONE;

	g_err.clear();

	if(!dev || !mute) {
		g_err = SONO_INVALID_ARGUMENT;
		result = SONO_ERR_INVALID;
		goto exit;
	}

	try {
		*mute = sono_manager::acquire()->control()->get_mute(dev->addr, dev->port, 
			svc_tmout, act_tmout);
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

sono_err_t 
sono_dev_act_get_playback_state(
	__out uint32_t *state,
	__in const sono_dev *dev,
	__in_opt uint32_t svc_tmout,
	__in_opt uint32_t act_tmout
	)
{
	sono_err_t result = SONO_ERR_NONE;

	g_err.clear();

	if(!dev || !state) {
		g_err = SONO_INVALID_ARGUMENT;
		result = SONO_ERR_INVALID;
		goto exit;
	}

	try {
		*state = sono_manager::acquire()->control()->get_playback_state(dev->addr, dev->port, 
			svc_tmout, act_tmout);
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

sono_err_t 
sono_dev_act_get_volume(
	__out uint32_t *volume,
	__in const sono_dev *dev,
	__in_opt uint32_t svc_tmout,
	__in_opt uint32_t act_tmout
	)
{
	sono_err_t result = SONO_ERR_NONE;

	g_err.clear();

	if(!dev || !volume) {
		g_err = SONO_INVALID_ARGUMENT;
		result = SONO_ERR_INVALID;
		goto exit;
	}

	try {
		*volume = sono_manager::acquire()->control()->get_volume(dev->addr, dev->port, 
			svc_tmout, act_tmout);
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

sono_err_t 
sono_dev_act_next(
	__in const sono_dev *dev,
	__in_opt uint32_t svc_tmout,
	__in_opt uint32_t act_tmout
	)
{
	sono_err_t result = SONO_ERR_NONE;

	g_err.clear();

	if(!dev) {
		g_err = SONO_INVALID_ARGUMENT;
		result = SONO_ERR_INVALID;
		goto exit;
	}

	try {
		sono_manager::acquire()->control()->next(dev->addr, dev->port, svc_tmout, 
			act_tmout);
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

sono_err_t 
sono_dev_act_pause(
	__in const sono_dev *dev,
	__in_opt uint32_t svc_tmout,
	__in_opt uint32_t act_tmout
	)
{
	sono_err_t result = SONO_ERR_NONE;

	g_err.clear();

	if(!dev) {
		g_err = SONO_INVALID_ARGUMENT;
		result = SONO_ERR_INVALID;
		goto exit;
	}

	try {
		sono_manager::acquire()->control()->pause(dev->addr, dev->port, svc_tmout, 
			act_tmout);
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

sono_err_t 
sono_dev_act_play(
	__in const sono_dev *dev,
	__in_opt uint32_t svc_tmout,
	__in_opt uint32_t act_tmout
	)
{
	sono_err_t result = SONO_ERR_NONE;

	g_err.clear();

	if(!dev) {
		g_err = SONO_INVALID_ARGUMENT;
		result = SONO_ERR_INVALID;
		goto exit;
	}

	try {
		sono_manager::acquire()->control()->play(dev->addr, dev->port, svc_tmout, 
			act_tmout);
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

sono_err_t 
sono_dev_act_previous(
	__in const sono_dev *dev,
	__in_opt uint32_t svc_tmout,
	__in_opt uint32_t act_tmout
	)
{
	sono_err_t result = SONO_ERR_NONE;

	g_err.clear();

	if(!dev) {
		g_err = SONO_INVALID_ARGUMENT;
		result = SONO_ERR_INVALID;
		goto exit;
	}

	try {
		sono_manager::acquire()->control()->previous(dev->addr, dev->port, svc_tmout, 
			act_tmout);
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

sono_err_t 
sono_dev_act_restart(
	__in const sono_dev *dev,
	__in_opt uint32_t svc_tmout,
	__in_opt uint32_t act_tmout
	)
{
	sono_err_t result = SONO_ERR_NONE;

	g_err.clear();

	if(!dev) {
		g_err = SONO_INVALID_ARGUMENT;
		result = SONO_ERR_INVALID;
		goto exit;
	}

	try {
		sono_manager::acquire()->control()->restart(dev->addr, dev->port, svc_tmout, 
			act_tmout);
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

sono_err_t 
sono_dev_act_seek(
	__in uint32_t hour,
	__in uint32_t minute,
	__in uint32_t second,
	__in const sono_dev *dev,
	__in_opt uint32_t svc_tmout,
	__in_opt uint32_t act_tmout
	)
{
	sono_err_t result = SONO_ERR_NONE;

	g_err.clear();

	if(!dev) {
		g_err = SONO_INVALID_ARGUMENT;
		result = SONO_ERR_INVALID;
		goto exit;
	}

	try {
		sono_manager::acquire()->control()->seek(dev->addr, dev->port, hour, minute,
			second, svc_tmout, act_tmout);
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

sono_err_t 
sono_dev_act_set_mute(
	__in uint8_t mute,
	__in const sono_dev *dev,
	__in_opt uint32_t svc_tmout,
	__in_opt uint32_t act_tmout
	)
{
	sono_err_t result = SONO_ERR_NONE;

	g_err.clear();

	if(!dev) {
		g_err = SONO_INVALID_ARGUMENT;
		result = SONO_ERR_INVALID;
		goto exit;
	}

	try {
		sono_manager::acquire()->control()->set_mute(dev->addr, dev->port, mute, 
			svc_tmout, act_tmout);
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

sono_err_t 
sono_dev_act_set_volume(
	__in uint32_t volume,
	__in const sono_dev *dev,
	__in_opt uint32_t svc_tmout,
	__in_opt uint32_t act_tmout
	)
{
	sono_err_t result = SONO_ERR_NONE;

	g_err.clear();

	if(!dev) {
		g_err = SONO_INVALID_ARGUMENT;
		result = SONO_ERR_INVALID;
		goto exit;
	}

	try {
		sono_manager::acquire()->control()->set_volume(dev->addr, dev->port, volume, 
			svc_tmout, act_tmout);
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

sono_err_t 
sono_dev_act_stop(
	__in const sono_dev *dev,
	__in_opt uint32_t svc_tmout,
	__in_opt uint32_t act_tmout
	)
{
	sono_err_t result = SONO_ERR_NONE;

	g_err.clear();

	if(!dev) {
		g_err = SONO_INVALID_ARGUMENT;
		result = SONO_ERR_INVALID;
		goto exit;
	}

	try {
		sono_manager::acquire()->control()->stop(dev->addr, dev->port, svc_tmout, 
			act_tmout);
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
	__in_opt uint32_t tmout
	)
{
	uint8_t iter;
	sono_err_t result = SONO_ERR_NONE;
	sono_action_argument input, output;
	sono_action_argument::iterator out_iter;

	g_err.clear();

	if(!dev || !svc || !act || (in && !in_cnt) || (!in && in_cnt) 
			|| (out && (!out_cnt || !*out_cnt)) || (!out && *out_cnt)) {
		g_err = SONO_INVALID_ARGUMENT;
		result = SONO_ERR_INVALID;
		goto exit;
	}

	try {

		for(iter = 0; iter < in_cnt; ++iter) {
			input.insert(std::pair<std::string, std::string>(in[iter].name, in[iter].val));
		}

		output = sono_manager::acquire()->device(dev->addr, dev->port).service(svc).run(
			act, input, tmout);

		for(out_iter = output.begin(), iter = 0; iter < *out_cnt; ++out_iter, ++iter) {

			if((out_iter == output.end())
					|| (iter >= output.size())) {
				break;
			}

			std::memset(out[iter].name, 0, SONO_ARG_LEN);
			std::memcpy(out[iter].name, out_iter->first.c_str(), 
				std::min((uint32_t) out_iter->first.size(), (uint32_t) SONO_ARG_LEN - 1));
			std::memset(out[iter].val, 0, SONO_ARG_LEN);
			std::memcpy(out[iter].val, out_iter->second.c_str(), 
				std::min((uint32_t) out_iter->second.size(), (uint32_t) SONO_ARG_LEN - 1));
		}

		if(iter < *out_cnt) {
			*out_cnt = iter;
		}

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
	__in_opt uint32_t disc_tmout,
	__in_opt uint32_t svc_tmout
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

		dev_list = instance->device_discovery(disc_tmout);
		for(dev_iter = dev_list.begin(); dev_iter != dev_list.end(); ++dev_iter) {
			instance->device(dev_iter->second.second.first, dev_iter->second.second.second)
				.service_discovery(svc_tmout);
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

			std::memset(lst[iter].addr, 0, SONO_DEV_ADDR_LEN);
			std::memcpy(lst[iter].addr, iter_dev->second.second.first.c_str(), 
				std::min((uint32_t) iter_dev->second.second.first.size(), 
				(uint32_t) SONO_DEV_ADDR_LEN - 1));
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

	if(g_ver.empty()) {
		g_ver = sono_manager::version();
	}

	return g_ver.c_str();
}
