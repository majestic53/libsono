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

#ifndef SONO_ACTION_TYPE_H_
#define SONO_ACTION_TYPE_H_

namespace SONO {

	namespace COMP {

		#define SONO_ACTION_EXCEPTION 0x800
		#define SONO_ACTION_HEADER "[SONO::ACTION]"
#ifndef NDEBUG
		#define SONO_ACTION_EXCEPTION_HEADER SONO_ACTION_HEADER " "
#else
		#define SONO_ACTION_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			SONO_ACTION_EXCEPTION_INVALID = 0,
			SONO_ACTION_EXCEPTION_INVALID_ARGUMENT_COUNT,
			SONO_ACTION_EXCEPTION_MISSING_ARGUMENT,
			SONO_ACTION_EXCEPTION_POST_RESPONSE,
		};

		#define SONO_ACTION_EXCEPTION_MAX SONO_ACTION_EXCEPTION_POST_RESPONSE

		static const std::string SONO_ACTION_EXCEPTION_STR[] = {
			SONO_ACTION_EXCEPTION_HEADER "Invalid action",
			SONO_ACTION_EXCEPTION_HEADER "Invalid argument count",
			SONO_ACTION_EXCEPTION_HEADER "Missing argument",
			SONO_ACTION_EXCEPTION_HEADER "POST request failed",
			};

		#define SONO_ACTION_EXCEPTION_STRING(_TYPE_) \
			((_TYPE_) > SONO_ACTION_EXCEPTION_MAX ? SONO_ACTION_EXCEPTION_HEADER EXCEPTION_UNKNOWN : \
			STRING_CHECK(SONO_ACTION_EXCEPTION_STR[_TYPE_]))

		#define THROW_SONO_ACTION_EXCEPTION(_EXCEPT_) \
			THROW_EXCEPTION(SONO_ACTION_EXCEPTION + (_EXCEPT_), SONO_ACTION_EXCEPTION_STRING(_EXCEPT_))
		#define THROW_SONO_ACTION_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(SONO_ACTION_EXCEPTION + (_EXCEPT_), SONO_ACTION_EXCEPTION_STRING(_EXCEPT_), \
			_FORMAT_, __VA_ARGS__)
	}
}

#endif // SONO_ACTION_TYPE_H_
