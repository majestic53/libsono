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

#ifndef SONO_UID_H_
#define SONO_UID_H_

namespace SONO {

	namespace COMP {

		#define SONO_UID_INVALID SCALAR_INVALID(sono_uid_t)

		typedef uint32_t sono_uid_t;

		typedef class _sono_uid_factory {

			public:

				~_sono_uid_factory(void);

				static _sono_uid_factory *acquire(void);

				size_t capacity(void);

				bool contains(
					__in sono_uid_t id
					);

				size_t decrement_reference(
					__in sono_uid_t id
					);

				sono_uid_t generate(void);

				size_t increment_reference(
					__in sono_uid_t id
					);

				void initialize(void);

				static bool is_allocated(void);

				bool is_initialized(void);

				size_t reference_count(
					__in sono_uid_t id
					);

				size_t size(void);

				std::string to_string(
					__in_opt bool verbose = false
					);

				void uninitialize(void);

			protected:

				_sono_uid_factory(void);

				_sono_uid_factory(
					__in const _sono_uid_factory &other
					);

				_sono_uid_factory &operator=(
					__in const _sono_uid_factory &other
					);

				static void _delete(void);

				std::map<sono_uid_t, size_t>::iterator find(
					__in sono_uid_t id
					);

				bool m_initialized;

				static _sono_uid_factory *m_instance;

				std::map<sono_uid_t, size_t> m_map;

				sono_uid_t m_next;

				std::set<sono_uid_t> m_surplus;

		} sono_uid_factory;

		typedef class _sono_uid_base {

			public:

				_sono_uid_base(void);

				_sono_uid_base(
					__in sono_uid_t id
					);

				_sono_uid_base(
					__in const _sono_uid_base &other
					);

				virtual ~_sono_uid_base(void);

				_sono_uid_base &operator=(
					__in const _sono_uid_base &other
					);

				sono_uid_t id(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				void decrement_reference(void);

				void generate(void);

				void increment_reference(void);

				sono_uid_t m_id;

		} sono_uid_base;
	}
}

#endif // SONO_UID_H_
