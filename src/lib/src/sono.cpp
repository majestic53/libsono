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

#include <regex>
#include "../include/sono.h"
#include "../include/sono_type.h"

namespace SONO {

	#define SONO_SOCKET_SSDP_ADDRESS "239.255.255.250"
	#define SONO_SOCKET_SSDP_MESSAGE \
		"M-SEARCH * HTTP/1.1\r\n" \
		"HOST: " SONO_SOCKET_SSDP_ADDRESS ":" STRING_CONCAT(SONO_SOCKET_SSDP_MESSAGE) "\r\n" \
		"MAN: \"ssdp:discover\"\r\n" \
		"MX: 1\r\n" \
		"ST: urn:schemas-upnp-org:device:ZonePlayer:1\r\n" \
		"\r\n"
	#define SONO_SOCKET_SSDP_PORT 1900
	#define SONO_SOCKET_SSDP_REGEX_HOUSEHOLD ".*X-RINCON-HOUSEHOLD: (.*)"
	#define SONO_SOCKET_SSDP_REGEX_LOCATION ".*LOCATION: .*//([.0-9]+):([0-9]+)(/.*)"
	#define SONO_SOCKET_SSDP_REGEX_USN ".*USN: uuid:(RINCON_.*)::urn.*"

	enum {
		SONO_SOCKET_SSDP_HOUSEHOLD_ROOT = 0,
		SONO_SOCKET_SSDP_HOUSEHOLD_NAME,
	};

	#define SONO_SOCKET_SSDP_HOUSEHOLD_MAX SONO_SOCKET_SSDP_HOUSEHOLD_NAME
	

	enum {
		SONO_SOCKET_SSDP_LOCATION_ROOT = 0,
		SONO_SOCKET_SSDP_LOCATION_ADDRESS,
		SONO_SOCKET_SSDP_LOCATION_PORT,
		SONO_SOCKET_SSDP_LOCATION_CONFIGURATION,
	};

	#define SONO_SOCKET_SSDP_LOCATION_MAX SONO_SOCKET_SSDP_LOCATION_CONFIGURATION
	

	enum {
		SONO_SOCKET_SSDP_USN_ROOT = 0,
		SONO_SOCKET_SSDP_USN_UUID,
	};

	#define SONO_SOCKET_SSDP_USN_MAX SONO_SOCKET_SSDP_USN_UUID

	sono_manager *sono_manager::m_instance = NULL;

	_sono_manager::_sono_manager(void) :
		m_factory_device(sono_device_factory::acquire()),
		m_factory_socket(sono_socket_factory::acquire()),
		m_factory_uid(sono_uid_factory::acquire()),
		m_initialized(false)
	{
		std::atexit(sono_manager::_delete);
	}

	_sono_manager::~_sono_manager(void)
	{

		if(m_initialized) {
			uninitialize();
		}
	}

	void 
	_sono_manager::_delete(void)
	{

		if(sono_manager::m_instance) {
			delete sono_manager::m_instance;
			sono_manager::m_instance = NULL;
		}
	}

	_sono_manager *
	_sono_manager::acquire(void)
	{

		if(!sono_manager::m_instance) {

			sono_manager::m_instance = new sono_manager;
			if(!sono_manager::m_instance) {
				THROW_SONO_EXCEPTION(SONO_EXCEPTION_ALLOCATED);
			}
		}

		return sono_manager::m_instance;
	}

	sono_device_factory *
	_sono_manager::device(void)
	{

		if(!m_initialized) {
			THROW_SONO_EXCEPTION(SONO_EXCEPTION_UNINITIALIZED);
		}

		return m_factory_device;
	}

	sono_device_list 
	_sono_manager::discover(
		__in_opt uint32_t timeout
		)
	{
		std::match_results<const char *> match;
		std::string address, config, house, output, port, uuid;

		if(!m_initialized) {
			THROW_SONO_EXCEPTION(SONO_EXCEPTION_UNINITIALIZED);
		}

		m_factory_device->clear();
		sono_socket sock(SONO_SOCKET_SSDP, SONO_SOCKET_SSDP_ADDRESS, SONO_SOCKET_SSDP_PORT);
		sock.connect(timeout);

		for(;;) {

			if(sock.write(SONO_SOCKET_SSDP_MESSAGE) == SONO_SOCKET_INVALID) {
				THROW_SONO_EXCEPTION(SONO_EXCEPTION_DEVICE_DISCOVERY);
			}

			if(sock.read(output) == SONO_SOCKET_INVALID) {
				break;
			}

			std::regex_search(output.c_str(), match, std::regex(SONO_SOCKET_SSDP_REGEX_HOUSEHOLD));
			if(match.size() != (SONO_SOCKET_SSDP_HOUSEHOLD_MAX + 1)) {
				THROW_SONO_EXCEPTION_FORMAT(SONO_EXCEPTION_DEVICE_MALFORMED,
					"std::regex_search(household) found %lu entries (should be %lu)", match.size(),
					SONO_SOCKET_SSDP_HOUSEHOLD_MAX + 1);
			}

			house = match[SONO_SOCKET_SSDP_HOUSEHOLD_NAME];

			std::regex_search(output.c_str(), match, std::regex(SONO_SOCKET_SSDP_REGEX_LOCATION));
			if(match.size() != (SONO_SOCKET_SSDP_LOCATION_MAX + 1)) {
				THROW_SONO_EXCEPTION_FORMAT(SONO_EXCEPTION_DEVICE_MALFORMED,
					"std::regex_search(location) found %lu entries (should be %lu)", match.size(),
					SONO_SOCKET_SSDP_LOCATION_MAX + 1);
			}
			
			address = match[SONO_SOCKET_SSDP_LOCATION_ADDRESS];
			port = match[SONO_SOCKET_SSDP_LOCATION_PORT];
			config = match[SONO_SOCKET_SSDP_LOCATION_CONFIGURATION];

			std::regex_search(output.c_str(), match, std::regex(SONO_SOCKET_SSDP_REGEX_USN));
			if(match.size() != (SONO_SOCKET_SSDP_USN_MAX + 1)) {
				THROW_SONO_EXCEPTION_FORMAT(SONO_EXCEPTION_DEVICE_MALFORMED,
					"std::regex_search(uuid) found %lu entries (should be %lu)", match.size(),
					SONO_SOCKET_SSDP_USN_MAX + 1);
			}

			uuid = match[SONO_SOCKET_SSDP_USN_UUID];
			m_factory_device->generate(uuid, house, config, address, std::atoi(port.c_str()));
		}

		sock.disconnect();

		return list();
	}

	void 
	_sono_manager::initialize(void)
	{

		if(m_initialized) {
			THROW_SONO_EXCEPTION(SONO_EXCEPTION_INITIALIZED);
		}

		m_factory_uid->initialize();
		m_factory_socket->initialize();
		m_factory_device->initialize();
		m_initialized = true;
	}

	bool 
	_sono_manager::is_allocated(void)
	{
		return (sono_manager::m_instance != NULL);
	}

	bool 
	_sono_manager::is_initialized(void)
	{
		return m_initialized;
	}

	sono_device_list 
	_sono_manager::list(void)
	{

		if(!m_initialized) {
			THROW_SONO_EXCEPTION(SONO_EXCEPTION_UNINITIALIZED);
		}

		return m_factory_device->list();
	}

	size_t 
	_sono_manager::size(void)
	{

		if(!m_initialized) {
			THROW_SONO_EXCEPTION(SONO_EXCEPTION_UNINITIALIZED);
		}

		return list().size();
	}

	sono_socket_factory *
	_sono_manager::socket(void)
	{

		if(!m_initialized) {
			THROW_SONO_EXCEPTION(SONO_EXCEPTION_UNINITIALIZED);
		}

		return m_factory_socket;
	}

	std::string 
	_sono_manager::to_string(
		__in_opt bool verbose
		)
	{
		std::stringstream result;

		UNREF_PARAM(verbose);

		result << SONO_HEADER << " -- " << (m_initialized ? "INIT" : "UNINIT")
			<< ", PTR. 0x" << SCALAR_AS_HEX(sono_manager *, this);

		return result.str();
	}

	sono_uid_factory *
	_sono_manager::uid(void)
	{

		if(!m_initialized) {
			THROW_SONO_EXCEPTION(SONO_EXCEPTION_UNINITIALIZED);
		}

		return m_factory_uid;
	}

	void 
	_sono_manager::uninitialize(void)
	{

		if(!m_initialized) {
			THROW_SONO_EXCEPTION(SONO_EXCEPTION_UNINITIALIZED);
		}

		m_factory_device->uninitialize();
		m_factory_socket->uninitialize();
		m_factory_uid->uninitialize();
		m_initialized = false;
	}

	std::string 
	_sono_manager::version(void)
	{
		return VERSION_STRING;
	}
}
