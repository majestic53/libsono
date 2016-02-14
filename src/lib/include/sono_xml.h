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

#ifndef SONO_XML_H_
#define SONO_XML_H_

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace SONO {

	namespace COMP {

		typedef class _sono_xml {

			public:

				_sono_xml(
					__in const std::string &path
					);

				_sono_xml(
					__in const _sono_xml &other
					);

				virtual ~_sono_xml(void);

				_sono_xml &operator=(
					__in const _sono_xml &other
					);

				std::vector<boost::property_tree::ptree::value_type> as_tree(
					__in boost::property_tree::ptree &root,
					__in_opt const std::string &child = std::string()
					);

				std::string path(void);

				void set(
					__in const std::string &source
					);

				std::string source(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				std::string m_path;

				std::string m_source;

		} sono_xml;
	}
}

#endif // SONO_XML_H_
