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

#ifndef SONO_CONTROLLER_H_
#define SONO_CONTROLLER_H_

namespace SONO {

	namespace COMP {

		typedef enum {
			SONO_STATE_PLAYING = 0,
			SONO_STATE_PAUSED,
			SONO_STATE_STOPPED,
		} sono_state_t;

		#define SONO_STATE_INVALID SCALAR_INVALID(sono_state_t)
		#define SONO_STATE_MAX SONO_STATE_STOPPED

		typedef class _sono_controller {

			public:

				~_sono_controller(void);

				static _sono_controller *acquire(void);

				void add_to_queue(
					__in const std::string &address,
					__in uint16_t port,
					__in const std::string &path,
					__in_opt uint32_t service_timeout = SONO_SOCKET_NO_TIMEOUT,
					__in_opt uint32_t action_timeout = SONO_SOCKET_NO_TIMEOUT
					);

				void clear_queue(
					__in const std::string &address,
					__in uint16_t port,
					__in_opt uint32_t service_timeout = SONO_SOCKET_NO_TIMEOUT,
					__in_opt uint32_t action_timeout = SONO_SOCKET_NO_TIMEOUT
					);

				bool get_mute(
					__in const std::string &address,
					__in uint16_t port,
					__in_opt uint32_t service_timeout = SONO_SOCKET_NO_TIMEOUT,
					__in_opt uint32_t action_timeout = SONO_SOCKET_NO_TIMEOUT
					);

				sono_state_t get_playback_state(
					__in const std::string &address,
					__in uint16_t port,
					__in_opt uint32_t service_timeout = SONO_SOCKET_NO_TIMEOUT,
					__in_opt uint32_t action_timeout = SONO_SOCKET_NO_TIMEOUT
					);

				uint32_t get_volume(
					__in const std::string &address,
					__in uint16_t port,
					__in_opt uint32_t service_timeout = SONO_SOCKET_NO_TIMEOUT,
					__in_opt uint32_t action_timeout = SONO_SOCKET_NO_TIMEOUT
					);

				void initialize(void);

				static bool is_allocated(void);

				bool is_initialized(void);

				void next(
					__in const std::string &address,
					__in uint16_t port,
					__in_opt uint32_t service_timeout = SONO_SOCKET_NO_TIMEOUT,
					__in_opt uint32_t action_timeout = SONO_SOCKET_NO_TIMEOUT
					);

				void pause(
					__in const std::string &address,
					__in uint16_t port,
					__in_opt uint32_t service_timeout = SONO_SOCKET_NO_TIMEOUT,
					__in_opt uint32_t action_timeout = SONO_SOCKET_NO_TIMEOUT
					);

				void play(
					__in const std::string &address,
					__in uint16_t port,
					__in_opt uint32_t service_timeout = SONO_SOCKET_NO_TIMEOUT,
					__in_opt uint32_t action_timeout = SONO_SOCKET_NO_TIMEOUT
					);

				void previous(
					__in const std::string &address,
					__in uint16_t port,
					__in_opt uint32_t service_timeout = SONO_SOCKET_NO_TIMEOUT,
					__in_opt uint32_t action_timeout = SONO_SOCKET_NO_TIMEOUT
					);

				void restart(
					__in const std::string &address,
					__in uint16_t port,
					__in_opt uint32_t service_timeout = SONO_SOCKET_NO_TIMEOUT,
					__in_opt uint32_t action_timeout = SONO_SOCKET_NO_TIMEOUT
					);

				void seek(
					__in const std::string &address,
					__in uint16_t port,
					__in uint32_t hour,
					__in uint32_t minute,
					__in uint32_t second,
					__in_opt uint32_t service_timeout = SONO_SOCKET_NO_TIMEOUT,
					__in_opt uint32_t action_timeout = SONO_SOCKET_NO_TIMEOUT
					);

				void set_mute(
					__in const std::string &address,
					__in uint16_t port,
					__in bool mute,
					__in_opt uint32_t service_timeout = SONO_SOCKET_NO_TIMEOUT,
					__in_opt uint32_t action_timeout = SONO_SOCKET_NO_TIMEOUT
					);

				void set_volume(
					__in const std::string &address,
					__in uint16_t port,
					__in uint32_t volume,
					__in_opt uint32_t service_timeout = SONO_SOCKET_NO_TIMEOUT,
					__in_opt uint32_t action_timeout = SONO_SOCKET_NO_TIMEOUT
					);

				void stop(
					__in const std::string &address,
					__in uint16_t port,
					__in_opt uint32_t service_timeout = SONO_SOCKET_NO_TIMEOUT,
					__in_opt uint32_t action_timeout = SONO_SOCKET_NO_TIMEOUT
					);

				std::string to_string(
					__in_opt bool verbose = false
					);

				void uninitialize(void);

			protected:

				_sono_controller(void);

				_sono_controller(
					__in const _sono_controller &other
					);

				_sono_controller &operator=(
					__in const _sono_controller &other
					);

				static void _delete(void);

				void run(
					__in const std::string &address,
					__in uint16_t port,
					__in const std::string &service,
					__in const std::string &action,
					__in const sono_action_argument &input,
					__in_opt uint32_t service_timeout = SONO_SOCKET_NO_TIMEOUT,
					__in_opt uint32_t action_timeout = SONO_SOCKET_NO_TIMEOUT
					);

				bool m_initialized;

				static _sono_controller *m_instance;

				sono_action_argument m_results;

		} sono_controller;
	}
}

#endif // SONO_CONTROLLER_H_
