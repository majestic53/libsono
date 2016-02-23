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

#ifndef SONO_LOADER_TYPE_H_
#define SONO_LOADER_TYPE_H_

namespace SONO {

	#define SONO_LOADER_EXCEPTION 0x800
	#define SONO_LOADER_HEADER "[SONO::LOADER]"
#ifndef NDEBUG
	#define SONO_LOADER_EXCEPTION_HEADER SONO_LOADER_HEADER " "
#else
	#define SONO_LOADER_EXCEPTION_HEADER
#endif // NDEBUG

	enum {
		SONO_LOADER_EXCEPTION_ALLOCATED = 0,
		SONO_LOADER_EXCEPTION_INITIALIZED,
		SONO_LOADER_EXCEPTION_UNINITIALIZED,
	};

	#define SONO_LOADER_EXCEPTION_MAX SONO_LOADER_EXCEPTION_UNINITIALIZED

	static const std::string SONO_LOADER_EXCEPTION_STR[] = {
		SONO_LOADER_EXCEPTION_HEADER "Failed to allocate loader",
		SONO_LOADER_EXCEPTION_HEADER "Loader is initialized",
		SONO_LOADER_EXCEPTION_HEADER "Loader is uninitialized",
		};

	#define SONO_LOADER_EXCEPTION_STRING(_TYPE_) \
		((_TYPE_) > SONO_LOADER_EXCEPTION_MAX ? SONO_LOADER_EXCEPTION_HEADER EXCEPTION_UNKNOWN : \
		STRING_CHECK(SONO_LOADER_EXCEPTION_STR[_TYPE_]))

	#define THROW_SONO_LOADER_EXCEPTION(_EXCEPT_) \
		THROW_EXCEPTION(SONO_LOADER_EXCEPTION + (_EXCEPT_), SONO_LOADER_EXCEPTION_STRING(_EXCEPT_))
	#define THROW_SONO_LOADER_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
		THROW_EXCEPTION_FORMAT(SONO_LOADER_EXCEPTION + (_EXCEPT_), SONO_LOADER_EXCEPTION_STRING(_EXCEPT_), \
		_FORMAT_, __VA_ARGS__)
}

#endif // SONO_LOADER_TYPE_H_
