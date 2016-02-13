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

#include "../include/sono.h"
#include "../include/sono_service_type.h"

namespace SONO {

	namespace COMP {

		static const std::string SONO_SERVICE_STR[] = {
			"ALARM_CLOCK",
			"DEVICE_PROPERTIES",
			"GROUP_MANAGEMENT",
			"MUSIC_SERVICES",
			"QPLAY",
			"RENDER_AV_TRANSPORT",
			"SONO_SERVICE_RENDER_CONNECTION_MANAGER",
			"SONO_SERVICE_RENDER_CONTROL",
			"SONO_SERVICE_RENDER_GROUP_CONTROL",
			"SONO_SERVICE_RENDER_QUEUE",
			"SONO_SERVICE_SERVER_CONNECTION_MANAGER",
			"SONO_SERVICE_SERVER_CONTENT_DIRECTORY",
			"SONO_SERVICE_SYSTEM_PROPERTIES",
			"SONO_SERVICE_ZONE_GROUP_TOPOLOGY",
			};

		#define SONO_SERVICE_STRING(_TYPE_) \
			((_TYPE_) > SONO_SERVICE_MAX ? STRING_UNKNOWN : \
			STRING_CHECK(SONO_SERVICE_STR[_TYPE_]))

		_sono_service::_sono_service(
			__in sono_service_t type,
			__in const sono_service_meta &data
			) :
				m_control(SONO_SOCKET_UDP, 0, 0),
				m_event(SONO_SOCKET_UDP, 0, 0),
				m_event_handler(NULL),
				m_registered(false),
				m_type(SOON_SERVICE_INVALID)
		{
			set(type, data);
		}

		_sono_service::_sono_service(
			__in const _sono_service &other
			) :
				m_control(other.m_control),
				m_data(other.m_data),
				m_event(other.m_event),
				m_event_handler(NULL),
				m_registered(false),
				m_type(other.m_type)
		{
			return;
		}

		_sono_service::~_sono_service(void)
		{

			if(m_registered) {
				unregister_event();
			}
		}

		_sono_service &
		_sono_service::operator=(
			__in const _sono_service &other
			)
		{

			if(this != &other) {
				m_control = other.m_control;
				m_data = other.m_data;
				m_event = other.m_event;
				m_event_handler = NULL;
				m_registered = false;
				m_type = other.m_type;
			}

			return *this;
		}

		void 
		_sono_service::discovery(
			__in_opt uint32_t timeout
			)
		{
			// TODO: discover service properties
		}

		bool 
		_sono_service::is_registered(void)
		{
			return m_registered;
		}

		void 
		_sono_service::register_event(
			__in sono_service_event handler,
			__in_opt uint32_t timeout
			)
		{

			if(m_registered) {
				THROW_SONO_SERVICE_EXCEPTION(SONO_SERVICE_EXCEPTION_REGISTERED);
			}

			if(!handler) {
				THROW_SONO_SERVICE_EXCEPTION(SONO_SERVICE_EXCEPTION_INVALID_HANDLER);
			}

			m_event_handler = handler;

			// TODO: register for service events

			m_registered = true;
		}

		size_t 
		_sono_service::send(
			__in const void *in_data,
			__in size_t in_data_length,
			__inout void *out_data,
			__inout size_t *out_data_length,
			__in_opt uint32_t timeout
			)
		{
			size_t result = 0;

			if((!in_data && in_data_length)
					|| (in_data && !in_data_length)
					|| (!out_data && out_data_length)
					|| (out_data && !out_data_length)) {
				THROW_SONO_SERVICE_EXCEPTION(SONO_SERVICE_EXCEPTION_INVALID_PARAMETER);
			}

			// TODO: send service command

			return result;
		}

		void 
		_sono_service::set(
			__in sono_service_t type,
			__in const sono_service_meta &data
			)
		{

			if(type > SONO_SERVICE_MAX) {
				THROW_SONO_SERVICE_EXCEPTION_FORMAT(SONO_SERVICE_EXCEPTION_INVALID_TYPE,
					"0x%x", type);
			}

			if(m_registered) {
				unregister_event();
			}

			m_control.socket().set(SONO_SOCKET_UDP, m_data.address, m_data.port);
			m_event.socket().set(SONO_SOCKET_UDP, m_data.address, m_data.port);
			m_data = m_data;
			m_event_handler = NULL;
			m_registered = false;
			m_type = type;
		}

		std::string 
		_sono_service::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			result << SONO_SERVICE_STRING(m_type) << " (";

			if(m_registered) {
				result << "REG, PTR. 0x" << SCALAR_AS_HEX(sono_service_event, m_event_handler);
			} else {
				result << "UNREG";
			}

			result << "), CTRL. " << m_control.to_string(verbose) << ", EVT. " << m_event.to_string(verbose);

			return result.str();
		}

		void 
		_sono_service::unregister_event(
			__in_opt uint32_t timeout
			)
		{

			if(!m_registered) {
				THROW_SONO_SERVICE_EXCEPTION(SONO_SERVICE_EXCEPTION_UNREGISTERED);
			}

			// TODO: unregister from service events

			m_event_handler = NULL;
			m_registered = false;
		}
	}
}
