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

#ifndef SONO_LOADER_H_
#define SONO_LOADER_H_

#include "../../lib/include/sono_manager.h"

namespace SONO {

	typedef enum {
		SONOCTL_ARGUMENT_ACTION,
		SONOCTL_ARGUMENT_DEVICE,
		SONOCTL_ARGUMENT_HELP,
		SONOCTL_ARGUMENT_LIST,
		SONOCTL_ARGUMENT_LIST_ACTIONS,
		SONOCTL_ARGUMENT_LIST_SERVICES,
		SONOCTL_ARGUMENT_SERVICE,
		SONOCTL_ARGUMENT_VERBOSE,
		SONOCTL_ARGUMENT_VERSION,
	} sono_argument_t;

	#define SONOCTL_ARGUMENT_INVALID SCALAR_INVALID(sono_argument_t)
	#define SONOCTL_ARGUMENT_MAX SONOCTL_ARGUMENT_VERSION
	#define SONOCTL_ARGUMENT_MIN SONOCTL_ARGUMENT_ACTION

	typedef enum {
		SONOCTL_SECTION_EXAMPLE = 0,
		SONOCTL_SECTION_OPTIONS,
		SONOCTL_SECTION_REPORT,
		SONOCTL_SECTION_USAGE,
		SONOCTL_SECTION_VERSION,
	} sonoctl_section_t;

	#define SONOCTL_SECTION_MAX SONOCTL_SECTION_VERSION

	typedef std::vector<std::string> sono_argument_list;

	typedef class _sono_loader {

		public:

			~_sono_loader(void);

			static _sono_loader *acquire(void);

			const sono_argument_list &arguments(void);

			void initialize(
				__in int count,
				__in const char **arguments
				);

			static bool is_allocated(void);

			bool is_initialized(void);

			const sono_action_argument &result(void);

			const sono_action_argument &run(
				__in_opt const sono_argument_list &arguments = sono_argument_list(),
				__in_opt uint32_t device_timeout = SONO_SOCKET_NO_TIMEOUT,
				__in_opt uint32_t service_timeout = SONO_SOCKET_NO_TIMEOUT,
				__in_opt uint32_t action_timeout = SONO_SOCKET_NO_TIMEOUT
				);

			size_t size(void);

			std::string to_string(
				__in_opt bool verbose = false
				);

			void uninitialize(void);

		protected:

			_sono_loader(void);

			_sono_loader(
				__in const _sono_loader &other
				);

			_sono_loader &operator=(
				__in const _sono_loader &other
				);

			static void _delete(void);

			static sono_argument_t determine_argument(
				__in const std::string &argument
				);

			static std::string display_section(
				__in sonoctl_section_t section
				);

			static void event_handler(
				__in sono_uid_t device,
				__in const std::string &service,
				__in const std::string &action,
				__in const std::string &data
				);

			sono_argument_list m_arguments;

			bool m_initialized;

			static _sono_loader *m_instance;

			sono_action_argument m_results;

	} sono_loader;
}

#endif // SONO_LOADER_H_
