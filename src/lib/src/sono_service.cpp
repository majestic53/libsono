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

		#define SONO_SERVICE_CONNECTION_MANAGER_CONNECTION_ID "ConnectionID"
		#define SONO_SERVICE_CONNECTION_MANAGER_CURRENT_CONNECTION_IDS "GetCurrentConnectionIDs"
		#define SONO_SERVICE_CONNECTION_MANAGER_CURRENT_CONNECTION_INFO "GetCurrentConnectionInfo"
		#define SONO_SERVICE_CONNECTION_MAANGER_PROTOCOL_INFO "GetProtocolInfo"
		#define SONO_SERVICE_DEVICE_PROPERTIES_LED_STATE_DESIRE "DesiredLEDState"
		#define SONO_SERVICE_DEVICE_PROPERTIES_LED_STATE_GET "GetLEDState"
		#define SONO_SERVICE_DEVICE_PROPERTIES_LED_OFF "Off"
		#define SONO_SERVICE_DEVICE_PROPERTIES_LED_ON "On"
		#define SONO_SERVICE_DEVICE_PROPERTIES_LED_STATE_SET "SetLEDState"
		#define SONO_SERVICE_GROUP_MANAGEMENT_ADD_MEMBER "AddMember"
		#define SONO_SERVICE_GROUP_MANAGEMENT_BOOT_SEQ "BootSeq"
		#define SONO_SERVICE_GROUP_MANAGEMENT_MEMBER_ID "MemberId"
		#define SONO_SERVICE_GROUP_MANAGEMENT_REMOVE_MEMBER "RemoveMember"
		#define SONO_SERVICE_GROUP_MANAGEMENT_REPORT_TRACK_BUFFERING_RESULT "ReportTrackBufferingResult"
		#define SONO_SERVICE_GROUP_MANAGEMENT_RESULT_CODE "ResultCode"
		#define SONO_SERVICE_MUSIC_SERVICES_GET_SESSION_ID "GetSessionId"
		#define SONO_SERVICE_MUSIC_SERVICES_LIST_AVAILABLE_SERVICES "ListAvailableServices"
		#define SONO_SERVICE_MUSIC_SERVICES_SERVICE_ID "ServiceId"
		#define SONO_SERVICE_MUSIC_SERVICES_UPDATE_AVAILABLE_SERVICES "UpdateAvailableServices"
		#define SONO_SERVICE_MUSIC_SERVICES_USERNAME "Username"
		#define SONO_SERVICE_QPLAY_AUTH "QPlayAuth"
		#define SONO_SERVICE_QPLAY_AUTH_SEED "Seed"
		#define SONO_SERVICE_XML_ACTION_TAG "action"
		#define SONO_SERVICE_XML_ACTION_TAG_LIST "actionList"
		#define SONO_SERVICE_XML_ACTION_TAG_NAME "name"
		#define SONO_SERVICE_XML_CLOSE_BRACKET ">"
		#define SONO_SERVICE_XML_OPEN_BRACKET "<"
		#define SONO_SERVICE_XML_OPEN_BRACKET_TERM "</"
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
		_sono_service::service_event(
			__in sono_service_t service,
			__in std::string &action,
			__in std::string &data
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

		_sono_service_alarm_clock::_sono_service_alarm_clock(
			__in const sono_service_meta &data
			) :
				sono_service(SONO_SERVICE_ALARM_CLOCK, data)
		{
			return;
		}

		_sono_service_alarm_clock::_sono_service_alarm_clock(
			__in const _sono_service_alarm_clock &other
			) :
				sono_service(other)
		{
			return;
		}

		_sono_service_alarm_clock::~_sono_service_alarm_clock(void)
		{
			return;
		}

		_sono_service_alarm_clock &
		_sono_service_alarm_clock::operator=(
			__in const _sono_service_alarm_clock &other
			)
		{

			if(this != &other) {
				sono_service::operator=(other);
			}

			return *this;
		}

		void 
		_sono_service_alarm_clock::service_event(
			__in std::string &action,
			__in std::string &data
			)
		{
			sono_service::service_event(SONO_SERVICE_ALARM_CLOCK, action, data);
		}

		_sono_service_device_properties::_sono_service_device_properties(
			__in const sono_service_meta &data
			) :
				sono_service(SONO_SERVICE_DEVICE_PROPERTIES, data)
		{
			return;
		}

		_sono_service_device_properties::_sono_service_device_properties(
			__in const _sono_service_device_properties &other
			) :
				sono_service(other)
		{
			return;
		}

		_sono_service_device_properties::~_sono_service_device_properties(void)
		{
			return;
		}

		_sono_service_device_properties &
		_sono_service_device_properties::operator=(
			__in const _sono_service_device_properties &other
			)
		{

			if(this != &other) {
				sono_service::operator=(other);
			}

			return *this;
		}

		std::string 
		_sono_service_device_properties::get_led_state(
			__in_opt uint32_t timeout
			)
		{
			int code;
			std::string response;
			std::match_results<const char *> match;

			response = run(SONO_SERVICE_DEVICE_PROPERTIES_LED_STATE_GET, std::string(), timeout);

			code = sono_http::parse_header(response);
			if(code != SONO_HTTP_SUCCESS) {
				THROW_SONO_SERVICE_EXCEPTION_FORMAT(SONO_SERVICE_EXCEPTION_POST_RESPONSE,
					"%s --> %u", SONO_SERVICE_DEVICE_PROPERTIES_LED_STATE_GET, code);
			}

			return sono_http::remove_header(response);
		}

		std::string 
		_sono_service_device_properties::set_led_state(
			__in bool state,
			__in_opt uint32_t timeout
			)
		{
			int code;
			std::string response;
			std::stringstream stream;

			stream << SONO_SERVICE_XML_OPEN_BRACKET << SONO_SERVICE_DEVICE_PROPERTIES_LED_STATE_DESIRE 
				<< SONO_SERVICE_XML_CLOSE_BRACKET
				<< (state ? SONO_SERVICE_DEVICE_PROPERTIES_LED_ON : SONO_SERVICE_DEVICE_PROPERTIES_LED_OFF)
				<< SONO_SERVICE_XML_OPEN_BRACKET_TERM << SONO_SERVICE_DEVICE_PROPERTIES_LED_STATE_DESIRE 
				<< SONO_SERVICE_XML_CLOSE_BRACKET;
			response = run(SONO_SERVICE_DEVICE_PROPERTIES_LED_STATE_SET, stream.str(), timeout);

			code = sono_http::parse_header(response);
			if(code != SONO_HTTP_SUCCESS) {
				THROW_SONO_SERVICE_EXCEPTION_FORMAT(SONO_SERVICE_EXCEPTION_POST_RESPONSE,
					"%s --> %u", SONO_SERVICE_DEVICE_PROPERTIES_LED_STATE_SET, code);
			}

			return sono_http::remove_header(response);
		}

		void 
		_sono_service_device_properties::service_event(
			__in std::string &action,
			__in std::string &data
			)
		{
			sono_service::service_event(SONO_SERVICE_DEVICE_PROPERTIES, action, data);
		}

		_sono_service_group_management::_sono_service_group_management(
			__in const sono_service_meta &data
			) :
				sono_service(SONO_SERVICE_GROUP_MANAGEMENT, data)
		{
			return;
		}

		_sono_service_group_management::_sono_service_group_management(
			__in const _sono_service_group_management &other
			) :
				sono_service(other)
		{
			return;
		}

		_sono_service_group_management::~_sono_service_group_management(void)
		{
			return;
		}

		_sono_service_group_management &
		_sono_service_group_management::operator=(
			__in const _sono_service_group_management &other
			)
		{

			if(this != &other) {
				sono_service::operator=(other);
			}

			return *this;
		}

		std::string 
		_sono_service_group_management::add_member(
			__in const std::string &id,
			__in uint32_t seq,
			__in_opt uint32_t timeout
			)
		{
			int code;
			std::string response;
			std::stringstream stream;

			stream << SONO_SERVICE_XML_OPEN_BRACKET << SONO_SERVICE_GROUP_MANAGEMENT_MEMBER_ID 
				<< SONO_SERVICE_XML_CLOSE_BRACKET << id << SONO_SERVICE_XML_OPEN_BRACKET_TERM 
				<< SONO_SERVICE_GROUP_MANAGEMENT_MEMBER_ID << SONO_SERVICE_XML_CLOSE_BRACKET
				<< SONO_SERVICE_XML_OPEN_BRACKET << SONO_SERVICE_GROUP_MANAGEMENT_BOOT_SEQ 
				<< SONO_SERVICE_XML_CLOSE_BRACKET << seq << SONO_SERVICE_XML_OPEN_BRACKET_TERM 
				<< SONO_SERVICE_GROUP_MANAGEMENT_BOOT_SEQ << SONO_SERVICE_XML_CLOSE_BRACKET;
			response = run(SONO_SERVICE_GROUP_MANAGEMENT_ADD_MEMBER, stream.str(), timeout);

			code = sono_http::parse_header(response);
			if(code != SONO_HTTP_SUCCESS) {
				THROW_SONO_SERVICE_EXCEPTION_FORMAT(SONO_SERVICE_EXCEPTION_POST_RESPONSE,
					"%s --> %u", SONO_SERVICE_GROUP_MANAGEMENT_ADD_MEMBER, code);
			}

			return sono_http::remove_header(response);
		}

		std::string 
		_sono_service_group_management::remove_member(
			__in const std::string &id,
			__in_opt uint32_t timeout
			)
		{
			int code;
			std::string response;
			std::stringstream stream;

			stream << SONO_SERVICE_XML_OPEN_BRACKET << SONO_SERVICE_GROUP_MANAGEMENT_MEMBER_ID 
				<< SONO_SERVICE_XML_CLOSE_BRACKET << id << SONO_SERVICE_XML_OPEN_BRACKET_TERM 
				<< SONO_SERVICE_GROUP_MANAGEMENT_MEMBER_ID << SONO_SERVICE_XML_CLOSE_BRACKET;
			response = run(SONO_SERVICE_GROUP_MANAGEMENT_REMOVE_MEMBER, stream.str(), timeout);

			code = sono_http::parse_header(response);
			if(code != SONO_HTTP_SUCCESS) {
				THROW_SONO_SERVICE_EXCEPTION_FORMAT(SONO_SERVICE_EXCEPTION_POST_RESPONSE,
					"%s --> %u", SONO_SERVICE_GROUP_MANAGEMENT_REMOVE_MEMBER, code);
			}

			return sono_http::remove_header(response);
		}

		std::string 
		_sono_service_group_management::report_track_buffering_result(
			__in const std::string &id,
			__in uint32_t result,
			__in_opt uint32_t timeout
			)
		{
			int code;
			std::string response;
			std::stringstream stream;

			stream << SONO_SERVICE_XML_OPEN_BRACKET << SONO_SERVICE_GROUP_MANAGEMENT_MEMBER_ID 
				<< SONO_SERVICE_XML_CLOSE_BRACKET << id << SONO_SERVICE_XML_OPEN_BRACKET_TERM 
				<< SONO_SERVICE_GROUP_MANAGEMENT_MEMBER_ID << SONO_SERVICE_XML_CLOSE_BRACKET
				<< SONO_SERVICE_XML_OPEN_BRACKET << SONO_SERVICE_GROUP_MANAGEMENT_RESULT_CODE 
				<< SONO_SERVICE_XML_CLOSE_BRACKET << result << SONO_SERVICE_XML_OPEN_BRACKET_TERM 
				<< SONO_SERVICE_GROUP_MANAGEMENT_RESULT_CODE << SONO_SERVICE_XML_CLOSE_BRACKET;
			response = run(SONO_SERVICE_GROUP_MANAGEMENT_REPORT_TRACK_BUFFERING_RESULT, stream.str(), timeout);

			code = sono_http::parse_header(response);
			if(code != SONO_HTTP_SUCCESS) {
				THROW_SONO_SERVICE_EXCEPTION_FORMAT(SONO_SERVICE_EXCEPTION_POST_RESPONSE,
					"%s --> %u", SONO_SERVICE_GROUP_MANAGEMENT_REPORT_TRACK_BUFFERING_RESULT, code);
			}

			return sono_http::remove_header(response);
		}

		void 
		_sono_service_group_management::service_event(
			__in std::string &action,
			__in std::string &data
			)
		{
			sono_service::service_event(SONO_SERVICE_GROUP_MANAGEMENT, action, data);
		}

		_sono_service_music_services::_sono_service_music_services(
			__in const sono_service_meta &data
			) :
				sono_service(SONO_SERVICE_MUSIC_SERVICES, data)
		{
			return;
		}

		_sono_service_music_services::_sono_service_music_services(
			__in const _sono_service_music_services &other
			) :
				sono_service(other)
		{
			return;
		}

		_sono_service_music_services::~_sono_service_music_services(void)
		{
			return;
		}

		_sono_service_music_services &
		_sono_service_music_services::operator=(
			__in const _sono_service_music_services &other
			)
		{

			if(this != &other) {
				sono_service::operator=(other);
			}

			return *this;
		}

		std::string 
		_sono_service_music_services::get_session_id(
			__in uint32_t service,
			__in const std::string &user,
			__in_opt uint32_t timeout
			)
		{
			int code;
			std::string response;
			std::stringstream stream;

			stream << SONO_SERVICE_XML_OPEN_BRACKET << SONO_SERVICE_MUSIC_SERVICES_SERVICE_ID 
				<< SONO_SERVICE_XML_CLOSE_BRACKET << service << SONO_SERVICE_XML_OPEN_BRACKET_TERM 
				<< SONO_SERVICE_MUSIC_SERVICES_SERVICE_ID << SONO_SERVICE_XML_CLOSE_BRACKET
				<< SONO_SERVICE_XML_OPEN_BRACKET << SONO_SERVICE_MUSIC_SERVICES_USERNAME 
				<< SONO_SERVICE_XML_CLOSE_BRACKET << user << SONO_SERVICE_XML_OPEN_BRACKET_TERM 
				<< SONO_SERVICE_MUSIC_SERVICES_USERNAME << SONO_SERVICE_XML_CLOSE_BRACKET;
			response = run(SONO_SERVICE_MUSIC_SERVICES_GET_SESSION_ID, stream.str(), timeout);

			code = sono_http::parse_header(response);
			if(code != SONO_HTTP_SUCCESS) {
				THROW_SONO_SERVICE_EXCEPTION_FORMAT(SONO_SERVICE_EXCEPTION_POST_RESPONSE,
					"%s --> %u", SONO_SERVICE_MUSIC_SERVICES_GET_SESSION_ID, code);
			}

			return sono_http::remove_header(response);
		}

		std::string 
		_sono_service_music_services::list_available_services(
			__in_opt uint32_t timeout
			)
		{
			int code;
			std::string response;
			std::stringstream stream;

			response = run(SONO_SERVICE_MUSIC_SERVICES_LIST_AVAILABLE_SERVICES, std::string(), timeout);

			code = sono_http::parse_header(response);
			if(code != SONO_HTTP_SUCCESS) {
				THROW_SONO_SERVICE_EXCEPTION_FORMAT(SONO_SERVICE_EXCEPTION_POST_RESPONSE,
					"%s --> %u", SONO_SERVICE_MUSIC_SERVICES_LIST_AVAILABLE_SERVICES, code);
			}

			return sono_http::remove_header(response);
		}

		std::string 
		_sono_service_music_services::update_available_services(
			__in_opt uint32_t timeout
			)
		{
			int code;
			std::string response;
			std::stringstream stream;

			response = run(SONO_SERVICE_MUSIC_SERVICES_UPDATE_AVAILABLE_SERVICES, std::string(), timeout);

			code = sono_http::parse_header(response);
			if(code != SONO_HTTP_SUCCESS) {
				THROW_SONO_SERVICE_EXCEPTION_FORMAT(SONO_SERVICE_EXCEPTION_POST_RESPONSE,
					"%s --> %u", SONO_SERVICE_MUSIC_SERVICES_UPDATE_AVAILABLE_SERVICES, code);
			}

			return sono_http::remove_header(response);
		}

		void 
		_sono_service_music_services::service_event(
			__in std::string &action,
			__in std::string &data
			)
		{
			sono_service::service_event(SONO_SERVICE_MUSIC_SERVICES, action, data);
		}

		_sono_service_qplay::_sono_service_qplay(
			__in const sono_service_meta &data
			) :
				sono_service(SONO_SERVICE_QPLAY, data)
		{
			return;
		}

		_sono_service_qplay::_sono_service_qplay(
			__in const _sono_service_qplay &other
			) :
				sono_service(other)
		{
			return;
		}

		_sono_service_qplay::~_sono_service_qplay(void)
		{
			return;
		}

		_sono_service_qplay &
		_sono_service_qplay::operator=(
			__in const _sono_service_qplay &other
			)
		{

			if(this != &other) {
				sono_service::operator=(other);
			}

			return *this;
		}

		std::string 
		_sono_service_qplay::auth(
			__in const std::string &seed,
			__in_opt uint32_t timeout
			)
		{
			int code;
			std::string response;
			std::stringstream stream;

			stream << SONO_SERVICE_XML_OPEN_BRACKET << SONO_SERVICE_QPLAY_AUTH_SEED 
				<< SONO_SERVICE_XML_CLOSE_BRACKET << seed << SONO_SERVICE_XML_OPEN_BRACKET_TERM 
				<< SONO_SERVICE_QPLAY_AUTH_SEED << SONO_SERVICE_XML_CLOSE_BRACKET;
			response = run(SONO_SERVICE_QPLAY_AUTH, stream.str(), timeout);

			code = sono_http::parse_header(response);
			if(code != SONO_HTTP_SUCCESS) {
				THROW_SONO_SERVICE_EXCEPTION_FORMAT(SONO_SERVICE_EXCEPTION_POST_RESPONSE,
					"%s --> %u", SONO_SERVICE_QPLAY_AUTH, code);
			}

			return sono_http::remove_header(response);
		}

		void 
		_sono_service_qplay::service_event(
			__in std::string &action,
			__in std::string &data
			)
		{
			sono_service::service_event(SONO_SERVICE_QPLAY, action, data);
		}

		_sono_service_render_av_transport::_sono_service_render_av_transport(
			__in const sono_service_meta &data
			) :
				sono_service(SONO_SERVICE_RENDER_AV_TRANSPORT, data)
		{
			return;
		}

		_sono_service_render_av_transport::_sono_service_render_av_transport(
			__in const _sono_service_render_av_transport &other
			) :
				sono_service(other)
		{
			return;
		}

		_sono_service_render_av_transport::~_sono_service_render_av_transport(void)
		{
			return;
		}

		_sono_service_render_av_transport &
		_sono_service_render_av_transport::operator=(
			__in const _sono_service_render_av_transport &other
			)
		{

			if(this != &other) {
				sono_service::operator=(other);
			}

			return *this;
		}

		void 
		_sono_service_render_av_transport::service_event(
			__in std::string &action,
			__in std::string &data
			)
		{
			sono_service::service_event(SONO_SERVICE_RENDER_AV_TRANSPORT, action, data);
		}

		_sono_service_render_connection_manager::_sono_service_render_connection_manager(
			__in const sono_service_meta &data
			) :
				sono_service(SONO_SERVICE_RENDER_CONNECTION_MANAGER, data)
		{
			return;
		}

		_sono_service_render_connection_manager::_sono_service_render_connection_manager(
			__in const _sono_service_render_connection_manager &other
			) :
				sono_service(other)
		{
			return;
		}

		_sono_service_render_connection_manager::~_sono_service_render_connection_manager(void)
		{
			return;
		}

		_sono_service_render_connection_manager &
		_sono_service_render_connection_manager::operator=(
			__in const _sono_service_render_connection_manager &other
			)
		{

			if(this != &other) {
				sono_service::operator=(other);
			}

			return *this;
		}

		std::string 
		_sono_service_render_connection_manager::get_current_connection_ids(
			__in_opt uint32_t timeout
			)
		{
			int code;
			std::string response;
			std::match_results<const char *> match;

			response = run(SONO_SERVICE_CONNECTION_MANAGER_CURRENT_CONNECTION_IDS, std::string(), timeout);

			code = sono_http::parse_header(response);
			if(code != SONO_HTTP_SUCCESS) {
				THROW_SONO_SERVICE_EXCEPTION_FORMAT(SONO_SERVICE_EXCEPTION_POST_RESPONSE,
					"%s --> %u", SONO_SERVICE_CONNECTION_MANAGER_CURRENT_CONNECTION_IDS, code);
			}

			return sono_http::remove_header(response);
		}

		std::string 
		_sono_service_render_connection_manager::get_current_connection_info(
			__in uint32_t id,
			__in_opt uint32_t timeout
			)
		{
			int code;
			std::string response;
			std::stringstream stream;

			stream << SONO_SERVICE_XML_OPEN_BRACKET << SONO_SERVICE_CONNECTION_MANAGER_CONNECTION_ID 
				<< SONO_SERVICE_XML_CLOSE_BRACKET << id << SONO_SERVICE_XML_OPEN_BRACKET_TERM 
				<< SONO_SERVICE_CONNECTION_MANAGER_CONNECTION_ID << SONO_SERVICE_XML_CLOSE_BRACKET;
			response = run(SONO_SERVICE_CONNECTION_MANAGER_CURRENT_CONNECTION_INFO, stream.str(), timeout);

			code = sono_http::parse_header(response);
			if(code != SONO_HTTP_SUCCESS) {
				THROW_SONO_SERVICE_EXCEPTION_FORMAT(SONO_SERVICE_EXCEPTION_POST_RESPONSE,
					"%s --> %u", SONO_SERVICE_CONNECTION_MANAGER_CURRENT_CONNECTION_INFO, code);
			}

			return sono_http::remove_header(response);
		}

		std::string 
		_sono_service_render_connection_manager::get_protocol_info(
			__in_opt uint32_t timeout
			)
		{
			int code;
			std::string response;
			std::match_results<const char *> match;

			response = run(SONO_SERVICE_CONNECTION_MAANGER_PROTOCOL_INFO, std::string(), timeout);

			code = sono_http::parse_header(response);
			if(code != SONO_HTTP_SUCCESS) {
				THROW_SONO_SERVICE_EXCEPTION_FORMAT(SONO_SERVICE_EXCEPTION_POST_RESPONSE,
					"%s --> %u", SONO_SERVICE_CONNECTION_MAANGER_PROTOCOL_INFO, code);
			}

			return sono_http::remove_header(response);
		}

		void 
		_sono_service_render_connection_manager::service_event(
			__in std::string &action,
			__in std::string &data
			)
		{
			sono_service::service_event(SONO_SERVICE_RENDER_CONNECTION_MANAGER, action, data);
		}

		_sono_service_render_control::_sono_service_render_control(
			__in const sono_service_meta &data
			) :
				sono_service(SONO_SERVICE_RENDER_CONTROL, data)
		{
			return;
		}

		_sono_service_render_control::_sono_service_render_control(
			__in const _sono_service_render_control &other
			) :
				sono_service(other)
		{
			return;
		}

		_sono_service_render_control::~_sono_service_render_control(void)
		{
			return;
		}

		_sono_service_render_control &
		_sono_service_render_control::operator=(
			__in const _sono_service_render_control &other
			)
		{

			if(this != &other) {
				sono_service::operator=(other);
			}

			return *this;
		}

		void 
		_sono_service_render_control::service_event(
			__in std::string &action,
			__in std::string &data
			)
		{
			sono_service::service_event(SONO_SERVICE_RENDER_CONTROL, action, data);
		}

		_sono_service_render_group_control::_sono_service_render_group_control(
			__in const sono_service_meta &data
			) :
				sono_service(SONO_SERVICE_RENDER_GROUP_CONTROL, data)
		{
			return;
		}

		_sono_service_render_group_control::_sono_service_render_group_control(
			__in const _sono_service_render_group_control &other
			) :
				sono_service(other)
		{
			return;
		}

		_sono_service_render_group_control::~_sono_service_render_group_control(void)
		{
			return;
		}

		_sono_service_render_group_control &
		_sono_service_render_group_control::operator=(
			__in const _sono_service_render_group_control &other
			)
		{

			if(this != &other) {
				sono_service::operator=(other);
			}

			return *this;
		}

		void 
		_sono_service_render_group_control::service_event(
			__in std::string &action,
			__in std::string &data
			)
		{
			sono_service::service_event(SONO_SERVICE_RENDER_GROUP_CONTROL, action, data);
		}

		_sono_service_render_queue::_sono_service_render_queue(
			__in const sono_service_meta &data
			) :
				sono_service(SONO_SERVICE_RENDER_QUEUE, data)
		{
			return;
		}

		_sono_service_render_queue::_sono_service_render_queue(
			__in const _sono_service_render_queue &other
			) :
				sono_service(other)
		{
			return;
		}

		_sono_service_render_queue::~_sono_service_render_queue(void)
		{
			return;
		}

		_sono_service_render_queue &
		_sono_service_render_queue::operator=(
			__in const _sono_service_render_queue &other
			)
		{

			if(this != &other) {
				sono_service::operator=(other);
			}

			return *this;
		}

		void 
		_sono_service_render_queue::service_event(
			__in std::string &action,
			__in std::string &data
			)
		{
			sono_service::service_event(SONO_SERVICE_RENDER_QUEUE, action, data);
		}

		_sono_service_server_connection_manager::_sono_service_server_connection_manager(
			__in const sono_service_meta &data
			) :
				sono_service(SONO_SERVICE_SERVER_CONNECTION_MANAGER, data)
		{
			return;
		}

		_sono_service_server_connection_manager::_sono_service_server_connection_manager(
			__in const _sono_service_server_connection_manager &other
			) :
				sono_service(other)
		{
			return;
		}

		_sono_service_server_connection_manager::~_sono_service_server_connection_manager(void)
		{
			return;
		}

		_sono_service_server_connection_manager &
		_sono_service_server_connection_manager::operator=(
			__in const _sono_service_server_connection_manager &other
			)
		{

			if(this != &other) {
				sono_service::operator=(other);
			}

			return *this;
		}

		std::string 
		_sono_service_server_connection_manager::get_current_connection_ids(
			__in_opt uint32_t timeout
			)
		{
			int code;
			std::string response;
			std::match_results<const char *> match;

			response = run(SONO_SERVICE_CONNECTION_MANAGER_CURRENT_CONNECTION_IDS, std::string(), timeout);

			code = sono_http::parse_header(response);
			if(code != SONO_HTTP_SUCCESS) {
				THROW_SONO_SERVICE_EXCEPTION_FORMAT(SONO_SERVICE_EXCEPTION_POST_RESPONSE,
					"%s --> %u", SONO_SERVICE_CONNECTION_MANAGER_CURRENT_CONNECTION_IDS, code);
			}

			return sono_http::remove_header(response);
		}

		std::string 
		_sono_service_server_connection_manager::get_current_connection_info(
			__in uint32_t id,
			__in_opt uint32_t timeout
			)
		{
			int code;
			std::string response;
			std::stringstream stream;

			stream << SONO_SERVICE_XML_OPEN_BRACKET << SONO_SERVICE_CONNECTION_MANAGER_CONNECTION_ID 
				<< SONO_SERVICE_XML_CLOSE_BRACKET << id << SONO_SERVICE_XML_OPEN_BRACKET_TERM 
				<< SONO_SERVICE_CONNECTION_MANAGER_CONNECTION_ID << SONO_SERVICE_XML_CLOSE_BRACKET;
			response = run(SONO_SERVICE_CONNECTION_MANAGER_CURRENT_CONNECTION_INFO, stream.str(), timeout);

			code = sono_http::parse_header(response);
			if(code != SONO_HTTP_SUCCESS) {
				THROW_SONO_SERVICE_EXCEPTION_FORMAT(SONO_SERVICE_EXCEPTION_POST_RESPONSE,
					"%s --> %u", SONO_SERVICE_CONNECTION_MANAGER_CURRENT_CONNECTION_INFO, code);
			}

			return sono_http::remove_header(response);
		}

		std::string 
		_sono_service_server_connection_manager::get_protocol_info(
			__in_opt uint32_t timeout
			)
		{
			int code;
			std::string response;
			std::match_results<const char *> match;

			response = run(SONO_SERVICE_CONNECTION_MAANGER_PROTOCOL_INFO, std::string(), timeout);

			code = sono_http::parse_header(response);
			if(code != SONO_HTTP_SUCCESS) {
				THROW_SONO_SERVICE_EXCEPTION_FORMAT(SONO_SERVICE_EXCEPTION_POST_RESPONSE,
					"%s --> %u", SONO_SERVICE_CONNECTION_MAANGER_PROTOCOL_INFO, code);
			}

			return sono_http::remove_header(response);
		}

		void 
		_sono_service_server_connection_manager::service_event(
			__in std::string &action,
			__in std::string &data
			)
		{
			sono_service::service_event(SONO_SERVICE_SERVER_CONNECTION_MANAGER, action, data);
		}

		_sono_service_server_content_directory::_sono_service_server_content_directory(
			__in const sono_service_meta &data
			) :
				sono_service(SONO_SERVICE_SERVER_CONTENT_DIRECTORY, data)
		{
			return;
		}

		_sono_service_server_content_directory::_sono_service_server_content_directory(
			__in const _sono_service_server_content_directory &other
			) :
				sono_service(other)
		{
			return;
		}

		_sono_service_server_content_directory::~_sono_service_server_content_directory(void)
		{
			return;
		}

		_sono_service_server_content_directory &
		_sono_service_server_content_directory::operator=(
			__in const _sono_service_server_content_directory &other
			)
		{

			if(this != &other) {
				sono_service::operator=(other);
			}

			return *this;
		}

		void 
		_sono_service_server_content_directory::service_event(
			__in std::string &action,
			__in std::string &data
			)
		{
			sono_service::service_event(SONO_SERVICE_SERVER_CONTENT_DIRECTORY, action, data);
		}

		_sono_service_system_properties::_sono_service_system_properties(
			__in const sono_service_meta &data
			) :
				sono_service(SONO_SERVICE_SYSTEM_PROPERTIES, data)
		{
			return;
		}

		_sono_service_system_properties::_sono_service_system_properties(
			__in const _sono_service_system_properties &other
			) :
				sono_service(other)
		{
			return;
		}

		_sono_service_system_properties::~_sono_service_system_properties(void)
		{
			return;
		}

		_sono_service_system_properties &
		_sono_service_system_properties::operator=(
			__in const _sono_service_system_properties &other
			)
		{

			if(this != &other) {
				sono_service::operator=(other);
			}

			return *this;
		}

		void 
		_sono_service_system_properties::service_event(
			__in std::string &action,
			__in std::string &data
			)
		{
			sono_service::service_event(SONO_SERVICE_SYSTEM_PROPERTIES, action, data);
		}

		_sono_service_zone_group_topology::_sono_service_zone_group_topology(
			__in const sono_service_meta &data
			) :
				sono_service(SONO_SERVICE_ZONE_GROUP_TOPOLOGY, data)
		{
			return;
		}

		_sono_service_zone_group_topology::_sono_service_zone_group_topology(
			__in const _sono_service_zone_group_topology &other
			) :
				sono_service(other)
		{
			return;
		}

		_sono_service_zone_group_topology::~_sono_service_zone_group_topology(void)
		{
			return;
		}

		_sono_service_zone_group_topology &
		_sono_service_zone_group_topology::operator=(
			__in const _sono_service_zone_group_topology &other
			)
		{

			if(this != &other) {
				sono_service::operator=(other);
			}

			return *this;
		}

		void 
		_sono_service_zone_group_topology::service_event(
			__in std::string &action,
			__in std::string &data
			)
		{
			sono_service::service_event(SONO_SERVICE_ZONE_GROUP_TOPOLOGY, action, data);
		}
	}
}
