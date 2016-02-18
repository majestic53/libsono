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
#include "../include/sono_action_type.h"

namespace SONO {

	namespace COMP {

		#define SONO_ACTION_BACKTRACE "(" SONO_ACTION_WILDCARD ")"
		#define SONO_ACTION_BODY_PREFIX \
			"<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">" \
			"<s:Body>"
		#define SONO_ACTION_BODY_SUFFIX "</s:Body></s:Envelope>"
		#define SONO_ACTION_COMMAND_PREFIX_0 "<u:"
		#define SONO_ACTION_COMMAND_PREFIX_1 " xmlns:u=\""
		#define SONO_ACTION_COMMAND_PREFIX_2 "\">"
		#define SONO_ACTION_COMMAND_SUFFIX_0 "</u:"
		#define SONO_ACTION_COMMAND_SUFFIX_1 ">"
		#define SONO_ACTION_DIVIDER "#"
		#define SONO_ACTION_PREFIX "SOAPAction: \""
		#define SONO_ACTION_SUFFIX "\""
		#define SONO_ACTION_CLOSE_BRACKET ">"
		#define SONO_ACTION_OPEN_BRACKET "<"
		#define SONO_ACTION_OPEN_BRACKET_TERM "</"
		#define SONO_ACTION_WILDCARD ".*"

		enum {
			SONO_ACTION_RESPONSE_ROOT = 0,
			SONO_ACTION_RESPONSE_VALUE,
		};

		#define SONO_ACTION_RESPONSE_MAX SONO_ACTION_RESPONSE_VALUE

		_sono_action::_sono_action(
			__in const std::string &type,
			__in const std::string &name,
			__in_opt const std::vector<std::string> &input,
			__in_opt const std::vector<std::string> &output
			)
		{
			set(type, name, input, output);
		}

		_sono_action::_sono_action(
			__in const _sono_action &other
			) :
				m_input(other.m_input),
				m_name(other.m_name),
				m_output(other.m_output),
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
				m_input = other.m_input;
				m_name = other.m_name;
				m_output = other.m_output;
				m_type = other.m_type;
			}

			return *this;
		}

		const std::vector<std::string> &
		_sono_action::input(void)
		{
			return m_input;
		}

		const std::string &
		_sono_action::name(void)
		{
			return m_name;
		}

		const std::vector<std::string> &
		_sono_action::output(void)
		{
			return m_output;
		}

		sono_action_argument 
		_sono_action::parse_response(
			__in const std::string &response
			)
		{
			std::stringstream search;
			sono_action_argument result;
			std::match_results<const char *> match;
			std::vector<std::string>::iterator iter;

			for(iter = m_output.begin(); iter != m_output.end(); ++iter) {
				search << SONO_ACTION_WILDCARD << SONO_ACTION_OPEN_BRACKET << *iter 
					<< SONO_ACTION_CLOSE_BRACKET << SONO_ACTION_BACKTRACE 
					<< SONO_ACTION_OPEN_BRACKET_TERM << *iter << SONO_ACTION_CLOSE_BRACKET
					<< SONO_ACTION_WILDCARD;

				std::regex_search(response.c_str(), match, std::regex(search.str().c_str()));
				if(match.size() == (SONO_ACTION_RESPONSE_MAX + 1)) {
					result.insert(std::pair<std::string, std::string>(*iter, match[SONO_ACTION_RESPONSE_VALUE]));	
				}
			}

			return result;
		}

		sono_action_argument 
		_sono_action::run(
			__in const std::string &path,
			__in const std::string &address,
			__in uint16_t port,
			__in const sono_action_argument &argument,
			__in_opt uint32_t timeout
			)
		{
			int code;
			size_t iter = 0;
			std::string response;
			std::stringstream head, body;
			sono_action_argument::const_iterator entry;

			if(argument.size() != m_input.size()) {
				THROW_SONO_ACTION_EXCEPTION_FORMAT(SONO_ACTION_EXCEPTION_INVALID_ARGUMENT_COUNT,
					"%s: found %lu arguments (expecting %lu)", STRING_CHECK(m_name), argument.size(), 
					m_input.size());
			}

			head << SONO_ACTION_PREFIX << m_type << SONO_ACTION_DIVIDER << m_name << SONO_ACTION_SUFFIX;
			body << SONO_ACTION_BODY_PREFIX << SONO_ACTION_COMMAND_PREFIX_0 << m_name << SONO_ACTION_COMMAND_PREFIX_1 
				<< m_type << SONO_ACTION_COMMAND_PREFIX_2;

			for(; iter < m_input.size(); ++iter) {

				entry = argument.find(m_input.at(iter));
				if(entry == argument.end()) {
					THROW_SONO_ACTION_EXCEPTION_FORMAT(SONO_ACTION_EXCEPTION_MISSING_ARGUMENT,
						"missing argument: %s", STRING_CHECK(m_input.at(iter)));
				}

				body << SONO_ACTION_OPEN_BRACKET << m_input.at(iter) << SONO_ACTION_CLOSE_BRACKET
					<< entry->second << SONO_ACTION_OPEN_BRACKET_TERM << m_input.at(iter) 
					<< SONO_ACTION_CLOSE_BRACKET;
			}

			body << SONO_ACTION_COMMAND_SUFFIX_0 << m_name << SONO_ACTION_COMMAND_SUFFIX_1 << SONO_ACTION_BODY_SUFFIX;
			response = sono_http::post(path, address, port, head.str(), body.str(), SONO_SOCKET_TCP, timeout);

			code = sono_http::parse_header(response);
			if(code != SONO_HTTP_SUCCESS) {
				THROW_SONO_ACTION_EXCEPTION_FORMAT(SONO_ACTION_EXCEPTION_POST_RESPONSE,
					"%s --> %u", STRING_CHECK(m_name), code);
			}

			return parse_response(sono_http::remove_header(response));
		}

		void 
		_sono_action::set(
			__in const std::string &type,
			__in const std::string &name,
			__in_opt const std::vector<std::string> &input,
			__in_opt const std::vector<std::string> &output
			)
		{

			if(name.empty() || type.empty()) {
				THROW_SONO_ACTION_EXCEPTION(SONO_ACTION_EXCEPTION_INVALID);
			}

			m_input = input;
			m_name = name;
			m_output = output;
			m_type = type;
		}

		std::string 
		_sono_action::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;
			std::vector<std::string>::iterator iter;

			result << SONO_ACTION_HEADER << ", \", ACT. \"" << m_name << "\", IN[" << m_input.size() << "]";

			if(verbose && !m_input.empty()) {
				result << " {";

				for(iter = m_input.begin(); iter != m_input.end(); ++iter) {

					if(iter != m_input.begin()) {
						result << ", ";
					}

					result << *iter;
				}

				result << "}";
			}

			result << ", OUT[" << m_output.size() << "]";

			if(verbose && !m_output.empty()) {
				result << " {";

				for(iter = m_output.begin(); iter != m_output.end(); ++iter) {

					if(iter != m_output.begin()) {
						result << ", ";
					}

					result << *iter;
				}

				result << "}";
			}

			return result.str();
		}

		const std::string &
		_sono_action::type(void)
		{
			return m_type;
		}
	}
}
