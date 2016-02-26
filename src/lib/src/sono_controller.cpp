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

		#define SONO_ACTION_ADD_URI_TO_QUEUE "AddURIToQueue"
		#define SONO_ACTION_GET_MEDIA_INFO "GetMediaInfo"
		#define SONO_ACTION_GET_MUTE "GetMute"
		#define SONO_ACTION_GET_POSITION_INFO "GetPositionInfo"
		#define SONO_ACTION_GET_TRANSPORT_INFO "GetTransportInfo"
		#define SONO_ACTION_GET_VOLUME "GetVolume"
		#define SONO_ACTION_NEXT "Next"
		#define SONO_ACTION_PAUSE "Pause"
		#define SONO_ACTION_PLAY "Play"
		#define SONO_ACTION_PREVIOUS "Previous"
		#define SONO_ACTION_REMOVE_ALL_TRACKS_FROM_QUEUE "RemoveAllTracksFromQueue"
		#define SONO_ACTION_SET_MUTE "SetMute"
		#define SONO_ACTION_SET_VOLUME "SetVolume"
		#define SONO_ACTION_STOP "Stop"
		#define SONO_SERVICE_AVTRANSPORT "AVTransport"
		#define SONO_SERVICE_RENDING_CONTROL "RenderingControl"

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
		_sono_controller::add(
			__in const std::string &address,
			__in uint16_t port,
			__in uint32_t instance,
			__in const std::string &uri,
			__in const std::string &metadata,
			__in_opt uint32_t position,
			__in_opt uint32_t next
			)
		{

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			// TODO: AVTransport::AddURIToQueue
		}

		void 
		_sono_controller::clear(
			__in const std::string &address,
			__in uint16_t port,
			__in uint32_t instance
			)
		{

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			// TODO: AVTransport::RemoveAllTracksFromQueue
		}

		void 
		_sono_controller::initialize(void)
		{

			if(m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_INITIALIZED);
			}

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

		bool 
		_sono_controller::is_muted(
			__in const std::string &address,
			__in uint16_t port,
			__in uint32_t instance,
			__in_opt const std::string &channel
			)
		{
			bool result;

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			// TODO: RenderingControl::GetMute
			result = false;
			// ---

			return result;
		}

		void 
		_sono_controller::next(
			__in const std::string &address,
			__in uint16_t port,
			__in uint32_t instance
			)
		{

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			// TODO: AVTransport::Next
		}

		void 
		_sono_controller::pause(
			__in const std::string &address,
			__in uint16_t port,
			__in uint32_t instance
			)
		{

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			// TODO: AVTransport::Pause
		}

		void 
		_sono_controller::play(
			__in const std::string &address,
			__in uint16_t port,
			__in uint32_t instance
			)
		{

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			// TODO: AVTransport::Play
		}

		void 
		_sono_controller::previous(
			__in const std::string &address,
			__in uint16_t port,
			__in uint32_t instance
			)
		{

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			// TODO: AVTransport::Previous
		}

		void 
		_sono_controller::set_mute(
			__in const std::string &address,
			__in uint16_t port,
			__in uint32_t instance,
			__in bool value,
			__in_opt const std::string &channel
			)
		{

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			// TODO: RenderingControl::SetMute
		}

		void 
		_sono_controller::set_volume(
			__in const std::string &address,
			__in uint16_t port,
			__in uint32_t instance,
			__in uint32_t value,
			__in_opt const std::string &channel
			)
		{

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			// TODO: RenderingControl::SetVolume
		}

		std::string 
		_sono_controller::state(
			__in const std::string &address,
			__in uint16_t port,
			__in uint32_t instance
			)
		{
			std::string result;

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			// TODO: AVTransport::GetTransportInfo

			return result;
		}

		void 
		_sono_controller::stop(
			__in const std::string &address,
			__in uint16_t port,
			__in uint32_t instance
			)
		{

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			// TODO: AVTransport::Stop
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

		uint32_t 
		_sono_controller::track_current(
			__in const std::string &address,
			__in uint16_t port,
			__in uint32_t instance
			)
		{
			uint32_t result;

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			// TODO: AVTransport::GetPositionInfo
			result = 0;
			// ---

			return result;
		}

		uint32_t 
		_sono_controller::track_total(
			__in const std::string &address,
			__in uint16_t port,
			__in uint32_t instance
			)
		{
			uint32_t result;

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			// TODO: AVTransport::GetMediaInfo
			result = 0;
			// ---

			return result;
		}

		void 
		_sono_controller::uninitialize(void)
		{

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			m_initialized = false;
		}

		uint32_t 
		_sono_controller::volume(
			__in const std::string &address,
			__in uint16_t port,
			__in uint32_t instance,
			__in_opt const std::string &channel
			)
		{
			uint32_t result;

			if(!m_initialized) {
				THROW_SONO_CONTROLLER_EXCEPTION(SONO_CONTROLLER_EXCEPTION_UNINITIALIZED);
			}

			// TODO: RenderingControl::GetVolume
			result = 0;
			// ---

			return result;
		}
	}
}
