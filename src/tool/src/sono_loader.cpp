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

#include <regex>
#include "../include/sono_loader.h"
#include "../include/sono_loader_type.h"

namespace SONO {

	#define SONO_LOADER_COUNT_MIN 1
	#define SONO_LOADER_DELIMETER '-'
	#define SONO_LOADER_DELIMETER_INDEX_LONG 1
	#define SONO_LOADER_DELIMETER_INDEX_SHORT 0
	#define SONO_LOADER_HELP_COLUMN_0 20
	#define SONO_LOADER_HELP_COLUMN_1 30
	#define SONO_LOADER_STRING_COPYRIGHT \
		"Copyright(C) 2016 David Jolly majestic53@gmail.com\
		\nLicense: GPLv3+ GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\
		\n\tThis is free software: you are free to change and redistribute it.\
		\n\tThere is NO WARRANTY, to the extent permitted by law."
	#define SONO_LOADER_STRING_EXAMPLE \
		"\n\tsonoctl -d 192.168.1.2:1400 -s DeviceProperties -a SetLEDState DesiredLEDState=On\
		\n\tsonoctl --list\
		\n\tsonoctl --list-services 192.168.1.2:1400\
		\n\tsonoctl --list-actions 192.168.1.2:1400 DeviceProperties"
	#define SOON_LOADER_STRING_PRODUCT "sonoctl"
	#define SONO_LOADER_STRING_REPORT \
		"Report bugs to: majestic53@gmail.com\
		\nHome page: <http://www.github.com/majestic53/libsono>"
	#define SONO_LOADER_STRING_TITLE "Sono Controller"
	#define SONO_LOADER_STRING_USAGE "[-h|-v][[-b][[-a args][-d args][-s args]]][-l[[a|s] args]]"

	static const std::string SONOCTL_ARGUMENT_STR[] = {
		"<action> <key>=<value>...",
		"<address>:<port>",
		"",
		"",
		"<address>:<port> <service>",
		"<address>:<port>",
		"<service>",
		"",
		"",
		};

	#define SONOCTL_ARGUMENT_STRING(_TYPE_) \
		((_TYPE_) > SONOCTL_ARGUMENT_MAX ? STRING_UNKNOWN : \
		SONOCTL_ARGUMENT_STR[_TYPE_])

	static const std::string SONOCTL_ARGUMENT_DESCRIPTION_STR[] = {
		"Specify target action/arguments",
		"Specify target device",
		"Display help information",
		"List available devices",
		"List available device service actions",
		"List available device services",
		"Specify target service",
		"Display verbose information",
		"Display version information",
		};

	#define SONOCTL_ARGUMENT_DESCRIPTION_STRING(_TYPE_) \
		((_TYPE_) > SONOCTL_ARGUMENT_MAX ? STRING_UNKNOWN : \
		STRING_CHECK(SONOCTL_ARGUMENT_DESCRIPTION_STR[_TYPE_]))

	static const std::string SONOCTL_ARGUMENT_LONG_STR[] = {
		"action", "device", "help", "list", "list-actions", "list-services", 
		"service", "verbose", "version",
		};

	#define SONOCTL_ARGUMENT_LONG_STRING(_TYPE_) \
		((_TYPE_) > SONOCTL_ARGUMENT_MAX ? STRING_UNKNOWN : \
		STRING_CHECK(SONOCTL_ARGUMENT_LONG_STR[_TYPE_]))

	static std::set<std::string> SONOCTL_ARGUMENT_LONG_SET = {
		SONOCTL_ARGUMENT_LONG_STR,
		SONOCTL_ARGUMENT_LONG_STR + (SONOCTL_ARGUMENT_MAX + 1)
		};

	#define IS_ARGUMENT_LONG(_STR_) \
		(SONOCTL_ARGUMENT_LONG_SET.find(_STR_) != SONOCTL_ARGUMENT_LONG_SET.end())

	static const std::string SONOCTL_ARGUMENT_SHORT_STR[] = {
		"a", "d", "h", "l", "la", "ls", "s", "b", "v",
		};

	#define SONOCTL_ARGUMENT_SHORT_STRING(_TYPE_) \
		((_TYPE_) > SONOCTL_ARGUMENT_MAX ? STRING_UNKNOWN : \
		STRING_CHECK(SONOCTL_ARGUMENT_SHORT_STR[_TYPE_]))

	static std::set<std::string> SONOCTL_ARGUMENT_SHORT_SET = {
		SONOCTL_ARGUMENT_SHORT_STR,
		SONOCTL_ARGUMENT_SHORT_STR + (SONOCTL_ARGUMENT_MAX + 1)
		};

	#define IS_ARGUMENT_SHORT(_STR_) \
		(SONOCTL_ARGUMENT_SHORT_SET.find(_STR_) != SONOCTL_ARGUMENT_SHORT_SET.end())

	static const std::string SONOCTL_SECTION_STR[] = {
		"Example", "Options", "", "Usage", "",
		};

	#define SONOCTL_SECTION_STRING(_TYPE_) \
		((_TYPE_) > SONOCTL_SECTION_MAX ? STRING_UNKNOWN : \
		STRING_CHECK(SONOCTL_SECTION_STR[_TYPE_]))

	enum {
		SONOCTL_ARGUMENT_DEVICE_ROOT = 0,
		SONOCTL_ARGUMENT_DEVICE_LEFT,
		SONOCTL_ARGUMENT_DEVICE_RIGHT,
	};

	#define SONOCTL_ARGUMENT_DEVICE_MAX SONOCTL_ARGUMENT_DEVICE_RIGHT
	#define SONOCTL_ARGUMENT_DEVICE_ADDRESS_REGEX "(.*):(.*)"
	#define SONOCTL_ARGUMENT_DEVICE_KEY_VALUE_REGEX "(.*)=(.*)"

	_sono_loader *_sono_loader::m_instance = NULL;

	_sono_loader::_sono_loader(void) :
		m_initialized(false)
	{
		std::atexit(sono_loader::_delete);
	}

	_sono_loader::~_sono_loader(void)
	{

		if(m_initialized) {
			uninitialize();
		}
	}

	void 
	_sono_loader::_delete(void)
	{

		if(sono_loader::m_instance) {
			delete sono_loader::m_instance;
			sono_loader::m_instance = NULL;
		}
	}

	_sono_loader *
	_sono_loader::acquire(void)
	{

		if(!sono_loader::m_instance) {

			sono_loader::m_instance = new sono_loader;
			if(!sono_loader::m_instance) {
				THROW_SONO_LOADER_EXCEPTION(SONO_LOADER_EXCEPTION_ALLOCATED);
			}
		}

		return sono_loader::m_instance;
	}

	const sono_argument_list &
	_sono_loader::arguments(void)
	{

		if(!m_initialized) {
			THROW_SONO_LOADER_EXCEPTION(SONO_LOADER_EXCEPTION_UNINITIALIZED);
		}

		return m_arguments;
	}

	sono_argument_t 
	_sono_loader::determine_argument(
		__in const std::string &argument
		)
	{
		const std::string *arguments = NULL;
		size_t result = SONOCTL_ARGUMENT_INVALID;

		if(IS_ARGUMENT_LONG(argument)) {
			arguments = SONOCTL_ARGUMENT_LONG_STR;
		} else if(IS_ARGUMENT_SHORT(argument)) {
			arguments = SONOCTL_ARGUMENT_SHORT_STR;
		}

		if(arguments) {

			for(result = SONOCTL_ARGUMENT_MIN; result <= SONOCTL_ARGUMENT_MAX; 
					++result) {

				if(argument == arguments[result]) {
					break;
				}
			}

			if(result > SONOCTL_ARGUMENT_MAX) {
				result = SONOCTL_ARGUMENT_INVALID;
			}
		}

		return (sono_argument_t) result;
	}

	std::string 
	_sono_loader::display_section(
		__in sonoctl_section_t section
		)
	{
		size_t iter = 0;
		std::stringstream result;
		std::string arg, arg_long, arg_short, desc;

		switch(section) {
			case SONOCTL_SECTION_EXAMPLE:
				result << SONOCTL_SECTION_STRING(section) << ":" << std::endl << SONO_LOADER_STRING_EXAMPLE;
				break;
			case SONOCTL_SECTION_OPTIONS:
				result << SONOCTL_SECTION_STRING(section) << ":";

				for(; iter <= SONOCTL_ARGUMENT_MAX; ++iter) {
					arg = SONOCTL_ARGUMENT_STRING(iter);
					arg_long = SONOCTL_ARGUMENT_LONG_STRING(iter);
					arg_short = SONOCTL_ARGUMENT_SHORT_STRING(iter);
					desc = SONOCTL_ARGUMENT_DESCRIPTION_STRING(iter);
					result << std::endl << "\t" << SONO_LOADER_DELIMETER << arg_short << "\t" << SONO_LOADER_DELIMETER 
						<< SONO_LOADER_DELIMETER << std::left << std::setw(SONO_LOADER_HELP_COLUMN_0)
						<< arg_long << std::left << std::setw(SONO_LOADER_HELP_COLUMN_1) << arg << desc;
				}
				break;
			case SONOCTL_SECTION_REPORT:
				result << SONO_LOADER_STRING_REPORT;
				break;
			case SONOCTL_SECTION_USAGE:
				result << SONOCTL_SECTION_STRING(section) << ":" << std::endl << "\t" << SOON_LOADER_STRING_PRODUCT 
					<< " " << SONO_LOADER_STRING_USAGE;
				break;
			case SONOCTL_SECTION_VERSION:
				result << SONO_LOADER_STRING_TITLE << " " << sono_manager::version() << std::endl 
					<< SONO_LOADER_STRING_COPYRIGHT;
				break;
			default:
				break;
		}

		return result.str();
	}

	void 
	_sono_loader::event_handler(
		__in sono_uid_t device,
		__in const std::string &service,
		__in const std::string &action,
		__in const std::string &data
		)
	{
		UNREF_PARAM(action);
		UNREF_PARAM(data);
		UNREF_PARAM(device);
		UNREF_PARAM(service);
	}

	void 
	_sono_loader::initialize(
		__in int count,
		__in const char **arguments
		)
	{

		if(m_initialized) {
			THROW_SONO_LOADER_EXCEPTION(SONO_LOADER_EXCEPTION_INITIALIZED);
		}

		m_arguments.clear();

		if(count > SONO_LOADER_COUNT_MIN) {
			m_arguments = sono_argument_list(&arguments[SONO_LOADER_COUNT_MIN], 
				&arguments[SONO_LOADER_COUNT_MIN] + (count - 1));
		}

		m_results.clear();
		sono_manager::acquire()->initialize(sono_loader::event_handler);
		m_initialized = true;
	}

	bool 
	_sono_loader::is_allocated(void)
	{
		return (sono_loader::m_instance != NULL);
	}

	bool 
	_sono_loader::is_initialized(void)
	{
		return m_initialized;
	}

	const sono_action_argument &
	_sono_loader::result(void)
	{

		if(!m_initialized) {
			THROW_SONO_LOADER_EXCEPTION(SONO_LOADER_EXCEPTION_UNINITIALIZED);
		}

		return m_results;
	}

	const sono_action_argument &
	_sono_loader::run(
		__in_opt const sono_argument_list &arguments,
		__in_opt uint32_t device_timeout,
		__in_opt uint32_t service_timeout,
		__in_opt uint32_t action_timeout
		)
	{
		size_t count;
		uint16_t port;
		sono_argument_t type;
		uint32_t mask = 0, offset;
		sono_action_argument input;
		sono_action_arguments args;
		sono_action_list list_action;
		sono_device_list list_device;
		sono_manager *instance = NULL;
		sono_service_list list_service;
		std::stringstream result, stream;
		sono_argument_list::iterator iter;
		std::string act, addr, arg, flag, svc;
		sono_action_list::iterator iter_action;
		sono_device_list::iterator iter_device;
		std::match_results<const char *> match;
		sono_action_argument::iterator iter_arg;
		sono_service_list::iterator iter_service;
		sono_action_arguments::iterator iter_args;

		if(!m_initialized) {
			THROW_SONO_LOADER_EXCEPTION(SONO_LOADER_EXCEPTION_UNINITIALIZED);
		}

		m_results.clear();

		if(!arguments.empty()) {
			m_arguments = arguments;
		}

		if(m_arguments.size() >= SONO_LOADER_COUNT_MIN) {
			instance = sono_manager::acquire();

			for(iter = m_arguments.begin(); iter != m_arguments.end(); ++iter) {
				flag = *iter;

				if((flag.at(SONO_LOADER_DELIMETER_INDEX_SHORT) != SONO_LOADER_DELIMETER)
						|| (flag.size() < (SONO_LOADER_DELIMETER_INDEX_SHORT + 1))) {
					std::cout << "Invalid argument: \'" << flag << "\'" << std::endl << std::endl 
						<< sono_loader::display_section(SONOCTL_SECTION_USAGE) << std::endl << std::endl 
						<< sono_loader::display_section(SONOCTL_SECTION_OPTIONS) << std::endl;
					break;
				}

				if((flag.size() >= (SONO_LOADER_DELIMETER_INDEX_LONG + 1)) && 
						flag.at(SONO_LOADER_DELIMETER_INDEX_LONG) == SONO_LOADER_DELIMETER) {
					flag = flag.substr(SONO_LOADER_DELIMETER_INDEX_LONG + 1, flag.size());
				} else {
					flag = flag.substr(SONO_LOADER_DELIMETER_INDEX_SHORT + 1, flag.size());
				}

				type = sono_loader::determine_argument(flag);
				switch(type) {
					case SONOCTL_ARGUMENT_ACTION:

						++iter;
						if(iter == m_arguments.end()) {
							std::cout << "Missing argument: \'" << flag << "\'" << std::endl << std::endl 
								<< sono_loader::display_section(SONOCTL_SECTION_USAGE) << std::endl << std::endl 
								<< sono_loader::display_section(SONOCTL_SECTION_OPTIONS) << std::endl;
							goto complete;
						}

						act = *iter;

						if(act.empty()) {
							std::cout << "Missing argument: \'" << flag << "\'" << std::endl << std::endl 
								<< sono_loader::display_section(SONOCTL_SECTION_USAGE) << std::endl << std::endl 
								<< sono_loader::display_section(SONOCTL_SECTION_OPTIONS) << std::endl;
							goto complete;
						}

						++iter;

						for(; iter != m_arguments.end(); ++iter) {

							arg = *iter;
							if(arg.at(SONO_LOADER_DELIMETER_INDEX_SHORT) == SONO_LOADER_DELIMETER) {
								--iter;
								break;
							}

							std::regex_search(arg.c_str(), match, std::regex(SONOCTL_ARGUMENT_DEVICE_KEY_VALUE_REGEX));
							if(match.size() != (SONOCTL_ARGUMENT_DEVICE_MAX + 1)) {
								std::cout << "Missing argument: \'" << flag << "\'" << std::endl << std::endl 
									<< sono_loader::display_section(SONOCTL_SECTION_USAGE) << std::endl << std::endl 
									<< sono_loader::display_section(SONOCTL_SECTION_OPTIONS) << std::endl;
								goto complete;
							}

							input.insert(std::pair<std::string, std::string>(match[SONOCTL_ARGUMENT_DEVICE_LEFT],
								match[SONOCTL_ARGUMENT_DEVICE_RIGHT]));
						}
						break;
					case SONOCTL_ARGUMENT_DEVICE:

						++iter;
						if(iter == m_arguments.end()) {
							std::cout << "Missing argument: \'" << flag << "\'" << std::endl << std::endl 
								<< sono_loader::display_section(SONOCTL_SECTION_USAGE) << std::endl << std::endl 
								<< sono_loader::display_section(SONOCTL_SECTION_OPTIONS) << std::endl;
							goto complete;
						}

						addr = *iter;

						std::regex_search(addr.c_str(), match, std::regex(SONOCTL_ARGUMENT_DEVICE_ADDRESS_REGEX));
						if(match.size() != (SONOCTL_ARGUMENT_DEVICE_MAX + 1)) {
							std::cout << "Missing argument: \'" << flag << "\'" << std::endl << std::endl 
								<< sono_loader::display_section(SONOCTL_SECTION_USAGE) << std::endl << std::endl 
								<< sono_loader::display_section(SONOCTL_SECTION_OPTIONS) << std::endl;
							goto complete;
						}

						stream << match[SONOCTL_ARGUMENT_DEVICE_RIGHT];
						stream >> port;
						addr = match[SONOCTL_ARGUMENT_DEVICE_LEFT];

						if(addr.empty()) {
							std::cout << "Missing argument: \'" << flag << "\'" << std::endl << std::endl 
								<< sono_loader::display_section(SONOCTL_SECTION_USAGE) << std::endl << std::endl 
								<< sono_loader::display_section(SONOCTL_SECTION_OPTIONS) << std::endl;
							goto complete;
						}
						break;
					case SONOCTL_ARGUMENT_HELP:
						std::cout << sono_loader::display_section(SONOCTL_SECTION_VERSION) << std::endl << std::endl
							<< sono_loader::display_section(SONOCTL_SECTION_USAGE) << std::endl << std::endl
							<< sono_loader::display_section(SONOCTL_SECTION_OPTIONS) << std::endl << std::endl
							<< sono_loader::display_section(SONOCTL_SECTION_EXAMPLE) << std::endl << std::endl
							<< sono_loader::display_section(SONOCTL_SECTION_REPORT) << std::endl;
						goto complete;
					case SONOCTL_ARGUMENT_LIST:

						if(!instance->device_count()) {
							list_device = instance->device_discovery(device_timeout);
						} else {
							list_device = instance->device_list();
						}

						std::cout << "Found " << list_device.size() << " device(s)" << std::endl << "---";

						for(count = 0, iter_device = list_device.begin(); iter_device != list_device.end(); 
								++count, ++iter_device) {
							std::cout << std::endl << "[" << count << "] " << iter_device->second.first
								<< " (" << iter_device->second.second.first << ":" 
								<< iter_device->second.second.second << ")";
						}

						std::cout << std::endl;
						goto complete;
					case SONOCTL_ARGUMENT_LIST_ACTIONS: {

							++iter;
							if(iter == m_arguments.end()) {
								std::cout << "Missing argument: \'" << flag << "\'" << std::endl << std::endl 
									<< sono_loader::display_section(SONOCTL_SECTION_USAGE) << std::endl << std::endl 
									<< sono_loader::display_section(SONOCTL_SECTION_OPTIONS) << std::endl;
								goto complete;
							}

							addr = *iter;

							std::regex_search(addr.c_str(), match, std::regex(SONOCTL_ARGUMENT_DEVICE_ADDRESS_REGEX));
							if(match.size() != (SONOCTL_ARGUMENT_DEVICE_MAX + 1)) {
								std::cout << "Missing argument: \'" << flag << "\'" << std::endl << std::endl 
									<< sono_loader::display_section(SONOCTL_SECTION_USAGE) << std::endl << std::endl 
									<< sono_loader::display_section(SONOCTL_SECTION_OPTIONS) << std::endl;
								goto complete;
							}

							stream << match[SONOCTL_ARGUMENT_DEVICE_RIGHT];
							stream >> port;
							addr = match[SONOCTL_ARGUMENT_DEVICE_LEFT];

							if(addr.empty()) {
								std::cout << "Missing argument: \'" << flag << "\'" << std::endl << std::endl 
									<< sono_loader::display_section(SONOCTL_SECTION_USAGE) << std::endl << std::endl 
									<< sono_loader::display_section(SONOCTL_SECTION_OPTIONS) << std::endl;
								goto complete;
							}

							sono_device &dev = instance->device(addr, port, true);
							if(!dev.size()) {
								dev.service_discovery(service_timeout);
							}

							++iter;
							if(iter == m_arguments.end()) {
								std::cout << "Missing argument: \'" << flag << "\'" << std::endl << std::endl 
									<< sono_loader::display_section(SONOCTL_SECTION_USAGE) << std::endl << std::endl 
									<< sono_loader::display_section(SONOCTL_SECTION_OPTIONS) << std::endl;
								goto complete;
							}

							svc = *iter;
							if(svc.empty()) {
								std::cout << "Missing argument: \'" << flag << "\'" << std::endl << std::endl 
									<< sono_loader::display_section(SONOCTL_SECTION_USAGE) << std::endl << std::endl 
									<< sono_loader::display_section(SONOCTL_SECTION_OPTIONS) << std::endl;
								goto complete;
							}

							sono_service &serv = dev.service(svc);
							list_action = serv.action_list();
							std::cout << "Found " << list_action.size() << " actions(s) at " << addr << ":" 
								<< port << ":" << svc << std::endl << "---";

							for(count = 0, iter_action = list_action.begin(); iter_action != list_action.end(); 
									++count, ++iter_action) {
								std::cout << std::endl << "[" << count << "] " << *iter_action << " {";

								args = serv.action(*iter_action).input();
								for(iter_args = args.begin(); iter_args != args.end(); ++iter_args) {

									if(iter_args != args.begin()) {
										std::cout << ", ";
									}

									std::cout << *iter_args;
								}

								std::cout << "}";
							}

							std::cout << std::endl;
						} goto complete;
					case SONOCTL_ARGUMENT_LIST_SERVICES: {

							++iter;
							if(iter == m_arguments.end()) {
								std::cout << "Missing argument: \'" << flag << "\'" << std::endl << std::endl 
									<< sono_loader::display_section(SONOCTL_SECTION_USAGE) << std::endl << std::endl 
									<< sono_loader::display_section(SONOCTL_SECTION_OPTIONS) << std::endl;
								goto complete;
							}

							addr = *iter;

							std::regex_search(addr.c_str(), match, std::regex(SONOCTL_ARGUMENT_DEVICE_ADDRESS_REGEX));
							if(match.size() != (SONOCTL_ARGUMENT_DEVICE_MAX + 1)) {
								std::cout << "Missing argument: \'" << flag << "\'" << std::endl << std::endl 
									<< sono_loader::display_section(SONOCTL_SECTION_USAGE) << std::endl << std::endl 
									<< sono_loader::display_section(SONOCTL_SECTION_OPTIONS) << std::endl;
								goto complete;
							}

							stream << match[SONOCTL_ARGUMENT_DEVICE_RIGHT];
							stream >> port;
							addr = match[SONOCTL_ARGUMENT_DEVICE_LEFT];

							if(addr.empty()) {
								std::cout << "Missing argument: \'" << flag << "\'" << std::endl << std::endl 
									<< sono_loader::display_section(SONOCTL_SECTION_USAGE) << std::endl << std::endl 
									<< sono_loader::display_section(SONOCTL_SECTION_OPTIONS) << std::endl;
								goto complete;
							}

							if(addr.empty()) {
								std::cout << "Missing argument: \'" << flag << "\'" << std::endl << std::endl 
									<< sono_loader::display_section(SONOCTL_SECTION_USAGE) << std::endl << std::endl 
									<< sono_loader::display_section(SONOCTL_SECTION_OPTIONS) << std::endl;
								goto complete;
							}

							sono_device &dev = instance->device(addr, port, true);
							if(!dev.size()) {
								list_service = dev.service_discovery(service_timeout);
							} else {
								list_service = dev.service_list();
							}

							std::cout << "Found " << list_service.size() << " services(s) at " << addr << ":" 
								<< port << std::endl << "---";

							for(count = 0, iter_service = list_service.begin(); iter_service != list_service.end(); 
									++count, ++iter_service) {
								std::cout << std::endl << "[" << count << "] " << *iter_service;
							}

							std::cout << std::endl;
						} goto complete;
					case SONOCTL_ARGUMENT_SERVICE:

						++iter;
						if(iter == m_arguments.end()) {
							std::cout << "Missing argument: \'" << flag << "\'" << std::endl << std::endl 
								<< sono_loader::display_section(SONOCTL_SECTION_USAGE) << std::endl << std::endl 
								<< sono_loader::display_section(SONOCTL_SECTION_OPTIONS) << std::endl;
							goto complete;
						}

						svc = *iter;
						break;
					case SONOCTL_ARGUMENT_VERBOSE:
						break;
					case SONOCTL_ARGUMENT_VERSION:
						std::cout << sono_loader::display_section(SONOCTL_SECTION_VERSION) << std::endl;
						goto complete;
					default:
						std::cout << "Unknown argument: \'" << flag << "\'" << std::endl << std::endl 
							<< sono_loader::display_section(SONOCTL_SECTION_USAGE) << std::endl << std::endl 
							<< sono_loader::display_section(SONOCTL_SECTION_OPTIONS) << std::endl;
						goto complete;
				}

				offset = (1 << type);
				if(mask & offset) {
					std::cout << "Duplicate argument: \'" << flag << "\'" << std::endl << std::endl 
						<< sono_loader::display_section(SONOCTL_SECTION_USAGE) << std::endl << std::endl 
						<< sono_loader::display_section(SONOCTL_SECTION_OPTIONS) << std::endl;
					break;
				}

				mask |= offset;

				if(iter == m_arguments.end()) {
					break;
				}
			}

			if(!(mask & (1 << SONOCTL_ARGUMENT_ACTION))) {
				std::cout << "Missing argument: \'" << SONOCTL_ARGUMENT_SHORT_STRING(SONOCTL_ARGUMENT_ACTION) << "\'" 
					<< std::endl << std::endl << sono_loader::display_section(SONOCTL_SECTION_USAGE) << std::endl 
					<< std::endl << sono_loader::display_section(SONOCTL_SECTION_OPTIONS) << std::endl;
				goto complete;
			}

			if(!(mask & (1 << SONOCTL_ARGUMENT_DEVICE))) {
				std::cout << "Missing argument: \'" << SONOCTL_ARGUMENT_SHORT_STRING(SONOCTL_ARGUMENT_DEVICE) << "\'" 
					<< std::endl << std::endl << sono_loader::display_section(SONOCTL_SECTION_USAGE) << std::endl 
					<< std::endl << sono_loader::display_section(SONOCTL_SECTION_OPTIONS) << std::endl;
				goto complete;
			}

			if(!(mask & (1 << SONOCTL_ARGUMENT_SERVICE))) {
				std::cout << "Missing argument: \'" << SONOCTL_ARGUMENT_SHORT_STRING(SONOCTL_ARGUMENT_SERVICE) << "\'" 
					<< std::endl << std::endl << sono_loader::display_section(SONOCTL_SECTION_USAGE) << std::endl 
					<< std::endl << sono_loader::display_section(SONOCTL_SECTION_OPTIONS) << std::endl;
				goto complete;
			}

			sono_device &dev = instance->device(addr, port, true);
			if(!dev.size()) {
				dev.service_discovery(service_timeout);
			}

			m_results = dev.service(svc).run(act, input, action_timeout);

			if(mask & (1 << SONOCTL_ARGUMENT_VERBOSE)) {
				std::cout << "Result: " << m_results.size() << " entries(s)." << std::endl << "---";

				for(count = 0, iter_arg = m_results.begin(); iter_arg != m_results.end(); ++count, ++iter_arg) {
					std::cout << std::endl << "[" << count << "] " << iter_arg->first << ": " << iter_arg->second;
				}

				std::cout << std::endl;
			}
		} else {
			std::cout << sono_loader::display_section(SONOCTL_SECTION_USAGE) << std::endl << std::endl 
				<< sono_loader::display_section(SONOCTL_SECTION_OPTIONS) << std::endl;
		}

complete:
		return m_results;
	}

	size_t 
	_sono_loader::size(void)
	{

		if(!m_initialized) {
			THROW_SONO_LOADER_EXCEPTION(SONO_LOADER_EXCEPTION_UNINITIALIZED);
		}

		return m_results.size();
	}

	std::string 
	_sono_loader::to_string(
		__in_opt bool verbose
		)
	{
		size_t count;
		std::stringstream result;
		sono_argument_list::iterator iter_arg;
		sono_action_argument::iterator iter_res;

		UNREF_PARAM(verbose);

		result << SONO_LOADER_HEADER << " -- " << (m_initialized ? "INIT" : "UNINIT");

		if(m_initialized) {
			result << ", PTR. 0x" << SCALAR_AS_HEX(sono_loader *, this) << ", ARGS. " << m_arguments.size() 
				<< ", RES. " << size();

			for(count = 0, iter_arg = m_arguments.begin(); iter_arg != m_arguments.end(); 
					++count, ++iter_arg) {
				result << std::endl << "--- ARG[" << count << "] " << *iter_arg;
			}

			for(count = 0, iter_res = m_results.begin(); iter_res != m_results.end(); 
					++count, ++iter_res) {
				result << std::endl << "--- RES[" << count << "] " << STRING_CHECK(iter_res->first) 
					<< ": " << STRING_CHECK(iter_res->second);
			}
		}

		return result.str();
	}

	void 
	_sono_loader::uninitialize(void)
	{
		sono_manager *instance = NULL;

		if(!m_initialized) {
			THROW_SONO_LOADER_EXCEPTION(SONO_LOADER_EXCEPTION_UNINITIALIZED);
		}

		if(sono_manager::is_allocated()) {

			instance = sono_manager::acquire();
			if(instance && instance->is_initialized()) {
				instance->uninitialize();
			}
		}
		m_arguments.clear();
		m_results.clear();
		m_initialized = false;
	}
}
