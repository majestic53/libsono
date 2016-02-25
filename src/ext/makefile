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

CC=clang
CC_FLAGS_0=-pthread -Wno-unused-result -Wsign-compare -Wunreachable-code -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes -march=native -mtune=generic -O3 -pipe -fstack-protector-strong -fPIC
CC_FLAGS_1=-pthread -shared -Wl,-O1,--sort-common,--as-needed,-z,relro
CC_LIB_0=/usr/include/python3.5m
CC_LIB_1=/usr/lib -lpython3.5m
DIR_BIN=./../../bin/
DIR_BUILD=./../../build/
DIR_SRC=./src/
LIB=libsono_py

all: build ext

ext:
	@echo ''
	@echo '--- BUILDING PYTHON MODULE -----------------'
	$(CC) $(CC_FLAGS_1) $(DIR_BUILD)sono_py.o -L$(CC_LIB_1) -o $(DIR_BIN)sono_py.so
	@echo '--- DONE -----------------------------------'
	@echo ''

build: sono_py.o

# MODULE

sono_py.o: $(DIR_SRC)sono_py.c
	$(CC) $(CC_FLAGS_0) -I$(CC_LIB_0) -c $(DIR_SRC)sono_py.c -o $(DIR_BUILD)sono_py.o
