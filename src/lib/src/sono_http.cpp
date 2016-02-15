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

#include <cstring>
#include <regex>
#include "../include/sono.h"
#include "../include/sono_http_type.h"

namespace SONO {

	namespace COMP {

		#define SONO_HTTP_ENCODING_CHUNKED "chunked"
		#define SONO_HTTP_GET_PREFIX "GET "
		#define SONO_HTTP_GET_SUFFIX " HTTP/1.1" SONO_HTTP_TERM_HEAD
		#define SONO_HTTP_POST_CONTENT_LENGTH_PREFIX "Content-Length: "
		#define SONO_HTTP_POST_CONTENT_LENGTH_SUFFIX SONO_HTTP_TERM
		#define SONO_HTTP_POST_CONTENT_TYPE "Content-Type: text/xml; charset=\"utf-8\"" SONO_HTTP_TERM
		#define SONO_HTTP_POST_PREFIX "POST "
		#define SONO_HTTP_POST_SUFFIX " HTTP/1.1" SONO_HTTP_TERM
		#define SONO_HTTP_REGEX_ENCODING "Transfer-Encoding: (.*)\r\n"
		#define SONO_HTTP_REGEX_STATUS "HTTP/1.1 ([0-9]+) .*\r\n"
		#define SONO_HTTP_TERM "\r\n"
		#define SONO_HTTP_TERM_LEN std::strlen(SONO_HTTP_TERM)
		#define SONO_HTTP_TERM_HEAD SONO_HTTP_TERM SONO_HTTP_TERM
		#define SONO_HTTP_TERM_HEAD_LEN std::strlen(SONO_HTTP_TERM_HEAD)

		enum {
			SONO_HTTP_REGEX_ENCODING_ROOT = 0,
			SONO_HTTP_REGEX_ENCODING_TYPE,
		};

		#define SONO_HTTP_REGEX_ENCODING_MAX SONO_HTTP_REGEX_ENCODING_TYPE

		enum {
			SONO_HTTP_REGEX_STATUS_ROOT = 0,
			SONO_HTTP_REGEX_STATUS_VALUE,
		};

		#define SONO_HTTP_REGEX_STATUS_MAX SONO_HTTP_REGEX_STATUS_VALUE

		std::string 
		_sono_http::get(
			__in const std::string &path,
			__in const std::string &address,
			__in uint16_t port,
			__in_opt sono_socket_t type,
			__in_opt uint32_t timeout
			)
		{
			int length;
			std::stringstream request;
			std::string fragment, result;

			sono_socket sock(type, address, port);
			request << SONO_HTTP_GET_PREFIX << path << SONO_HTTP_GET_SUFFIX;
			sock.connect(timeout);

			if(sock.write(STRING_CHECK(request.str())) == SONO_SOCKET_INVALID) {
				THROW_SONO_HTTP_EXCEPTION_FORMAT(SONO_HTTP_EXCEPTION_GET,
					"%s:%u --> %s", STRING_CHECK(address), port, STRING_CHECK(path));
			}

			for(;;) {

				length = sock.read(fragment);
				if(length <= 0) {
					break;
				}

				result.insert(result.end(), fragment.begin(), fragment.begin() + length);
			}

			sock.disconnect();

			return result;
		}

		std::string 
		_sono_http::parse_body(
			__in const std::string &source,
			__in sono_http_encode_t encoding
			)
		{
			bool mode = true;
			std::stringstream result;
			size_t current = 0, delta, next;

			switch(encoding) {
				case SONO_HTTP_ENCODE_NONE:
					result << source;
					break;
				case SONO_HTTP_ENCODE_CHUNKED:

					for(;;) {

						if(current >= source.size()) {
							break;
						}

						next = source.find(SONO_HTTP_TERM, current);
						if(next == std::string::npos) {
							break;
						}

						if(!mode) {
							result << source.substr(current, next - current);
						} else {
							delta = sono_http::parse_body_length(source.substr(current, next - current));
						}

						current = next + SONO_HTTP_TERM_LEN;
						mode = !mode;
					}
					break;
				default:
					THROW_SONO_HTTP_EXCEPTION_FORMAT(SONO_HTTP_EXCEPTION_UNSUPPORTED,
						"0x%x", encoding);
			}

			return result.str();
		}

		uint32_t 
		_sono_http::parse_body_length(
			__in const std::string &source
			)
		{
			uint32_t result = 0;
			std::stringstream stream;

			if(!source.empty()) {
				stream << source;
				stream >> result;
			}

			return result;
		}

		int 
		_sono_http::parse_header(
			__in const std::string &source
			)
		{
			sono_http_encode_t encoding;

			return sono_http::parse_header(source, encoding);
		}

		int 
		_sono_http::parse_header(
			__in const std::string &source,
			__out sono_http_encode_t &encoded
			)
		{
			size_t position;
			std::string entry, header;
			int result = SONO_HTTP_SUCCESS;
			std::match_results<const char *> match;

			encoded = SONO_HTTP_ENCODE_NONE;

			if(!source.empty()) {

				position = source.find(SONO_HTTP_TERM_HEAD);
				if(position == std::string::npos) {
					THROW_SONO_HTTP_EXCEPTION(SONO_HTTP_EXCEPTION_MALFORMED);
				}

				try {
					header = source.substr(0, position);
				} catch(std::exception &exc) {
					THROW_SONO_HTTP_EXCEPTION_FORMAT(SONO_HTTP_EXCEPTION_MALFORMED,
						"%s", exc.what());
				}

				std::regex_search(header.c_str(), match, std::regex(SONO_HTTP_REGEX_STATUS));
				if(match.size() != (SONO_HTTP_REGEX_STATUS_MAX + 1)) {
					THROW_SONO_HTTP_EXCEPTION_FORMAT(SONO_HTTP_EXCEPTION_MALFORMED,
						"std::regex_search(status) found %lu entries (should be %lu)", match.size(),
						SONO_HTTP_REGEX_STATUS_MAX + 1);
				}

				entry = match[SONO_HTTP_REGEX_STATUS_VALUE];

				result = std::atoi(entry.c_str());
				if(result == SONO_HTTP_SUCCESS) {

					std::regex_search(header.c_str(), match, std::regex(SONO_HTTP_REGEX_ENCODING));

					if(match.size()) {

						if(match.size() != (SONO_HTTP_REGEX_ENCODING_MAX + 1)) {
							THROW_SONO_HTTP_EXCEPTION_FORMAT(SONO_HTTP_EXCEPTION_MALFORMED,
								"std::regex_search(encoding) found %lu entries (should be %lu)", match.size(),
								SONO_HTTP_REGEX_ENCODING_MAX + 1);
						}

						encoded = (match[SONO_HTTP_REGEX_ENCODING_TYPE] == SONO_HTTP_ENCODING_CHUNKED) ? 
							SONO_HTTP_ENCODE_CHUNKED : SONO_HTTP_ENCODE_NONE;
					}
				}
			}

			return result;
		}

		std::string 
		_sono_http::post(
			__in const std::string &path,
			__in const std::string &address,
			__in uint16_t port,
			__in const std::string &head,
			__in const std::string &body,
			__in_opt sono_socket_t type,
			__in_opt uint32_t timeout
			)
		{
			int length;
			std::stringstream request;
			std::string fragment, result;

			sono_socket sock(type, address, port);
			request << SONO_HTTP_POST_PREFIX << path << SONO_HTTP_POST_SUFFIX
				<< SONO_HTTP_POST_CONTENT_TYPE << SONO_HTTP_POST_CONTENT_LENGTH_PREFIX << body.size() 
				<< SONO_HTTP_POST_CONTENT_LENGTH_SUFFIX << head << SONO_HTTP_TERM_HEAD << body 
				<< SONO_HTTP_TERM;

			sock.connect(timeout);

			if(sock.write(STRING_CHECK(request.str())) == SONO_SOCKET_INVALID) {
				THROW_SONO_HTTP_EXCEPTION_FORMAT(SONO_HTTP_EXCEPTION_POST,
					"%s:%u --> %s", STRING_CHECK(address), port, STRING_CHECK(path));
			}

			for(;;) {

				length = sock.read(fragment);
				if(length <= 0) {
					break;
				}

				result.insert(result.end(), fragment.begin(), fragment.begin() + length);
			}

			sock.disconnect();

			return result;
		}

		std::string 
		_sono_http::remove_header(
			__in const std::string &source
			)
		{
			size_t position;
			std::string result;

			if(!source.empty()) {

				position = source.find(SONO_HTTP_TERM_HEAD);
				if(position == std::string::npos) {
					THROW_SONO_HTTP_EXCEPTION(SONO_HTTP_EXCEPTION_MALFORMED);
				}

				try {
					result = source.substr(position + SONO_HTTP_TERM_HEAD_LEN, source.size());
				} catch(std::exception &exc) {
					THROW_SONO_HTTP_EXCEPTION_FORMAT(SONO_HTTP_EXCEPTION_MALFORMED,
						"%s", exc.what());
				}
			}

			return result;
		}
	}
}
