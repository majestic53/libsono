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
#include "../include/sono_service_type.h"

namespace SONO {

	namespace COMP {

		static const std::string SONO_SERVICE_STR[] = {
			"ALARM_CLOCK",
			"DEVICE_PROPERTIES",
			"GROUP_MANAGEMENT",
			"MUSIC_SERVICES",
			"QPLAY",
			"RENDER_AV_TRANSPORT",
			"SONO_SERVICE_RENDER_CONNECTION_MANAGER",
			"SONO_SERVICE_RENDER_CONTROL",
			"SONO_SERVICE_RENDER_GROUP_CONTROL",
			"SONO_SERVICE_RENDER_QUEUE",
			"SONO_SERVICE_SERVER_CONNECTION_MANAGER",
			"SONO_SERVICE_SERVER_CONTENT_DIRECTORY",
			"SONO_SERVICE_SYSTEM_PROPERTIES",
			"SONO_SERVICE_ZONE_GROUP_TOPOLOGY",
			};

		#define SONO_SERVICE_STRING(_TYPE_) \
			((_TYPE_) > SONO_SERVICE_MAX ? STRING_UNKNOWN : \
			STRING_CHECK(SONO_SERVICE_STR[_TYPE_]))

		static const std::string SONO_SERVICE_TYPE_STR[] = {
			"AlarmClock",
			"DeviceProperties",
			"GroupManagement",
			"MusicServices",
			"QPlay",
			"AVTransport",
			"ConnectionManager",
			"RenderingControl",
			"GroupRenderingControl",
			"Queue",
			"ConnectionManager",
			"ContentDirectory",
			"SystemProperties",
			"ZoneGroupTopology",
			};

		#define SONO_SERVICE_TYPE_STRING(_TYPE_) \
			((_TYPE_) > SONO_SERVICE_MAX ? STRING_UNKNOWN : \
			STRING_CHECK(SONO_SERVICE_TYPE_STR[_TYPE_]))

		enum {
			SONO_SERVICE_MEDIA_SERVER = 0,
			SONO_SERVICE_MEDIA_RENDERER,
		};

		#define SONO_SERVICE_MEDIA_MAX SONO_SERVICE_MEDIA_RENDERER

		static const std::string SONO_SERVICE_MEDIA_STR[] = {
			"MediaServer", "MediaRenderer",
			};

		#define SONO_SERVICE_MEDIA_STRING(_TYPE_) \
			((_TYPE_) > SONO_SERVICE_MEDIA_MAX ? STRING_UNKNOWN : \
			STRING_CHECK(SONO_SERVICE_MEDIA_STR[_TYPE_]))

		#define SONO_SERVICE_XML_ACTION_TAG "action"
		#define SONO_SERVICE_XML_ACTION_TAG_LIST "actionList"
		#define SONO_SERVICE_XML_ACTION_TAG_NAME "name"
		#define SONO_SERVICE_XML_ROOT_TAG "scpd"

		_sono_service::_sono_service(
			__in sono_service_t type,
			__in const sono_service_meta &data
			) :
				m_configuration(std::string()),
				m_type(SONO_SERVICE_INVALID)
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

		void 
		_sono_service::add_action(
			__in const std::string &name
			)
		{
			std::map<std::string, sono_action>::iterator iter;

			iter = m_action_map.find(name);
			if(iter == m_action_map.end()) {
				m_action_map.insert(std::pair<std::string, sono_action>(name, sono_action(name, 
					m_data.type)));
			}
		}

		sono_service_meta &
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
			std::string body, body_encoded, response;
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
								add_action(iter_child->second.get<std::string>(
									SONO_SERVICE_XML_ACTION_TAG_NAME));
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

		sono_service_t 
		_sono_service::metadata_as_type(
			__in const sono_service_meta &data
			)
		{
			size_t iter_media = 0, iter_type = 0;
			sono_service_t result = SONO_SERVICE_INVALID;

			for(; iter_type <= SONO_SERVICE_MAX; ++iter_type) {

				if(data.type.find(SONO_SERVICE_TYPE_STRING(iter_type)) != std::string::npos) {
					break;
				}
			}

			if(iter_type > SONO_SERVICE_MAX) {
				THROW_SONO_SERVICE_EXCEPTION(SONO_SERVICE_EXCEPTION_UNKNOWN_TYPE);
			} else if((iter_type == SONO_SERVICE_RENDER_CONNECTION_MANAGER)
					|| (iter_type == SONO_SERVICE_SERVER_CONNECTION_MANAGER)) {

				for(; iter_media <= SONO_SERVICE_MEDIA_MAX; ++iter_media) {

					if(data.control.find(SONO_SERVICE_MEDIA_STRING(iter_media)) != std::string::npos) {
						break;
					}
				}

				switch(iter_media) {
					case SONO_SERVICE_MEDIA_SERVER:
						iter_type = SONO_SERVICE_SERVER_CONNECTION_MANAGER;
						break;
					case SONO_SERVICE_MEDIA_RENDERER:
						iter_type = SONO_SERVICE_RENDER_CONNECTION_MANAGER;
						break;
					default:
						THROW_SONO_SERVICE_EXCEPTION(SONO_SERVICE_EXCEPTION_UNKNOWN_TYPE);
				}
			}

			result = (sono_service_t) iter_type;

			return result;
		}

		std::string 
		_sono_service::run(
			__in const std::string &name,
			__in const std::string &parameters,
			__in_opt uint32_t timeout
			)
		{
			return find(name)->second.run(m_data.control, m_data.address, m_data.port, parameters, timeout);
		}

		void 
		_sono_service::set(
			__in sono_service_t type,
			__in const sono_service_meta &data
			)
		{

			if(type > SONO_SERVICE_MAX) {
				THROW_SONO_SERVICE_EXCEPTION_FORMAT(SONO_SERVICE_EXCEPTION_INVALID_TYPE,
					"0x%x", type);
			}

			m_data = data;
			m_type = type;
		}

		std::string 
		_sono_service::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;
			std::map<std::string, sono_action>::iterator iter;

			result << SONO_SERVICE_HEADER << " " << SONO_SERVICE_STRING(m_type) << ", ACT. " 
				<< m_action_map.size();

			for(iter = m_action_map.begin(); iter != m_action_map.end(); ++iter) {
				result << std::endl << "---- " << iter->second.to_string(verbose);
			}

			return result.str();
		}

		sono_service_t 
		_sono_service::type(void)
		{
			return m_type;
		}
	}
}
