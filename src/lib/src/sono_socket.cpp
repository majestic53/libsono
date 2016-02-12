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

#include <cstring>
#include <ctime>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "../include/sono.h"
#include "../include/sono_socket_type.h"

namespace SONO {

	namespace COMP {

		#define SONO_SOCKET_FRAGMENT_LENGTH 1024
		#define SONO_SOCKET_SSDP_IF "127.0.0.1"
		#define SONO_SOCKET_SSDP_TTL 4

		static const int SONO_SOCKET_DOM[] = {
			AF_INET, AF_INET, AF_INET,
			};

		#define SONO_SOCKET_DOMAIN(_TYPE_) \
			((_TYPE_) > SONO_SOCKET_MAX ? SONO_SOCKET_INVALID : \
			SONO_SOCKET_DOM[_TYPE_])

		static const int SONO_SOCKET_PR[] = {
			IPPROTO_TCP, IPPROTO_UDP, IPPROTO_UDP,
			};
	
		#define SONO_SOCKET_PROTOCOL(_TYPE_) \
			((_TYPE_) > SONO_SOCKET_MAX ? SONO_SOCKET_INVALID : \
			SONO_SOCKET_PR[_TYPE_])

		static const int SONO_SOCKET_TY[] = {
			SOCK_STREAM, SOCK_DGRAM, SOCK_DGRAM,
			};

		#define SONO_SOCKET_TYPE(_TYPE_) \
			((_TYPE_) > SONO_SOCKET_MAX ? SONO_SOCKET_INVALID : \
			SONO_SOCKET_TY[_TYPE_])

		static const std::string SONO_SOCKET_STR[] = {
			"TCP", "UDP", "SSDP",
			};

		#define SONO_SOCKET_STRING(_TYPE_) \
			((_TYPE_) > SONO_SOCKET_MAX ? STRING_UNKNOWN : \
			STRING_CHECK(SONO_SOCKET_STR[_TYPE_]))

		_sono_socket::_sono_socket(
			__in sono_socket_t type,
			__in const std::string &address,
			__in uint16_t port
			) :
				m_connected(false),
				m_port(0),
				m_socket(0),
				m_socket_address({ 0 }),
				m_socket_address_length(0),
				m_type(SONO_SOCKET_TCP)
		{
			set(type, address, port);
		}

		_sono_socket::_sono_socket(
			__in const _sono_socket &other
			) :
				sono_uid_base(other),
				m_address(other.m_address),
				m_connected(false),
				m_port(other.m_port),
				m_socket(0),
				m_socket_address({ 0 }),
				m_socket_address_length(0),
				m_type(other.m_type)
		{
			return;
		}

		_sono_socket::~_sono_socket(void)
		{

			if(m_connected) {
				disconnect();
			}
		}

		_sono_socket &
		_sono_socket::operator=(
			__in const _sono_socket &other
			)
		{

			if(this != &other) {
				sono_uid_base::operator=(other);
				m_address = other.m_address;
				m_connected = false;
				m_port = other.m_port;
				m_socket = 0;
				m_socket_address = { 0 };
				m_socket_address_length = 0;
				m_type = other.m_type;
			}

			return *this;
		}

		std::string 
		_sono_socket::address(void)
		{
			return m_address;
		}

		void 
		_sono_socket::connect(
			__in_opt uint32_t timeout
			)
		{			
			struct timeval tm = { 0 };
			struct in_addr inter = { 0 };
			uint8_t ttl = SONO_SOCKET_SSDP_TTL;
			int domain = SONO_SOCKET_DOMAIN(m_type), protocol = SONO_SOCKET_PROTOCOL(m_type), 
				type = SONO_SOCKET_TYPE(m_type);

			if(m_connected) {
				THROW_SONO_SOCKET_EXCEPTION_FORMAT(SONO_SOCKET_EXCEPTION_CONNECTED, 
					"[%s] %s:%u", SONO_SOCKET_STRING(m_type), STRING_CHECK(m_address),
					m_port);
			}

			m_connected = true;

			m_socket = socket(domain, type, protocol);
			if(m_socket == SONO_SOCKET_INVALID) {
				THROW_SONO_SOCKET_EXCEPTION_FORMAT(SONO_SOCKET_EXCEPTION_INTERNAL,
					"[%s] %s:%u --> socket failed: 0x%x", SONO_SOCKET_STRING(m_type), 
					STRING_CHECK(m_address), m_port, errno);
			}

			if(timeout != SONO_SOCKET_NO_TIMEOUT) {
				tm.tv_sec = timeout;

				if(setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, &tm, sizeof(tm)) == SONO_SOCKET_INVALID) {
					THROW_SONO_SOCKET_EXCEPTION_FORMAT(SONO_SOCKET_EXCEPTION_INTERNAL,
						"[%s] %s:%u --> setsockopt(timeout) failed: 0x%x", SONO_SOCKET_STRING(m_type), 
						STRING_CHECK(m_address), m_port, errno);
				}
			}

			switch(m_type) {
				case SONO_SOCKET_TCP:
				case SONO_SOCKET_UDP:
					break;
				case SONO_SOCKET_SSDP:

					if(!inet_aton(SONO_SOCKET_SSDP_IF, &inter)) {

						if(setsockopt(m_socket, IPPROTO_IP, IP_MULTICAST_IF, &inter, sizeof(inter)) 
								== SONO_SOCKET_INVALID) {
							THROW_SONO_SOCKET_EXCEPTION_FORMAT(SONO_SOCKET_EXCEPTION_INTERNAL,
								"[%s] %s:%u --> setsockopt(multicast_if) failed: 0x%x", SONO_SOCKET_STRING(m_type), 
								STRING_CHECK(m_address), m_port, errno);
						}
					}

					if(setsockopt(m_socket, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) == SONO_SOCKET_INVALID) {
						THROW_SONO_SOCKET_EXCEPTION_FORMAT(SONO_SOCKET_EXCEPTION_INTERNAL,
							"[%s] %s:%u --> setsockopt(multicast_ttl) failed: 0x%x", SONO_SOCKET_STRING(m_type), 
							STRING_CHECK(m_address), m_port, errno);
					}
					break;
				default:
					THROW_SONO_SOCKET_EXCEPTION_FORMAT(SONO_SOCKET_EXCEPTION_TYPE_INVALID,
						"0x%x", m_type);
			}

			std::memset(&m_socket_address, 0, sizeof(m_socket_address));
			m_socket_address.sin_family = domain;
			m_socket_address.sin_port = htons(m_port);
			m_socket_address_length = sizeof(struct sockaddr_in);

			if(!inet_aton(STRING_CHECK(m_address), &m_socket_address.sin_addr)) {
				THROW_SONO_SOCKET_EXCEPTION_FORMAT(SONO_SOCKET_EXCEPTION_INTERNAL,
					"[%s] %s:%u --> inet_aton(remote) failed: 0x%x", SONO_SOCKET_STRING(m_type), 
					STRING_CHECK(m_address), m_port, errno);
			}

			switch(m_type) {
				case SONO_SOCKET_TCP:

					if(::connect(m_socket, (struct sockaddr *) &m_socket_address, sizeof(m_socket_address)) 
							== SONO_SOCKET_INVALID) {
						THROW_SONO_SOCKET_EXCEPTION_FORMAT(SONO_SOCKET_EXCEPTION_INTERNAL,
							"[%s] %s:%u --> connect failed: 0x%x", SONO_SOCKET_STRING(m_type), 
							STRING_CHECK(m_address), m_port, errno);
					}
					break;
				case SONO_SOCKET_UDP:
				case SONO_SOCKET_SSDP:
					break;
				default:
					THROW_SONO_SOCKET_EXCEPTION_FORMAT(SONO_SOCKET_EXCEPTION_TYPE_INVALID,
						"0x%x", m_type);
			}
		}

		void 
		_sono_socket::disconnect(void)
		{

			if(!m_connected) {
				THROW_SONO_SOCKET_EXCEPTION_FORMAT(SONO_SOCKET_EXCEPTION_DISCONNECTED, 
					"[%s] %s:%u", SONO_SOCKET_STRING(m_type), STRING_CHECK(m_address),
					m_port);
			}

			if(m_socket && (close(m_socket) == SONO_SOCKET_INVALID)) {
				THROW_SONO_SOCKET_EXCEPTION_FORMAT(SONO_SOCKET_EXCEPTION_INTERNAL,
					"[%s] %s:%u --> close failed: 0x%x", SONO_SOCKET_STRING(m_type), STRING_CHECK(m_address),
					m_port, errno);
			}

			m_socket = 0;
			m_socket_address = { 0 };
			m_socket_address_length = 0;
			m_connected = false;
		}

		bool 
		_sono_socket::is_connected(void)
		{
			return m_connected;
		}

		uint16_t 
		_sono_socket::port(void)
		{
			return m_port;
		}

		int 
		_sono_socket::read(
			__out std::string &data
			)
		{
			int flag = 0, result = 0;

			if(!m_connected) {
				THROW_SONO_SOCKET_EXCEPTION_FORMAT(SONO_SOCKET_EXCEPTION_DISCONNECTED, 
					"[%s] %s:%u", SONO_SOCKET_STRING(m_type), STRING_CHECK(m_address),
					m_port);
			}

			data.clear();
			data.resize(SONO_SOCKET_FRAGMENT_LENGTH);

			switch(m_type) {
				case SONO_SOCKET_TCP:
					result = recv(m_socket, (char *) &data[0], data.size(), flag);
					break;
				case SONO_SOCKET_UDP:
				case SONO_SOCKET_SSDP:
					result = recvfrom(m_socket, (char *) &data[0], data.size(), flag, 
						(struct sockaddr *) &m_socket_address, &m_socket_address_length);
					break;
				default:
					THROW_SONO_SOCKET_EXCEPTION_FORMAT(SONO_SOCKET_EXCEPTION_TYPE_INVALID,
						"0x%x", m_type);
			}

			return result;
		}

		void 
		_sono_socket::set(
			__in sono_socket_t type,
			__in const std::string &address,
			__in uint16_t port
			)
		{

			if(m_connected) {
				THROW_SONO_SOCKET_EXCEPTION_FORMAT(SONO_SOCKET_EXCEPTION_CONNECTED, 
					"[%s] %s:%u", SONO_SOCKET_STRING(m_type), STRING_CHECK(m_address),
					m_port);
			}

			if(type > SONO_SOCKET_MAX) {
				THROW_SONO_SOCKET_EXCEPTION_FORMAT(SONO_SOCKET_EXCEPTION_TYPE_INVALID,
					"0x%x", type);
			}

			if(address.empty()) {
				THROW_SONO_SOCKET_EXCEPTION(SONO_SOCKET_EXCEPTION_ADDRESS_INVALID);
			}

			m_type = type;
			m_address = address;
			m_port = port;
		}

		std::string 
		_sono_socket::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			if(verbose) {
				result << sono_uid_base::to_string() << " ";
			}

			result << "[" << SONO_SOCKET_STRING(m_type) << "] (" << (m_connected ? "CONN" : "DISC") << ") " 
				<< STRING_CHECK(m_address) << ":" << m_port;

			return result.str();
		}

		sono_socket_t 
		_sono_socket::type(void)
		{
			return m_type;
		}

		int 
		_sono_socket::write(
			__in const std::string &data
			)
		{
			int flag = 0, result = 0;

			if(!m_connected) {
				THROW_SONO_SOCKET_EXCEPTION_FORMAT(SONO_SOCKET_EXCEPTION_DISCONNECTED, 
					"[%s] %s:%u", SONO_SOCKET_STRING(m_type), STRING_CHECK(m_address),
					m_port);
			}

			if(!data.empty()) {

				switch(m_type) {
					case SONO_SOCKET_TCP:
						result = send(m_socket, (char *) &data[0], std::strlen(data.c_str()), flag);
						break;
					case SONO_SOCKET_UDP:
					case SONO_SOCKET_SSDP:
						result = sendto(m_socket, (char *) &data[0], std::strlen(data.c_str()), flag, 
							(struct sockaddr *) &m_socket_address, m_socket_address_length);
						break;
					
					default:
						THROW_SONO_SOCKET_EXCEPTION_FORMAT(SONO_SOCKET_EXCEPTION_TYPE_INVALID,
							"0x%x", m_type);
				}
			}

			return result;
		}

		_sono_socket_factory *_sono_socket_factory::m_instance = NULL;

		_sono_socket_factory::_sono_socket_factory(void) :
			m_initialized(false)
		{
			std::atexit(sono_socket_factory::_delete);
		}

		_sono_socket_factory::~_sono_socket_factory(void)
		{

			if(m_initialized) {
				uninitialize();
			}
		}

		void 
		_sono_socket_factory::_delete(void)
		{

			if(sono_socket_factory::m_instance) {
				delete sono_socket_factory::m_instance;
				sono_socket_factory::m_instance = NULL;
			}
		}

		_sono_socket_factory *
		_sono_socket_factory::acquire(void)
		{

			if(!sono_socket_factory::m_instance) {

				sono_socket_factory::m_instance = new sono_socket_factory;
				if(!sono_socket_factory::m_instance) {
					THROW_SONO_SOCKET_EXCEPTION(SONO_SOCKET_EXCEPTION_ALLOCATED);
				}
			}

			return sono_socket_factory::m_instance;
		}

		sono_socket &
		_sono_socket_factory::at(
			__in sono_uid_t id
			)
		{

			if(!m_initialized) {
				THROW_SONO_SOCKET_EXCEPTION(SONO_SOCKET_EXCEPTION_UNINITIALIZED);
			}

			return find(id)->second.first;
		}

		bool 
		_sono_socket_factory::contains(
			__in sono_uid_t id
			)
		{

			if(!m_initialized) {
				THROW_SONO_SOCKET_EXCEPTION(SONO_SOCKET_EXCEPTION_UNINITIALIZED);
			}

			return (m_map.find(id) != m_map.end());
		}

		size_t 
		_sono_socket_factory::decrement_reference(
			__in sono_uid_t id
			)
		{
			size_t result;
			std::map<sono_uid_t, std::pair<sono_socket, size_t>>::iterator iter;

			if(!m_initialized) {
				THROW_SONO_SOCKET_EXCEPTION(SONO_SOCKET_EXCEPTION_UNINITIALIZED);
			}

			iter = find(id);

			result = --iter->second.second;
			if(result < REF_INIT) {
				m_map.erase(iter);
			}

			return result;
		}

		std::map<sono_uid_t, std::pair<sono_socket, size_t>>::iterator 
		_sono_socket_factory::find(
			__in sono_uid_t id
			)
		{
			std::map<sono_uid_t, std::pair<sono_socket, size_t>>::iterator result;

			if(!m_initialized) {
				THROW_SONO_SOCKET_EXCEPTION(SONO_SOCKET_EXCEPTION_UNINITIALIZED);
			}

			result = m_map.find(id);
			if(result == m_map.end()) {
				THROW_SONO_SOCKET_EXCEPTION_FORMAT(SONO_SOCKET_EXCEPTION_NOT_FOUND,
					"{0x%x}", id);
			}

			return result;
		}

		sono_socket &
		_sono_socket_factory::generate(
			__in sono_socket_t type,
			__in const std::string &address,
			__in uint16_t port
			)
		{

			if(!m_initialized) {
				THROW_SONO_SOCKET_EXCEPTION(SONO_SOCKET_EXCEPTION_UNINITIALIZED);
			}

			sono_socket sock(type, address, port);
			m_map.insert(std::pair<sono_uid_t, std::pair<sono_socket, size_t>>(sock.id(), std::pair<sono_socket, size_t>(
				sock, REF_INIT)));

			return at(sock.id());
		}

		size_t 
		_sono_socket_factory::increment_reference(
			__in sono_uid_t id
			)
		{

			if(!m_initialized) {
				THROW_SONO_SOCKET_EXCEPTION(SONO_SOCKET_EXCEPTION_UNINITIALIZED);
			}

			return ++find(id)->second.second;
		}

		void 
		_sono_socket_factory::initialize(void)
		{

			if(m_initialized) {
				THROW_SONO_SOCKET_EXCEPTION(SONO_SOCKET_EXCEPTION_INITIALIZED);
			}

			m_map.clear();
			m_initialized = true;
		}

		bool 
		_sono_socket_factory::is_allocated(void)
		{
			return (sono_socket_factory::m_instance != NULL);
		}

		bool 
		_sono_socket_factory::is_initialized(void)
		{
			return m_initialized;
		}

		size_t 
		_sono_socket_factory::reference_count(
			__in sono_uid_t id
			)
		{

			if(!m_initialized) {
				THROW_SONO_SOCKET_EXCEPTION(SONO_SOCKET_EXCEPTION_UNINITIALIZED);
			}

			return find(id)->second.second;
		}

		size_t 
		_sono_socket_factory::size(void)
		{

			if(!m_initialized) {
				THROW_SONO_SOCKET_EXCEPTION(SONO_SOCKET_EXCEPTION_UNINITIALIZED);
			}

			return m_map.size();
		}

		std::string 
		_sono_socket_factory::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;
			std::map<sono_uid_t, std::pair<sono_socket, size_t>>::iterator iter;

			result << SONO_SOCKET_HEADER << " -- " << (m_initialized ? "INIT" : "UNINIT");

			if(m_initialized) {
				result << ", PTR. 0x" << SCALAR_AS_HEX(sono_socket_factory *, this) << ", SZ. " << size();

				for(iter = m_map.begin(); iter != m_map.end(); ++iter) {
					result << std::endl << "--- " << iter->second.first.to_string(true);

					if(verbose) {
						result << ", REF. " << iter->second.second;
					}
				}
			}

			return result.str();
		}

		void 
		_sono_socket_factory::uninitialize(void)
		{

			if(!m_initialized) {
				THROW_SONO_SOCKET_EXCEPTION(SONO_SOCKET_EXCEPTION_UNINITIALIZED);
			}

			m_map.clear();
			m_initialized = false;
		}

		_sono_socket_base::_sono_socket_base(
			__in sono_socket_t type,
			__in const std::string &address,
			__in uint16_t port
			) :
				m_id(SONO_UID_INVALID)
		{
			generate(type, address, port);
		}

		_sono_socket_base::_sono_socket_base(
			__in sono_uid_t id
			) :
				m_id(id)
		{
			increment_reference();
		}

		_sono_socket_base::_sono_socket_base(
			__in const _sono_socket_base &other
			) :
				m_id(other.m_id)
		{
			increment_reference();
		}

		_sono_socket_base::~_sono_socket_base(void)
		{
			decrement_reference();
		}

		_sono_socket_base &
		_sono_socket_base::operator=(
			__in const _sono_socket_base &other
			)
		{

			if(this != &other) {
				decrement_reference();
				m_id = other.m_id;
				increment_reference();
			}

			return *this;
		}

		void 
		_sono_socket_base::decrement_reference(void)
		{
			sono_socket_factory *instance = NULL;

			if(sono_socket_factory::is_allocated()) {

				instance = sono_socket_factory::acquire();
				if(instance->is_initialized()
						&& instance->contains(m_id)) {
					instance->decrement_reference(m_id);
				}
			}
		}

		void 
		_sono_socket_base::generate(
			__in sono_socket_t type,
			__in const std::string &address,
			__in uint16_t port
			)
		{
			sono_socket_factory *instance = NULL;

			if(sono_socket_factory::is_allocated()) {

				instance = sono_socket_factory::acquire();
				if(instance->is_initialized()) {
					m_id = instance->generate(type, address, port).id();
				}
			}
		}

		sono_uid_t 
		_sono_socket_base::id(void)
		{
			return m_id;
		}

		void 
		_sono_socket_base::increment_reference(void)
		{
			sono_socket_factory *instance = NULL;

			if(sono_socket_factory::is_allocated()) {

				instance = sono_socket_factory::acquire();
				if(instance->is_initialized()
						&& instance->contains(m_id)) {
					instance->increment_reference(m_id);
				}
			}
		}

		sono_socket &
		_sono_socket_base::socket(void)
		{

			if(!sono_socket_factory::is_allocated()) {
				THROW_SONO_SOCKET_EXCEPTION(SONO_SOCKET_EXCEPTION_UNALLOCATED);
			}

			return sono_socket_factory::acquire()->at(m_id);
		}

		std::string 
		_sono_socket_base::to_string(
			__in_opt bool verbose
			)
		{
			return socket().to_string(verbose);
		}
	}
}
