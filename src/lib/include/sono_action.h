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

		typedef class _sono_action {

			public:

				_sono_action(
					__in const std::string &name,
					__in const std::string &type
					);

				_sono_action(
					__in const _sono_action &other
					);

				virtual ~_sono_action(void);

				_sono_action &operator=(
					__in const _sono_action &other
					);

				std::string name(void);

				std::string run(
					__in const std::string &path,
					__in const std::string &address,
					__in uint16_t port,
					__in const std::string &parameters,
					__in_opt uint32_t timeout = SONO_SOCKET_NO_TIMEOUT
					);

				void set(
					__in const std::string &name,
					__in const std::string &type
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

				std::string type(void);

			protected:

				std::string m_name;

				std::string m_type;

		} sono_action;
	}
}

#endif // SONO_ACTION_H_
