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

#ifndef SONO_DEFINE_H_
#define SONO_DEFINE_H_

#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>

namespace SONO {

	#ifndef __in
	#define __in
	#endif // __in
	#ifndef __in_opt
	#define __in_opt
	#endif // __in_opt

	#ifndef __inout
	#define __inout
	#endif // __inout
	#ifndef __inout_opt
	#define __inout_opt
	#endif // __inout_opt

	#ifndef __out
	#define __out
	#endif // __out
	#ifndef __out_opt
	#define __out_opt
	#endif // __out_opt

	#define REF_INIT 1

	#define SCALAR_AS_HEX(_TYPE_, _VAL_) \
		std::setw(sizeof(_TYPE_) * 2) << std::setfill('0') << std::hex \
		<< ((uintmax_t) (_TYPE_) (_VAL_)) << std::dec

	#define SCALAR_INVALID(_TYPE_) ((_TYPE_) -1)

	#define STRING_EMPTY "EMPTY"
	#define STRING_UNKNOWN "UNKNOWN"

	#define STRING_CHECK(_STR_) (_STR_.empty() ? STRING_EMPTY : _STR_.c_str())

	#define _STRING_CONCAT(_STR_) # _STR_
	#define STRING_CONCAT(_STR_) _STRING_CONCAT(_STR_)

	#define UNREF_PARAM(_PARAM_) (void) _PARAM_

	#define VERSION_MAJOR 0
	#define VERSION_MINOR 1
	#define VERSION_REVISION 4
	#define VERSION_WEEK 1606

	#define VERSION_STRING \
		STRING_CONCAT(VERSION_MAJOR) "." STRING_CONCAT(VERSION_MINOR) "." \
		STRING_CONCAT(VERSION_WEEK) "." STRING_CONCAT(VERSION_REVISION)
}

#endif // SONO_DEFINE_H_
