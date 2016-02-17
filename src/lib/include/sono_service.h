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

		typedef std::set<sono_service_t> sono_service_list;

		typedef struct {
			sono_uid_t device;
			std::string address;
			std::string control;
			std::string event;
			std::string id;
			uint16_t port;
			std::string scpd;
			std::string type;
		} sono_service_meta;

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

				sono_action &action(
					__in const std::string &name
					);

				sono_action_list action_list(void);

				bool contains(
					__in const std::string &name
					);

				const sono_service_meta &data(void);

				virtual void discovery(
					__in_opt uint32_t timeout = SONO_SOCKET_NO_TIMEOUT
					);

				static sono_service_t metadata_as_type(
					__in const sono_service_meta &data
					);

				std::map<std::string, std::string> run(
					__in const std::string &name,
					__in const std::map<std::string, std::string> &argument,
					__in_opt uint32_t timeout = SONO_SOCKET_NO_TIMEOUT
					);

				size_t size(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

				sono_service_t type(void);

			protected:

				void add_action(
					__in const std::string &name
					);

				void add_action(
					__in const std::string &name,
					__in const boost::property_tree::ptree &arguments
					);

				std::map<std::string, sono_action>::iterator find(
					__in const std::string &name
					);

				void service_event(
					__in sono_service_t service,
					__in std::string &action,
					__in std::string &data
					);

				void set(
					__in sono_service_t type,
					__in const sono_service_meta &data
					);

				std::map<std::string, sono_action> m_action_map;

				sono_xml m_configuration;
		
				sono_service_meta m_data;
	
				sono_service_t m_type;

		} sono_service;
	}
}

#endif // SONO_SERVICE_H_
