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
		typedef std::vector<std::string> sono_action_arguments;
		typedef std::set<std::string> sono_action_list;

		typedef class _sono_action {

			public:

				_sono_action(
					__in const std::string &type,
					__in const std::string &name,
					__in_opt const sono_action_arguments &input = sono_action_arguments(),
					__in_opt const sono_action_arguments &output = sono_action_arguments()
					);

				_sono_action(
					__in const _sono_action &other
					);

				virtual ~_sono_action(void);

				_sono_action &operator=(
					__in const _sono_action &other
					);

				const sono_action_arguments &input(void);

				const std::string &name(void);

				const sono_action_arguments &output(void);

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
					__in_opt const sono_action_arguments &input = sono_action_arguments(),
					__in_opt const sono_action_arguments &output = sono_action_arguments()
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

				const std::string &type(void);

			protected:

				sono_action_argument parse_response(
					__in const std::string &response
					);

				sono_action_arguments m_input;

				std::string m_name;

				sono_action_arguments m_output;

				std::string m_type;

		} sono_action;
	}
}

#endif // SONO_ACTION_H_
