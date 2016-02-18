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

#ifndef SONO_DEVICE_H_
#define SONO_DEVICE_H_

namespace SONO {

	namespace COMP {

		typedef std::map<sono_uid_t, std::pair<std::string, std::pair<std::string, uint16_t>>> sono_device_list;

		typedef class _sono_device :
				public sono_socket_base {

			public:

				_sono_device(
					__in const std::string &uuid,					
					__in const std::string &household,
					__in const std::string &configuration,
					__in const std::string &address,
					__in uint16_t port
					);

				_sono_device(
					__in const _sono_device &other
					);

				virtual ~_sono_device(void);

				_sono_device &operator=(
					__in const _sono_device &other
					);

				bool contains(
					__in const std::string &type
					);

				const std::string &household(void);

				sono_service &service(
					__in const std::string &type
					);

				sono_service_list service_discovery(
					__in_opt uint32_t timeout = SONO_SOCKET_NO_TIMEOUT
					);

				sono_service_list service_list(void);

				size_t size(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

				const std::string &uuid(void);

			protected:

				void add_service(
					__in const sono_service_meta &data,
					__in_opt bool discover = false,
					__in_opt uint32_t timeout = SONO_SOCKET_NO_TIMEOUT
					);

				std::map<std::string, sono_service>::iterator find(
					__in const std::string &type
					);

				void extract_service_metadata(
					__in const boost::property_tree::ptree::const_iterator child,
					__inout sono_service_meta &data
					);

				sono_xml m_configuration;

				std::string m_household;

				std::map<std::string, sono_service> m_service_map;

				std::string m_uuid;

		} sono_device;

		typedef class _sono_device_factory {

			public:

				~_sono_device_factory(void);

				static _sono_device_factory *acquire(void);

				sono_device &at(
					__in sono_uid_t id
					);

				sono_device &at(
					__in const std::string &address,
					__in uint16_t port
					);

				void clear(void);

				bool contains(
					__in sono_uid_t id
					);

				size_t decrement_reference(
					__in sono_uid_t id
					);

				sono_device &generate(
					__in const std::string &uuid,					
					__in const std::string &household,
					__in const std::string &configuration,
					__in const std::string &address,
					__in uint16_t port
					);

				size_t increment_reference(
					__in sono_uid_t id
					);

				void initialize(void);

				static bool is_allocated(void);

				bool is_initialized(void);

				sono_device_list list(void);

				size_t reference_count(
					__in sono_uid_t id
					);

				size_t size(void);

				std::string to_string(
					__in_opt bool verbose = false
					);

				void uninitialize(void);

			protected:

				_sono_device_factory(void);

				_sono_device_factory(
					__in const _sono_device_factory &other
					);

				_sono_device_factory &operator=(
					__in const _sono_device_factory &other
					);

				static void _delete(void);

				std::map<sono_uid_t, std::pair<sono_device, size_t>>::iterator find(
					__in sono_uid_t id
					);

				bool m_initialized;

				static _sono_device_factory *m_instance;

				std::map<sono_uid_t, std::pair<sono_device, size_t>> m_map;

		} sono_device_factory;
	}
}

#endif // SONO_DEVICE_H_
