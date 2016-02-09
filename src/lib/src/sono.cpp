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

#include "../include/sono.h"
#include "../include/sono_type.h"

namespace SONO {

	sono_manager *sono_manager::m_instance = NULL;

	_sono_manager::_sono_manager(void) :
		m_factory_device(sono_device_factory::acquire()),
		m_factory_uid(sono_uid_factory::acquire()),
		m_initialized(false)
	{
		std::atexit(sono_manager::_delete);
	}

	_sono_manager::~_sono_manager(void)
	{

		if(m_initialized) {
			uninitialize();
		}
	}

	void 
	_sono_manager::_delete(void)
	{

		if(sono_manager::m_instance) {
			delete sono_manager::m_instance;
			sono_manager::m_instance = NULL;
		}
	}

	_sono_manager *
	_sono_manager::acquire(void)
	{

		if(!sono_manager::m_instance) {

			sono_manager::m_instance = new sono_manager;
			if(!sono_manager::m_instance) {
				THROW_SONO_EXCEPTION(SONO_EXCEPTION_ALLOCATION);
			}
		}

		return sono_manager::m_instance;
	}

	sono_device_factory *
	_sono_manager::device(void)
	{

		if(!m_initialized) {
			THROW_SONO_EXCEPTION(SONO_EXCEPTION_UNINITIALIZED);
		}

		return m_factory_device;
	}

	void 
	_sono_manager::initialize(void)
	{

		if(m_initialized) {
			THROW_SONO_EXCEPTION(SONO_EXCEPTION_INITIALIZED);
		}

		m_factory_uid->initialize();
		m_factory_device->initialize();
		m_initialized = true;
	}

	bool 
	_sono_manager::is_allocated(void)
	{
		return (sono_manager::m_instance != NULL);
	}

	bool 
	_sono_manager::is_initialized(void)
	{
		return m_initialized;
	}

	std::string 
	_sono_manager::to_string(
		__in_opt bool verbose
		)
	{
		std::stringstream result;

		UNREF_PARAM(verbose);

		result << SONO_HEADER << " -- " << (m_initialized ? "INIT" : "UNINIT")
			<< ", PTR. 0x" << SCALAR_AS_HEX(sono_manager *, this);

		return result.str();
	}

	sono_uid_factory *
	_sono_manager::uid(void)
	{

		if(!m_initialized) {
			THROW_SONO_EXCEPTION(SONO_EXCEPTION_UNINITIALIZED);
		}

		return m_factory_uid;
	}

	void 
	_sono_manager::uninitialize(void)
	{

		if(!m_initialized) {
			THROW_SONO_EXCEPTION(SONO_EXCEPTION_UNINITIALIZED);
		}

		m_factory_device->uninitialize();
		m_factory_uid->uninitialize();
		m_initialized = false;
	}

	std::string 
	_sono_manager::version(void)
	{
		return VERSION_STRING;
	}
}
