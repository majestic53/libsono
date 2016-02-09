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

#ifndef SONO_UID_TYPE_H_
#define SONO_UID_TYPE_H_

namespace SONO {

	namespace COMP {

		#define SONO_UID_EXCEPTION 0x200
		#define SONO_UID_HEADER "[SONO::UID]"
#ifndef NDEBUG
		#define SONO_UID_EXCEPTION_HEADER SONO_UID_HEADER " "
#else
		#define SONO_UID_EXCEPTION_HEADER 
#endif // NDEBUG

		enum {
			SONO_UID_EXCEPTION_ALLOCATION = 0,
			SONO_UID_EXCEPTION_FULL,
			SONO_UID_EXCEPTION_INITIALIZED,
			SONO_UID_EXCEPTION_NOT_FOUND,
			SONO_UID_EXCEPTION_UNINITIALIZED,
		};

		#define SONO_UID_EXCEPTION_MAX SONO_UID_EXCEPTION_UNINITIALIZED

		static const std::string SONO_UID_EXCEPTION_STR[] = {
			SONO_UID_EXCEPTION_HEADER "Failed to allocate component",
			SONO_UID_EXCEPTION_HEADER "Uid exceeded capacity",
			SONO_UID_EXCEPTION_HEADER "Component is initialized",
			SONO_UID_EXCEPTION_HEADER "Uid does not exist",
			SONO_UID_EXCEPTION_HEADER "Component is uninitialized",
			};

		#define SONO_UID_EXCEPTION_STRING(_TYPE_) \
			((_TYPE_) > SONO_UID_EXCEPTION_MAX ? EXCEPTION_UNKNOWN : \
			STRING_CHECK(SONO_UID_EXCEPTION_STR[_TYPE_]))

		#define THROW_SONO_UID_EXCEPTION(_EXCEPT_) \
			THROW_EXCEPTION(SONO_UID_EXCEPTION + _EXCEPT_, SONO_UID_EXCEPTION_STRING(_EXCEPT_))
		#define THROW_SONO_UID_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(SONO_UID_EXCEPTION + _EXCEPT_, SONO_UID_EXCEPTION_STRING(_EXCEPT_), \
			_FORMAT_, __VA_ARGS__)
	}
}

#endif // SONO_UID_TYPE_H_
