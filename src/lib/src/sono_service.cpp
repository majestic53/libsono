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
#include "../include/sono_service_type.h"

namespace SONO {

	namespace COMP {

		#define SONO_SERVICE_XML_ACTION_TAG "action"
		#define SONO_SERVICE_XML_ACTION_TAG_DIRECTION "direction"
		#define SONO_SERVICE_XML_ACTION_TAG_LIST "actionList"
		#define SONO_SERVICE_XML_ACTION_TAG_NAME "name"
		#define SONO_SERVICE_XML_ACTION_TAG_ARGUMENT "argument"
		#define SONO_SERVICE_XML_ACTION_TAG_ARGUMENT_LIST "argumentList"
		#define SONO_SERVICE_XML_ACTION_VALUE_IN "in"
		#define SONO_SERVICE_XML_ACTION_VALUE_OUT "out"
		#define SONO_SERVICE_XML_ROOT_TAG "scpd"

		_sono_service::_sono_service(
			__in const std::string &type,
			__in const sono_service_meta &data
			) :
				m_configuration(std::string())
		{
			set(type, data);
		}

		_sono_service::_sono_service(
			__in const _sono_service &other
			) :
				m_action_map(other.m_action_map),
				m_configuration(other.m_configuration),
				m_data(other.m_data),
				m_type(other.m_type)
		{
			return;
		}

		_sono_service::~_sono_service(void)
		{
			return;
		}

		_sono_service &
		_sono_service::operator=(
			__in const _sono_service &other
			)
		{

			if(this != &other) {
				m_action_map = other.m_action_map;
				m_configuration = other.m_configuration;
				m_data = other.m_data;
				m_type = other.m_type;
			}

			return *this;
		}

		sono_action &
		_sono_service::action(
			__in const std::string &name
			)
		{
			return find(name)->second;
		}

		sono_action_list 
		_sono_service::action_list(void)
		{
			sono_action_list result;
			std::map<std::string, sono_action>::iterator iter;

			for(iter = m_action_map.begin(); iter != m_action_map.end(); ++iter) {
				result.insert(iter->second.name());
			}

			return result;
		}

		void 
		_sono_service::add_action(
			__in const std::string &name
			)
		{
			std::map<std::string, sono_action>::iterator iter;

			iter = m_action_map.find(name);
			if(iter == m_action_map.end()) {
				m_action_map.insert(std::pair<std::string, sono_action>(name, sono_action(m_data.type, name)));
			}
		}

		void 
		_sono_service::add_action(
			__in const std::string &name,
			__in const boost::property_tree::ptree &arguments
			)
		{
			std::string dir_arg, name_arg;
			std::vector<std::string> input, output;
			std::map<std::string, sono_action>::iterator iter;
			boost::property_tree::ptree::const_iterator iter_arg;

			iter = m_action_map.find(name);
			if(iter == m_action_map.end()) {

				for(iter_arg = arguments.begin(); iter_arg != arguments.end(); ++iter_arg) {

					if(iter_arg->first == SONO_SERVICE_XML_ACTION_TAG_ARGUMENT) {
						name_arg = iter_arg->second.get<std::string>(SONO_SERVICE_XML_ACTION_TAG_NAME);

						dir_arg = iter_arg->second.get<std::string>(SONO_SERVICE_XML_ACTION_TAG_DIRECTION);
						if(dir_arg == SONO_SERVICE_XML_ACTION_VALUE_IN) {
							input.push_back(name_arg);
						} else if(dir_arg == SONO_SERVICE_XML_ACTION_VALUE_OUT) {
							output.push_back(name_arg);
						}	
					}
				}

				m_action_map.insert(std::pair<std::string, sono_action>(name, sono_action(m_data.type, name, 
					input, output)));
			}
		}

		bool 
		_sono_service::contains(
			__in const std::string &name
			)
		{
			return (m_action_map.find(name) != m_action_map.end());
		}

		const sono_service_meta &
		_sono_service::data(void)
		{
			return m_data;
		}

		void 
		_sono_service::discovery(
			__in_opt uint32_t timeout
			)
		{
			int code;
			sono_http_encode_t encoding;
			boost::property_tree::ptree child, root;
			std::string body, body_encoded, name, response;
			boost::property_tree::ptree::const_iterator iter_child;
			std::vector<boost::property_tree::ptree::value_type> child_root;
			std::vector<boost::property_tree::ptree::value_type>::const_iterator iter_root;

			m_action_map.clear();

			try {
				response = sono_http::get(m_data.scpd, m_data.address, m_data.port, SONO_SOCKET_TCP, timeout);

				code = sono_http::parse_header(response, encoding);
				if(code != SONO_HTTP_SUCCESS) {
					THROW_SONO_SERVICE_EXCEPTION_FORMAT(SONO_SERVICE_EXCEPTION_GET_RESPONSE,
						"%u", code);
				}

				body = sono_http::remove_header(response);

				if(encoding != SONO_HTTP_ENCODE_NONE) {
					body_encoded = body;
					body = sono_http::parse_body(body_encoded, encoding);
				}

				m_configuration.set(body);

				child_root = m_configuration.as_tree(root, SONO_SERVICE_XML_ROOT_TAG);
				for(iter_root = child_root.begin(); iter_root != child_root.end(); ++iter_root) {

					if(iter_root->first == SONO_SERVICE_XML_ACTION_TAG_LIST) {

						child = iter_root->second;
						for(iter_child = child.begin(); iter_child != child.end(); ++iter_child) {

							if(iter_child->first == SONO_SERVICE_XML_ACTION_TAG) {
								name = iter_child->second.get<std::string>(SONO_SERVICE_XML_ACTION_TAG_NAME);

								try {
									add_action(name, iter_child->second.get_child(
										SONO_SERVICE_XML_ACTION_TAG_ARGUMENT_LIST));
								} catch(...) { 
									add_action(name);
								}
							}
						}
					}
				}
			} catch(sono_exception &exc) {
				THROW_SONO_SERVICE_EXCEPTION_FORMAT(SONO_SERVICE_EXCEPTION_SERVICE_DISCOVERY,
					"%s:%u --> %s", STRING_CHECK(m_data.address), m_data.port, STRING_CHECK(exc.to_string()));
			} catch(std::exception &exc) {
				THROW_SONO_SERVICE_EXCEPTION_FORMAT(SONO_SERVICE_EXCEPTION_SERVICE_DISCOVERY,
					"%s:%u --> %s", STRING_CHECK(m_data.address), m_data.port, exc.what());
			}
		}

		std::map<std::string, sono_action>::iterator 
		_sono_service::find(
			__in const std::string &name
			)
		{
			std::map<std::string, sono_action>::iterator result;

			result = m_action_map.find(name);
			if(result == m_action_map.end()) {
				THROW_SONO_SERVICE_EXCEPTION_FORMAT(SONO_SERVICE_EXCEPTION_NOT_FOUND,
					"%s", STRING_CHECK(name));
			}

			return result;
		}

		sono_action_argument 
		_sono_service::run(
			__in const std::string &name,
			__in const sono_action_argument &argument,
			__in_opt uint32_t timeout
			)
		{
			return find(name)->second.run(m_data.control, m_data.address, m_data.port, argument, timeout);
		}

		void 
		_sono_service::service_event(
			__in const std::string &service,
			__in const std::string &action,
			__in const std::string &data
			)
		{
			sono_manager *instance = NULL;

			if(sono_manager::is_allocated()) {

				instance = sono_manager::acquire();
				if(instance->is_initialized()) {
					instance->service_event(m_data.device, service, action, data);
				}
			}
		}

		void 
		_sono_service::set(
			__in const std::string &type,
			__in const sono_service_meta &data
			)
		{

			if(type.empty()) {
				THROW_SONO_SERVICE_EXCEPTION(SONO_SERVICE_EXCEPTION_INVALID_TYPE);
			}

			m_data = data;
			m_type = type;
		}

		size_t 
		_sono_service::size(void)
		{
			return m_action_map.size();
		}

		std::string 
		_sono_service::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;
			std::map<std::string, sono_action>::iterator iter;

			result << SONO_SERVICE_HEADER << " " << STRING_CHECK(m_type) << ", ACT. " 
				<< m_action_map.size();

			for(iter = m_action_map.begin(); iter != m_action_map.end(); ++iter) {
				result << std::endl << "---- " << iter->second.to_string(verbose);
			}

			return result.str();
		}

		const std::string &
		_sono_service::type(void)
		{
			return m_type;
		}
	}
}
