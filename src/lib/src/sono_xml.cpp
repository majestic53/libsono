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

#include "../include/sono_manager.h"
#include "../include/sono_xml_type.h"

namespace SONO {

	namespace COMP {

		_sono_xml::_sono_xml(
			__in const std::string &path
			) :
				m_path(path)
		{
			return;
		}

		_sono_xml::_sono_xml(
			__in const _sono_xml &other
			) :
				m_path(other.m_path),
				m_source(other.m_source)
		{
			return;
		}

		_sono_xml::~_sono_xml(void)
		{
			return;
		}

		_sono_xml &
		_sono_xml::operator=(
			__in const _sono_xml &other
			)
		{

			if(this != &other) {
				m_path = other.m_path;
				m_source = other.m_source;
			}

			return *this;
		}

		std::vector<boost::property_tree::ptree::value_type> 
		_sono_xml::as_tree(
			__in boost::property_tree::ptree &root,
			__in_opt const std::string &child
			)
		{
			std::stringstream stream;
			boost::property_tree::ptree::const_iterator iter;
			std::vector<boost::property_tree::ptree::value_type> result;

			try {
				stream << m_source;
				boost::property_tree::read_xml(stream, root);

				if(!child.empty()) {

					boost::property_tree::ptree const &children = root.get_child(child.c_str());
					for(iter = children.begin(); iter != children.end(); ++iter) {
						result.push_back(*iter);
					}
				}
			} catch(boost::property_tree::xml_parser_error &exc) {
				THROW_SONO_XML_EXCEPTION_FORMAT(SONO_XML_EXCEPTION_INTERNAL,
					"boost::property_tree::read_xml failed: %s", exc.what());
			} catch(std::exception &exc) {
				THROW_SONO_XML_EXCEPTION_FORMAT(SONO_XML_EXCEPTION_INTERNAL,
					"boost::property_tree::read_xml failed: %s", exc.what());
			}

			return result;
		}

		std::string 
		_sono_xml::path(void)
		{
			return m_path;
		}

		void 
		_sono_xml::set(
			__in const std::string &source
			)
		{
			m_source = source;
		}

		std::string 
		_sono_xml::source(void)
		{
			return m_source;
		}

		std::string 
		_sono_xml::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			result << SONO_XML_HEADER << " (PATH. " << STRING_CHECK(m_path) << ", LEN. " 
				<< m_source.size() << ")";

			if(verbose) {
				result << std::endl << "---" << std::endl << STRING_CHECK(m_source) 
					<< std::endl << "---" << std::endl;
			}

			return result.str();
		}
	}
}
