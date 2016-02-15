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

#ifndef SONO_SERVICE_TYPE_H_
#define SONO_SERVICE_TYPE_H_

namespace SONO {

	namespace COMP {
		#define SONO_SERVICE_EXCEPTION 0x500
		#define SONO_SERVICE_HEADER "[SONO::SERVICE]"
#ifndef NDEBUG
		#define SONO_SERVICE_EXCEPTION_HEADER SONO_SERVICE_HEADER " "
#else
		#define SONO_SERVICE_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			SONO_SERVICE_EXCEPTION_GET_RESPONSE = 0,
			SONO_SERVICE_EXCEPTION_INVALID_TYPE,
			SONO_SERVICE_EXCEPTION_MALFORMED,
			SONO_SERVICE_EXCEPTION_NOT_FOUND,
			SONO_SERVICE_EXCEPTION_POST_RESPONSE,
			SONO_SERVICE_EXCEPTION_SERVICE_DISCOVERY,
			SONO_SERVICE_EXCEPTION_UNKNOWN_TYPE,
		};

		#define SONO_SERVICE_EXCEPTION_MAX SONO_SERVICE_EXCEPTION_UNKNOWN_TYPE

		static const std::string SONO_SERVICE_EXCEPTION_STR[] = {
			SONO_SERVICE_EXCEPTION_HEADER "GET request was unsuccessful",
			SONO_SERVICE_EXCEPTION_HEADER "Invalid service type",
			SONO_SERVICE_EXCEPTION_HEADER "Malformed POST response",
			SONO_SERVICE_EXCEPTION_HEADER "Action does not exist",
			SONO_SERVICE_EXCEPTION_HEADER "POST request was unsuccessful",
			SONO_SERVICE_EXCEPTION_HEADER "Service discovery failed",
			SONO_SERVICE_EXCEPTION_HEADER "Unknown service type",
			};

		#define SONO_SERVICE_EXCEPTION_STRING(_TYPE_) \
			((_TYPE_) > SONO_SERVICE_EXCEPTION_MAX ? SONO_SERVICE_EXCEPTION_HEADER EXCEPTION_UNKNOWN : \
			STRING_CHECK(SONO_SERVICE_EXCEPTION_STR[_TYPE_]))

		#define THROW_SONO_SERVICE_EXCEPTION(_EXCEPT_) \
			THROW_EXCEPTION(SONO_SERVICE_EXCEPTION + (_EXCEPT_), SONO_SERVICE_EXCEPTION_STRING(_EXCEPT_))
		#define THROW_SONO_SERVICE_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(SONO_SERVICE_EXCEPTION + (_EXCEPT_), SONO_SERVICE_EXCEPTION_STRING(_EXCEPT_), \
			_FORMAT_, __VA_ARGS__)
	}
}

#endif // SONO_SERVICE_TYPE_H_
