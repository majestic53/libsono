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

#include "../lib/include/sono.h"

int 
main(
	__in int argc,
	__in const char **argv
	)
{
	int result = 0;
	sono_manager *instance = NULL;

	std::cout << sono_manager::version() << std::endl;

	try {
		instance = sono_manager::acquire();
		instance->initialize();

		// TODO
		#define DEVICE_DISCOVERY_TIMEOUT 1 // sec
		#define SERVICE_DISCOVERY_TIMEOUT 2 // sec

		sono_device_list dev_list;
		sono_service_list svc_list;
		sono_device_list::iterator dev_iter;
		sono_service_list::iterator svc_iter;

		dev_list = instance->device_discovery(DEVICE_DISCOVERY_TIMEOUT);
		std::cout << std::endl << "Found " << dev_list.size() << " device(s)." << std::endl << "---";

		for(dev_iter = dev_list.begin(); dev_iter != dev_list.end(); ++dev_iter) {
			std::cout << std::endl << "{" << SCALAR_AS_HEX(sono_uid_t, dev_iter->first) << "} (" << dev_iter->second.first
				<< ") " << dev_iter->second.second.first << ":" << dev_iter->second.second.second;

			svc_list = instance->device_service_discovery(dev_iter->first, SERVICE_DISCOVERY_TIMEOUT);
			std::cout << std::endl << "--- " << svc_list.size() << " service(s)." << std::endl << "----";

			for(svc_iter = svc_list.begin(); svc_iter != svc_list.end(); ++svc_iter) {
				std::cout << std::endl << "--- " << instance->device_service(dev_iter->first, *svc_iter).to_string(true);
			}

			std::cout << std::endl;
		}

		std::cout << std::endl;
		// ---

		instance->uninitialize();
	} catch(sono_exception &exc) {
		std::cerr << exc.to_string(true) << std::endl;
		result = SCALAR_INVALID(int);
	} catch(std::exception &exc) {
		std::cerr << exc.what() << std::endl;
		result = SCALAR_INVALID(int);
	}

	return result;
}
