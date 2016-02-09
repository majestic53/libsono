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

#ifndef SONO_EXCEPTION_H_
#define SONO_EXCEPTION_H_

#include <stdexcept>

namespace SONO {

	#define EXCEPTION_INVALID SCALAR_INVALID(sono_exc_t)
	#define EXCEPTION_UNKNOWN "Unknown exception"

	#define THROW_EXCEPTION(_TYPE_, _EXCEPT_) \
		_sono_exception::generate(_TYPE_, _EXCEPT_, __FUNCTION__, __FILE__, \
		__LINE__, NULL)
	#define THROW_EXCEPTION_FORMAT(_TYPE_, _EXCEPT_, _FORMAT_, ...) \
		_sono_exception::generate(_TYPE_, _EXCEPT_, __FUNCTION__, __FILE__, \
		__LINE__, _FORMAT_, __VA_ARGS__)

	typedef uint32_t sono_exc_t;

	typedef class _sono_exception :
			public std::runtime_error {

		public:

			_sono_exception(
				__in sono_exc_t type,
				__in const std::string &message,
				__in const std::string &function,
				__in const std::string &file,
				__in size_t line
				);

			_sono_exception(
				__in const _sono_exception &other
				);

			virtual ~_sono_exception(void);

			_sono_exception &operator=(
				__in const _sono_exception &other
				);

			std::string file(void);

			std::string function(void);

			static void generate(
				__in sono_exc_t type,
				__in const std::string &message,
				__in const std::string &function,
				__in const std::string &file,
				__in size_t line,
				__in const char *format,
				...
				);

			size_t line(void);

			virtual std::string to_string(
				__in_opt bool verbose = false
				);

			sono_exc_t type(void);

		protected:

			std::string m_file;

			std::string m_function;

			size_t m_line;

			sono_exc_t m_type;

	} sono_exception;
}

#endif // SONO_EXCEPTION_H_
