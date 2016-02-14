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

		#define SONO_SERVICE_INVALID SCALAR_INVALID(sono_service_t)
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

		typedef class _sono_service {

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

				static sono_service_t metadata_as_type(
					__in const sono_service_meta &data
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

				sono_service_t type(void);

			protected:

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

				void unregister_event(
					__in_opt uint32_t timeout = SONO_SOCKET_NO_TIMEOUT
					);

				sono_xml m_configuration;

				sono_socket_base m_control;
		
				sono_service_meta m_data;
		
				sono_socket_base m_event;
		
				sono_service_event m_event_handler;
	
				bool m_registered;
	
				sono_service_t m_type;

		} sono_service;

		typedef class _sono_service_alarm_clock :
				public sono_service {

			public:

				_sono_service_alarm_clock(
					__in const sono_service_meta &data
					);

				_sono_service_alarm_clock(
					__in const _sono_service_alarm_clock &other
					);

				virtual ~_sono_service_alarm_clock(void);

				_sono_service_alarm_clock &operator=(
					__in const _sono_service_alarm_clock &other
					);

				// TODO

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				// TODO

		} sono_service_alarm_clock;

		typedef class _sono_service_device_properties :
				public sono_service {

			public:

				_sono_service_device_properties(
					__in const sono_service_meta &data
					);

				_sono_service_device_properties(
					__in const _sono_service_device_properties &other
					);

				virtual ~_sono_service_device_properties(void);

				_sono_service_device_properties &operator=(
					__in const _sono_service_device_properties &other
					);

				// TODO

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				// TODO

		} sono_service_device_properties;

		typedef class _sono_service_group_management :
				public sono_service {

			public:

				_sono_service_group_management(
					__in const sono_service_meta &data
					);

				_sono_service_group_management(
					__in const _sono_service_group_management &other
					);

				virtual ~_sono_service_group_management(void);

				_sono_service_group_management &operator=(
					__in const _sono_service_group_management &other
					);

				// TODO

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				// TODO

		} sono_service_group_management;

		typedef class _sono_service_music_services :
				public sono_service {

			public:

				_sono_service_music_services(
					__in const sono_service_meta &data
					);

				_sono_service_music_services(
					__in const _sono_service_music_services &other
					);

				virtual ~_sono_service_music_services(void);

				_sono_service_music_services &operator=(
					__in const _sono_service_music_services &other
					);

				// TODO

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				// TODO

		} sono_service_music_services;

		typedef class _sono_service_qplay :
				public sono_service {

			public:

				_sono_service_qplay(
					__in const sono_service_meta &data
					);

				_sono_service_qplay(
					__in const _sono_service_qplay &other
					);

				virtual ~_sono_service_qplay(void);

				_sono_service_qplay &operator=(
					__in const _sono_service_qplay &other
					);

				// TODO

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				// TODO

		} sono_service_qplay;

		typedef class _sono_service_render_av_transport :
				public sono_service {

			public:

				_sono_service_render_av_transport(
					__in const sono_service_meta &data
					);

				_sono_service_render_av_transport(
					__in const _sono_service_render_av_transport &other
					);

				virtual ~_sono_service_render_av_transport(void);

				_sono_service_render_av_transport &operator=(
					__in const _sono_service_render_av_transport &other
					);

				// TODO

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				// TODO

		} sono_service_render_av_transport;

		typedef class _sono_service_render_connection_manager :
				public sono_service {

			public:

				_sono_service_render_connection_manager(
					__in const sono_service_meta &data
					);

				_sono_service_render_connection_manager(
					__in const _sono_service_render_connection_manager &other
					);

				virtual ~_sono_service_render_connection_manager(void);

				_sono_service_render_connection_manager &operator=(
					__in const _sono_service_render_connection_manager &other
					);

				// TODO

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				// TODO

		} sono_service_render_connection_manager;

		typedef class _sono_service_render_control :
				public sono_service {

			public:

				_sono_service_render_control(
					__in const sono_service_meta &data
					);

				_sono_service_render_control(
					__in const _sono_service_render_control &other
					);

				virtual ~_sono_service_render_control(void);

				_sono_service_render_control &operator=(
					__in const _sono_service_render_control &other
					);

				// TODO

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				// TODO

		} sono_service_render_control;

		typedef class _sono_service_render_group_control :
				public sono_service {

			public:

				_sono_service_render_group_control(
					__in const sono_service_meta &data
					);

				_sono_service_render_group_control(
					__in const _sono_service_render_group_control &other
					);

				virtual ~_sono_service_render_group_control(void);

				_sono_service_render_group_control &operator=(
					__in const _sono_service_render_group_control &other
					);

				// TODO

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				// TODO

		} sono_service_render_group_control;

		typedef class _sono_service_render_queue :
				public sono_service {

			public:

				_sono_service_render_queue(
					__in const sono_service_meta &data
					);

				_sono_service_render_queue(
					__in const _sono_service_render_queue &other
					);

				virtual ~_sono_service_render_queue(void);

				_sono_service_render_queue &operator=(
					__in const _sono_service_render_queue &other
					);

				// TODO

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				// TODO

		} sono_service_render_queue;

		typedef class _sono_service_server_connection_manager :
				public sono_service {

			public:

				_sono_service_server_connection_manager(
					__in const sono_service_meta &data
					);

				_sono_service_server_connection_manager(
					__in const _sono_service_server_connection_manager &other
					);

				virtual ~_sono_service_server_connection_manager(void);

				_sono_service_server_connection_manager &operator=(
					__in const _sono_service_server_connection_manager &other
					);

				// TODO

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				// TODO

		} sono_service_server_connection_manager;

		typedef class _sono_service_server_content_directory :
				public sono_service {

			public:

				_sono_service_server_content_directory(
					__in const sono_service_meta &data
					);

				_sono_service_server_content_directory(
					__in const _sono_service_server_content_directory &other
					);

				virtual ~_sono_service_server_content_directory(void);

				_sono_service_server_content_directory &operator=(
					__in const _sono_service_server_content_directory &other
					);

				// TODO

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				// TODO

		} sono_service_server_content_directory;

		typedef class _sono_service_system_properties :
				public sono_service {

			public:

				_sono_service_system_properties(
					__in const sono_service_meta &data
					);

				_sono_service_system_properties(
					__in const _sono_service_system_properties &other
					);

				virtual ~_sono_service_system_properties(void);

				_sono_service_system_properties &operator=(
					__in const _sono_service_system_properties &other
					);

				// TODO

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				// TODO

		} sono_service_system_properties;

		typedef class _sono_service_zone_group_topology :
				public sono_service {

			public:

				_sono_service_zone_group_topology(
					__in const sono_service_meta &data
					);

				_sono_service_zone_group_topology(
					__in const _sono_service_zone_group_topology &other
					);

				virtual ~_sono_service_zone_group_topology(void);

				_sono_service_zone_group_topology &operator=(
					__in const _sono_service_zone_group_topology &other
					);

				// TODO

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				// TODO

		} sono_service_zone_group_topology;
	}
}

#endif // SONO_SERVICE_H_
