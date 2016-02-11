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

#ifndef SONO_TYPE_H_
#define SONO_TYPE_H_

namespace SONO {

	#define SONO_EXCEPTION 0x100
	#define SONO_HEADER "[SONO]"
#ifndef NDEBUG
	#define SONO_EXCEPTION_HEADER SONO_HEADER " "
#else
	#define SONO_EXCEPTION_HEADER 
#endif // NDEBUG

	enum {
		SONO_EXCEPTION_ALLOCATED = 0,
		SONO_EXCEPTION_DEVICE_DISCOVERY,
		SONO_EXCEPTION_DEVICE_MALFORMED,
		SONO_EXCEPTION_INITIALIZED,
		SONO_EXCEPTION_UNINITIALIZED,
	};

	#define SONO_EXCEPTION_MAX SONO_EXCEPTION_UNINITIALIZED

	static const std::string SONO_EXCEPTION_STR[] = {
		SONO_EXCEPTION_HEADER "Failed to allocate library",
		SONO_EXCEPTION_HEADER "Device discovery failed",
		SONO_EXCEPTION_HEADER "Device malformed",
		SONO_EXCEPTION_HEADER "Library is initialized",
		SONO_EXCEPTION_HEADER "Library is uninitialized",
		};

	#define SONO_EXCEPTION_STRING(_TYPE_) \
		((_TYPE_) > SONO_EXCEPTION_MAX ? EXCEPTION_UNKNOWN : \
		STRING_CHECK(SONO_EXCEPTION_STR[_TYPE_]))

	#define THROW_SONO_EXCEPTION(_EXCEPT_) \
		THROW_EXCEPTION(SONO_EXCEPTION + _EXCEPT_, SONO_EXCEPTION_STRING(_EXCEPT_))
	#define THROW_SONO_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
		THROW_EXCEPTION_FORMAT(SONO_EXCEPTION + _EXCEPT_, SONO_EXCEPTION_STRING(_EXCEPT_), \
		_FORMAT_, __VA_ARGS__)
}

#endif // SONO_TYPE_H_
