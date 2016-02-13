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

#ifndef SONO_HTTP_H_
#define SONO_HTTP_H_

namespace SONO {

	namespace COMP {

		typedef enum {
			SONO_HTTP_ENCODE_NONE = 0,
			SONO_HTTP_ENCODE_CHUNKED,
		} sono_http_encode_t;

		#define SONO_HTTP_ENCODE_MAX SONO_HTTP_ENCODE_CHUNKED

		#define SONO_HTTP_SUCCESS 200

		typedef class _sono_http {

			public:

				static std::string get(
					__in const std::string &path,
					__in const std::string &address,
					__in uint16_t port,
					__in_opt sono_socket_t type = SONO_SOCKET_TCP,
					__in_opt uint32_t timeout = SONO_SOCKET_NO_TIMEOUT
					);

				static std::string parse_body(
					__in const std::string &source,
					__in sono_http_encode_t encoding
					);

				static int parse_header(
					__in const std::string &source,
					__out sono_http_encode_t &encoding
					);

				static std::string remove_header(
					__in const std::string &source
					);

		} sono_http;
	}
}

#endif // SONO_HTTP_H_
