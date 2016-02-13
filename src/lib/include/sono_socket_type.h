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

#ifndef SONO_DISCOVER_TYPE_H_
#define SONO_DISCOVER_TYPE_H_

namespace SONO {

	namespace COMP {

		#define SONO_SOCKET_EXCEPTION 0x400
		#define SONO_SOCKET_HEADER "[SONO:SOCKET]"
#ifndef NDEBUG
		#define SONO_SOCKET_EXCEPTION_HEADER SONO_SOCKET_HEADER " "
#else
		#define SONO_SOCKET_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			SONO_SOCKET_EXCEPTION_ADDRESS_INVALID = 0,
			SONO_SOCKET_EXCEPTION_ALLOCATED,
			SONO_SOCKET_EXCEPTION_CONNECTED,
			SONO_SOCKET_EXCEPTION_DISCONNECTED,
			SONO_SOCKET_EXCEPTION_INITIALIZED,
			SONO_SOCKET_EXCEPTION_INTERNAL,
			SONO_SOCKET_EXCEPTION_NOT_FOUND,
			SONO_SOCKET_EXCEPTION_TYPE_INVALID,
			SONO_SOCKET_EXCEPTION_UNALLOCATED,
			SONO_SOCKET_EXCEPTION_UNINITIALIZED,
		};

		#define SONO_SOCKET_EXCEPTION_MAX SONO_SOCKET_EXCEPTION_UNINITIALIZED

		static const std::string SONO_SOCKET_EXCEPTION_STR[] = {
			SONO_SOCKET_EXCEPTION_HEADER "Invalid socket address",
			SONO_SOCKET_EXCEPTION_HEADER "Failed to allocate socket manager",
			SONO_SOCKET_EXCEPTION_HEADER "Socket is connected",
			SONO_SOCKET_EXCEPTION_HEADER "Socket is disconnected",
			SONO_SOCKET_EXCEPTION_HEADER "Socket manager is initialized",
			SONO_SOCKET_EXCEPTION_HEADER "Socket exception",
			SONO_SOCKET_EXCEPTION_HEADER "Socket does not exist",
			SONO_SOCKET_EXCEPTION_HEADER "Invalid socket type",
			SONO_SOCKET_EXCEPTION_HEADER "Socket manager is not allocated",
			SONO_SOCKET_EXCEPTION_HEADER "Socket manager is uninitialized",
			};

		#define SONO_SOCKET_EXCEPTION_STRING(_TYPE_) \
			((_TYPE_) > SONO_SOCKET_EXCEPTION_MAX ? SONO_SOCKET_EXCEPTION_HEADER EXCEPTION_UNKNOWN : \
			STRING_CHECK(SONO_SOCKET_EXCEPTION_STR[_TYPE_]))

		#define THROW_SONO_SOCKET_EXCEPTION(_EXCEPT_) \
			THROW_EXCEPTION(SONO_SOCKET_EXCEPTION + (_EXCEPT_), SONO_SOCKET_EXCEPTION_STRING(_EXCEPT_))
		#define THROW_SONO_SOCKET_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(SONO_SOCKET_EXCEPTION + (_EXCEPT_), SONO_SOCKET_EXCEPTION_STRING(_EXCEPT_), \
			_FORMAT_, __VA_ARGS__)
	}
}

#endif // SONO_DISCOVER_TYPE_H_
