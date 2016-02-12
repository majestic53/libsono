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

#ifndef SONO_DISCOVER_H_
#define SONO_DISCOVER_H_

#include <netinet/in.h>

namespace SONO {

	namespace COMP {

		typedef enum {
			SONO_SOCKET_TCP = 0,
			SONO_SOCKET_UDP,
			SONO_SOCKET_SSDP,
		} sono_socket_t;

		#define SONO_SOCKET_MAX SONO_SOCKET_SSDP

		#define SONO_SOCKET_INVALID SCALAR_INVALID(int)
		#define SONO_SOCKET_NO_TIMEOUT SCALAR_INVALID(uint32_t)

		typedef class _sono_socket :
				public sono_uid_base {

			public:

				_sono_socket(
					__in sono_socket_t type,
					__in const std::string &address,
					__in uint16_t port
					);

				_sono_socket(
					__in const _sono_socket &other
					);

				virtual ~_sono_socket(void);

				_sono_socket &operator=(
					__in const _sono_socket &other
					);

				std::string address(void);

				void connect(
					__in_opt uint32_t timeout = SONO_SOCKET_NO_TIMEOUT
					);

				void disconnect(void);

				bool is_connected(void);

				uint16_t port(void);

				int read(
					__out std::string &data
					);

				void set(
					__in sono_socket_t type,
					__in const std::string &address,
					__in uint16_t port
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

				sono_socket_t type(void);

				int write(
					__in const std::string &data
					);

			protected:

				std::string m_address;

				bool m_connected;

				uint16_t m_port;

				int m_socket;

				struct sockaddr_in m_socket_address;

				socklen_t m_socket_address_length;

				sono_socket_t m_type;

		} sono_socket;

		typedef class _sono_socket_factory {

			public:

				~_sono_socket_factory(void);

				static _sono_socket_factory *acquire(void);

				sono_socket &at(
					__in sono_uid_t id
					);

				bool contains(
					__in sono_uid_t id
					);

				size_t decrement_reference(
					__in sono_uid_t id
					);

				sono_socket &generate(
					__in sono_socket_t type,
					__in const std::string &address,
					__in uint16_t port
					);

				size_t increment_reference(
					__in sono_uid_t id
					);

				void initialize(void);

				static bool is_allocated(void);

				bool is_initialized(void);

				size_t reference_count(
					__in sono_uid_t id
					);

				size_t size(void);

				std::string to_string(
					__in_opt bool verbose = false
					);

				void uninitialize(void);

			protected:

				_sono_socket_factory(void);

				_sono_socket_factory(
					__in const _sono_socket_factory &other
					);

				_sono_socket_factory &operator=(
					__in const _sono_socket_factory &other
					);

				static void _delete(void);

				std::map<sono_uid_t, std::pair<sono_socket, size_t>>::iterator find(
					__in sono_uid_t id
					);

				bool m_initialized;

				static _sono_socket_factory *m_instance;

				std::map<sono_uid_t, std::pair<sono_socket, size_t>> m_map;

		} sono_socket_factory;

		typedef class _sono_socket_base {

			public:

				_sono_socket_base(
					__in sono_socket_t type,
					__in const std::string &address,
					__in uint16_t port
					);

				_sono_socket_base(
					__in sono_uid_t id
					);

				_sono_socket_base(
					__in const _sono_socket_base &other
					);

				virtual ~_sono_socket_base(void);

				_sono_socket_base &operator=(
					__in const _sono_socket_base &other
					);

				sono_uid_t id(void);

				sono_socket &socket(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				void decrement_reference(void);

				void generate(
					__in sono_socket_t type,
					__in const std::string &address,
					__in uint16_t port
					);

				void increment_reference(void);

				sono_uid_t m_id;

		} sono_socket_base;
	}
}

#endif // SONO_DISCOVER_H_
