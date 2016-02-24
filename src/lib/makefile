# libsono
# Copyright (C) 2016 David Jolly
# ----------------------
#
# libsono is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# libsono is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

CC=clang++
CC_FLAGS=-march=native -std=gnu++11 -fPIC -pthread -O3 -Wall -Werror
DIR_BIN=./../../bin/
DIR_BUILD=./../../build/
DIR_INC=include/
DIR_ROOT=./
DIR_SRC=src/
LIB=libsono
LIB_VER=1

all: build archive

archive:
	@echo ''
	@echo '--- BUILDING STATIC LIBRARY ----------------'
	ar rcs $(DIR_BIN)$(LIB).a $(DIR_BUILD)sono.o $(DIR_BUILD)sono_action.o $(DIR_BUILD)sono_device.o \
		$(DIR_BUILD)sono_exception.o $(DIR_BUILD)sono_http.o $(DIR_BUILD)sono_manager.o \
		$(DIR_BUILD)sono_service.o $(DIR_BUILD)sono_socket.o $(DIR_BUILD)sono_uid.o $(DIR_BUILD)sono_xml.o
	@echo '--- DONE -----------------------------------'
	@echo ''
	@echo '--- BUILDING SHARED LIBRARY ----------------'
	$(CC) -shared -Wl,-soname,$(LIB).so.$(LIB_VER) -o $(DIR_BIN)$(LIB).so \
		$(DIR_BUILD)sono.o $(DIR_BUILD)sono_action.o $(DIR_BUILD)sono_device.o $(DIR_BUILD)sono_exception.o \
		$(DIR_BUILD)sono_http.o $(DIR_BUILD)sono_manager.o $(DIR_BUILD)sono_service.o $(DIR_BUILD)sono_socket.o \
		$(DIR_BUILD)sono_uid.o $(DIR_BUILD)sono_xml.o
	@echo '--- DONE -----------------------------------'
	@echo ''

build: sono.o sono_action.o sono_device.o sono_exception.o sono_http.o sono_manager.o sono_service.o \
	sono_socket.o sono_uid.o sono_xml.o

# LIB

sono.o: $(DIR_SRC)sono.cpp $(DIR_ROOT)sono.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)sono.cpp -o $(DIR_BUILD)sono.o

# SONO

sono_exception.o: $(DIR_SRC)sono_exception.cpp $(DIR_INC)sono_exception.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)sono_exception.cpp -o $(DIR_BUILD)sono_exception.o

sono_manager.o: $(DIR_SRC)sono_manager.cpp $(DIR_INC)sono_manager.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)sono_manager.cpp -o $(DIR_BUILD)sono_manager.o

# SONO::COMP

sono_action.o: $(DIR_SRC)sono_action.cpp $(DIR_INC)sono_action.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)sono_action.cpp -o $(DIR_BUILD)sono_action.o

sono_device.o: $(DIR_SRC)sono_device.cpp $(DIR_INC)sono_device.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)sono_device.cpp -o $(DIR_BUILD)sono_device.o

sono_http.o: $(DIR_SRC)sono_http.cpp $(DIR_INC)sono_http.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)sono_http.cpp -o $(DIR_BUILD)sono_http.o

sono_service.o: $(DIR_SRC)sono_service.cpp $(DIR_INC)sono_service.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)sono_service.cpp -o $(DIR_BUILD)sono_service.o

sono_socket.o: $(DIR_SRC)sono_socket.cpp $(DIR_INC)sono_socket.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)sono_socket.cpp -o $(DIR_BUILD)sono_socket.o

sono_uid.o: $(DIR_SRC)sono_uid.cpp $(DIR_INC)sono_uid.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)sono_uid.cpp -o $(DIR_BUILD)sono_uid.o

sono_xml.o: $(DIR_SRC)sono_xml.cpp $(DIR_INC)sono_xml.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)sono_xml.cpp -o $(DIR_BUILD)sono_xml.o
