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

#include <cstdarg>
#include "../include/sono.h"

namespace SONO {

	#define EXCEPTION_MALFORMED "Malformed exception"

	_sono_exception::_sono_exception(
		__in sono_exc_t type,
		__in const std::string &message,
		__in const std::string &function,
		__in const std::string &file,
		__in size_t line
		) :
			std::runtime_error(message),
			m_file(file),
			m_function(function),
			m_line(line),
			m_type(type)
	{
		return;
	}

	_sono_exception::_sono_exception(
		__in const _sono_exception &other
		) :
			std::runtime_error(other),
			m_file(other.m_file),
			m_function(other.m_function),
			m_line(other.m_line),
			m_type(other.m_type)
	{
		return;
	}

	_sono_exception::~_sono_exception(void)
	{
		return;
	}

	_sono_exception &
	_sono_exception::operator=(
		__in const _sono_exception &other
		)
	{

		if(this != &other) {
			std::runtime_error::operator=(other);
			m_file = other.m_file;
			m_function = other.m_function;
			m_line = other.m_line;
			m_type = other.m_type;
		}

		return *this;
	}

	std::string 
	_sono_exception::file(void)
	{
		return m_file;
	}

	std::string 
	_sono_exception::function(void)
	{
		return m_function;
	}

	void 
	_sono_exception::generate(
		__in sono_exc_t type,
		__in const std::string &message,
		__in const std::string &function,
		__in const std::string &file,
		__in size_t line,
		__in const char *format,
		...
		)
	{
		int length;
		va_list list;
		std::string buffer;
		std::stringstream result;

		if(format) {
			va_start(list, format);
			length = std::vsnprintf(NULL, 0, format, list);
			va_end(list);

			if(length < 0) {
				buffer = EXCEPTION_MALFORMED;
				type = EXCEPTION_INVALID;
			} else if(length > 0) {				
				va_start(list, format);
				buffer.resize(length + 1);
				length = std::vsnprintf((char *) &buffer[0], length + 1, format, list);
				va_end(list);

				if(length < 0) {
					buffer = EXCEPTION_MALFORMED;
					type = EXCEPTION_INVALID;
				}
			}
		}

		if(!message.empty()) {
			result << message;
		}

		if(!buffer.empty()) {

			if(!result.str().empty()) {
				result << ": ";
			}

			result << buffer;
		}

		throw sono_exception(type, result.str(), function, file, line);
	}

	size_t 
	_sono_exception::line(void)
	{
		return m_line;
	}

	std::string 
	_sono_exception::to_string(
		__in_opt bool verbose
		)
	{
		std::stringstream result;

		result << what();

		if(verbose) {

			if(!result.str().empty()) {
				result << " ";
			}

			result << "(" << STRING_CHECK(m_file) << "::" << STRING_CHECK(m_function) 
				<< "@" << m_line << ")";
		}

		return result.str();
	}

	sono_exc_t 
	_sono_exception::type(void)
	{
		return m_type;
	}
}
