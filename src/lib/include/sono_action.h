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

#ifndef SONO_ACTION_H_
#define SONO_ACTION_H_

namespace SONO {

	namespace COMP {

		typedef std::map<std::string, std::string> sono_action_argument;
		typedef std::set<std::string> sono_action_list;

		typedef class _sono_action {

			public:

				_sono_action(
					__in const std::string &type,
					__in const std::string &name,
					__in_opt const std::vector<std::string> &input = std::vector<std::string>(),
					__in_opt const std::vector<std::string> &output = std::vector<std::string>()
					);

				_sono_action(
					__in const _sono_action &other
					);

				virtual ~_sono_action(void);

				_sono_action &operator=(
					__in const _sono_action &other
					);

				const std::vector<std::string> &input(void);

				const std::string &name(void);

				const std::vector<std::string> &output(void);

				sono_action_argument run(
					__in const std::string &path,
					__in const std::string &address,
					__in uint16_t port,
					__in const sono_action_argument &argument,
					__in_opt uint32_t timeout = SONO_SOCKET_NO_TIMEOUT
					);

				void set(
					__in const std::string &type,
					__in const std::string &name,
					__in_opt const std::vector<std::string> &input = std::vector<std::string>(),
					__in_opt const std::vector<std::string> &output = std::vector<std::string>()
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

				const std::string &type(void);

			protected:

				sono_action_argument parse_response(
					__in const std::string &response
					);

				std::vector<std::string> m_input;

				std::string m_name;

				std::vector<std::string> m_output;

				std::string m_type;

		} sono_action;
	}
}

#endif // SONO_ACTION_H_
