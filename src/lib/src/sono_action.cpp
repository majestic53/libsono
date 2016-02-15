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
#include "../include/sono_action_type.h"

namespace SONO {

	namespace COMP {

		#define SONO_HTTP_ACTION_BODY_PREFIX \
			"<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">" \
			"<s:Body>"
		#define SONO_HTTP_ACTION_BODY_SUFFIX "</s:Body></s:Envelope>"
		#define SONO_HTTP_ACTION_COMMAND_PREFIX_0 "<u:"
		#define SONO_HTTP_ACTION_COMMAND_PREFIX_1 " xmlns:u=\""
		#define SONO_HTTP_ACTION_COMMAND_PREFIX_2 "\">"
		#define SONO_HTTP_ACTION_COMMAND_SUFFIX_0 "</u:"
		#define SONO_HTTP_ACTION_COMMAND_SUFFIX_1 ">"
		#define SONO_HTTP_ACTION_DIVIDER "#"
		#define SONO_HTTP_ACTION_PREFIX "SOAPAction: \""
		#define SONO_HTTP_ACTION_SUFFIX "\""

		_sono_action::_sono_action(
			__in const std::string &name,
			__in const std::string &type
			)
		{
			set(name, type);
		}

		_sono_action::_sono_action(
			__in const _sono_action &other
			) :
				m_name(other.m_name),
				m_type(other.m_type)
		{
			return;
		}

		_sono_action::~_sono_action(void)
		{
			return;
		}

		_sono_action &
		_sono_action::operator=(
			__in const _sono_action &other
			)
		{

			if(this != &other) {
				m_name = other.m_name;
				m_type = other.m_type;
			}

			return *this;
		}

		std::string 
		_sono_action::name(void)
		{
			return m_name;
		}

		std::string 
		_sono_action::run(
			__in const std::string &path,
			__in const std::string &address,
			__in uint16_t port,
			__in const std::string &parameters,
			__in_opt uint32_t timeout
			)
		{
			std::stringstream head, body;

			head << SONO_HTTP_ACTION_PREFIX << m_type << SONO_HTTP_ACTION_DIVIDER << m_name 
				<< SONO_HTTP_ACTION_SUFFIX;
			body << SONO_HTTP_ACTION_BODY_PREFIX << SONO_HTTP_ACTION_COMMAND_PREFIX_0 << m_name 
				<< SONO_HTTP_ACTION_COMMAND_PREFIX_1 << m_type << SONO_HTTP_ACTION_COMMAND_PREFIX_2 
				<< parameters << SONO_HTTP_ACTION_COMMAND_SUFFIX_0 << m_name << SONO_HTTP_ACTION_COMMAND_SUFFIX_1
				<< SONO_HTTP_ACTION_BODY_SUFFIX;

			return sono_http::post(path, address, port, head.str(), body.str(), SONO_SOCKET_TCP, timeout);
		}

		void 
		_sono_action::set(
			__in const std::string &name,
			__in const std::string &type
			)
		{

			if(name.empty() || type.empty()) {
				THROW_SONO_ACTION_EXCEPTION(SONO_ACTION_EXCEPTION_UNSUPPORTED);
			}

			m_name = name;
			m_type = type;
		}

		std::string 
		_sono_action::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			UNREF_PARAM(verbose);

			result << SONO_ACTION_HEADER << ", TYPE. \"" << m_type << "\", ACT. \"" << m_name << "\"";

			return result.str();
		}

		std::string 
		_sono_action::type(void)
		{
			return m_type;
		}
	}
}
