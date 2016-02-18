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

		typedef std::set<std::string> sono_service_list;

		typedef class _sono_service {

			public:

				_sono_service(
					__in const std::string &type,
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

				sono_action_argument run(
					__in const std::string &name,
					__in const sono_action_argument &argument,
					__in_opt uint32_t timeout = SONO_SOCKET_NO_TIMEOUT
					);

				size_t size(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

				const std::string &type(void);

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
					__in const std::string &service,
					__in const std::string &action,
					__in const std::string &data
					);

				void set(
					__in const std::string &type,
					__in const sono_service_meta &data
					);

				std::map<std::string, sono_action> m_action_map;

				sono_xml m_configuration;
		
				sono_service_meta m_data;
	
				std::string m_type;

		} sono_service;
	}
}

#endif // SONO_SERVICE_H_
