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

#include "../include/sono_manager.h"
#include "../include/sono_controller_type.h"

namespace SONO {

	namespace COMP {
		
		#define SONO_CHANNEL_MASTER "Master"
		#define SONO_DESIRED_FIRST_TRACK_NUMBER_ENQUEUED_DEFAULT "0"
		#define SONO_ENQUEUE_AS_NEXT_DEFAULT "1"
		#define SONO_INPUT_DESIRED_FIRST_TRACK_NUMBER_ENQUEUED "DesiredFirstTrackNumberEnqueued"
		#define SONO_INPUT_DESIRED_MUTE "DesiredMute"
		#define SONO_INPUT_DESIRED_VOLUME "DesiredVolume"
		#define SONO_INPUT_CHANNEL "Channel"
		#define SONO_INPUT_ENQUEUE_AS_NEXT "EnqueueAsNext"
		#define SONO_INPUT_ENQUEUE_URI "EnqueuedURI"
		#define SONO_INPUT_ENQUEUE_URI_META_DATA "EnqueuedURIMetaData"
		#define SONO_INPUT_INSTANCE_ID "InstanceID"
		#define SONO_INPUT_SPEED "Speed"
		#define SONO_INPUT_TARGET "Target"
		#define SONO_INPUT_UNIT "Unit"
		#define SONO_INSTANCE_DEFAULT "0"
		#define SONO_OUTPUT_CURRENT_MUTE "CurrentMute"
		#define SONO_OUTPUT_CURRENT_TRANSPORT_STATE "CurrentTransportState"
		#define SONO_OUTPUT_CURRENT_VOLUME "CurrentVolume"
		#define SONO_SERVICE_ACTION_ADD_TO_QUEUE "AddURIToQueue"
		#define SONO_SERVICE_ACTION_CLEAR_QUEUE "RemoveAllTracksFromQueue"
		#define SONO_SERVICE_ACTION_GET_MUTE "GetMute"
		#define SONO_SERVICE_ACTION_GET_PLAYBACK_STATE "GetTransportInfo"
		#define SONO_SERVICE_ACTION_GET_VOLUME "GetVolume"
		#define SONO_SERVICE_ACTION_NEXT "Next"
		#define SONO_SERVICE_ACTION_PAUSE "Pause"
		#define SONO_SERVICE_ACTION_PLAY "Play"
		#define SONO_SERVICE_ACTION_PREVIOUS "Previous"
		#define SONO_SERVICE_ACTION_SEEK "Seek"
		#define SONO_SERVICE_ACTION_SET_MUTE "SetMute"
		#define SONO_SERVICE_ACTION_SET_VOLUME "SetVolume"
		#define SONO_SERVICE_ACTION_STOP "Stop"
		#define SONO_SERVICE_AV_TRANSPORT "AVTransport"
		#define SONO_SERVICE_RENDERING_CONTROL "RenderingControl"
		#define SONO_SPEED_DEFAULT "1"
		#define SONO_TARGET_DEFAULT "00:00:00"
		#define SONO_UNIT_DEFAULT "REL_TIME"
		#define SONO_URI_PREFIX_FILE "x-file-cifs://"

		static const std::string SONO_STATE_STR[] = {
			"PLAYING", "PAUSED_PLAYBACK", "STOPPED",
			};

		#define SONO_STATE_STRING(_TYPE_) \
			((_TYPE_) > SONO_STATE_MAX ? STRING_UNKNOWN : \
			STRING_CHECK(SONO_STATE_STR[_TYPE_]))

		_sono_controller *_sono_controller::m_instance = NULL;

		_sono_controller::_sono_controller(void) :
			m_initialized(false)
		{
			std::atexit(sono_controller::_delete);
		}

		_sono_controller::~_sono_controller(void)
		{

			if(m_initialized) {
				uninitialize();
			}
		}

		void 
		_sono_controller::_delete(void)
		{

			if(sono_controller::m_instance) {
				delete sono_controller::m_instance;
				sono_controller::m_instance = NULL;
			}
		}

		_sono_controller *
		_sono_controller::acquire(void)
		{

			if(!sono_controller::m_instance) {

				sono_controller::m_instance = new sono_controller;
				if(!sono_controller::m_instance) {
					THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_ALLOCATED);
				}
			}

			return sono_controller::m_instance;
		}

		void 
		_sono_controller::add_to_queue(
			__in const std::string &address,
			__in uint16_t port,
			__in const std::string &path,
			__in_opt uint32_t service_timeout,
			__in_opt uint32_t action_timeout
			)
		{
			std::stringstream stream;
			sono_action_argument input;

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			stream << SONO_URI_PREFIX_FILE << path;
			input.insert(std::pair<std::string, std::string>(SONO_INPUT_ENQUEUE_URI, stream.str()));
			input.insert(std::pair<std::string, std::string>(SONO_INPUT_ENQUEUE_URI_META_DATA, std::string()));
			input.insert(std::pair<std::string, std::string>(SONO_INPUT_DESIRED_FIRST_TRACK_NUMBER_ENQUEUED, 
				SONO_DESIRED_FIRST_TRACK_NUMBER_ENQUEUED_DEFAULT));
			input.insert(std::pair<std::string, std::string>(SONO_INPUT_ENQUEUE_AS_NEXT, SONO_ENQUEUE_AS_NEXT_DEFAULT));
			input.insert(std::pair<std::string, std::string>(SONO_INPUT_INSTANCE_ID, SONO_INSTANCE_DEFAULT));
			run(address, port, SONO_SERVICE_AV_TRANSPORT, SONO_SERVICE_ACTION_ADD_TO_QUEUE, input, 
				service_timeout, action_timeout);
		}

		void 
		_sono_controller::clear_queue(
			__in const std::string &address,
			__in uint16_t port,
			__in_opt uint32_t service_timeout,
			__in_opt uint32_t action_timeout
			)
		{
			sono_action_argument input;

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			input.insert(std::pair<std::string, std::string>(SONO_INPUT_INSTANCE_ID, SONO_INSTANCE_DEFAULT));
			run(address, port, SONO_SERVICE_AV_TRANSPORT, SONO_SERVICE_ACTION_CLEAR_QUEUE, input, 
				service_timeout, action_timeout);
		}

		bool 
		_sono_controller::get_mute(
			__in const std::string &address,
			__in uint16_t port,
			__in_opt uint32_t service_timeout,
			__in_opt uint32_t action_timeout
			)
		{
			sono_action_argument input;
			sono_action_argument::iterator iter;

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			input.insert(std::pair<std::string, std::string>(SONO_INPUT_INSTANCE_ID, SONO_INSTANCE_DEFAULT));
			input.insert(std::pair<std::string, std::string>(SONO_INPUT_CHANNEL, SONO_CHANNEL_MASTER));
			run(address, port, SONO_SERVICE_RENDERING_CONTROL, SONO_SERVICE_ACTION_GET_MUTE, input, 
				service_timeout, action_timeout);

			iter = m_results.find(SONO_OUTPUT_CURRENT_MUTE);
			if(iter == m_results.end()) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_MALFORMED);
			}

			return (std::atoi(iter->second.c_str()) > 0);
		}

		sono_state_t 
		_sono_controller::get_playback_state(
			__in const std::string &address,
			__in uint16_t port,
			__in_opt uint32_t service_timeout,
			__in_opt uint32_t action_timeout
			)
		{
			size_t iter = 0;
			sono_action_argument input;
			sono_action_argument::iterator iter_out;

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			input.insert(std::pair<std::string, std::string>(SONO_INPUT_INSTANCE_ID, SONO_INSTANCE_DEFAULT));
			run(address, port, SONO_SERVICE_AV_TRANSPORT, SONO_SERVICE_ACTION_GET_PLAYBACK_STATE, input, 
				service_timeout, action_timeout);

			iter_out = m_results.find(SONO_OUTPUT_CURRENT_TRANSPORT_STATE);
			if(iter_out == m_results.end()) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_MALFORMED);
			}

			for(; iter <= SONO_STATE_MAX; ++iter) {

				if(SONO_STATE_STRING(iter) == iter_out->second) {
					break;
				}
			}

			return (iter > SONO_STATE_MAX) ? SONO_STATE_INVALID : (sono_state_t) iter;
		}

		uint32_t 
		_sono_controller::get_volume(
			__in const std::string &address,
			__in uint16_t port,
			__in_opt uint32_t service_timeout,
			__in_opt uint32_t action_timeout
			)
		{
			sono_action_argument input;
			sono_action_argument::iterator iter;

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			input.insert(std::pair<std::string, std::string>(SONO_INPUT_INSTANCE_ID, SONO_INSTANCE_DEFAULT));
			input.insert(std::pair<std::string, std::string>(SONO_INPUT_CHANNEL, SONO_CHANNEL_MASTER));
			run(address, port, SONO_SERVICE_RENDERING_CONTROL, SONO_SERVICE_ACTION_GET_VOLUME, input, 
				service_timeout, action_timeout);

			iter = m_results.find(SONO_OUTPUT_CURRENT_VOLUME);
			if(iter == m_results.end()) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_MALFORMED);
			}

			return std::atoi(iter->second.c_str());
		}

		void 
		_sono_controller::initialize(void)
		{

			if(m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_INITIALIZED);
			}

			m_results.clear();
			m_initialized = true;
		}

		bool 
		_sono_controller::is_allocated(void)
		{
			return (sono_controller::m_instance != NULL);
		}

		bool 
		_sono_controller::is_initialized(void)
		{
			return m_initialized;
		}

		void 
		_sono_controller::next(
			__in const std::string &address,
			__in uint16_t port,
			__in_opt uint32_t service_timeout,
			__in_opt uint32_t action_timeout
			)
		{
			sono_action_argument input;

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			input.insert(std::pair<std::string, std::string>(SONO_INPUT_INSTANCE_ID, SONO_INSTANCE_DEFAULT));
			run(address, port, SONO_SERVICE_AV_TRANSPORT, SONO_SERVICE_ACTION_NEXT, input, 
				service_timeout, action_timeout);
		}

		void 
		_sono_controller::pause(
			__in const std::string &address,
			__in uint16_t port,
			__in_opt uint32_t service_timeout,
			__in_opt uint32_t action_timeout
			)
		{
			sono_action_argument input;

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			input.insert(std::pair<std::string, std::string>(SONO_INPUT_INSTANCE_ID, SONO_INSTANCE_DEFAULT));
			run(address, port, SONO_SERVICE_AV_TRANSPORT, SONO_SERVICE_ACTION_PAUSE, input, 
				service_timeout, action_timeout);
		}

		void 
		_sono_controller::play(
			__in const std::string &address,
			__in uint16_t port,
			__in_opt uint32_t service_timeout,
			__in_opt uint32_t action_timeout
			)
		{
			sono_action_argument input;

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			input.insert(std::pair<std::string, std::string>(SONO_INPUT_INSTANCE_ID, SONO_INSTANCE_DEFAULT));
			input.insert(std::pair<std::string, std::string>(SONO_INPUT_SPEED, SONO_SPEED_DEFAULT));
			run(address, port, SONO_SERVICE_AV_TRANSPORT, SONO_SERVICE_ACTION_PLAY, input, 
				service_timeout, action_timeout);
		}

		void 
		_sono_controller::previous(
			__in const std::string &address,
			__in uint16_t port,
			__in_opt uint32_t service_timeout,
			__in_opt uint32_t action_timeout
			)
		{
			sono_action_argument input;

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			input.insert(std::pair<std::string, std::string>(SONO_INPUT_INSTANCE_ID, SONO_INSTANCE_DEFAULT));
			run(address, port, SONO_SERVICE_AV_TRANSPORT, SONO_SERVICE_ACTION_PREVIOUS, input, 
				service_timeout, action_timeout);
		}

		void 
		_sono_controller::restart(
			__in const std::string &address,
			__in uint16_t port,
			__in_opt uint32_t service_timeout,
			__in_opt uint32_t action_timeout
			)
		{
			sono_action_argument input;

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			input.insert(std::pair<std::string, std::string>(SONO_INPUT_INSTANCE_ID, SONO_INSTANCE_DEFAULT));
			input.insert(std::pair<std::string, std::string>(SONO_INPUT_TARGET, SONO_TARGET_DEFAULT));
			input.insert(std::pair<std::string, std::string>(SONO_INPUT_UNIT, SONO_UNIT_DEFAULT));
			run(address, port, SONO_SERVICE_AV_TRANSPORT, SONO_SERVICE_ACTION_SEEK, input, 
				service_timeout, action_timeout);
		}

		void 
		_sono_controller::run(
			__in const std::string &address,
			__in uint16_t port,
			__in const std::string &service,
			__in const std::string &action,
			__in const sono_action_argument &input,
			__in_opt uint32_t service_timeout,
			__in_opt uint32_t action_timeout
			)
		{

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			sono_device &dev = sono_manager::acquire()->device(address, port, true);
			if(!dev.size()) {
				dev.service_discovery(service_timeout);
			}

			m_results = dev.service(service).run(action, input, action_timeout);
		}

		void 
		_sono_controller::seek(
			__in const std::string &address,
			__in uint16_t port,
			__in uint32_t hour,
			__in uint32_t minute,
			__in uint32_t second,
			__in_opt uint32_t service_timeout,
			__in_opt uint32_t action_timeout
			)
		{
			std::stringstream stream;
			sono_action_argument input;

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			stream << std::setw(2) << std::setfill('0') << hour << ":" << minute << ":" << second;
			input.insert(std::pair<std::string, std::string>(SONO_INPUT_INSTANCE_ID, SONO_INSTANCE_DEFAULT));
			input.insert(std::pair<std::string, std::string>(SONO_INPUT_TARGET, stream.str()));
			input.insert(std::pair<std::string, std::string>(SONO_INPUT_UNIT, SONO_UNIT_DEFAULT));
			run(address, port, SONO_SERVICE_AV_TRANSPORT, SONO_SERVICE_ACTION_SEEK, input, 
				service_timeout, action_timeout);
		}

		void 
		_sono_controller::set_mute(
			__in const std::string &address,
			__in uint16_t port,
			__in bool mute,
			__in_opt uint32_t service_timeout,
			__in_opt uint32_t action_timeout
			)
		{
			sono_action_argument input;

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			input.insert(std::pair<std::string, std::string>(SONO_INPUT_INSTANCE_ID, SONO_INSTANCE_DEFAULT));
			input.insert(std::pair<std::string, std::string>(SONO_INPUT_CHANNEL, SONO_CHANNEL_MASTER));
			input.insert(std::pair<std::string, std::string>(SONO_INPUT_DESIRED_MUTE, mute ? "1" : "0"));
			run(address, port, SONO_SERVICE_RENDERING_CONTROL, SONO_SERVICE_ACTION_SET_MUTE, input, 
				service_timeout, action_timeout);
		}

		void 
		_sono_controller::set_volume(
			__in const std::string &address,
			__in uint16_t port,
			__in uint32_t volume,
			__in_opt uint32_t service_timeout,
			__in_opt uint32_t action_timeout
			)
		{
			std::stringstream stream;
			sono_action_argument input;

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			stream << volume;
			input.insert(std::pair<std::string, std::string>(SONO_INPUT_INSTANCE_ID, SONO_INSTANCE_DEFAULT));
			input.insert(std::pair<std::string, std::string>(SONO_INPUT_CHANNEL, SONO_CHANNEL_MASTER));
			input.insert(std::pair<std::string, std::string>(SONO_INPUT_DESIRED_VOLUME, stream.str()));
			run(address, port, SONO_SERVICE_RENDERING_CONTROL, SONO_SERVICE_ACTION_SET_VOLUME, input, 
				service_timeout, action_timeout);
		}

		void 
		_sono_controller::stop(
			__in const std::string &address,
			__in uint16_t port,
			__in_opt uint32_t service_timeout,
			__in_opt uint32_t action_timeout
			)
		{
			sono_action_argument input;

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			input.insert(std::pair<std::string, std::string>(SONO_INPUT_INSTANCE_ID, SONO_INSTANCE_DEFAULT));
			run(address, port, SONO_SERVICE_AV_TRANSPORT, SONO_SERVICE_ACTION_STOP, input, 
				service_timeout, action_timeout);
		}

		std::string 
		_sono_controller::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			result << SONO_CONTROLLER_HEADER << " -- " << (m_initialized ? "INIT" : "UNINIT");

			if(m_initialized) {
				result << ", PTR. 0x" << SCALAR_AS_HEX(sono_controller *, this);
			}

			return result.str();
		}

		void 
		_sono_controller::uninitialize(void)
		{

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			m_results.clear();
			m_initialized = false;
		}
	}
}
