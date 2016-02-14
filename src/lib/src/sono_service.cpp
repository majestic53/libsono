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

		static const std::string SONO_SERVICE_TYPE_STR[] = {
			"AlarmClock",
			"DeviceProperties",
			"GroupManagement",
			"MusicServices",
			"QPlay",
			"AVTransport",
			"ConnectionManager",
			"RenderingControl",
			"GroupRenderingControl",
			"Queue",
			"ConnectionManager",
			"ContentDirectory",
			"SystemProperties",
			"ZoneGroupTopology",
			};

		#define SONO_SERVICE_TYPE_STRING(_TYPE_) \
			((_TYPE_) > SONO_SERVICE_MAX ? STRING_UNKNOWN : \
			STRING_CHECK(SONO_SERVICE_TYPE_STR[_TYPE_]))

		enum {
			SONO_SERVICE_MEDIA_SERVER = 0,
			SONO_SERVICE_MEDIA_RENDERER,
		};

		#define SONO_SERVICE_MEDIA_MAX SONO_SERVICE_MEDIA_RENDERER

		static const std::string SONO_SERVICE_MEDIA_STR[] = {
			"MediaServer", "MediaRenderer",
			};

		#define SONO_SERVICE_MEDIA_STRING(_TYPE_) \
			((_TYPE_) > SONO_SERVICE_MEDIA_MAX ? STRING_UNKNOWN : \
			STRING_CHECK(SONO_SERVICE_MEDIA_STR[_TYPE_]))

		_sono_service::_sono_service(
			__in sono_service_t type,
			__in const sono_service_meta &data
			) :
				m_configuration(std::string()),
				m_control(SONO_SOCKET_UDP, SONO_SOCKET_LOCAL_ADDRESS, 0),
				m_event(SONO_SOCKET_UDP, SONO_SOCKET_LOCAL_ADDRESS, 0),
				m_event_handler(NULL),
				m_registered(false),
				m_type(SONO_SERVICE_INVALID)
		{
			set(type, data);
		}

		_sono_service::_sono_service(
			__in const _sono_service &other
			) :
				m_configuration(other.m_configuration),
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
				m_configuration = other.m_configuration;
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
			int code;
			sono_http_encode_t encoding;
			std::string body, body_encoded, response;

			if(m_registered) {
				unregister_event();
			}

			try {
				response = sono_http::get(m_data.scpd, m_data.address, m_data.port, SONO_SOCKET_TCP, timeout);

				code = sono_http::parse_header(response, encoding);
				if(code != SONO_HTTP_SUCCESS) {
					THROW_SONO_SERVICE_EXCEPTION_FORMAT(SONO_SERVICE_EXCEPTION_GET_RESPONSE,
						"%u", code);
				}

				body = sono_http::remove_header(response);

				if(encoding != SONO_HTTP_ENCODE_NONE) {
					body_encoded = body;
					body = sono_http::parse_body(body_encoded, encoding);
				}

				m_configuration.set(body);

				// TODO: parse serviceStateTable and actionList

			} catch(sono_exception &exc) {
				THROW_SONO_SERVICE_EXCEPTION_FORMAT(SONO_SERVICE_EXCEPTION_SERVICE_DISCOVERY,
					"%s:%u --> %s", STRING_CHECK(m_data.address), m_data.port, STRING_CHECK(exc.to_string()));
			} catch(std::exception &exc) {
				THROW_SONO_SERVICE_EXCEPTION_FORMAT(SONO_SERVICE_EXCEPTION_SERVICE_DISCOVERY,
					"%s:%u --> %s", STRING_CHECK(m_data.address), m_data.port, exc.what());
			}
		}

		bool 
		_sono_service::is_registered(void)
		{
			return m_registered;
		}

		sono_service_t 
		_sono_service::metadata_as_type(
			__in const sono_service_meta &data
			)
		{
			size_t iter_media = 0, iter_type = 0;
			sono_service_t result = SONO_SERVICE_INVALID;

			for(; iter_type <= SONO_SERVICE_MAX; ++iter_type) {

				if(data.type.find(SONO_SERVICE_TYPE_STRING(iter_type)) != std::string::npos) {
					break;
				}
			}

			if(iter_type > SONO_SERVICE_MAX) {
				THROW_SONO_SERVICE_EXCEPTION(SONO_SERVICE_EXCEPTION_UNKNOWN_TYPE);
			} else if((iter_type == SONO_SERVICE_RENDER_CONNECTION_MANAGER)
					|| (iter_type == SONO_SERVICE_SERVER_CONNECTION_MANAGER)) {

				for(; iter_media <= SONO_SERVICE_MEDIA_MAX; ++iter_media) {

					if(data.control.find(SONO_SERVICE_MEDIA_STRING(iter_media)) != std::string::npos) {
						break;
					}
				}

				switch(iter_media) {
					case SONO_SERVICE_MEDIA_SERVER:
						iter_type = SONO_SERVICE_SERVER_CONNECTION_MANAGER;
						break;
					case SONO_SERVICE_MEDIA_RENDERER:
						iter_type = SONO_SERVICE_RENDER_CONNECTION_MANAGER;
						break;
					default:
						THROW_SONO_SERVICE_EXCEPTION(SONO_SERVICE_EXCEPTION_UNKNOWN_TYPE);
				}
			}

			result = (sono_service_t) iter_type;

			return result;
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

			m_data = data;
			m_control.socket().set(SONO_SOCKET_UDP, m_data.address, m_data.port);
			m_event.socket().set(SONO_SOCKET_UDP, m_data.address, m_data.port);
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

		sono_service_t 
		_sono_service::type(void)
		{
			return m_type;
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
