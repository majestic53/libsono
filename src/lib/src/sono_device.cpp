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
#include "../include/sono_device_type.h"

namespace SONO {

	namespace COMP {

		_sono_device::_sono_device(
			__in const std::string &uuid,					
			__in const std::string &household,
			__in const std::string &configuration,
			__in const std::string &address,
			__in uint16_t port
			) :
				sono_socket_base(SONO_SOCKET_UDP, address, port),
				m_configuration(configuration),
				m_household(household),
				m_uuid(uuid)
		{
			return;
		}

		_sono_device::_sono_device(
			__in const _sono_device &other
			) :
				sono_socket_base(other),
				m_configuration(other.m_configuration),
				m_household(other.m_household),
				m_service_map(other.m_service_map),
				m_uuid(other.m_uuid)
		{
			return;
		}

		_sono_device::~_sono_device(void)
		{
			return;
		}

		_sono_device &
		_sono_device::operator=(
			__in const _sono_device &other
			)
		{

			if(this != &other) {
				sono_socket_base::operator=(other);
				m_configuration = other.m_configuration;
				m_household = other.m_household;
				m_service_map = other.m_service_map;
				m_uuid = other.m_uuid;
			}

			return *this;
		}

		bool 
		_sono_device::contains(
			__in sono_service_t type
			)
		{
			return (m_service_map.find(type) != m_service_map.end());
		}

		std::map<sono_service_t, sono_service>::iterator 
		_sono_device::find(
			__in sono_service_t type
			)
		{
			std::map<sono_service_t, sono_service>::iterator result;

			result = m_service_map.find(type);
			if(result == m_service_map.end()) {
				THROW_SONO_DEVICE_EXCEPTION_FORMAT(SONO_DEVICE_EXCEPTION_SERVICE_NOT_FOUND,
					"%s --> service: 0x%x", STRING_CHECK(sono_socket_base::to_string()), 
					type);
			}

			return result;
		}

		std::string 
		_sono_device::household(void)
		{
			return m_household;
		}

		sono_service &
		_sono_device::service(
			__in sono_service_t type
			)
		{
			return find(type)->second;
		}

		sono_service_list 
		_sono_device::service_discovery(
			__in_opt uint32_t timeout
			)
		{
			int code;
			sono_http_encode_t encoding;
			std::string body, body_encoded, response;

			m_service_map.clear();

			try {
				response = sono_http::get(m_configuration.path(), socket().address(), socket().port(), 
					SONO_SOCKET_TCP, timeout);

				code = sono_http::parse_header(response, encoding);
				if(code != SONO_HTTP_SUCCESS) {
					THROW_SONO_DEVICE_EXCEPTION_FORMAT(SONO_DEVICE_EXCEPTION_GET_RESPONSE,
						"%u", code);
				}

				body = sono_http::remove_header(response);

				if(encoding != SONO_HTTP_ENCODE_NONE) {
					body_encoded = body;
					body = sono_http::parse_body(body_encoded, encoding);
				}

				m_configuration.set(body);

				// TODO: build up list of supported services by parsing m_configuration xml document for service tags

			} catch(sono_exception &exc) {
				THROW_SONO_DEVICE_EXCEPTION_FORMAT(SONO_DEVICE_EXCEPTION_SERVICE_DISCOVERY,
					"%s --> %s", STRING_CHECK(sono_socket_base::to_string()), STRING_CHECK(exc.to_string()));
			} catch(std::exception &exc) {
				THROW_SONO_DEVICE_EXCEPTION_FORMAT(SONO_DEVICE_EXCEPTION_SERVICE_DISCOVERY,
					"%s --> %s", STRING_CHECK(sono_socket_base::to_string()), exc.what());
			}

			return service_list();
		}

		sono_service_list 
		_sono_device::service_list(void)
		{
			sono_service_list result;
			std::map<sono_service_t, sono_service>::iterator iter;

			for(iter = m_service_map.begin(); iter != m_service_map.end(); ++iter) {
				result.insert(iter->first);
			}

			return result;
		}

		size_t 
		_sono_device::size(void)
		{
			return m_service_map.size();
		}

		std::string 
		_sono_device::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;
			std::map<sono_service_t, sono_service>::iterator iter;

			result << "{" << STRING_CHECK(m_household) << ":" << STRING_CHECK(m_uuid) << "}, "
				<< sono_socket_base::to_string(verbose) << ", SVC. " << m_service_map.size();

			for(iter = m_service_map.begin(); iter != m_service_map.end(); ++iter) {
				result << std::endl << "---- " << iter->second.to_string(verbose);
			}

			return result.str();
		}

		std::string 
		_sono_device::uuid(void)
		{
			return m_uuid;
		}

		_sono_device_factory *_sono_device_factory::m_instance = NULL;

		_sono_device_factory::_sono_device_factory(void) :
			m_initialized(false)
		{
			std::atexit(sono_device_factory::_delete);
		}

		_sono_device_factory::~_sono_device_factory(void)
		{

			if(m_initialized) {
				uninitialize();
			}
		}

		void 
		_sono_device_factory::_delete(void)
		{

			if(sono_device_factory::m_instance) {
				delete sono_device_factory::m_instance;
				sono_device_factory::m_instance = NULL;
			}
		}

		_sono_device_factory *
		_sono_device_factory::acquire(void)
		{

			if(!sono_device_factory::m_instance) {

				sono_device_factory::m_instance = new sono_device_factory;
				if(!sono_device_factory::m_instance) {
					THROW_SONO_DEVICE_EXCEPTION(SONO_DEVICE_EXCEPTION_ALLOCATED);
				}
			}

			return sono_device_factory::m_instance;
		}

		sono_device &
		_sono_device_factory::at(
			__in sono_uid_t id
			)
		{

			if(!m_initialized) {
				THROW_SONO_DEVICE_EXCEPTION(SONO_DEVICE_EXCEPTION_UNINITIALIZED);
			}

			return find(id)->second.first;
		}

		void 
		_sono_device_factory::clear(void)
		{

			if(!m_initialized) {
				THROW_SONO_DEVICE_EXCEPTION(SONO_DEVICE_EXCEPTION_UNINITIALIZED);
			}

			m_map.clear();
		}

		bool 
		_sono_device_factory::contains(
			__in sono_uid_t id
			)
		{

			if(!m_initialized) {
				THROW_SONO_DEVICE_EXCEPTION(SONO_DEVICE_EXCEPTION_UNINITIALIZED);
			}

			return (m_map.find(id) != m_map.end());
		}

		size_t 
		_sono_device_factory::decrement_reference(
			__in sono_uid_t id
			)
		{
			size_t result;
			std::map<sono_uid_t, std::pair<sono_device, size_t>>::iterator iter;

			if(!m_initialized) {
				THROW_SONO_DEVICE_EXCEPTION(SONO_DEVICE_EXCEPTION_UNINITIALIZED);
			}

			iter = find(id);

			result = --iter->second.second;
			if(result < REF_INIT) {
				m_map.erase(iter);
			}

			return result;
		}

		std::map<sono_uid_t, std::pair<sono_device, size_t>>::iterator 
		_sono_device_factory::find(
			__in sono_uid_t id
			)
		{
			std::map<sono_uid_t, std::pair<sono_device, size_t>>::iterator result;

			if(!m_initialized) {
				THROW_SONO_DEVICE_EXCEPTION(SONO_DEVICE_EXCEPTION_UNINITIALIZED);
			}

			result = m_map.find(id);
			if(result == m_map.end()) {
				THROW_SONO_DEVICE_EXCEPTION_FORMAT(SONO_DEVICE_EXCEPTION_NOT_FOUND, "{%x}", id);
			}

			return result;
		}

		sono_device &
		_sono_device_factory::generate(
			__in const std::string &uuid,					
			__in const std::string &household,
			__in const std::string &configuration,
			__in const std::string &address,
			__in uint16_t port
			)
		{

			if(!m_initialized) {
				THROW_SONO_DEVICE_EXCEPTION(SONO_DEVICE_EXCEPTION_UNINITIALIZED);
			}

			sono_device dev(uuid, household, configuration, address, port);
			m_map.insert(std::pair<sono_uid_t, std::pair<sono_device, size_t>>(dev.id(), 
				std::pair<sono_device, size_t>(dev, REF_INIT)));

			return at(dev.id());
		}

		size_t 
		_sono_device_factory::increment_reference(
			__in sono_uid_t id
			)
		{

			if(!m_initialized) {
				THROW_SONO_DEVICE_EXCEPTION(SONO_DEVICE_EXCEPTION_UNINITIALIZED);
			}

			return ++find(id)->second.second;
		}

		void 
		_sono_device_factory::initialize(void)
		{

			if(m_initialized) {
				THROW_SONO_DEVICE_EXCEPTION(SONO_DEVICE_EXCEPTION_INITIALIZED);
			}

			m_map.clear();
			m_initialized = true;
		}

		bool 
		_sono_device_factory::is_allocated(void)
		{
			return (sono_device_factory::m_instance != NULL);
		}

		bool 
		_sono_device_factory::is_initialized(void)
		{
			return m_initialized;
		}

		sono_device_list 
		_sono_device_factory::list(void)
		{
			sono_device_list result;
			std::map<sono_uid_t, std::pair<sono_device, size_t>>::iterator iter;
			
			for(iter = m_map.begin(); iter != m_map.end(); ++iter) {
				result.insert(std::pair<sono_uid_t, std::pair<std::string, std::pair<std::string, uint16_t>>>(
					iter->first, std::pair<std::string, std::pair<std::string, uint16_t>>(iter->second.first.uuid(), 
					std::pair<std::string, uint16_t>(iter->second.first.socket().address(), 
					iter->second.first.socket().port()))));
			}

			return result;
		}

		size_t 
		_sono_device_factory::reference_count(
			__in sono_uid_t id
			)
		{

			if(!m_initialized) {
				THROW_SONO_DEVICE_EXCEPTION(SONO_DEVICE_EXCEPTION_UNINITIALIZED);
			}

			return find(id)->second.second;
		}

		size_t 
		_sono_device_factory::size(void)
		{

			if(!m_initialized) {
				THROW_SONO_DEVICE_EXCEPTION(SONO_DEVICE_EXCEPTION_UNINITIALIZED);
			}

			return m_map.size();
		}

		std::string 
		_sono_device_factory::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;
			std::map<sono_uid_t, std::pair<sono_device, size_t>>::iterator iter;

			result << SONO_DEVICE_HEADER << " -- " << (m_initialized ? "INIT" : "UNINIT");

			if(m_initialized) {
				result << ", PTR. 0x" << SCALAR_AS_HEX(sono_device_factory *, this) << ", SZ. " << size();

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
		_sono_device_factory::uninitialize(void)
		{

			if(!m_initialized) {
				THROW_SONO_DEVICE_EXCEPTION(SONO_DEVICE_EXCEPTION_UNINITIALIZED);
			}

			m_map.clear();
			m_initialized = false;
		}
	}
}
