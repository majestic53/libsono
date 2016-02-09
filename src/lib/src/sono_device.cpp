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
#include "../include/sono_device_type.h"

namespace SONO {

	namespace COMP {

		_sono_device::_sono_device(
			__in const std::string &address,
			__in uint16_t port
			) :
				m_address(address),
				m_port(port)
		{
			return;
		}

		_sono_device::_sono_device(
			__in const _sono_device &other
			) :
				sono_uid_base(other),
				m_address(other.m_address),
				m_port(other.m_port)
		{
			return;
		}

		_sono_device::~_sono_device(void)
		{
			return;
		}

		_sono_device &
		_sono_device::operator=(
			__in const _sono_device &other
			)
		{

			if(this != &other) {
				sono_uid_base::operator=(other);
				m_address = other.m_address;
				m_port = other.m_port;
			}

			return *this;
		}

		std::string 
		_sono_device::address(void)
		{
			return m_address;
		}

		uint16_t 
		_sono_device::port(void)
		{
			return m_port;
		}

		std::string 
		_sono_device::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			if(verbose) {
				result << sono_uid_base::to_string() << " ";
			}

			result << STRING_CHECK(m_address) << ":" << m_port;

			return result.str();
		}

		_sono_device_factory *_sono_device_factory::m_instance = NULL;

		_sono_device_factory::_sono_device_factory(void) :
			m_initialized(false)
		{
			std::atexit(sono_device_factory::_delete);
		}

		_sono_device_factory::~_sono_device_factory(void)
		{

			if(m_initialized) {
				uninitialize();
			}
		}

		void 
		_sono_device_factory::_delete(void)
		{

			if(sono_device_factory::m_instance) {
				delete sono_device_factory::m_instance;
				sono_device_factory::m_instance = NULL;
			}
		}

		_sono_device_factory *
		_sono_device_factory::acquire(void)
		{

			if(!sono_device_factory::m_instance) {

				sono_device_factory::m_instance = new sono_device_factory;
				if(!sono_device_factory::m_instance) {
					THROW_SONO_DEVICE_EXCEPTION(SONO_DEVICE_EXCEPTION_ALLOCATION);
				}
			}

			return sono_device_factory::m_instance;
		}

		sono_device &
		_sono_device_factory::at(
			__in sono_uid_t id
			)
		{

			if(!m_initialized) {
				THROW_SONO_DEVICE_EXCEPTION(SONO_DEVICE_EXCEPTION_UNINITIALIZED);
			}

			return find(id)->second.first;
		}

		bool 
		_sono_device_factory::contains(
			__in sono_uid_t id
			)
		{

			if(!m_initialized) {
				THROW_SONO_DEVICE_EXCEPTION(SONO_DEVICE_EXCEPTION_UNINITIALIZED);
			}

			return (m_map.find(id) != m_map.end());
		}

		size_t 
		_sono_device_factory::decrement_reference(
			__in sono_uid_t id
			)
		{
			size_t result;
			std::map<sono_uid_t, std::pair<sono_device, size_t>>::iterator iter;

			if(!m_initialized) {
				THROW_SONO_DEVICE_EXCEPTION(SONO_DEVICE_EXCEPTION_UNINITIALIZED);
			}

			iter = find(id);

			result = --iter->second.second;
			if(result < REF_INIT) {
				m_map.erase(iter);
			}

			return result;
		}

		std::map<sono_uid_t, std::pair<sono_device, size_t>>::iterator 
		_sono_device_factory::find(
			__in sono_uid_t id
			)
		{
			std::map<sono_uid_t, std::pair<sono_device, size_t>>::iterator result;

			if(!m_initialized) {
				THROW_SONO_DEVICE_EXCEPTION(SONO_DEVICE_EXCEPTION_UNINITIALIZED);
			}

			result = m_map.find(id);
			if(result == m_map.end()) {
				THROW_SONO_DEVICE_EXCEPTION_FORMAT(SONO_DEVICE_EXCEPTION_NOT_FOUND, "{%x}", id);
			}

			return result;
		}

		size_t 
		_sono_device_factory::increment_reference(
			__in sono_uid_t id
			)
		{

			if(!m_initialized) {
				THROW_SONO_DEVICE_EXCEPTION(SONO_DEVICE_EXCEPTION_UNINITIALIZED);
			}

			return ++find(id)->second.second;
		}

		void 
		_sono_device_factory::initialize(void)
		{

			if(m_initialized) {
				THROW_SONO_DEVICE_EXCEPTION(SONO_DEVICE_EXCEPTION_INITIALIZED);
			}

			m_map.clear();
			m_initialized = true;
		}

		bool 
		_sono_device_factory::is_allocated(void)
		{
			return (sono_device_factory::m_instance != NULL);
		}

		bool 
		_sono_device_factory::is_initialized(void)
		{
			return m_initialized;
		}

		size_t 
		_sono_device_factory::reference_count(
			__in sono_uid_t id
			)
		{

			if(!m_initialized) {
				THROW_SONO_DEVICE_EXCEPTION(SONO_DEVICE_EXCEPTION_UNINITIALIZED);
			}

			return find(id)->second.second;
		}

		size_t 
		_sono_device_factory::size(void)
		{

			if(!m_initialized) {
				THROW_SONO_DEVICE_EXCEPTION(SONO_DEVICE_EXCEPTION_UNINITIALIZED);
			}

			return m_map.size();
		}

		std::string 
		_sono_device_factory::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;
			std::map<sono_uid_t, std::pair<sono_device, size_t>>::iterator iter;

			result << SONO_DEVICE_HEADER << " -- " << (m_initialized ? "INIT" : "UNINIT")
				<< ", PTR. 0x" << SCALAR_AS_HEX(sono_device_factory *, this) << ", SZ. " << size();

			if(verbose) {

				for(iter = m_map.begin(); iter != m_map.end(); ++iter) {
					result << std::endl << "--- " << iter->second.first.to_string(true) 
					<< ", REF. " << iter->second.second;
				}
			}

			return result.str();
		}

		void 
		_sono_device_factory::uninitialize(void)
		{

			if(!m_initialized) {
				THROW_SONO_DEVICE_EXCEPTION(SONO_DEVICE_EXCEPTION_UNINITIALIZED);
			}

			m_map.clear();
			m_initialized = false;
		}
	}
}
