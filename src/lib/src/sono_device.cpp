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
#include "../include/sono_manager.h"
#include "../include/sono_device_type.h"

namespace SONO {

	namespace COMP {

		#define SONO_DEVICE_XML_DEVICE_TAG "device"
		#define SONO_DEVICE_XML_DEVICE_TAG_LIST "deviceList"
		#define SONO_DEVICE_XML_ROOT_TAG "root"
		#define SONO_DEVICE_XML_SERVICE_TAG "service"
		#define SONO_DEVICE_XML_SERVICE_TAG_CONTROL_URL "controlURL"
		#define SONO_DEVICE_XML_SERVICE_TAG_EVENT_URL "eventSubURL"
		#define SONO_DEVICE_XML_SERVICE_TAG_ID "serviceId"
		#define SONO_DEVICE_XML_SERVICE_TAG_LIST "serviceList"
		#define SONO_DEVICE_XML_SERVICE_TAG_SCPD_URL "SCPDURL"
		#define SONO_DEVICE_XML_SERVICE_TAG_TYPE "serviceType"

		enum {
			SONO_DEVICE_SERVICE_ROOT = 0,
			SONO_DEVICE_SERVICE_NAME,
		};

		#define SONO_DEVICE_SERVICE_MAX SONO_DEVICE_SERVICE_NAME
		#define SONO_DEVICE_SERVICE_REGEX ".*:service:(.*):.*"

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

		void 
		_sono_device::add_service(
			__in const sono_service_meta &data,
			__in_opt bool discover,
			__in_opt uint32_t timeout
			)
		{
			std::string name;
			std::match_results<const char *> match;
			std::map<std::string, sono_service>::iterator iter;

			std::regex_search(data.type.c_str(), match, std::regex(SONO_DEVICE_SERVICE_REGEX));
			if(match.size() != (SONO_DEVICE_SERVICE_MAX + 1)) {
				THROW_SONO_DEVICE_EXCEPTION_FORMAT(SONO_DEVICE_EXCEPTION_SERVICE_MALFORMED,
					"std::regex_search(service) found %lu entries (should be %lu)", match.size(),
					SONO_DEVICE_SERVICE_MAX + 1);
			}

			name = match[SONO_DEVICE_SERVICE_NAME];

			iter = m_service_map.find(name);
			if(iter == m_service_map.end()) {
				m_service_map.insert(std::pair<std::string, sono_service>(name, sono_service(name, data)));

				if(discover) {

					iter = m_service_map.find(name);
					if(iter != m_service_map.end()) {
						iter->second.discovery(timeout);
					}
				}
			}
		}

		bool 
		_sono_device::contains(
			__in const std::string &type
			)
		{
			return (m_service_map.find(type) != m_service_map.end());
		}

		std::map<std::string, sono_service>::iterator 
		_sono_device::find(
			__in const std::string &type
			)
		{
			std::map<std::string, sono_service>::iterator result;

			result = m_service_map.find(type);
			if(result == m_service_map.end()) {
				THROW_SONO_DEVICE_EXCEPTION_FORMAT(SONO_DEVICE_EXCEPTION_SERVICE_NOT_FOUND, 
					"%s --> service: %s", STRING_CHECK(sono_socket_base::to_string()), STRING_CHECK(type));
			}

			return result;
		}

		void 
		_sono_device::extract_service_metadata(
			__in const boost::property_tree::ptree::const_iterator child,
			__inout sono_service_meta &data
			)
		{
			data.control = child->second.get<std::string>(SONO_DEVICE_XML_SERVICE_TAG_CONTROL_URL);
			data.event = child->second.get<std::string>(SONO_DEVICE_XML_SERVICE_TAG_EVENT_URL);
			data.id = child->second.get<std::string>(SONO_DEVICE_XML_SERVICE_TAG_ID);
			data.scpd = child->second.get<std::string>(SONO_DEVICE_XML_SERVICE_TAG_SCPD_URL);
			data.type = child->second.get<std::string>(SONO_DEVICE_XML_SERVICE_TAG_TYPE);
		}

		const std::string &
		_sono_device::household(void)
		{
			return m_household;
		}

		sono_service &
		_sono_device::service(
			__in const std::string &type
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
			sono_service_meta data;
			sono_http_encode_t encoding;			
			std::string body, body_encoded, response;
			boost::property_tree::ptree child, child_inner, root;
			std::map<std::string, sono_service>::iterator iter_svc;
			std::vector<boost::property_tree::ptree::value_type> child_root;
			boost::property_tree::ptree::const_iterator iter_child, iter_child_inner;
			std::vector<boost::property_tree::ptree::value_type>::const_iterator iter_root;

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

				child_root = m_configuration.as_tree(root, SONO_DEVICE_XML_ROOT_TAG);
				for(iter_root = child_root.begin(); iter_root != child_root.end(); ++iter_root) {

					if(iter_root->first == SONO_DEVICE_XML_DEVICE_TAG) {
						data.device = id();
						data.address = socket().address();
						data.port = socket().port();

						child = iter_root->second.get_child(SONO_DEVICE_XML_SERVICE_TAG_LIST);
						for(iter_child = child.begin(); iter_child != child.end(); ++iter_child) {

							if(iter_child->first == SONO_DEVICE_XML_SERVICE_TAG) {
								extract_service_metadata(iter_child, data);
								add_service(data);
							}
						}

						child = iter_root->second.get_child(SONO_DEVICE_XML_DEVICE_TAG_LIST);
						for(iter_child = child.begin(); iter_child != child.end(); ++iter_child) {

							if(iter_child->first == SONO_DEVICE_XML_DEVICE_TAG) {

								child_inner = iter_child->second.get_child(SONO_DEVICE_XML_SERVICE_TAG_LIST);
								for(iter_child_inner = child_inner.begin(); iter_child_inner != child_inner.end(); 
										++iter_child_inner) {

									if(iter_child_inner->first == SONO_DEVICE_XML_SERVICE_TAG) {
										extract_service_metadata(iter_child_inner, data);
										add_service(data);
									}
								}
							}
						}
					}
				}

				for(iter_svc = m_service_map.begin(); iter_svc != m_service_map.end(); ++iter_svc) {
					iter_svc->second.discovery(timeout);
				}
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
			std::map<std::string, sono_service>::iterator iter;

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
			std::map<std::string, sono_service>::iterator iter;

			result << "{" << STRING_CHECK(m_household) << ":" << STRING_CHECK(m_uuid) << "}, "
				<< sono_socket_base::to_string(verbose) << ", SVC. " << m_service_map.size();

			for(iter = m_service_map.begin(); iter != m_service_map.end(); ++iter) {
				result << std::endl << "---- " << iter->second.to_string(verbose);
			}

			return result.str();
		}

		const std::string &
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

		sono_device &
		_sono_device_factory::at(
			__in const std::string &address,
			__in uint16_t port
			)
		{
			std::map<sono_uid_t, std::pair<sono_device, size_t>>::iterator iter;

			if(!m_initialized) {
				THROW_SONO_DEVICE_EXCEPTION(SONO_DEVICE_EXCEPTION_UNINITIALIZED);
			}

			for(iter = m_map.begin(); iter != m_map.end(); ++iter) {

				if((iter->second.first.socket().address() == address)
						&& (iter->second.first.socket().port() == port)) {
					break;
				}
			}

			if(iter == m_map.end()) {
				THROW_SONO_DEVICE_EXCEPTION_FORMAT(SONO_DEVICE_EXCEPTION_NOT_FOUND, "%s:%u", 
					STRING_CHECK(address), port);
			}

			return iter->second.first;
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
