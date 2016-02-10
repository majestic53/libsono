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
#include "../include/sono_uid_type.h"

namespace SONO {

	namespace COMP {

		_sono_uid_factory *_sono_uid_factory::m_instance = NULL;

		_sono_uid_factory::_sono_uid_factory(void) :
			m_initialized(false),
			m_next(SONO_UID_INVALID)
		{
			std::atexit(sono_uid_factory::_delete);
		}

		_sono_uid_factory::~_sono_uid_factory(void)
		{

			if(m_initialized) {
				uninitialize();
			}
		}

		void 
		_sono_uid_factory::_delete(void)
		{

			if(sono_uid_factory::m_instance) {
				delete sono_uid_factory::m_instance;
				sono_uid_factory::m_instance = NULL;
			}
		}

		_sono_uid_factory *
		_sono_uid_factory::acquire(void)
		{

			if(!sono_uid_factory::m_instance) {

				sono_uid_factory::m_instance = new sono_uid_factory;
				if(!sono_uid_factory::m_instance) {
					THROW_SONO_UID_EXCEPTION(SONO_UID_EXCEPTION_ALLOCATION);
				}
			}

			return sono_uid_factory::m_instance;
		}

		size_t 
		_sono_uid_factory::capacity(void)
		{

			if(!m_initialized) {
				THROW_SONO_UID_EXCEPTION(SONO_UID_EXCEPTION_UNINITIALIZED);
			}

			return (UINT32_MAX - m_map.size() - 1);
		}

		bool 
		_sono_uid_factory::contains(
			__in sono_uid_t id
			)
		{

			if(!m_initialized) {
				THROW_SONO_UID_EXCEPTION(SONO_UID_EXCEPTION_UNINITIALIZED);
			}

			return (m_map.find(id) != m_map.end());
		}

		size_t 
		_sono_uid_factory::decrement_reference(
			__in sono_uid_t id
			)
		{
			size_t result;
			std::map<sono_uid_t, size_t>::iterator iter;

			if(!m_initialized) {
				THROW_SONO_UID_EXCEPTION(SONO_UID_EXCEPTION_UNINITIALIZED);
			}

			iter = find(id);

			result = --iter->second;
			if(result < REF_INIT) {
				m_surplus.insert(iter->first);
				m_map.erase(iter);
			}

			return result;
		}

		std::map<sono_uid_t, size_t>::iterator 
		_sono_uid_factory::find(
			__in sono_uid_t id
			)
		{
			std::map<sono_uid_t, size_t>::iterator result;

			if(!m_initialized) {
				THROW_SONO_UID_EXCEPTION(SONO_UID_EXCEPTION_UNINITIALIZED);
			}

			result = m_map.find(id);
			if(result == m_map.end()) {
				THROW_SONO_UID_EXCEPTION_FORMAT(SONO_UID_EXCEPTION_NOT_FOUND, "{%x}", id);
			}

			return result;
		}

		sono_uid_t 
		_sono_uid_factory::generate(void)
		{
			std::set<sono_uid_t>::iterator iter;
			sono_uid_t result = SONO_UID_INVALID;

			if(!m_initialized) {
				THROW_SONO_UID_EXCEPTION(SONO_UID_EXCEPTION_UNINITIALIZED);
			}

			if(!m_surplus.empty()) {
				iter = m_surplus.begin();
				result = *iter;
				m_surplus.erase(iter);
			} else if(m_next < SONO_UID_INVALID) {
				result = m_next++;
			} else {
				THROW_SONO_UID_EXCEPTION(SONO_UID_EXCEPTION_FULL);
			}

			m_map.insert(std::pair<sono_uid_t, size_t>(result, REF_INIT));

			return result;
		}

		size_t 
		_sono_uid_factory::increment_reference(
			__in sono_uid_t id
			)
		{

			if(!m_initialized) {
				THROW_SONO_UID_EXCEPTION(SONO_UID_EXCEPTION_UNINITIALIZED);
			}

			return ++find(id)->second;
		}

		void 
		_sono_uid_factory::initialize(void)
		{

			if(m_initialized) {
				THROW_SONO_UID_EXCEPTION(SONO_UID_EXCEPTION_INITIALIZED);
			}

			m_map.clear();
			m_next = 0;
			m_surplus.clear();
			m_initialized = true;
		}

		bool 
		_sono_uid_factory::is_allocated(void)
		{
			return (sono_uid_factory::m_instance != NULL);
		}

		bool 
		_sono_uid_factory::is_initialized(void)
		{
			return m_initialized;
		}

		size_t 
		_sono_uid_factory::reference_count(
			__in sono_uid_t id
			)
		{

			if(!m_initialized) {
				THROW_SONO_UID_EXCEPTION(SONO_UID_EXCEPTION_UNINITIALIZED);
			}

			return find(id)->second;
		}

		size_t 
		_sono_uid_factory::size(void)
		{

			if(!m_initialized) {
				THROW_SONO_UID_EXCEPTION(SONO_UID_EXCEPTION_UNINITIALIZED);
			}

			return m_map.size();
		}

		std::string 
		_sono_uid_factory::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;
			std::map<sono_uid_t, size_t>::iterator iter;

			result << SONO_UID_HEADER << " -- " << (m_initialized ? "INIT" : "UNINIT");

			if(m_initialized) {
				result << ", PTR. 0x" << SCALAR_AS_HEX(sono_uid_factory *, this) << ", CAP. " << capacity()
				<< ", SZ. " << size();

				for(iter = m_map.begin(); iter != m_map.end(); ++iter) {
					result << std::endl << "--- {" << SCALAR_AS_HEX(sono_uid_t, iter->first) << "}";

					if(verbose) {
						result << ", REF. " << iter->second;
					}
				}
			}

			return result.str();
		}

		void 
		_sono_uid_factory::uninitialize(void)
		{

			if(!m_initialized) {
				THROW_SONO_UID_EXCEPTION(SONO_UID_EXCEPTION_UNINITIALIZED);
			}

			m_map.clear();
			m_next = 0;
			m_surplus.clear();
			m_initialized = false;
		}

		_sono_uid_base::_sono_uid_base(void) :
			m_id(SONO_UID_INVALID)
		{
			generate();
		}

		_sono_uid_base::_sono_uid_base(
			__in sono_uid_t id
			) :
				m_id(id)
		{
			increment_reference();
		}

		_sono_uid_base::_sono_uid_base(
			__in const _sono_uid_base &other
			) :
				m_id(other.m_id)
		{
			increment_reference();
		}

		_sono_uid_base::~_sono_uid_base(void)
		{
			decrement_reference();
		}

		_sono_uid_base &
		_sono_uid_base::operator=(
			__in const _sono_uid_base &other
			)
		{

			if(this != &other) {
				decrement_reference();
				m_id = other.m_id;
				increment_reference();
			}

			return *this;
		}

		void 
		_sono_uid_base::decrement_reference(void)
		{
			sono_uid_factory *instance = NULL;

			if(sono_uid_factory::is_allocated()) {

				instance = sono_uid_factory::acquire();
				if(instance->is_initialized()
						&& instance->contains(m_id)) {
					instance->decrement_reference(m_id);
				}
			}
		}

		void 
		_sono_uid_base::generate(void)
		{
			sono_uid_factory *instance = NULL;

			if(sono_uid_factory::is_allocated()) {

				instance = sono_uid_factory::acquire();
				if(instance->is_initialized()) {
					m_id = instance->generate();
				}
			}
		}

		sono_uid_t 
		_sono_uid_base::id(void)
		{
			return m_id;
		}

		void 
		_sono_uid_base::increment_reference(void)
		{
			sono_uid_factory *instance = NULL;

			if(sono_uid_factory::is_allocated()) {

				instance = sono_uid_factory::acquire();
				if(instance->is_initialized()
						&& instance->contains(m_id)) {
					instance->increment_reference(m_id);
				}
			}
		}

		std::string 
		_sono_uid_base::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			UNREF_PARAM(verbose);

			result << "{" << SCALAR_AS_HEX(sono_uid_t, m_id) << "}";

			return result.str();
		}
	}
}
