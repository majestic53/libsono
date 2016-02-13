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

#ifndef SONO_XML_TYPE_H_
#define SONO_XML_TYPE_H_

namespace SONO {

	namespace COMP {

		#define SONO_XML_EXCEPTION 0x600
		#define SONO_XML_HEADER "[SONO::XML]"
#ifndef NDEBUG
		#define SONO_XML_EXCEPTION_HEADER SONO_XML_HEADER " "
#else
		#define SONO_XML_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			SONO_XML_EXCEPTION_MALFORMED_SOURCE = 0,
		};

		#define SONO_XML_EXCEPTION_MAX SONO_XML_EXCEPTION_MALFORMED_SOURCE

		static const std::string SONO_XML_EXCEPTION_STR[] = {
			SONO_XML_EXCEPTION_HEADER "Malformed xml source",
			};

		#define SONO_XML_EXCEPTION_STRING(_TYPE_) \
			((_TYPE_) > SONO_XML_EXCEPTION_MAX ? EXCEPTION_UNKNOWN : \
			STRING_CHECK(SONO_XML_EXCEPTION_STR[_TYPE_]))

		#define THROW_SONO_XML_EXCEPTION(_EXCEPT_) \
			THROW_EXCEPTION(SONO_XML_EXCEPTION + (_EXCEPT_), SONO_XML_EXCEPTION_STRING(_EXCEPT_))
		#define THROW_SONO_XML_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(SONO_XML_EXCEPTION + (_EXCEPT_), SONO_XML_EXCEPTION_STRING(_EXCEPT_), \
			_FORMAT_, __VA_ARGS__)
	}
}

#endif // SONO_XML_TYPE_H_
