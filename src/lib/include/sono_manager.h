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

#ifndef SONO_MANAGER_H_
#define SONO_MANAGER_H_

#define NDEBUG

#ifndef SONO
#define SONO sono
#endif // SONO

#include "sono_define.h"
#include "sono_exception.h"

using namespace SONO;

#ifndef COMP
#define COMP comp
#endif // COMP

#include "sono_uid.h"
#include "sono_socket.h"
#include "sono_http.h"
#include "sono_action.h"
#include "sono_xml.h"
#include "sono_service.h"
#include "sono_device.h"
#include "sono_controller.h"

using namespace SONO::COMP;

namespace SONO {

	typedef void (*sono_event_handler)(
		__in sono_uid_t device,
		__in const std::string &service,
		__in const std::string &action,
		__in const std::string &data
		);

	typedef class _sono_manager {

		public:

			~_sono_manager(void);

			static _sono_manager *acquire(void);

			sono_controller *control(void);

			sono_device &device(
				__in sono_uid_t id
				);

			sono_device &device(
				__in const std::string &address,
				__in uint16_t port,
				__in_opt bool create = false
				);

			size_t device_count(void);

			sono_device_list device_discovery(
				__in_opt uint32_t timeout = SONO_SOCKET_NO_TIMEOUT
				);

			sono_device_list device_list(void);

			void initialize(
				__in sono_event_handler handler
				);

			static bool is_allocated(void);

			bool is_initialized(void);

			void service_event(
				__in sono_uid_t device,
				__in const std::string &service,
				__in const std::string &action,
				__in const std::string &data
				);

			std::string to_string(
				__in_opt bool verbose = false
				);

			void uninitialize(void);

			static std::string version(void);

		protected:

			_sono_manager(void);

			_sono_manager(
				__in const _sono_manager &other
				);

			_sono_manager &operator=(
				__in const _sono_manager &other
				);

			static void _delete(void);

			sono_controller *m_controller;

			sono_device_factory *m_factory_device;

			sono_socket_factory *m_factory_socket;

			sono_uid_factory *m_factory_uid;

			sono_event_handler m_handler;

			bool m_initialized;

			static _sono_manager *m_instance;

	} sono_manager;
}

#endif // SONO_MANAGER_H_
