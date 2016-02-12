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

#ifndef SONO_DEVICE_TYPE_H_
#define SONO_DEVICE_TYPE_H_

namespace SONO {

	namespace COMP {

		#define SONO_DEVICE_EXCEPTION 0x300
		#define SONO_DEVICE_HEADER "[SONO::DEVICE]"
#ifndef NDEBUG
		#define SONO_DEVICE_EXCEPTION_HEADER SONO_DEVICE_HEADER " "
#else
		#define SONO_DEVICE_EXCEPTION_HEADER 
#endif // NDEBUG

		enum {
			SONO_DEVICE_EXCEPTION_ALLOCATED = 0,
			SONO_DEVICE_EXCEPTION_INITIALIZED,
			SONO_DEVICE_EXCEPTION_NOT_FOUND,
			SONO_DEVICE_EXCEPTION_SERVICE_DISCOVERY,
			SONO_DEVICE_EXCEPTION_SERVICE_NOT_FOUND,
			SONO_DEVICE_EXCEPTION_UNINITIALIZED,
		};

		#define SONO_DEVICE_EXCEPTION_MAX SONO_DEVICE_EXCEPTION_UNINITIALIZED

		static const std::string SONO_DEVICE_EXCEPTION_STR[] = {
			SONO_DEVICE_EXCEPTION_HEADER "Failed to allocate device manager",
			SONO_DEVICE_EXCEPTION_HEADER "Device manager is initialized",
			SONO_DEVICE_EXCEPTION_HEADER "Device does not exist",
			SONO_DEVICE_EXCEPTION_HEADER "Device service discovery failed",
			SONO_DEVICE_EXCEPTION_HEADER "Device service does not exist",
			SONO_DEVICE_EXCEPTION_HEADER "Device manager is uninitialized",
			};

		#define SONO_DEVICE_EXCEPTION_STRING(_TYPE_) \
			((_TYPE_) > SONO_DEVICE_EXCEPTION_MAX ? SONO_DEVICE_EXCEPTION_HEADER EXCEPTION_UNKNOWN : \
			STRING_CHECK(SONO_DEVICE_EXCEPTION_STR[_TYPE_]))

		#define THROW_SONO_DEVICE_EXCEPTION(_EXCEPT_) \
			THROW_EXCEPTION(SONO_DEVICE_EXCEPTION + _EXCEPT_, SONO_DEVICE_EXCEPTION_STRING(_EXCEPT_))
		#define THROW_SONO_DEVICE_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(SONO_DEVICE_EXCEPTION + _EXCEPT_, SONO_DEVICE_EXCEPTION_STRING(_EXCEPT_), \
			_FORMAT_, __VA_ARGS__)
	}
}

#endif // SONO_DEVICE_TYPE_H_
