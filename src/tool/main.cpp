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

#include "./include/sono_loader.h"

#define DEVICE_DISCOVERY_TIMEOUT 1 // sec
#define SERVICE_ACTION_TIMEOUT 2 // sec
#define SERVICE_DISCOVERY_TIMEOUT 2 // sec

int 
main(
	__in int argc,
	__in const char **argv
	)
{
	int result = 0;
	sono_loader *instance = NULL;

	try {
		instance = sono_loader::acquire();
		instance->initialize(argc, argv);
		instance->run(sono_argument_list(), DEVICE_DISCOVERY_TIMEOUT, SERVICE_DISCOVERY_TIMEOUT, 
			SERVICE_ACTION_TIMEOUT);
		instance->uninitialize();
	} catch(sono_exception &exc) {
		std::cerr << exc.to_string() << std::endl;
		result = SCALAR_INVALID(int);
	} catch(std::exception &exc) {
		std::cerr << exc.what() << std::endl;
		result = SCALAR_INVALID(int);
	}

	return result;
}
