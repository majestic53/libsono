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

#ifndef SONO_HTTP_TYPE_H_
#define SONO_HTTP_TYPE_H_

namespace SONO {

	namespace COMP {

		#define SONO_HTTP_EXCEPTION 0x700
		#define SONO_HTTP_HEADER "[SONO::HTTP]"
#ifndef NDEBUG
		#define SONO_HTTP_EXCEPTION_HEADER SONO_HTTP_HEADER " "
#else
		#define SONO_HTTP_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			SONO_HTTP_EXCEPTION_GET = 0,
			SONO_HTTP_EXCEPTION_MALFORMED,
			SONO_HTTP_EXCEPTION_UNSUPPORTED,
		};

		#define SONO_HTTP_EXCEPTION_MAX SONO_HTTP_EXCEPTION_UNSUPPORTED

		static const std::string SONO_HTTP_EXCEPTION_STR[] = {
			SONO_HTTP_EXCEPTION_HEADER "GET request failed",
			SONO_HTTP_EXCEPTION_HEADER "Malformed GET response",
			SONO_HTTP_EXCEPTION_HEADER "Unsupported encoding",
			};

		#define SONO_HTTP_EXCEPTION_STRING(_TYPE_) \
			((_TYPE_) > SONO_HTTP_EXCEPTION_MAX ? SONO_HTTP_EXCEPTION_HEADER EXCEPTION_UNKNOWN : \
			STRING_CHECK(SONO_HTTP_EXCEPTION_STR[_TYPE_]))

		#define THROW_SONO_HTTP_EXCEPTION(_EXCEPT_) \
			THROW_EXCEPTION(SONO_HTTP_EXCEPTION + (_EXCEPT_), SONO_HTTP_EXCEPTION_STRING(_EXCEPT_))
		#define THROW_SONO_HTTP_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(SONO_HTTP_EXCEPTION + (_EXCEPT_), SONO_HTTP_EXCEPTION_STRING(_EXCEPT_), \
			_FORMAT_, __VA_ARGS__)
	}
}

#endif // SONO_HTTP_TYPE_H_
