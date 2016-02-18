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

void 
event_handler(
	__in sono_uid_t device,
	__in const std::string &service,
	__in const std::string &action,
	__in const std::string &data
	)
{
	std::cout << "{" << SCALAR_AS_HEX(sono_uid_t, device) << "} -- SVC. 0x" << STRING_CHECK(service) 
		<< ", ACT. " << STRING_CHECK(action) << ", DATA[" << data.size() << "]";

	if(!data.empty()) {
		std::cout << " " << data;
	}

	std::cout << std::endl;
}

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
		instance->initialize(event_handler);

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
			//std::cout << std::endl << "{" << SCALAR_AS_HEX(sono_uid_t, dev_iter->first) << "} (" << dev_iter->second.first
			//	<< ") " << dev_iter->second.second.first << ":" << dev_iter->second.second.second;
			sono_device &dev = instance->device(dev_iter->second.second.first, dev_iter->second.second.second);
			svc_list = dev.service_discovery(SERVICE_DISCOVERY_TIMEOUT);
			//std::cout << std::endl << "--- " << svc_list.size() << " service(s)." << std::endl << "----";

			for(svc_iter = svc_list.begin(); svc_iter != svc_list.end(); ++svc_iter) {
				//std::cout << std::endl << "--- " << dev.service(*svc_iter).to_string(true);
			}

			std::cout << std::endl;
		}

		// TODO
		#define SERVICE_ACTION_TIMEOUT 2 // sec
		#define SERVICE_ACTION_PROPERTIES_LED_OFF "Off"
		#define SERVICE_ACTION_PROPERTIES_LED_ON "On"
		#define SERVICE_ACTION_PROPERTIES_LED_STATE_DESIRE "DesiredLEDState"
		#define SERVICE_ACTION_PROPERTIES_LED_STATE_GET "GetLEDState"
		#define SERVICE_ACTION_PROPERTIES_LED_STATE_SET "SetLEDState"
		#define SERVICE_DEVICE_PROPERTIES "DeviceProperties"

		sono_action_argument input, output;
		sono_device &dev = instance->device(1);
		sono_action_argument::iterator iter_out;
		sono_service &svc = dev.service(SERVICE_DEVICE_PROPERTIES);

		// LED OFF
		input.insert(std::pair<std::string, std::string>(SERVICE_ACTION_PROPERTIES_LED_STATE_DESIRE, SERVICE_ACTION_PROPERTIES_LED_OFF));
		output = svc.run(SERVICE_ACTION_PROPERTIES_LED_STATE_SET, input, SERVICE_ACTION_TIMEOUT);
		std::cout << SERVICE_ACTION_PROPERTIES_LED_STATE_SET << "[" << output.size() << "]";

		for(iter_out = output.begin(); iter_out != output.end(); ++iter_out) {
			std::cout << std::endl << iter_out->first << " --> " << iter_out->second;
		}

		std::cout << std::endl;
		input.clear();
		output = svc.run(SERVICE_ACTION_PROPERTIES_LED_STATE_GET, input, SERVICE_ACTION_TIMEOUT);
		std::cout << SERVICE_ACTION_PROPERTIES_LED_STATE_GET << "[" << output.size() << "]";

		for(iter_out = output.begin(); iter_out != output.end(); ++iter_out) {
			std::cout << std::endl << iter_out->first << " --> " << iter_out->second;
		}

		std::cout << std::endl;
		std::cin.get();

		// LED ON
		input.clear();
		input.insert(std::pair<std::string, std::string>(SERVICE_ACTION_PROPERTIES_LED_STATE_DESIRE, SERVICE_ACTION_PROPERTIES_LED_ON));
		output = svc.run(SERVICE_ACTION_PROPERTIES_LED_STATE_SET, input, SERVICE_ACTION_TIMEOUT);
		std::cout << SERVICE_ACTION_PROPERTIES_LED_STATE_SET << "[" << output.size() << "]";

		for(iter_out = output.begin(); iter_out != output.end(); ++iter_out) {
			std::cout << std::endl << iter_out->first << " --> " << iter_out->second;
		}

		std::cout << std::endl;
		input.clear();
		output = svc.run(SERVICE_ACTION_PROPERTIES_LED_STATE_GET, input, SERVICE_ACTION_TIMEOUT);
		std::cout << SERVICE_ACTION_PROPERTIES_LED_STATE_GET << "[" << output.size() << "]";

		for(iter_out = output.begin(); iter_out != output.end(); ++iter_out) {
			std::cout << std::endl << iter_out->first << " --> " << iter_out->second;
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
