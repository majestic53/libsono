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
CC_FLAGS=-march=native -pthread -std=gnu++11 -O3 -Wall -Werror
DIR_BIN=./../../bin/
DIR_BUILD=./../../build/
DIR_INC=./include/
DIR_SRC=./src/
EXE=sonoctl
LIB=libsono.a

all: build tool

tool:
	@echo ''
	@echo '--- BUILDING TOOL --------------------------' 
	$(CC) $(CC_FLAGS) main.cpp $(DIR_BUILD)sono_loader.o $(DIR_BIN)$(LIB) -o $(DIR_BIN)$(EXE)
	@echo '--- DONE -----------------------------------'
	@echo ''

build: sono_loader.o

# SONO

sono_loader.o: $(DIR_SRC)sono_loader.cpp $(DIR_INC)sono_loader.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)sono_loader.cpp -o $(DIR_BUILD)sono_loader.o
