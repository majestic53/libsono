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

#ifndef SONO_SERVICE_H_
#define SONO_SERVICE_H_

namespace SONO {

	namespace COMP {

		typedef enum {
			SONO_SERVICE_ALARM_CLOCK = 0,
			SONO_SERVICE_DEVICE_PROPERTIES,
			SONO_SERVICE_GROUP_MANAGEMENT,
			SONO_SERVICE_MUSIC_SERVICES,
			SONO_SERVICE_QPLAY,
			SONO_SERVICE_RENDER_AV_TRANSPORT,
			SONO_SERVICE_RENDER_CONNECTION_MANAGER,
			SONO_SERVICE_RENDER_CONTROL,
			SONO_SERVICE_RENDER_GROUP_CONTROL,
			SONO_SERVICE_RENDER_QUEUE,
			SONO_SERVICE_SERVER_CONNECTION_MANAGER,
			SONO_SERVICE_SERVER_CONTENT_DIRECTORY,
			SONO_SERVICE_SYSTEM_PROPERTIES,
			SONO_SERVICE_ZONE_GROUP_TOPOLOGY,
		} sono_service_t;

		#define SOON_SERVICE_INVALID SCALAR_INVALID(sono_service_t)
		#define SONO_SERVICE_MAX SONO_SERVICE_ZONE_GROUP_TOPOLOGY

		typedef struct {
			std::string address;
			std::string control;
			std::string event;
			std::string id;
			uint16_t port;
			std::string scpd;
			std::string type;
		} sono_service_meta;

		typedef void(*sono_service_event)(
			__in const void *service,
			__in const void *data,
			__in size_t length
			);

		typedef struct _sono_service {

			public:

				_sono_service(
					__in sono_service_t type,
					__in const sono_service_meta &data
					);

				_sono_service(
					__in const _sono_service &other
					);

				virtual ~_sono_service(void);

				_sono_service &operator=(
					__in const _sono_service &other
					);

				virtual void discovery(
					__in_opt uint32_t timeout = SONO_SOCKET_NO_TIMEOUT
					);

				bool is_registered(void);

				void register_event(
					__in sono_service_event handler,
					__in_opt uint32_t timeout = SONO_SOCKET_NO_TIMEOUT
					);

				size_t send(
					__in const void *in_data,
					__in size_t in_data_length,
					__inout void *out_data,
					__inout size_t *out_data_length,
					__in_opt uint32_t timeout = SONO_SOCKET_NO_TIMEOUT
					);

				void set(
					__in sono_service_t type,
					__in const sono_service_meta &data
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

				void unregister_event(
					__in_opt uint32_t timeout = SONO_SOCKET_NO_TIMEOUT
					);

			protected:

				sono_socket_base m_control;
		
				sono_service_meta m_data;
		
				sono_socket_base m_event;
		
				sono_service_event m_event_handler;
	
				bool m_registered;
	
				sono_service_t m_type;

		} sono_service;
	}
}

#endif // SONO_SERVICE_H_
